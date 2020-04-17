#include "Client.h"

#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <cstdarg>

template <class KeyType, class ValueType>
class HashClient:
	public Client<KeyType, ValueType>
{
private:
	std::unordered_map<KeyType, ValueType> data_;

public:
	const ValueType& get(const KeyType& key);
	bool set(const KeyType& key, const ValueType& value...);	
};

template <class KeyType, class ValueType>
const ValueType& HashClient<KeyType, ValueType>::get(const KeyType& key)
{
	auto iter = data_.find(key);
	if (iter == data_.end())
	{
		std::stringstream ss;
		ss << "key " << key << " does not exist!";
		throw std::runtime_error(ss.str());
	}

	return iter->second;
}

template <class KeyType, class ValueType>
bool HashClient<KeyType, ValueType>::set(const KeyType& key, const ValueType& value...)
{
	bool all_ok = false;
	bool has_opt = false;

	auto iter = data_.find(key) ;
	
	std::va_list args;
    va_start(args, value);
    // TODO - Handle time options
    char opt = va_arg(args, int);
    if (opt == 'n')
    {
		if (iter == data_.end()) {
			all_ok = true;
		}
		has_opt = true;
    }
    else if (opt == 'x')
    {
    	// OK if key exists
		if (iter != data_.end()) {
			all_ok = true ;
		}
		has_opt = true;
    }
	va_end(args);

	// If no options, then set directly, otherwise, check options flag
	if (!has_opt || all_ok) {
		data_[key] = value;
		return true;
	}
	else {
		return false;
	}
}