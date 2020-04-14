#include <iostream>
#include <unordered_map>
#include "Redis.h"

int main() {
	Redis redis ;
	std::unordered_map<std::string, uint64_t> options {
		{"NX", 1}
	} ;
	std::cout << redis.set("hello", "world", options) << "\n" ;
	std::cout << redis.set("hello", "change", options) << "\n" ;
	std::cout << redis.get("hello") << "\n" ;
	std::cout << redis.getrange("hello", 1, 3) << "\n" ;
	std::cout << redis.getset("new", "thing") << "\n" ;
	std::cout << redis.get("new") << "\n" ;
	return 0 ;
}