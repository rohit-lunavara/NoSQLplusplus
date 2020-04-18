#pragma once
#include <variant>
#include <exception>

template <class KeyType, class ValueType>
class DataBase
{
public:
    const ValueType& get(const KeyType& key);
	bool set(const KeyType& key, const ValueType& value...);
    bool del(const KeyType& key);
};

template <class KeyType, class ValueType>
const ValueType& DataBase<KeyType, ValueType>::get(const KeyType& key)
{
    KeyType::unimplemented_function;
}

template <class KeyType, class ValueType>
bool DataBase<KeyType, ValueType>::set(const KeyType& key, const ValueType& value...)
{
    KeyType::unimplemented_function;
}

template <class KeyType, class ValueType>
bool DataBase<KeyType, ValueType>::del(const KeyType& key)
{
    KeyType::unimplemented_function;
}

#include "DataBase_string.h"
//#include "DataBase_geo.h"

