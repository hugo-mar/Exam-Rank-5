#ifndef SET_HPP
# define SET_HPP

# include "searchable_bag.hpp"
# include <cstddef>

class set
{

private:
	searchable_bag*	_bag;

public:

	set(searchable_bag& bag);
	set(const set& other);
	set& operator=(const set& other);
	~set();

	void	insert(int x);
	void	insert(int* array, int n);
	void	print() const;
	void	clear();
	bool	has(int x) const;

	searchable_bag&			get_bag();
	const searchable_bag&	get_bag() const;
};

#endif