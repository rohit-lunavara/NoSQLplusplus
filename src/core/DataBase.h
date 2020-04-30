#pragma once

// for size and index, we use int64_t
// 

template <class KeyType, class ValueType>
class DataBase
{
public:
        ValueType get(const KeyType& key);
        bool set(const KeyType& key, const ValueType& value, 
                std::initializer_list<std::string> options);
        bool remove(const KeyType& key);
        bool rename(const KeyType& key, const KeyType& newkey);
        bool exist(const KeyType& key);
};

template <class KeyType, class ValueType>
ValueType DataBase<KeyType, ValueType>::get(const KeyType& key)
{
    KeyType::unimplemented_function;
    ValueType::unimplemented_function;
    return ValueType();
}

template <class KeyType, class ValueType>
bool DataBase<KeyType, ValueType>::set(const KeyType& key, const ValueType& value,
        std::initializer_list<std::string> options)
{
    KeyType::unimplemented_function;
    ValueType::unimplemented_function;
    return false;
}

template <class KeyType, class ValueType>
bool DataBase<KeyType, ValueType>::remove(const KeyType& key)
{
    KeyType::unimplemented_function;
    ValueType::unimplemented_function;
    return false;
}

template <class KeyType, class ValueType>
bool DataBase<KeyType, ValueType>::rename(const KeyType& key, const KeyType& newkey)
{
    KeyType::unimplemented_function;
    ValueType::unimplemented_function;

    return false;
}

template <class KeyType, class ValueType>
bool DataBase<KeyType, ValueType>::exist(const KeyType& key)
{
    KeyType::unimplemented_function;
    ValueType::unimplemented_function;

    return false;
}

#include "DataBase_Errors.h"
#include "DataBase_GeoCoordinate.h"
#include "DataBase_String.h"
#include "DataBase_List.h"
