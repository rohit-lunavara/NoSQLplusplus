#include "Database_String.h"


// Refactor to include options
std::string Database_String::set(const std::string& k, const std::string& v) {
	r_strings_[k] = v ;
	return R_STR_OK ;
}

std::string Database_String::get(const std::string& k) {
	auto k_str = r_strings_.find(k) ;
	if ( k_str == r_strings_.end() ) {
		return R_STR_NIL ;
	}
	// Refactor typeid to concepts
	else if ( typeid(k_str->second) != typeid(k) ) {
		throw NonStringValueException() ;
	}
	return k_str->second ;
}

std::string Database_String::getset(const std::string& k, const std::string& v) {
	std::string old_v { Database_String::get(k) } ;
	Database_String::set(k, v) ;
	return old_v ;
}

int32_t Database_String::append(const std::string& k, const std::string& v) {
	auto k_str = r_strings_.find(k) ;

	// Key already exists and is a string
	// Refactor typeid to concepts
	if ( k_str != r_strings_.end() && (typeid(k_str->second) == typeid(v))) {
		k_str->second += v ;
		return (k_str->second.size()) ;
	}
	// Key does not exist, therefore, it is created and the value is set
	r_strings_[k] = v ;
	return (v.size()) ;
}

std::string Database_String::getrange(const std::string& k, int32_t str_start, int32_t str_end) {
	std::string v_str { Database_String::get(k) } ;
	int32_t v_str_size { static_cast<int32_t>(v_str.size()) } ;

	if ( str_start < 0 ) {
		str_start = v_str_size + str_start ;
	}
	str_start = str_start < 0 ? 0 : str_start ;

	if ( str_end < 0 ) {
		str_end = v_str_size + str_end ;
	}
	str_end = str_end > v_str_size - 1 ? v_str_size - 1 : str_end ;

	if ( str_start < str_end ) {
		return v_str.substr(str_start, str_end - str_start) ;
	}
	return {} ;
}

int32_t Database_String::setrange(const std::string& k, uint32_t offset, const std::string& v) {
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
		// Check if value is a string
		// Refactor typeid to concepts
		if ( typeid(k_str->second) != typeid(k) ) {
			throw NonStringValueException() ;
		}
		auto k_str_size = k_str->second.size() ;
		if ( offset > k_str->second.size() ) {
			std::string padding (offset - k_str_size, ' ') ;
			k_str->second += padding ;
		}
		k_str->second.replace(offset, v.size(), v) ;
		return k_str->second.size() ;
	}
}

int32_t Database_String::strlen(const std::string& k) {
	return Database_String::get(k).size() ;
}

















