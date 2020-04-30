#pragma once
#include <exception>

class KeyNotFoundException : public std::exception {
public :
	KeyNotFoundException() = default ;
	~KeyNotFoundException() = default ;
	virtual const char* what() const noexcept {
		return "ERROR : Expected to find a key." ;
	}
} ;

class OutOfIndexException : public std::exception {
public :
    OutOfIndexException() = default ;
    ~OutOfIndexException() = default ;
    virtual const char* what() const noexcept {
        return "ERROR : Index is out of bounds." ;
    }
} ;
