#include "Redis.h"

std::string Redis::set(std::string key, std::string value, std::unordered_map<std::string, uint64_t> options) {
	bool all_ok { false } ;
	auto k_str = r_strings.find(key) ;
	
	for (const auto& [k_opt, v_opt] : options) {
		// TODO - Handle time options
		// NX
		if (k_opt == "NX") {
			// OK if key does not exist
			if (k_str == r_strings.end()) {
				all_ok = true ;
			}
		}
		// XX
		if (k_opt == "XX") {
			// OK if key exists
			if (k_str != r_strings.end()) {
				all_ok = true ;
			}
		}
	}
	
	// If no options, then set directly, otherwise, check options flag
	if (!options.size() || all_ok) {
		r_strings[key] = value ;
		return RET_MSG_OK ;
	}
	else {
		return RET_MSG_NIL ;
	}
}

std::string Redis::get(std::string key) {
	auto k_str = r_strings.find(key) ;
	if (k_str != r_strings.end()) {
		auto value = k_str->second ;
		if (typeid(key).name() != typeid(value).name()) {
			// Todo - Handle error
			std::cerr << "ERROR : Cannot handle non-string values.\n" ;
			// return RET_MSG_EMPTY ;
		}
		else {
			return value ;
		}
	}
	else {
		return RET_MSG_NIL ;
	}
}

std::string Redis::getrange(std::string key, int32_t val_start, int32_t val_end) {
	auto value = Redis::get(key) ;
	if (val_start < 0) {
		val_start = value.size() + val_start ;
	}
	if (val_end < 0) {
		val_end = value.size() + val_end ;
	}
	return value.substr(val_start, val_end - val_start) ;
}

std::string Redis::getset(std::string key, std::string value) {
	auto old_value = Redis::get(key) ;
	Redis::set(key, value) ;
	return old_value ;
}