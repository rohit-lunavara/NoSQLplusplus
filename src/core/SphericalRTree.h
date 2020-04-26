#pragma once
#include "GeoCoordinate.h"

namespace RTree
{

template <class KeyType, int NMAXNODES = 8, int NMINNODES = NMAXNODES / 2>
class SphericalRTree
{
private:

public:
        SphericalRTree();
        ~SphericalRTree();
        
        void insert(const KeyType& key, const Geography::GeoCoordinate& value, double radius);
        void remove(const KeyType& key, const Geography::GeoCoordinate& value, double radius);
        int search(const Geography::GeoCoordinate& value, double radius);
        void remove_all();
};

template <class KeyType, int NMAXNODES, int NMINNODES>
SphericalRTree<KeyType, NMAXNODES, NMINNODES>::SphericalRTree()
{

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