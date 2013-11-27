#include <functional>

template<int C> struct constant {};

template<typename Func, int Arity> struct function_object;

template<typename Func, typename ...Args, int Arity>
void make_function_object(constant<Arity>, Func&& fn, Args&&... args);

struct create_bind {};
struct call_functor_by_default : create_bind {};

template<class Func, int Arity>
struct function_object : Func {
	function_object(Func&& fn) : Func(std::forward<Func>(fn)) {}

	template<typename ...Args>
	auto dispatch(call_functor_by_default, Args&&... args) const
		-> decltype(std::declval<Func const>()(std::forward<Args>(args)...))
	{
		return static_cast<Func const&>(*this)(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	auto dispatch(create_bind, Args&&... args) const
		-> decltype(make_function_object(
				constant< Arity - sizeof...(Args) >(), 
				static_cast<Func const&>(*this),
				std::forward<Args>(args)...				 
		)) // disable if decltype is void
	{
		return make_function_object( 
				constant< Arity - sizeof...(Args) >(),
				static_cast<Func const&>(*this),
				std::forward<Args>(args)...				 
		);
	}

	template<typename ...Args>
	auto operator()(Args&&... args) const
		-> decltype(std::declval<function_object<Func, Arity> const>().dispatch(call_functor_by_default(), std::forward<Args>(args)...))
	{
		return dispatch(call_functor_by_default(), std::forward<Args>(args)...);
	}
};

template<int Arity, typename Func>
function_object<Func, Arity> make_function_object(Func&& func) { 
	return function_object<Func, Arity>(std::forward<Func>(func)); 
}

template<typename R, typename ...Args>
function_object<std::function< R(Args...) >, sizeof...(Args)> make_function_object(R (*func)(Args...)) { 
	return function_object<std::function< R(Args...) >, sizeof...(Args)>(std::function< R(Args...) >(func)); 
}

template<typename Func, typename ...Args>
auto make_function_object(constant<1>, Func&& fn, Args&&... args) 
	-> decltype(make_function_object<1>(std::bind(std::forward<Func>(fn), std::forward<Args>(args)..., std::placeholders::_1))) 
{
	return make_function_object<1>(std::bind(std::forward<Func>(fn), std::forward<Args>(args)..., std::placeholders::_1));
}

template<typename Func, typename ...Args>
auto make_function_object(constant<2>, Func&& fn, Args&&... args) 
	-> decltype(make_function_object<2>(std::bind(std::forward<Func>(fn), std::forward<Args>(args)..., std::placeholders::_1, std::placeholders::_2))) 
{
	return make_function_object<2>(std::bind(std::forward<Func>(fn), std::forward<Args>(args)..., std::placeholders::_1, std::placeholders::_2));
}

template<typename Func, typename ...Args>
auto make_function_object(constant<3>, Func&& fn, Args&&... args) 
	-> decltype(make_function_object<3>(std::bind(std::forward<Func>(fn), std::forward<Args>(args)..., std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))) 
{
	return make_function_object<3>(std::bind(std::forward<Func>(fn), std::forward<Args>(args)..., std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
