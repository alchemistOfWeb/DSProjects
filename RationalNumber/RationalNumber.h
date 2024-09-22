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
			if (first > second) first %= second;
			else second %= first;
			if (first == 0) return second;
			if (second == 0) return first;
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
		RationalNumber(T numerator = 0, T denominator = 1);

		RationalNumber(long double& other);

		RationalNumber(RationalNumber<T>& other);
	

		// Plus--------------------------------------:
		RationalNumber<T> operator+(RationalNumber<T>& other);

		template<IntegralValue T>
		friend RationalNumber<T> operator+(T left, const RationalNumber<T>& right);

		template<IntegralValue T>
		friend RationalNumber<T> operator+(const RationalNumber<T>& left, T right);

		RationalNumber<T> operator+();

		RationalNumber<T>& operator+=(RationalNumber& other);

		RationalNumber<T>& operator+=(int& other);

		RationalNumber<T>& operator++();

		RationalNumber<T> operator++(int);


		// Minus--------------------------------------:
		RationalNumber<T> operator-(RationalNumber<T>& other);

		template<IntegralValue T>
		friend RationalNumber<T> operator-(RationalNumber<T>& left, int right);

		template<IntegralValue T>
		friend RationalNumber<T> operator-(int left, RationalNumber<T>& right);

		RationalNumber<T> operator-();

		RationalNumber<T>& operator-=(RationalNumber<T>& other);

		RationalNumber<T>& operator-=(int& other);

		RationalNumber<T>& operator--();

		RationalNumber<T> operator--(int);


		// Multiplying--------------------------------------:
		RationalNumber<T> operator*(RationalNumber<T>& other);

		RationalNumber& operator*=(RationalNumber<T>& other);

		template<IntegralValue T>
		friend RationalNumber<T> operator*(RationalNumber<T>& left, int right);

		template<IntegralValue T>
		friend RationalNumber<T> operator*(int left, RationalNumber<T>& right);


		// Dividing--------------------------------------:
		RationalNumber<T> operator/(RationalNumber<T>& other);

		RationalNumber& operator/=(RationalNumber<T>& other);
	
		RationalNumber<T> operator/(const RationalNumber<T>& other) const;

		template<IntegralValue T>
		friend RationalNumber<T> operator/(RationalNumber<T>& left, T right);

		template<IntegralValue T>
		friend RationalNumber<T> operator/(T left, RationalNumber<T>& right);

		// Input & Output--------------------------------------
		template<IntegralValue T>
		friend std::ostream& operator<<(std::ostream& out, const RationalNumber<T>& number);

		template<IntegralValue T>
		friend std::istream& operator>>(std::istream& in, RationalNumber<T>& number);
	
		// Comparison operators--------------------------------------:
		bool operator>(RationalNumber<T>& other);

		bool operator<(RationalNumber<T>& other);

		bool operator==(RationalNumber<T>& other);

		bool operator!=(RationalNumber<T>& other);

		bool operator!=(const RationalNumber<T>& other) const;

		bool operator>=(RationalNumber<T>& other);

		bool operator<=(RationalNumber<T>& other);

		// Type conversion operators--------------------------------------:
		operator int();

		operator float();

		operator double();

	}; // class RationalNumber
	
	namespace literals {
		RationalNumber<unsigned long long> operator"" _r(unsigned long long value) {
			return RationalNumber<unsigned long long>(value, 1);
		}
	}


	// RationalNumber methods implementations:

	template<IntegralValue T>
	RationalNumber<T>::RationalNumber(T numerator, T denominator) {
		// TODO: simplify or optimize

		if (numerator == denominator) {
			numerator = denominator = 1;
		}
		else {
			int gcd = calculate_gcd<T>(numerator, denominator);
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

	template<IntegralValue T>
	RationalNumber<T>::RationalNumber(long double& other) {
		m_denominator = other.m_denominator;
		m_numerator = other.m_numerator;
	}

	template<IntegralValue T>
	RationalNumber<T>::RationalNumber(RationalNumber<T>& other) {
		m_denominator = other.m_denominator;
		m_numerator = other.m_numerator;
	}


	// Plus--------------------------------------:
	template<IntegralValue T>
	RationalNumber<T> RationalNumber<T>::operator+(RationalNumber<T>& other) {
		T outerMultiplyier = calculate_gcd(m_numerator, other.m_numerator);
		T outerDivisor = calculate_gcd(m_denominator, other.m_denominator);

		T left_denominator = (other.m_denominator / outerDivisor);
		T right_denominator = (m_denominator / outerDivisor);

		T left_numerator = (m_numerator / outerMultiplyier) * left_denominator;
		T right_numerator = (other.m_numerator / outerMultiplyier) * right_denominator;
		T numerator = left_numerator + right_numerator;
		T denominator = left_denominator * right_denominator;

		return RationalNumber<T>(outerMultiplyier * numerator, denominator * outerDivisor);
	}

	template<IntegralValue T>
	RationalNumber<T> operator+(T left, const RationalNumber<T>& right) {
		T numerator = left * right.m_denominator + right.m_numerator;
		return RationalNumber<T>(numerator, right.m_denominator);
	}

	template<IntegralValue T>
	RationalNumber<T> operator+(const RationalNumber<T>& left, T right) {
		return right + left;
	}
	
	template<IntegralValue T>
	RationalNumber<T> RationalNumber<T>::operator+() {
		return RationalNumber<T>(+m_numerator, +m_denominator);
	}
	
	template<IntegralValue T>
	RationalNumber<T>& RationalNumber<T>::operator+=(RationalNumber& other) {
		return *this = *this + other;
	}
	
	template<IntegralValue T>
	RationalNumber<T>& RationalNumber<T>::operator+=(int& other) {
		return *this = *this + other;
	}
	
	template<IntegralValue T>
	RationalNumber<T>& RationalNumber<T>::operator++() {
		m_numerator += m_denominator;
		return *this;
	}
	
	template<IntegralValue T>
	RationalNumber<T> RationalNumber<T>::operator++(int) {
		RationalNumber<T> old_value = *this;
		m_numerator += m_denominator;
		return old_value;
	}

	// Minus--------------------------------------:

	template<IntegralValue T>
	RationalNumber<T> RationalNumber<T>::operator-(RationalNumber<T>& other)
	{
		T numerator = m_numerator * other.m_denominator - other.m_numerator * m_denominator;
		T denominator = m_denominator * other.m_denominator;
		return RationalNumber<T>(numerator, denominator);
	}
	
	template<IntegralValue T>
	RationalNumber<T> operator-(RationalNumber<T>& left, int right) {
		T numerator = left.m_numerator - right * left.m_denominator;
		return RationalNumber<T>(numerator, left.m_denominator);
	}
	
	template<IntegralValue T>
	RationalNumber<T> operator-(int left, RationalNumber<T>& right) {
		T numerator = left * right.m_denominator - right.m_numerator;
		return RationalNumber<T>(numerator, right.m_denominator);
	}
	
	template<IntegralValue T>
	RationalNumber<T> RationalNumber<T>::operator-() {
		return RationalNumber<T>(-m_numerator, m_denominator);
	}
	
	template<IntegralValue T>
	RationalNumber<T>& RationalNumber<T>::operator-=(RationalNumber<T>& other) {
		*this = *this - other;
		return *this;
	}
	
	template<IntegralValue T>
	RationalNumber<T>& RationalNumber<T>::operator-=(int& other) {
		return *this - other;
	}
	
	template<IntegralValue T>
	RationalNumber<T>& RationalNumber<T>::operator--() {
		return *this -= 1;
	}
	
	template<IntegralValue T>
	RationalNumber<T> RationalNumber<T>::operator--(int) {
		RationalNumber<T> old_value = *this;
		m_numerator -= m_denominator;
		return old_value;
	}

	// Multiplying--------------------------------------:
	template<IntegralValue T>
	RationalNumber<T> RationalNumber<T>::operator*(RationalNumber<T>& other) {
		// from left up to right down
		T leftRightGCD = calculate_gcd(m_numerator, other.m_denominator);
		// from right up to left down
		T rightLeftGCD = calculate_gcd(m_denominator, other.m_numerator);

		T left_numerator = m_numerator / leftRightGCD;
		T right_numerator = other.m_numerator / rightLeftGCD;

		T left_denominator = m_denominator / rightLeftGCD;
		T right_denominator = other.m_denominator / leftRightGCD;

		return RationalNumber<T>(
			left_numerator * right_numerator,
			left_denominator * right_denominator
		);
	}

	template<IntegralValue T>
	RationalNumber<T>& RationalNumber<T>::operator*=(RationalNumber<T>& other) {
		m_numerator = m_numerator * other.m_numerator;
		m_denominator = m_denominator * other.m_denominator;
		this->simplify();
		return *this;
	}

	template<IntegralValue T>
	RationalNumber<T> operator*(RationalNumber<T>& left, int right) {
		return RationalNumber<T>(
			left.m_numerator * right,
			left.m_denominator
		);
	}

	template<IntegralValue T>
	RationalNumber<T> operator*(int left, RationalNumber<T>& right) {
		return RationalNumber<T>(
			right.m_numerator * left,
			right.m_denominator
		);
	}

	// Dividing--------------------------------------:
	template<IntegralValue T>
	RationalNumber<T> RationalNumber<T>::operator/(RationalNumber<T>& other) {
		return RationalNumber<T>(
			m_numerator * other.m_denominator,
			m_denominator * other.m_numerator
		);
	}

	template<IntegralValue T>
	RationalNumber<T>& RationalNumber<T>::operator/=(RationalNumber<T>& other) {
		m_numerator = m_numerator * other.m_denominator;
		m_denominator = m_denominator * other.m_numerator;
		this->simplify();
		return *this;
	}

	template<IntegralValue T>
	RationalNumber<T> RationalNumber<T>::operator/(const RationalNumber<T>& other) const {
		return RationalNumber<T>(
			m_numerator * other.m_denominator,
			m_denominator * other.m_numerator
		);
	}


	template<IntegralValue T>
	RationalNumber<T> operator/(RationalNumber<T>& left, T right) {
		return RationalNumber<T>(
			left.m_numerator,
			left.m_denominator * right
		);
	}

	template<IntegralValue T>
	RationalNumber<T> operator/(T left, RationalNumber<T>& right) {
		return RationalNumber<T>(
			right.m_numerator,
			right.m_denominator * left
		);
	}


	// Input & Output--------------------------------------
	template<IntegralValue T>
	std::ostream& operator<<(std::ostream& out, const RationalNumber<T>& number) {
		return out << number.m_numerator << '/' << number.m_denominator;
	}

	template<IntegralValue T>
	std::istream& operator>>(std::istream& in, RationalNumber<T>& number) {
		T a, b;
		in >> a >> b;
		number.m_numerator = a;
		number.m_denominator = a;
		return in;
	}


	// Comparison operators--------------------------------------:
	
	template<IntegralValue T>
	bool RationalNumber<T>::operator>(RationalNumber<T>& other) {
		return (m_numerator * other.m_denominator) > (other.m_numerator * m_denominator);
	}

	template<IntegralValue T>
	bool RationalNumber<T>::operator<(RationalNumber<T>& other) {
		return (m_numerator * other.m_denominator) < (other.m_numerator * m_denominator);
	}

	template<IntegralValue T>
	bool RationalNumber<T>::operator==(RationalNumber<T>& other) {
		return (m_numerator * other.m_denominator) == (other.m_numerator * m_denominator);
	}
	
	template<IntegralValue T>
	bool RationalNumber<T>::operator!=(RationalNumber<T>& other) {
		return (m_numerator * other.m_denominator) != (other.m_numerator * m_denominator);
	}

	template<IntegralValue T>
	bool RationalNumber<T>::operator!=(const RationalNumber<T>& other) const {
		return (m_numerator * other.m_denominator) != (other.m_numerator * m_denominator);
	}

	template<IntegralValue T>
	bool RationalNumber<T>::operator>=(RationalNumber<T>& other) {
		return (m_numerator * other.m_denominator) >= (other.m_numerator * m_denominator);
	}
	
	template<IntegralValue T>
	bool RationalNumber<T>::operator<=(RationalNumber<T>& other) {
		return (m_numerator * other.m_denominator) <= (other.m_numerator * m_denominator);
	}

	// Type conversion operators--------------------------------------:
	template<IntegralValue T>
	RationalNumber<T>::operator int() {
		return m_numerator / m_denominator;
	};

	template<IntegralValue T>
	RationalNumber<T>::operator float() {
		return (float)m_numerator / m_denominator;
	};

	template<IntegralValue T>
	RationalNumber<T>::operator double() {
		return (double)m_numerator / m_denominator;
	};

} // namespace numbers


// To avoid overflow problems, 
// I would think about adding support for integers 
// that can grow as large as needed.
// Also I would consider adding the ability to convert from float, double, 
// and other types to the RationalNumber.
// 
// 
// In some cases, I might consider using an improved RationalNumber class, 
// such as in financial applications, mathematical software, 
// and other scenarios where precise calculations are critically important.
// But in most cases, I would still prefer double or float. 
// These types are widely supported by hardware, 
// making arithmetic operations faster and more efficient. 
// They are also sufficient for many real-world applications,  
// where absolute precision isn't required but performance is critical.
// 
// 


