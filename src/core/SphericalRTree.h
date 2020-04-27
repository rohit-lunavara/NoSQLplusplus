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

        void classify(int index, int group, PartitionVars& parVars);
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
        // root split
        if (insert_to_node_recursive_(key, value, radius, root, newNode, level))
        {

                return true;
        }

        return false;
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
        if (node->level > level) // above insertion level
        {

        }
        else if (node->level == level) // reach 
        {
                Branch branch;
                branch.center = value;
                branch.storage = key;
                // 
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
                node->branch[node->count] = branch;
                ++(node->count);
                return false;
        }
        else
        {
                split_node(node, branch, newNode);
                return true;
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
        for (int i = 0; i < NMAXNODES; ++i)
        {
                partitionVars.branch_buffer[i] = node->branches[i];
        }
        partitionVars.branch_buffer[NMAXNODES] = branch;

        // calculate the bound for all branches
        partitionVars.split_center = partitionVars.branchBuffer[0].center;
        partitionVars.split_radius = partitionVars.branchBuffer[0].radius;
        for (int i = 1; i < NMAXNODES+1; ++i)
        {
                Geography::combine_sphere(
                        partitionVars.split_center, 
                        partitionVars.split_radius, 
                        partitionVars.split_center,
                        partitionVars.split_radius,
                        partitionVars.branch_buffer[i].center,
                        partitionVars.branch_buffer[i].radius);
        }
        partitionVars.split_area = 
                Geography::spherical_area(partitionVars.split_radius);

        // pick seeds
        pick_seeds(partitionVars);

        node->level = -1;
        node->count = 0;


        int level = node->level;
        int count_all = NMAXNODES+1;
        int min_fill = NMINNODES;

        // choose partition
        int count_l = 0;
        int count_r = 0;
        double area_l = 0.;
        double area_r = 0.;
}

template <class KeyType, int NMAXNODES, int NMINNODES>
void SphericalRTree<KeyType, NMAXNODES, NMINNODES>::pick_seeds(PartitionVars& parVars)
{
        double area[NMAXNODES+1];
        for (int i = 0; i < parVars.count_total; ++i)
        {
                area[i] = Geography::spherical_area(
                        parVars.branch_buffer[i].radius);
        }

        int seed0, seed1;
        double worst = -parVars.split_area - 1.;
        // for all pairs
        for (int i = 0; i < parVars.count_total-1; ++i)
        {
                for (int j = i + 1; j < parVars.count_total; ++i)
                {
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

        if (parVars.branch_count[group] == 0)
        {
                parVars.branch_center[group] = parVars.branch_buffer[index].center;
                parVars.branch_radius[group] = parVars.branch_buffer[index].radius;
        }
        else
        {

        }
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