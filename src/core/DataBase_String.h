/*
Author : Rohit Lalit Lunavara
UNI : rll2181

String module for the database.
*/

#pragma once

// DEBUG
// #include <iostream>

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
	/**
		Sets a string key to a string value in the database.
		Returns true -> If set successfully, false otherwise.
	*/
	bool set(const std::string& k, const std::string& v, std::initializer_list<std::string> options = {}) ;

	/**
		Get a value for the key from the database.
		Throws KeyNotFoundException if key is not found.
		Returns value which exists for the key.
	*/
	std::string get(const std::string& k) ;

	/**
		Get multiple values for the keys from the database.
		Throws KeyNotFoundException if any key is not found.
		Returns values which exist for the keys.
	*/
	std::vector<std::string> get(const std::vector<std::string>& ks) ;

	/**
		Combines the get and set methods (without options).
		Throws KeyNotFoundException if key is not found.
		Returns old value which existed for the key.
	*/
	std::string getset(const std::string& k, const std::string& v) ;

	/**
		Removes key if it exists in the database.
		Returns true -> If key removed successfully, false otherwise.
	*/
	bool remove(const std::string& k) ;

	/**
		Renames old key name to new key name if it exists in the database.
		Returns true -> If it exists and is renamed, false otherwise.
	*/
	bool rename(const std::string& old_k, const std::string& new_k) ;

	/**
		Checks if a key exists in the database.
		Returns true -> If it exists, false otherwise.
	*/
	bool exist(const std::string& k) ;

	/**
		Get all the keys in the database.
		Returns all keys present in the database.
	*/
	std::vector<std::string> get_keys() ;

	// String specific methods

	/**
		Appends a value to an existing key or sets a value if key does not exist.
		Returns size of the value after the append operation.
	*/
	uint64_t append(const std::string& k, const std::string& v) ;

	/**
		Get the substring of the value stored at key.
		If start < end, returns substring specified by the indices.
		If start >= end, returns an empty substring.
	*/
	std::string getrange(const std::string& k, int64_t str_start, int64_t str_end) ;

	/**
		Sets a value for the key starting at specified offset.
		Returns size of the value after the setrange operation.
	*/
	uint64_t setrange(const std::string& k, uint64_t offset, const std::string& v) ;
	
	/**
		Get the size of the value at the key.
		Throws KeyNotFoundException if key is not found.
		Returns size of the value at key.
	*/
	uint64_t size(const std::string& k) ;


	// TODO
	// Overloads for set, getset
	// mset
	// std::vector<bool> set(const key_value_container<std::string, std::string> k_v) ;
	// mgetset
	// std::vector<std::string> getset(const key_value_container<std::string, std::string> k_v) ;
private :
	std::unordered_map<std::string, std::string> r_strings_ ;
} ;

/**
@brief
Sets a string key to a string value in the database.

@details
Time Complexity -> O(1)
Options include "NX" and "XX".
"NX" -> Set if it does not exist
"XX" -> Set if it exists

@param[in]
std::string key

@param[in]
std::string value

@param[in]
std::initializer_list<std::string> options

@class
DataBase<std::string, std::string>

@example
str_db.set("great", "amazing") ;

@return
bool 
true -> If set successfully, false otherwise.
*/
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

/*!

@brief
Get a value for the key from the database.

@details
Time Complexity -> O(1)
Throws KeyNotFoundException if key is not found.

@param[in]
std::string key

@class
DataBase<std::string, std::string>

@example
str_db.get("great") ;

@return
std::string
Value which exists for the key.
*/
std::string DataBase<std::string, std::string>::get(
	const std::string& k) {
	auto k_str = r_strings_.find(k) ;
	if ( k_str == r_strings_.end() ) {
		throw KeyNotFoundException();
	}
	return k_str->second ;
}

/*!

@brief
Get multiple values for the keys from the database.

@details
Time Complexity -> O(n)
where, n -> Number of key-value pairs in the database.
Throws KeyNotFoundException if any get fails.

@param[in]
std::vector<std::string> keys

@class
DataBase<std::string, std::string>

@example
str_db.set("great", "amazing") ;
str_db.set("fantastic", "awesome") ;
str_db.get({"great", "fantastic"}) ; // Returns { "amazing", "awesome" }

@return
std::vector<std::string>
Values which exist for the keys.
*/
std::vector<std::string> DataBase<std::string, std::string>::get(
	const std::vector<std::string>& ks) {
	std::vector<std::string> vs ;
	vs.reserve(ks.size()) ;
	for ( const auto& k : ks ) {
		vs.push_back(DataBase<std::string, std::string>::get(k)) ;
	}
	return vs ;
}

/*!

@brief
Combines the get and set methods (without options).

@details
Time Complexity -> O(1)
Does not allow set with options.
Throws KeyNotFoundException if get fails.

@param[in]
std::string key

@param[in]
std::string value

@class
DataBase<std::string, std::string>

@example
str_db.set("great", "awesome") ;
str_db.getset("great", "fantastic") ; // Returns "awesome"

@return
std::string
Old value for the given key.
*/
std::string DataBase<std::string, std::string>::getset(
	const std::string& k, 
	const std::string& v) {
	std::string old_v { DataBase<std::string, std::string>::get(k) } ;
	DataBase<std::string, std::string>::set(k, v) ;
	return old_v ;
}

