#include <string>
#include <sstream>
#include <iostream>
#include <cstdarg>
#include "HashTable.h"

template <>
class DataBase<std::string, std::string>
{
private:
	HashTable<std::string, std::string> internalStorage_;
public:
    const std::string& get(const std::string& key);
	bool set(const std::string& key, const std::string& value...);
    bool del(const std::string& key);
};


const std::string& DataBase<std::string, std::string>::get(const std::string& key)
{
	auto iter = internalStorage_.find(key);
	if (iter == internalStorage_.end())
	{
		std::stringstream ss;
		ss << "key " << key << " does not exist!";
		throw std::runtime_error(ss.str());
	}
	return iter->second;
}


bool DataBase<std::string, std::string>::set(const std::string& key,
 const std::string& value...)
{
	bool all_ok = false;
	bool has_opt = false;

	auto iter = internalStorage_.find(key) ;
	
	std::va_list args;
    va_start(args, value);
    // TODO - Handle time options
    char opt = static_cast<char>(va_arg(args, int));
    if (opt == 'n')
    {
		if (iter == internalStorage_.end()) {
			all_ok = true;
		}
		has_opt = true;
    }
    else if (opt == 'x')
    {
    	// OK if key exists
		if (iter != internalStorage_.end()) {
			all_ok = true ;
		}
		has_opt = true;
    }
    else if (opt == 't')
    {
		// time 	
    }
	va_end(args);

	// If no options, then set directly, otherwise, check options flag
	if (!has_opt || all_ok) {
		internalStorage_[key] = value;
		return true;
	}
	else {
		return false;
	}
}