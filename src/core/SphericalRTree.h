#pragma once

#include <array>
#include <variant>
#include <memory>
#include <stdexcept>

#include "GeoCoordinate.h"

namespace RTree
{

template <class KeyType, int NMAXNODES = 8, int NMINNODES = NMAXNODES / 2>
class SphericalRTree
{
private:
        struct Node;
        struct Branch;

        struct Node
        {
                bool is_leaf() const { return level == 0; }
                bool is_internal() const { return level > 0; }
                int count;
                int level;
                std::array<Branch, NMAXNODES> branches;
        };

        struct Branch
        {
                Geography::GeoCoordinate center;
                double radius;
                std::variant<std::shared_ptr<Node>, KeyType> storage;
        };

        struct PartitionVars
        {
                int partition[NMAXNODES+1];
                bool is_taken[NMAXNODES+1];

                int count_total;
                int min_fill;

                int branch_count[2];
                Geography::GeoCoordinate branch_center[2];
                double branch_radius[2];
                double branch_area[2];

                std::array<Branch, NMAXNODES+1> branch_buffer;
                Geography::GeoCoordinate split_center;
                double split_radius;
                double split_area;
                
                PartitionVars(int count_total, int min_fill):
                        count_total(count_total),
                        min_fill(min_fill)
                {
                        for (int i = 0; i < NMAXNODES + 1; ++i)
                        {
                                partition[i] = -1;
                                is_taken[i] = false;
                        }
                }
        };

        std::shared_ptr<Node> root_;
public:
        SphericalRTree();
        ~SphericalRTree();
        
        void insert(
                const KeyType& key, 
                const Geography::GeoCoordinate& value, 
                double radius);
        void remove(
                const KeyType& key, 
                const Geography::GeoCoordinate& value, 
                double radius);
        int search(
                const Geography::GeoCoordinate& value, 
                double radius);
        
        void remove_all();

private:
        bool insert_to_node_(
                const KeyType& key,
                const Geography::GeoCoordinate& value,
                double radius,
                std::shared_ptr<Node>& root,
                int level);
        
        bool insert_to_node_recursive_(
                const KeyType& key,
                const Geography::GeoCoordinate& value,
                double radius,
                std::shared_ptr<Node> node,
                std::shared_ptr<Node>& newNode,
                int level);
        
        // Add a branch to a node.  Split the node if necessary.
        // Returns 0 if node not split.  Old node updated.
        // Returns 1 if node split, sets *new_node to address of new node.
        // Old node updated, becomes one of two.
        bool add_branch(
                const Branch& branch,
                std::shared_ptr<Node> node,
                std::shared_ptr<Node>& newNode);
        
        void split_node(
                std::shared_ptr<Node> node, 
                const Branch& branch, 
                std::shared_ptr<Node>& newNode);
        
        void pick_seeds(PartitionVars& parVars);

        void choose_partition(PartitionVars& parVars);

        void classify(int index, int group, PartitionVars& parVars);

        void node_cover(Geography::GeoCoordinate& center, double& radius,
                std::shared_ptr<Node> node);
        
