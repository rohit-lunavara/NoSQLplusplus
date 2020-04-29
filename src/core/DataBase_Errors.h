#include <exception>

class ValueNotFoundException : public std::exception {
public :
	ValueNotFoundException() ;
	~ValueNotFoundException() ;
	virtual const char* what() const noexcept {
		return "ERROR : Expected to find a value.\n" ;
	}
} ;