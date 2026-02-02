#include "vect2.hpp"


vect2::vect2()
	: _x(0), _y(0)
{}


vect2::vect2(int x, int y)
	: _x(x), _y(y)
{}


vect2::vect2(const vect2& other)
	: _x(other._x), _y(other._y)
{}


vect2& vect2::operator=(const vect2& other) {

	if (this != &other) {
		_x = other._x;
		_y = other._y;
	}

	return *this;
}


vect2::~vect2()
{}


vect2 vect2::operator+(const vect2& rhs) const {

	return vect2(_x + rhs._x, _y + rhs._y);

}


vect2 vect2::operator-(const vect2& rhs) const {

	return vect2(_x - rhs._x, _y - rhs._y);

}


vect2 vect2::operator*(int scalar) const {

	return vect2(_x * scalar, _y * scalar);

}


vect2&	vect2::operator+=(const vect2& rhs) {

	_x = _x + rhs._x;
	_y = _y + rhs._y;

	return *this;
}


vect2&	vect2::operator-=(const vect2& rhs) {

	_x = _x - rhs._x;
	_y = _y - rhs._y;

	return *this;
}


vect2&	vect2::operator*=(int scalar) {

	_x = _x * scalar;
	_y = _y * scalar;

	return *this;
}


vect2	vect2::operator-() const {

	return vect2(-_x, -_y);
}


vect2&	vect2::operator++() {

	++_x;
	++_y;

	return *this;
}


vect2	vect2::operator++(int) {

	vect2 tmp(*this);

	++(*this);

	return tmp;
}


vect2&	vect2::operator--() {

	--_x;
	--_y;

	return *this;
}


vect2	vect2::operator--(int) {

	vect2 tmp(*this);

	--(*this);

	return tmp;
}


bool	vect2::operator==(const vect2& rhs) const {

	return _x == rhs._x && _y == rhs._y;
}


bool	vect2::operator!=(const vect2& rhs) const {

	return !(*this == rhs);
}


int&		vect2::operator[](int i) {

	return i == 0 ? _x : _y;
}


const int&	vect2::operator[](int i) const {

	return i == 0 ? _x : _y;
}


vect2	operator*(int scalar, const vect2& v) {

	return v * scalar;
}


std::ostream& operator<<(std::ostream&oss, const vect2& v) {

	oss << "{" << v[0] << ", " << v[1] << "}";

	return oss;
}