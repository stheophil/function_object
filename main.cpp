#include "function_object.h"

#include <iostream>
#include <sstream>

int sum_(int i, int j) {
	return i + j;
}
auto sum = make_function_object(sum_);

struct overloaded_sum_ {
	int operator()(int i, int j) const { return i + j; }
	double operator()(double i, double j) const { return i + j; }
};
auto overloaded_sum = make_function_object<2>(overloaded_sum_());

template<class T>
struct cast_ {
	// Simply returning T does not work. The function is always in the 
	// overload set, regardless of S's type
	// template<class S> T operator()(S&& s) const { return T{std::forward<S>(s)}; }
	template<class S>
	auto operator()(S&& s) const -> decltype( T{std::forward<S>(s)} ) { 
		return T{std::forward<S>(s)}; 
	}
};

template<typename T, typename ...Args>
auto cast(Args&&... args) 
	-> decltype( make_function_object<1>( cast_<T>() )(std::forward<Args>(args)...)) 
{
	return make_function_object<1>( cast_<T>() )(std::forward<Args>(args)...);
}

// Wrapping InternalTemplated_ with a struct template<typename T> Templated_ always requires to construct an object
// of type Templated_ first, before InternalTemplated_::operator() can be called.
// InternalTemplated_ can be a static constexpr member of Templated_ but you cannot call operator() on such
// a constexpr object.

int main() {
	std::cout << " 5 + 4 = " << sum(5, 4) << "\n";
	auto increment = sum(1);
	std::cout << " increment(5) = " << increment(5) << "\n";

	// This test fails:
	// std::cout << OverloadedTwoArgFunction(1)(4.5) << "\n";
	std::cout << " 5 + 4 = " << overloaded_sum(5, 4) << "\n";

	auto plus2 = overloaded_sum(2.0);
	std::cout << " 2 + 4.3 = " << plus2(4.3) << "\n";
	std::cout << " 2 + 2.4 = " << plus2(2.4) << "\n";

	std::cout << " (int)3.5 = " << cast<int>(3.5) << "\n";
	auto to_double = cast<double>();
	std::cout << " ((double)4) / 3 = " << to_double(4)/3 << "\n";
}