        int pick_branch(const Geography::GeoCoordinate& center,
                double radius, 
                std::shared_ptr<Node> node);
};

template <class KeyType, int NMAXNODES, int NMINNODES>
SphericalRTree<KeyType, NMAXNODES, NMINNODES>::SphericalRTree():
        root_(new Node())
{
        root_->count = 0;
        root_->level = 0;
}

template <class KeyType, int NMAXNODES, int NMINNODES>
SphericalRTree<KeyType, NMAXNODES, NMINNODES>::~SphericalRTree()
{

}

template <class KeyType, int NMAXNODES, int NMINNODES>
void SphericalRTree<KeyType, NMAXNODES, NMINNODES>::insert(
        const KeyType& key, 
        const Geography::GeoCoordinate& value, 
        double radius)
{
        insert_to_node_(key, value, radius, root_, 0);
}

template <class KeyType, int NMAXNODES, int NMINNODES>
bool SphericalRTree<KeyType, NMAXNODES, NMINNODES>::insert_to_node_(
        const KeyType& key,
        const Geography::GeoCoordinate& value,
        double radius,
        std::shared_ptr<Node>& root,
        int level)
{
        std::shared_ptr<Node> newNode;
       
        if (insert_to_node_recursive_(key, value, radius, root, newNode, level))
        {  // root split
                //std::cout << "root split\n";
                auto newRoot = std::make_shared<Node>();
                newRoot->level = root->level + 1;
                newRoot->count = 0;

                Branch branch0, branch1;
                node_cover(branch0.center, branch0.radius, root);
                node_cover(branch1.center, branch1.radius, newNode);

                branch0.storage = root;
                branch1.storage = newNode;

                std::shared_ptr<Node> temp;
                add_branch(branch0, newRoot, temp);
                add_branch(branch1, newRoot, temp);

                return true;
        }

        return false;
}

template <class KeyType, int NMAXNODES, int NMINNODES>
int SphericalRTree<KeyType, NMAXNODES, NMINNODES>::pick_branch(
        const Geography::GeoCoordinate& center,
        double radius, 
        std::shared_ptr<Node> node)
{
        bool firstTime = true;
        double bestIncr = -1.;
        double bestArea = -1.;
        int best = -1;
        for (int i = 0; i < node->count; ++i)
        {
                const Geography::GeoCoordinate& curctr = node->branches[i].center;
                double currad = node->branches[i].radius;
                double curarea = Geography::spherical_area(currad);

                Geography::GeoCoordinate combctr;
                double combrad;
                Geography::combine_sphere(combctr, combrad,
                        curctr, currad, center, radius);
                
                double increase = Geography::spherical_area(combrad) - curarea;
                if (increase < bestIncr || firstTime)
                {
                        firstTime = false;
                        bestIncr = increase;
                        bestArea = curarea;
                        best = i;
                }
                else if (increase == bestIncr && curarea < bestArea)
                {
                        bestArea = curarea;
                        best = i;
                }

        }

        return best;
}

template <class KeyType, int NMAXNODES, int NMINNODES>
bool SphericalRTree<KeyType, NMAXNODES, NMINNODES>::insert_to_node_recursive_(
        const KeyType& key,
        const Geography::GeoCoordinate& value,
        double radius,
        std::shared_ptr<Node> node,
        std::shared_ptr<Node>& newNode,
        int level)
{
        std::shared_ptr<Node> otherNode;

        if (node->level > level) // above insertion level
        {
                int index = pick_branch(value, radius, node);
                if (!insert_to_node_recursive_(key, value, radius, 
                        std::get<std::shared_ptr<Node>>(node->branches[index].storage),
                        otherNode, level))
                { // child was not split
                        Geography::combine_sphere(
                                node->branches[index].center,
                                node->branches[index].radius,
                                node->branches[index].center,
                                node->branches[index].radius,
                                value, radius);
                        return false;
                }
                else // child was split
                {
                        node_cover(node->branches[index].center, 
                                node->branches[index].radius,
                        std::get<std::shared_ptr<Node>>(node->branches[index].storage));

                        Branch branch;
                        branch.storage = otherNode;
                        node_cover(branch.center, branch.radius, otherNode);
                        return add_branch(branch, node, newNode);
                }

        }
        else if (node->level == level) // reach 
        {
                //std::cout << "node = level\n";
                Branch branch;
                branch.center = value;
                branch.radius = radius;
                branch.storage = key;
                
                return add_branch(branch, node, newNode);
        }
        else
        {
                throw std::runtime_error("should never happen!");
                return false;
        }
}

template <class KeyType, int NMAXNODES, int NMINNODES>
bool SphericalRTree<KeyType, NMAXNODES, NMINNODES>::add_branch(
        const Branch& branch,
        std::shared_ptr<Node> node,
        std::shared_ptr<Node>& newNode)
{
        if (node->count < NMAXNODES)
        {
                node->branches[node->count] = branch;
                ++(node->count);
                return false;
        }
        else
        {
                //std::cout << "split node\n";
                split_node(node, branch, newNode);
                return true;
        }
}

template <class KeyType, int NMAXNODES, int NMINNODES>
void SphericalRTree<KeyType, NMAXNODES, NMINNODES>::node_cover(
        Geography::GeoCoordinate& center, double& radius,
        std::shared_ptr<Node> node)
{
        center = node->branches[0].center;
        radius = node->branches[0].radius;
        for (int i = 1; i < node->count; ++i)
        {
                Geography::combine_sphere(
                        center, radius,
                        center, radius,
                        node->branches[i].center,
                        node->branches[i].radius);
        }
}

template <class KeyType, int NMAXNODES, int NMINNODES>
void SphericalRTree<KeyType, NMAXNODES, NMINNODES>::split_node(
        std::shared_ptr<Node> node, 
        const Branch& branch, 
        std::shared_ptr<Node>& newNode)
{
        // load branch buffer from full node and an extra branch
        PartitionVars partitionVars(NMAXNODES+1, NMINNODES);
        //std::cout << "initialize partition vars\n";

        for (int i = 0; i < NMAXNODES; ++i)
        {
                partitionVars.branch_buffer[i] = node->branches[i];
        }
        partitionVars.branch_buffer[NMAXNODES] = branch;
        //std::cout << "branch buffer\n";
        // calculate the bound for all branches
        partitionVars.split_center = partitionVars.branch_buffer[0].center;
        partitionVars.split_radius = partitionVars.branch_buffer[0].radius;
        for (int i = 1; i < NMAXNODES+1; ++i)
        {
                //std::cout << "center: " << partitionVars.split_center << "\n";
                //std::cout << "radius: " << partitionVars.split_radius << "\n";

                //std::cout << "merge with: " << partitionVars.branch_buffer[i].center << "\n";
                //std::cout << "merge with: " << partitionVars.branch_buffer[i].radius << "\n";
                
                Geography::combine_sphere(
                        partitionVars.split_center, 
                        partitionVars.split_radius, 
                        partitionVars.split_center,
                        partitionVars.split_radius,
                        partitionVars.branch_buffer[i].center,
                        partitionVars.branch_buffer[i].radius);
                
                //std::cout << "merge center: " << partitionVars.split_center << "\n";
                //std::cout << "merge radius: " << partitionVars.split_radius << "\n";
        }

        //std::cout << "split radius: " << partitionVars.split_radius << "\n";
        partitionVars.split_area = 
                Geography::spherical_area(partitionVars.split_radius);
        //std::cout << "split area: " << partitionVars.split_area << "\n";

        pick_seeds(partitionVars);
        //std::cout << "pick seeds finished\n";
        choose_partition(partitionVars);
        //std::cout << "choose partition finished\n";


        newNode = std::make_shared<Node>();
        newNode->level = node->level;
        newNode->count = node->count = 0;

        for (int i = 0; i < partitionVars.count_total; ++i)
        {
                std::shared_ptr<Node> temp;
                if (partitionVars.partition[i] == 0)
                {
                       
                        add_branch(partitionVars.branch_buffer[i], node, temp);
                }
                else if (partitionVars.partition[i] == 1)
                {
                        add_branch(partitionVars.branch_buffer[i], newNode, temp);
                }
        }
}

template <class KeyType, int NMAXNODES, int NMINNODES>
void SphericalRTree<KeyType, NMAXNODES, NMINNODES>::choose_partition(
        PartitionVars& parVars)
{
        int group = -1, chosen = -1, betterGroup = -1;

        while (parVars.branch_count[0] + parVars.branch_count[1] 
                < parVars.count_total &&
                parVars.branch_count[0] < parVars.count_total - parVars.min_fill &&
                parVars.branch_count[1] < parVars.count_total - parVars.min_fill)
        {
                double biggestDiff = -1.;
                for (int i = 0; i < parVars.count_total; ++i)
                {
                        if (!parVars.is_taken[i])
                        {
                                const auto& center = parVars.branch_buffer[i].center;
                                double radius = parVars.branch_buffer[i].radius;

                                Geography::GeoCoordinate center0, center1;
                                double rad0, rad1;

                                using Geography::combine_sphere;
                                using Geography::spherical_area;

                                combine_sphere(center0, rad0,
                                        center, radius,
                                        parVars.branch_center[0],
                                        parVars.branch_radius[0]);
                                
                                combine_sphere(center1, rad1,
                                        center, radius,
                                        parVars.branch_center[1],
                                        parVars.branch_radius[1]);
                                double growth0 = spherical_area(rad0) - parVars.branch_area[0];
                                double growth1 = spherical_area(rad1) - parVars.branch_area[1];

                                double diff = growth1 - growth0;

                                if (diff >= 0)
                                {
                                        group = 0;
                                }
                                else
                                {
                                        group = 1;
                                        diff = -diff;
                                }


                                if(diff > biggestDiff)
                                {
                                        biggestDiff = diff;
                                        chosen = i;
                                        betterGroup = group;
                                }
                                else if((diff == biggestDiff) &&
                                 (parVars.branch_count[group] 
                                 < parVars.branch_count[betterGroup]))
                                {
                                        chosen = i;
                                        betterGroup = group;
                                }

                        }
                }
                classify(chosen, betterGroup, parVars);
        }

        // If one group too full, put remaining rects in the other
        if((parVars.branch_count[0] + parVars.branch_count[1]) < parVars.count_total)
        {
                if(parVars.branch_count[0] >= 
                        parVars.count_total - parVars.min_fill)
                {
                        group = 1;
                }
                else
                {
                        group = 0;
                }

                for(int index=0; index< parVars.count_total; ++index)
                {
                        if(!parVars.is_taken[index])
                        {
                                classify(index, group, parVars);
                        }
                }
        }

}


template <class KeyType, int NMAXNODES, int NMINNODES>
void SphericalRTree<KeyType, NMAXNODES, NMINNODES>::pick_seeds(PartitionVars& parVars)
{
        //std::cout << "begin pick seeds\n";
        double area[NMAXNODES+1];
        for (int i = 0; i < parVars.count_total; ++i)
        {
                area[i] = Geography::spherical_area(
                        parVars.branch_buffer[i].radius);
        }


        int seed0 = -1, seed1 = -1;
        double worst = -parVars.split_area - 1.;
        // for all pairs
        for (int i = 0; i < parVars.count_total-1; ++i)
        {
                for (int j = i + 1; j < parVars.count_total; ++j)
                {
                        //std::cout << "pair " << i << ", " << j << "\n";
                        Geography::GeoCoordinate geo;
                        double radius;
                        Geography::combine_sphere(geo, radius, 
                                parVars.branch_buffer[i].center,
                                parVars.branch_buffer[i].radius,
                                parVars.branch_buffer[j].center,
                                parVars.branch_buffer[j].radius);
                        double newArea = Geography::spherical_area(radius);
                        double waste = newArea - area[i] - area[j];
                        if(waste > worst)
                        {
                                worst = waste;
                                seed0 = i;
                                seed1 = j;
                        }
                }
        }

        //std::cout << "before classify\n";

        classify(seed0, 0, parVars);
        classify(seed1, 1, parVars);
}

template <class KeyType, int NMAXNODES, int NMINNODES>
void SphericalRTree<KeyType, NMAXNODES, NMINNODES>::classify(
        int index, int group, PartitionVars& parVars)
{
        if (parVars.is_taken[index])
        {
                throw std::runtime_error("the index is already taken!");
        }

        parVars.partition[index] = group;
        parVars.is_taken[index] = true;

        if (parVars.branch_count[group] == 0)
        {
                parVars.branch_center[group] = parVars.branch_buffer[index].center;
                parVars.branch_radius[group] = parVars.branch_buffer[index].radius;
        }
        else
        {
                Geography::combine_sphere(
                        parVars.branch_center[group],
                        parVars.branch_radius[group],
                        parVars.branch_center[group],
                        parVars.branch_radius[group], 
                        parVars.branch_buffer[index].center,
                        parVars.branch_buffer[index].radius);     
        }
        parVars.branch_area[group] = Geography::spherical_area(parVars.branch_radius[group]);
        ++parVars.branch_count[group];
}

template <class KeyType, int NMAXNODES, int NMINNODES>
void SphericalRTree<KeyType, NMAXNODES, NMINNODES>::remove(
        const KeyType& key, 
        const Geography::GeoCoordinate& value, 
        double radius)
{

}


template <class KeyType, int NMAXNODES, int NMINNODES>
int SphericalRTree<KeyType, NMAXNODES, NMINNODES>::search(
        const Geography::GeoCoordinate& value, 
        double radius)
{
    return 0;
}


template <class KeyType, int NMAXNODES, int NMINNODES>
void SphericalRTree<KeyType, NMAXNODES, NMINNODES>::remove_all()
{

}


}