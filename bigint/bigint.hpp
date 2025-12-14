#ifndef BIGINT_HPP
# define BIGINT_HPP

# include <string>
# include <ostream>

class bigint
{	
private:

	std::string	_digits;

public:
	bigint();
	bigint(unsigned int n);
	bigint(const bigint& other);	
	~bigint();

	bigint&	operator=(const bigint& other);

	bigint&	operator+=(const bigint& rhs);
	bigint	operator+(const bigint& rhs)const;

	bigint&	operator++();
	bigint	operator++(int);

	bigint&	operator<<=(const bigint& shift);
	bigint	operator<<(const bigint& shift) const;

	bigint&	operator>>=(const bigint& shift);
	bigint	operator>>(const bigint& shift) const;

	bool	operator<(const bigint& rhs) const;
	bool	operator>(const bigint& rhs) const;
	bool	operator==(const bigint& rhs) const;
	bool	operator!=(const bigint& rhs) const;
	bool	operator<=(const bigint& rhs) const;
	bool	operator>=(const bigint& rhs) const;
	
};

std::ostream&	operator<<(std::ostream&, const bigint&);

#endif