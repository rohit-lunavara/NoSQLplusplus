#pragma once

template <class KeyType, class ValueType>
class DataBase
{
public:
        const ValueType& get(const KeyType& key);
        bool set(const KeyType& key, const ValueType& value...);
        bool del(const KeyType& key);
        bool rename(const KeyType& key);
        bool exist(const KeyType& key);
        const KeyType& random();
};

template <class KeyType, class ValueType>
const ValueType& DataBase<KeyType, ValueType>::get(const KeyType& key)
{
    KeyType::unimplemented_function;
    ValueType::unimplemented_function;
}

template <class KeyType, class ValueType>
bool DataBase<KeyType, ValueType>::set(const KeyType& key, const ValueType& value...)
{
    KeyType::unimplemented_function;
    ValueType::unimplemented_function;
}

template <class KeyType, class ValueType>
bool DataBase<KeyType, ValueType>::del(const KeyType& key)
{
    KeyType::unimplemented_function;
    ValueType::unimplemented_function;
}

template <class KeyType, class ValueType>
bool DataBase<KeyType, ValueType>::rename(const KeyType& key)
{
    KeyType::unimplemented_function;
    ValueType::unimplemented_function;    
}

template <class KeyType, class ValueType>
bool DataBase<KeyType, ValueType>::exist(const KeyType& key)
{
    KeyType::unimplemented_function;
    ValueType::unimplemented_function;
}

template <class KeyType, class ValueType>
const DataBase<KeyType, ValueType>::KeyType& random()
{
    KeyType::unimplemented_function;
    ValueType::unimplemented_function;
}


#include "DataBase_string.h"
#include "DataBase_GeoCoordinate.h"

