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

// template<class T>
// concept key_value_container = requires (const std::remove_reference_t<T>& t) {
// 	std::equality_comparable_with<T, std::map> ;
// 	std::equality_comparable_with<T, std::unordered_map> ;
// } ;

template<>
class DataBase<std::string, std::string>
{
public :
	bool set(const std::string& k, const std::string& v, std::initializer_list<std::string> options = {}) ;
	std::string get(const std::string& k) ;
	std::string getset(const std::string& k, const std::string& v, std::initializer_list<std::string> options = {}) ;
	bool remove(const std::string& k) ;
	bool rename(const std::string& old_k, const std::string& new_k) ;
	bool exist(const std::string& k) ;
	// Overloads for set, get, getset
	// mset
	// std::vector<bool> set(const key_value_container<std::string, std::string> k_v) ;
	// mget
	std::vector<std::string> get(const std::vector<std::string>& ks) ;
	// mgetset
	// std::vector<std::string> getset(const key_value_container<std::string, std::string> k_v) ;
	std::vector<std::string> get_keys() ;

	// String specific methods
	int64_t append(const std::string& k, const std::string& v) ;
	std::string getrange(const std::string& k, int64_t str_start, int64_t str_end) ;
	int64_t setrange(const std::string& k, uint64_t offset, const std::string& v) ;
	int64_t strlen(const std::string& k) ;
private :
	std::unordered_map<std::string, std::string> r_strings_ ;
} ;

// Find a better option compared to exceptions for missing values
class NonStringValueException : public std::exception {
public :
	NonStringValueException() = default;
	~NonStringValueException() = default;
	virtual const char* what() const noexcept {
		return "ERROR : Expected string value." ;
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
		if ( ( option == "NX" && k_str == r_strings_.end() ) ||
                  ( option == "XX" && k_str != r_strings_.end() ) ) {
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
		throw KeyNotFoundException();
	}
	return k_str->second ;
}

std::string DataBase<std::string, std::string>::getset(
	const std::string& k, 
	const std::string& v,
	std::initializer_list<std::string> options) {
	std::string old_v { DataBase<std::string, std::string>::get(k) } ;
	DataBase<std::string, std::string>::set(k, v, options) ;
	return old_v ;
}

bool DataBase<std::string, std::string>::remove(const std::string& k) {
	auto k_str = r_strings_.find(k) ;
	if ( k_str == r_strings_.end() ) {
		return false ;
	}
	else {
		r_strings_.erase(k_str) ;
		return true ;
	}
}

bool DataBase<std::string, std::string>::rename(const std::string& old_k, const std::string& new_k) {
	auto k_handle = r_strings_.extract(old_k) ;
	if ( k_handle.empty() ) {
		return false ;
	}
	else {
		k_handle.key() = new_k ;
		r_strings_.insert(std::move(k_handle)) ;
		return true ;
	}
}

bool DataBase<std::string, std::string>::exist(const std::string& k) {
	return ( r_strings_.find(k) != r_strings_.end() ) ;
}

// std::vector<bool> DataBase<std::string, std::string>::set(
// 	const key_value_container<std::string, std::string> k_v) {
// 	std::vector<bool> res ;
// 	res.reserve(k_v.size()) ;
// 	for ( const auto& [k, v] : k_v ) {
// 		res.push_back(DataBase<std::string, std::string>::set(k, v)) ;
// 	}
// 	return res ;
// }

std::vector<std::string> DataBase<std::string, std::string>::get(
	const std::vector<std::string>& ks) {
	std::vector<std::string> vs ;
	vs.reserve(ks.size()) ;
	for ( const auto& k : ks ) {
		vs.push_back(DataBase<std::string, std::string>::get(k)) ;
	}
	return vs ;
}

// std::vector<std::string> getset(
// 	const key_value_container<std::string, std::string> ks_vs) {
// 	std::vector<std::string> ks ;
// 	std::vector<std::string> vs ;
// 	for ( auto it = ks_vs.begin() ; it != ks_vs.end() ; it++ ) {
// 		ks.push_back(it->first);
// 		vs.push_back(it->second);
// 	}
// 	std::vector<std::string> res ;
// 	res.reserve(ks_vs.size()) ;
// 	res = DataBase<std::string, std::string>::get(ks) ;
// 	DataBase<std::string, std::string>::set(ks_vs) ;
// 	return res ;
// }

std::vector<std::string> DataBase<std::string, std::string>::get_keys() {
	std::vector<std::string> res ;
	res.reserve(r_strings_.size()) ;
	for ( const auto& [k, v] : r_strings_ ) {
		res.push_back(k) ;
	}
	return res ;
}

// String specific methods

int64_t DataBase<std::string, std::string>::append(
	const std::string& k, 
	const std::string& v) {
	auto k_str = r_strings_.find(k) ;

	// Key already exists and is a string
	if ( k_str != r_strings_.end() ) {
		k_str->second += v ;
		return ( k_str->second.size() ) ;
	}
	// Key does not exist, therefore, it is created and the value is set
	r_strings_[k] = v ;
	return ( v.size() ) ;
}

std::string DataBase<std::string, std::string>::getrange(
	const std::string& k, 
	int64_t str_start, 
	int64_t str_end) {
	std::string v_str { DataBase<std::string, std::string>::get(k) } ;
	int64_t v_str_size { static_cast<int64_t>( v_str.size() ) } ;

	if ( str_start < 0 ) {
		str_start = v_str_size + str_start ;
	}
	str_start = str_start < 0 ? 0 : str_start ;

	if ( str_end < 0 ) {
		str_end = v_str_size + str_end ;
	}
	str_end = str_end > ( v_str_size - 1 ) ? v_str_size - 1 : str_end ;

	if ( str_start < str_end ) {
		return v_str.substr(str_start, str_end - str_start) ;
	}
	return {} ;
}

int64_t DataBase<std::string, std::string>::setrange(
	const std::string& k, 
	uint64_t offset, 
	const std::string& v) {
	auto k_str = r_strings_.find(k) ;

	// Padding with blank spaces instead of null characters
	// Key does not exist
	if ( k_str == r_strings_.end() ) {
		std::string padding (offset, ' ') ;
		r_strings_[k] = padding + v ;
		return r_strings_[k].size() ;
	}
	// Key exists
	else {
		auto k_str_size = k_str->second.size() ;
		if ( offset > k_str->second.size() ) {
			std::string padding (offset - k_str_size, ' ') ;
			k_str->second += padding ;
		}
		k_str->second.replace(offset, v.size(), v) ;
		return k_str->second.size() ;
	}
}

int64_t DataBase<std::string, std::string>::strlen(
	const std::string& k) {
	return DataBase<std::string, std::string>::get(k).size() ;
}

