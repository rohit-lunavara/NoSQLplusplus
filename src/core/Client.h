#pragma once

template <class KeyType, class ValueType>
class Client
{
public:
	virtual const ValueType& get(const KeyType& key) = 0;
	virtual bool set(const KeyType& key, const ValueType& value...) = 0;
	// TODO add more interfaces
};


