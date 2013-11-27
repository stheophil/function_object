# Better Function Objects in C++

An (experimental) implementation of Scala-like function objects in C++11 that support [currying](http://en.wikipedia.org/wiki/Currying):

	#include "function_object.h"

	int sum_(int i, int j) {
		return i + j;
	}
	auto sum = make_function_object(sum_); // sum is a binary function

	int main() {
		// sum can be called like any binary function:
		std::cout << " 5 + 4 = " << sum(5, 4) << "\n";

		// if you supply only one argument, sum turns into a unary function:
		auto increment = sum(1);
		std::cout << " increment(5) = " << increment(5) << "\n";
	}

# Documentation

See the `function_object` header and check out the examples in `main.cpp`. It isn't much code. If you create function objects out of lambdas or callable objects you have to specify the arity. There is currently no way to derive the function signatur from a callable object in C++. All I can do is check if an object can be called with a given signature, but that is not the same thing. 

It compiles successfully with GCC 4.8.2
It does not compile with Visual Studio 2013 at the moment.

# To Do

Function objects cannot be chained. That is obviously useful. It would allow you to create a function object like this:

	auto unary_func = ternary_function( some_unary_function, 4, 5 ); 

Calling `unary_func(x)` should expand to `ternay_function(some_unary_function(x), 4, 5)`.

# Acknowledgements

The idea came to me while listening to [Eric Niebler's](http://ericniebler.com/) Keynote speech at this year's [Meeting C++ conference](http://meetingcpp.com). He had experimented with function objects and recommended writing them.  We are using them at [think-cell](http://think-cell.com) because they are easier to use in std::bind statements. I wondered if it was possible to extend to support currying. 