/*!

@brief
Removes key if it exists in the database.

@details
Time Complexity -> O(1)

@param[in]
std::string key

@class
DataBase<std::string, std::string>

@example
db.set("great", "amazing") ;
db.remove("great") ;

@return
bool
true -> If key removed successfully, false otherwise.
*/
bool DataBase<std::string, std::string>::remove(
	const std::string& k) {
	auto k_str = r_strings_.find(k) ;
	if ( k_str == r_strings_.end() ) {
		return false ;
	}
	else {
		r_strings_.erase(k_str) ;
		return true ;
	}
}

/*!

@brief
Renames old key name to new key name if it exists in the database.

@details
Time Complexity -> O(1)

@param[in]
std::string old_key_name

@param[in]
std::string new_key_name

@class
DataBase<std::string, std::string>

@example
str_db.set("great", "amazing") ;
str_db.rename("great", "fantastic") ;
str_db.get("fantastic") ; // Returns "amazing"

@return
bool
true -> If it exists and is renamed, false otherwise.
*/
bool DataBase<std::string, std::string>::rename(
	const std::string& old_k, 
	const std::string& new_k) {
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

/*!

@brief
Checks if a key exists in the database.

@details
Time Complexity -> O(1)

@param[in]
std::string key

@class

@example

@return
bool
true -> If it exists, false otherwise.
*/
bool DataBase<std::string, std::string>::exist(
	const std::string& k) {
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

/*!

@brief
Get all the keys in the database.

@details
Time Complexity -> O(n)
where, n -> Number of key-value pairs in the database.

@class
DataBase<std::string, std::string>

@example
str_db.set("great", "amazing") ;
str_db.set("fantastic", "awesome") ;
str_db.get_keys() ; // Returns { "great", "fantastic" }

@return
std::vector<std::string>
All keys present in the database.
*/
std::vector<std::string> DataBase<std::string, std::string>::get_keys() {
	std::vector<std::string> res ;
	res.reserve(r_strings_.size()) ;
	for ( const auto& [k, v] : r_strings_ ) {
		res.push_back(k) ;
	}
	return res ;
}

/********************************
*
* String specific methods
*
*********************************/


/*!

@brief
Appends a value to a key.

@details
Time Complexity -> O(n + m)
where, n -> Size of original string value.
m -> Size of string value to be appended.
Behaves like set if key does not exist.

@param[in]
std::string key

@param[in]
std::string append_value

@class
DataBase<std::string, std::string>

@example
str_db.set("great", "amazing") ;
str_db.append("great", " fantastic") ; // Returns 17

@return
int64_t
Size of the value after the append operation.
*/
uint64_t DataBase<std::string, std::string>::append(
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

/*!

@brief
Get the substring of the value stored at key.

@details
Time Complexity -> O(n)
where, n -> Size of the returned substring.
Can specify negative start and end indices as well.

@param[in]
std::string key

@param[in]
int64_t string_start

@param[in]
int64_t string_end

@class
DataBase<std::string, std::string>

@example
str_db.set("great", "amazing") ;
str_db.getrange("great", 3, 7) ; // Returns "zing"
str_db.getrange("great", 0, -1) ; // Returns "amazin"

@return
std::string
If start < end, returns substring specified by the indices.
If start >= end, returns an empty substring.
*/
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
	str_end = str_end > ( v_str_size - 1 ) ? ( v_str_size - 1 ) : str_end ;

	if ( str_start < str_end ) {
		return v_str.substr(str_start, str_end - str_start) ;
	}
	return {} ;
}

/*!

@brief
Sets a value for the key starting at specified offset.

@details
Time Complexity -> O(*)
?? UNKNOWN
If the offset is out of the key's range, then it is padded with whitespaces.

@param[in]
std::string key

@param[in]
uint64_t offset

@param[in]
std::string value

@class
DataBase<std::string, std::string>

@example
str_db.setrange("great", 4, "amazing") ; // Returns "    amazing"

@return
uint64_t
Size of the value after the setrange operation
*/
uint64_t DataBase<std::string, std::string>::setrange(
	const std::string& k, 
	uint64_t offset, 
	const std::string& v) {
	auto k_str = r_strings_.find(k) ;

	// Padding with blank spaces instead of null characters
	// Key does not exist
	if ( k_str == r_strings_.end() ) {
		std::string padding (offset, ' ') ;
		r_strings_[k] = padding + v ;
		return ( r_strings_[k].size() ) ;
	}
	// Key exists
	else {
		auto k_str_size = k_str->second.size() ;
		if ( offset > k_str->second.size() ) {
			std::string padding (offset - k_str_size, ' ') ;
			k_str->second += padding ;
		}
		k_str->second.replace(offset, v.size(), v) ;
		return ( k_str->second.size() ) ;
	}
}

/**

@brief
Get the size of the value at the key.

@details
Time Complexity -> O(1)
Throws KeyNotFoundException if key is not found.

@param[in]
std::string key

@class
DataBase<std::string, std::string>

@example
str_db.set("great", "amazing") ;
str_db.size("great") ; // Returns 7

@return
uint64_t
Size of the value at key.
*/
uint64_t DataBase<std::string, std::string>::size(
	const std::string& k) {
	return DataBase<std::string, std::string>::get(k).size() ;
}

