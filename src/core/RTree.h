#pragma once

#include <vector>

template <class KeyType, class ValueType, class BoundType>
class RTreeNode
{
private:
    std::vector<std::shared_ptr<RTreeNode>> children_;
    std::weak_ptr<RTreeNode> parent_;

public:
    RTreeNode() {}

};

template <class KeyType, class ValueType>
class RTreeLeafNode
{
private:
    
}

template <class KeyType, class ValueType, class BoundType>
class RTree
{
private:
    std::shared_ptr<RTreeNode> root_;

public:
    RTree() {}

    void insert(const KeyType& key, const ValueType& value, const BoundType& bound);

};