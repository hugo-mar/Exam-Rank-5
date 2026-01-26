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
	int			i = _digits.size() - 1;
	int			j = rhs._digits.size() - 1;
	int			carry = 0;

	while (i >= 0 || j >= 0 || carry) {

		int sum = carry;

		if (i >= 0)
			sum += _digits[i--] - '0';
		if (j >= 0)
			sum += rhs._digits[j--] - '0';
		
		result.insert(result.begin(), static_cast<char>('0' + (sum % 10)));
		carry = sum / 10;
	}

	_digits = result;

	return *this;
}

bigint bigint::operator+(const bigint& rhs) const {

	bigint tmp(*this);

	tmp += rhs;

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

bigint& bigint::operator<<=(const bigint& shift) {

	if (_digits == "0")
		return *this;

	size_t digitsToShift = 0;
	for (size_t i = 0; i < shift._digits.size(); ++i)
		digitsToShift = digitsToShift * 10 + static_cast<size_t>(shift._digits[i] - '0');

	if (digitsToShift == 0)
		return *this;

	_digits.append(digitsToShift, '0');

	return *this;
}

bigint bigint::operator<<(const bigint& shift) const {

	bigint tmp(*this);

	tmp <<= shift;

	return tmp;
}

bigint& bigint::operator>>=(const bigint& shift) {

	if (_digits == "0")
		return *this;

	size_t digitsToShift = 0;
	for (size_t i = 0; i < shift._digits.size(); ++i)
		digitsToShift = digitsToShift * 10 + static_cast<size_t>(shift._digits[i] - '0');
	
	if (digitsToShift == 0)
		return *this;

	if (digitsToShift >= _digits.size()) {
		_digits = "0";
		return *this;
	}

	_digits.erase(_digits.size() - digitsToShift);

	return *this;
}

bigint bigint::operator>>(const bigint& shift) const {

	bigint tmp(*this);

	tmp >>= shift;

	return tmp;
}

bool bigint::operator<(const bigint& rhs) const {

	if (_digits.size() != rhs._digits.size())
		return _digits.size() < rhs._digits.size();
	
	return _digits < rhs._digits;
}

bool bigint::operator>(const bigint& rhs) const {

	if (_digits.size() != rhs._digits.size())
		return
			_digits.size() > rhs._digits.size();

	return _digits > rhs._digits;
}

bool bigint::operator==(const bigint& rhs) const {

	return _digits == rhs._digits;
}

bool bigint::operator!=(const bigint& rhs) const {

	return _digits != rhs._digits;
}

bool	bigint::operator<=(const bigint& rhs) const {

	return !(*this > rhs);
}

bool	bigint::operator>=(const bigint& rhs) const {

	return !(*this < rhs);
}

const std::string& bigint::getDigits() const {

	return _digits;
}

std::ostream& operator<<(std::ostream& oss, const bigint& value) {

	oss << value.getDigits();

	return oss;
}