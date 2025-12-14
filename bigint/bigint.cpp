#include "bigint.hpp"

bigint::bigint() : _digits("0") {}

bigint::bigint(unsigned int n){	

	if (n == 0) {
		_digits = "0";
		return;	
	}

	while (n) {
		_digits.insert(_digits.begin(), static_cast<char>('0' + (n % 10)));
		n /= 10;
	}
}

bigint::bigint(const bigint& other) : _digits(other._digits) {}

bigint::~bigint() {}

bigint& bigint::operator=(const bigint& other) {

	if (this != &other) {
		_digits = other._digits;
	}
	return *this;
}

bigint& bigint::operator+=(const bigint& rhs) {

	std::string	result;
	int			i = _digits.size();
	int			j = rhs._digits.size();
	int			carry = 0;

	while (i >= 0 || j >= 0 || carry) {

		int sum = carry;

		if (i >= 0)
			sum += _digits[i--] - '0';
		if (j >= 0)
			sum += rhs._digits[j--] - '0';
		
		result.insert(result.begin(), static_cast<char>(sum % 10));
		carry = sum / 10;
	}

	_digits = result;

	return *this;
}

bigint bigint::operator+(const bigint& rhs) const {

	bigint tmp(rhs);

	tmp += *this;

	return tmp;	
}

bigint& bigint::operator++() {

	*this += bigint(1);

	return *this;
}

bigint bigint::operator++(int) {

	bigint tmp(*this);

	++(*this);
	
	return tmp;
}