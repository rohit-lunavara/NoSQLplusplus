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

// ERROR HANDLING
#include <exception>

template<>
class DataBase<std::string, std::string>
{
public :
        // TODO change return value
	bool set(const std::string& k, const std::string& v);
        
	std::string get(const std::string& k) ;
	std::string getset(const std::string& k, const std::string& v) ;
	// Overloads for set, get, getset
	// mset, mget, mgetset
        
        // TODO
        bool remove(const std::string& key);

        // TODO
        bool rename(const std::string& key, const std::string& newkey);

        // TODO
        bool exist(const std::string& key);

	int32_t append(const std::string& k, const std::string& v) ;
	std::string getrange(const std::string& k, int32_t str_start, int32_t str_end) ;
	int32_t setrange(const std::string& k, uint32_t offset, const std::string& v) ;
	int32_t strlen(const std::string& k) ;
private :
	constexpr static char R_STR_OK[] { "OK" } ;
	constexpr static char R_STR_NIL[] { "(nil)" } ;
	std::unordered_map<std::string, std::string> r_strings_ ;
} ;

// Find a better option compared to exceptions for missing values
class NonStringValueException : public std::exception {
public :
	NonStringValueException() ;
	~NonStringValueException() ;
	virtual const char* what() const noexcept {
		return "ERROR : Expected string value.\n" ;
	}
} ;