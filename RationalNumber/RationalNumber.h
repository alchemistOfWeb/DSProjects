// Your goal is to create a rational number class that would
// support each of the operations given in main.cpp.
//
// In this file you must declare only the interface of your class
// and implement the given functions separately from the class (at the bottom of
// this file inside the namespace).
// Notice that the RationalNumber is a class template, where the
// template parameter is an integer type for numerator and denominator.
//
// Note - Rename the namespace "yourname" to whatever you want, feel creative
// ( but not too much :) ).
//
// After you wrote RationalNumber class and the tests in the main function work
// - write at the bottom of the file the downsides of such rational numbers,
// what would you change/remove/add? Would you use such rational numbers instead
// of double/float numbers? There is no right/wrong answer, this question is
// more of a philosofical kind than technical.
#pragma once
#include <iostream>

namespace numbers 
{
	


template <typename T>
concept IntegralValue = std::is_integral_v<T>;

template <IntegralValue T>
T calculate_gcd(T first, T second) {
	first = first < 0 ? -(signed)first : first;
	second = second < 0 ? -(signed)second : second;
	if (first == 0 || second == 0) return 1;
	while (first != second) {
		if (first > second) first -= second;
		else second -= first;
	}
	return first;
}

template <IntegralValue T>
class RationalNumber
{
private:
	T m_numerator;
	T m_denominator;

	void simplify() {
		int gcd = calculate_gcd(m_numerator, m_denominator);
		m_numerator /= gcd;
		m_denominator /= gcd;
	}
public:
	RationalNumber(T numerator = 0, T denominator = 1) {
		// TODO: simplify or optimize
		
		if (numerator == denominator) {
			numerator = denominator = 1;
		} else {
			int gcd = calculate_gcd(numerator, denominator);
			if (gcd != 1) {
				numerator /= gcd;
				denominator /= gcd;
			}
		}
		if (numerator < 0 && denominator < 0) {
			numerator = -(signed)numerator;
			denominator = -(signed)denominator;
		}
		if (denominator < 0) {
			numerator = -(signed)numerator;
			denominator = -(signed)denominator;
		}

		m_numerator = numerator;
		m_denominator = denominator;
	}

	RationalNumber(long double& other) {
		m_denominator = other.m_denominator;
		m_numerator = other.m_numerator;
	}

	RationalNumber(RationalNumber<T>& other) {
		m_denominator = other.m_denominator;
		m_numerator = other.m_numerator;
	}
	
	
	RationalNumber& operator*=(RationalNumber<T>& other) {
		m_numerator = m_numerator * other.m_numerator;
		m_denominator = m_denominator * other.m_denominator;
		this->simplify();
		return *this;
	}

	RationalNumber& operator/=(RationalNumber<T>& other) {
		m_numerator = m_numerator * other.m_denominator;
		m_denominator = m_denominator * other.m_numerator;
		this->simplify();
		return *this;
	}
	

	// Plus--------------------------------------:
	RationalNumber<T> operator+(RationalNumber<T>& other) {
		T numerator = m_numerator * other.m_denominator + other.m_numerator * m_denominator;
		T denominator = m_denominator * other.m_denominator;

		return RationalNumber<T>(numerator, denominator);
	}

	friend RationalNumber<T> operator+(T left, RationalNumber<T>& right) {
		T numerator = left * right.m_denominator + right.m_numerator;
		return RationalNumber(numerator, right.m_denominator);
	}

	friend RationalNumber<T> operator+(RationalNumber<T>& left, T right) {
		return right + left;
	}


	RationalNumber<T> operator+()
	{
		return RationalNumber<T>(+m_numerator, +m_denominator);
	}

	RationalNumber<T>& operator+=(RationalNumber& other)
	{
		*this = *this + other;
		return *this;
	}

	RationalNumber<T>& operator+=(int& other)
	{
		*this = *this + other;
		return *this;
	}

	RationalNumber& operator++()
	{
		m_numerator += m_denominator;
		return *this;
	}

	RationalNumber<T> operator++(int)
	{
		RationalNumber<T> old_value = *this;
		m_numerator += m_denominator;
		return old_value;
	}

