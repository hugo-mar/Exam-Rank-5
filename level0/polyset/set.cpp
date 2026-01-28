#include "set.hpp"

set::set(searchable_bag& bag)
	: _bag(&bag)
{}

set::set(const set& other)
	: _bag(other._bag)
{}

set& set::operator=(const set& other) {

	if (this != &other)
		_bag = other._bag;

	return *this;
}

set::~set()
{}

void set::insert(int x) {

	if (!_bag->has(x))
		_bag->insert(x);
}

void set::insert(int* array, int n) {

	if (!array || n <= 0)
		return;

	for (int i = 0; i < n; ++i)
		insert(array[i]);
}

void set::print() const {

	_bag->print();
}

void set::clear() {

	_bag->clear();
}

bool set::has(int x) const {

	return _bag->has(x);
}

searchable_bag& set::get_bag() {

    return *_bag;
}

const searchable_bag& set::get_bag() const {

	return *_bag;
}