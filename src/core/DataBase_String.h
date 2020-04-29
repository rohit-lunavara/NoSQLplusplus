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

// OPTIONS
#include <initializer_list>

// HELPERS
#include <algorithm>

#include "DataBase_Errors.h"

template<>
class DataBase<std::string, std::string>
{
public :
    // TODO change return value
	bool set(const std::string& k, const std::string& v, std::initializer_list<std::string> options = {}) ;
	std::string get(const std::string& k) ;
	std::string getset(const std::string& k, const std::string& v) ;
	// Overloads for set, get, getset
	// mset, mget, mgetset
        
        // TODO
    bool remove(const std::string& k);

    // TODO
    bool rename(const std::string& src_k, const std::string& dest_k);

    // TODO
    bool exist(const std::string& k);

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
	NonStringValueException() = default;
	~NonStringValueException() = default;
	virtual const char* what() const noexcept {
		return "ERROR : Expected string value.\n" ;
	}
} ;

bool DataBase<std::string, std::string>::set(
        const std::string& k, 
        const std::string& v, 
        std::initializer_list<std::string> options) {
	auto k_str = r_strings_.find(k) ;

	// Make all options uppercase
	for ( auto& option : options ) {
		std::for_each(option.begin(), option.end(), ::toupper);
	}

	for ( const auto& option : options ) {
		// NX -> Set if it does not exist
		// XX -> Set if it exists
		if ( ( option == "NX" && k_str == r_strings_.end() ) || ( option == "XX" && k_str != r_strings_.end() ) ) {
			r_strings_[k] = v ;
			return true ;
		}
	}

	if ( options.size() == 0 ) {
		r_strings_[k] = v ;
		return true ;
	}
	else {
		return false ;
	}
}

std::string DataBase<std::string, std::string>::get(const std::string& k) {
	auto k_str = r_strings_.find(k) ;
	if ( k_str == r_strings_.end() ) {
		throw ValueNotFoundException();
	}
	return k_str->second ;
}

// template<>
// std::string DataBase<std::string, std::string>::getset(const std::string& k, const std::string& v) {
// 	std::string old_v { Database_String::get(k) } ;
// 	Database_String::set(k, v) ;
// 	return old_v ;
// }

// template<>
// int32_t DataBase<std::string, std::string>::append(const std::string& k, const std::string& v) {
// 	auto k_str = r_strings_.find(k) ;

// 	// Key already exists and is a string
// 	// Refactor typeid to concepts
// 	if ( k_str != r_strings_.end() && (typeid(k_str->second) == typeid(v))) {
// 		k_str->second += v ;
// 		return (k_str->second.size()) ;
// 	}
// 	// Key does not exist, therefore, it is created and the value is set
// 	r_strings_[k] = v ;
// 	return (v.size()) ;
// }

// template<>
// std::string DataBase<std::string, std::string>::getrange(const std::string& k, int32_t str_start, int32_t str_end) {
// 	std::string v_str { Database_String::get(k) } ;
// 	int32_t v_str_size { static_cast<int32_t>(v_str.size()) } ;

// 	if ( str_start < 0 ) {
// 		str_start = v_str_size + str_start ;
// 	}
// 	str_start = str_start < 0 ? 0 : str_start ;

// 	if ( str_end < 0 ) {
// 		str_end = v_str_size + str_end ;
// 	}
// 	str_end = str_end > v_str_size - 1 ? v_str_size - 1 : str_end ;

// 	if ( str_start < str_end ) {
// 		return v_str.substr(str_start, str_end - str_start) ;
// 	}
// 	return {} ;
// }

// template<>
// int32_t DataBase<std::string, std::string>::setrange(const std::string& k, uint32_t offset, const std::string& v) {
// 	auto k_str = r_strings_.find(k) ;

// 	// Padding with blank spaces instead of null characters
// 	// Key does not exist
// 	if ( k_str == r_strings_.end() ) {
// 		std::string padding (offset, ' ') ;
// 		r_strings_[k] = padding + v ;
// 		return r_strings_[k].size() ;
// 	}
// 	// Key exists
// 	else {
// 		// Check if value is a string
// 		// Refactor typeid to concepts
// 		if ( typeid(k_str->second) != typeid(k) ) {
// 			throw NonStringValueException() ;
// 		}
// 		auto k_str_size = k_str->second.size() ;
// 		if ( offset > k_str->second.size() ) {
// 			std::string padding (offset - k_str_size, ' ') ;
// 			k_str->second += padding ;
// 		}
// 		k_str->second.replace(offset, v.size(), v) ;
// 		return k_str->second.size() ;
// 	}
// }

// template<>
// int32_t DataBase<std::string, std::string>::strlen(const std::string& k) {
// 	return Database_String::get(k).size() ;
// }