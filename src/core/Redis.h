#pragma once

// DEBUG
#include <iostream>

// TYPE INFORMATION
#include <cstdint>
#include <typeinfo>

// DATA STRUCTURES
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

class Redis
{
public:
	std::string set(std::string key, std::string value, std::unordered_map<std::string, uint64_t> options = {}) ;
	// Find way to make get, getrange const
	std::string get(std::string key) ;
	std::string getrange(std::string key, int32_t val_start, int32_t val_end) ;
	std::string getset(std::string key, std::string value) ;
	
private:
	constexpr static char RET_MSG_OK[] = "OK" ;
	constexpr static char RET_MSG_NIL[] = "(nil)" ;
	constexpr static char RET_MSG_EMPTY[] = "" ;
	std::unordered_map<std::string, std::string> r_strings ;
};

