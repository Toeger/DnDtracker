#ifndef UNREACHABLE_H
#define UNREACHABLE_H

#if defined(__clang__)

#define assume_unreachable __builtin_unreachable

#else

#include <exception>
#include <cassert>

inline void assume_unreachable() {
	assert(false);
	std::terminate();
}

#endif //compilers

#endif // UNREACHABLE_H
