#ifndef OVERLOADED_FUNCTION_H
#define OVERLOADED_FUNCTION_H

#include <type_traits>
#include <utility>

template <class... Functions>
struct Overloaded_function : Functions... {
	template <class... T>
	Overloaded_function(T &&... t)
		: Functions(std::forward<T>(t))... {}
	using Functions::operator()...; //this actually requires C++17 but compiles in clang's -std=c++14 anyways
};

#if __cplusplus > 201700
template <class... Functions>
Overloaded_function(Functions...)->Overloaded_function<std::decay_t<Functions>...>;
#else
template <class... Functions>
auto make_overloaded_function(Functions &&... functions) {
	return Overloaded_function<std::decay_t<Functions>...>(std::forward<Functions>(functions)...);
}
#endif

#endif // OVERLOADED_FUNCTION_H