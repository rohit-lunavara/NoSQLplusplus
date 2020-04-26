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
                std::get<KeyType>(branch.storage) = key;
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
        std::array<Branch, NMAXNODES+1> branchBuffer;
        for (int i = 0; i < NMAXNODES; ++i)
        {
                branchBuffer[i] = node->branches[i];
        }
        branchBuffer[NMAXNODES] = branch;

        // calculate the bound for all branches
        
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