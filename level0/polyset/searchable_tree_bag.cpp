#include "searchable_tree_bag.hpp"

searchable_tree_bag::searchable_tree_bag() : tree_bag() {}

searchable_tree_bag::searchable_tree_bag(const searchable_tree_bag& other)
	: tree_bag(other) {}

searchable_tree_bag& searchable_tree_bag::operator=(const searchable_tree_bag& other) {

	if (this != &other)
		tree_bag::operator=(other);

	return *this;
}

bool searchable_tree_bag::has(int x) const {

	node *cur = tree_;

	while (cur) {

		if (x == cur->value)
			return true;
		
		if (x > cur->value)
			cur = cur->r;
		else
			cur = cur->l;
	}

	return false;
}

searchable_tree_bag::~searchable_tree_bag() {}