	// Minus--------------------------------------:
	RationalNumber<T> operator-(RationalNumber<T>& other)
	{
		T numerator = m_numerator * other.m_denominator - other.m_numerator * m_denominator;
		T denominator = m_denominator * other.m_denominator;
		return RationalNumber<T>(numerator, denominator);
	}

	friend RationalNumber<T> operator-(RationalNumber<T>& left, int right) {
		T numerator = left.m_numerator - right * left.m_denominator;
		return RationalNumber<T>(numerator, left.m_denominator);
	}

	friend RationalNumber<T> operator-(int left, RationalNumber<T>& right) {
		T numerator = left * right.m_denominator - right.m_numerator;
		return RationalNumber<T>(numerator, right.m_denominator);
	}

	RationalNumber<T> operator-() {
		return RationalNumber<T>(-m_numerator, m_denominator);
	}

	RationalNumber<T>& operator-=(RationalNumber<T>& other) {
		*this = *this - other;
		return *this;
	}

	RationalNumber<T>& operator-=(int& other) {
		return *this - other;
	}

	RationalNumber& operator--() {
		return *this -= 1;
	}
	RationalNumber operator--(int) {
		return;
	}

	// Multiplying--------------------------------------:
	RationalNumber<T> operator*(RationalNumber<T>& other) {
		return RationalNumber<T>(
			m_numerator * other.m_numerator, 
			m_denominator * other.m_denominator
		);
	}

	friend RationalNumber<T> operator*(RationalNumber<T>& left, int right) {
		return RationalNumber<T>(
			left.m_numerator * right,
			left.m_denominator
		);
	}

	friend RationalNumber<T> operator*(int left, RationalNumber<T>& right) {
		return RationalNumber<T>(
			right.m_numerator * left,
			right.m_denominator
		);
	}

	// Dividing--------------------------------------:
	RationalNumber<T> operator/(RationalNumber<T>& other) {
		return RationalNumber<T>(
			m_numerator * other.m_denominator,
			m_denominator * other.m_numerator
		);
	}

	friend RationalNumber<T> operator/(RationalNumber<T>& left, int right) {
		return RationalNumber<T>(
			left.m_numerator,
			left.m_denominator * right
		);
	}

	friend RationalNumber<T> operator/(int left, RationalNumber<T>& right) {
		return RationalNumber<T>(
			right.m_numerator,
			right.m_denominator * left
		);
	}

	// Input & Output--------------------------------------
	friend std::ostream& operator<<(std::ostream& out, const RationalNumber<T>& number) {
		return out << number.m_numerator << '/' << number.m_denominator;
	}

	friend std::istream& operator>>(std::istream& in, RationalNumber<T>& number) {
		T a, b;
		in >> a >> b;
		number.m_numerator = a;
		number.m_denominator = a;
		return in;
	}
	
	// Comparison operators--------------------------------------:
	bool operator>(RationalNumber& other) {
		return (m_numerator * other.m_denominator) > (other.m_numerator * m_denominator);
	}

	bool operator<(RationalNumber& other) {
		return (m_numerator * other.m_denominator) < (other.m_numerator * m_denominator);
	}

	bool operator==(RationalNumber& other) {
		return (m_numerator * other.m_denominator) = (other.m_numerator * m_denominator);
	}

	bool operator!=(RationalNumber& other) {
		return (m_numerator * other.m_denominator) != (other.m_numerator * m_denominator);
	}

	bool operator>=(RationalNumber& other) {
		return (m_numerator * other.m_denominator) >= (other.m_numerator * m_denominator);
	}

	bool operator<=(RationalNumber & other) {
		return (m_numerator * other.m_denominator) <= (other.m_numerator * m_denominator);
	}

	// Type conversion operators--------------------------------------:
	operator int() {
		return m_numerator / m_denominator;
	};
	operator float() {
		return (float)m_numerator / m_denominator;
	};
	operator double() {
		return (double)m_numerator / m_denominator;
	};
};
	
namespace literals {
	RationalNumber<unsigned long long> operator "" _r(unsigned long long value) {
		return RationalNumber<unsigned long long>(value);
	}
}

} // namespace numbers


