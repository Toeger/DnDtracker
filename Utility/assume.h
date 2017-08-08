#ifndef ASSUME_H
#define ASSUME_H

#include "unreachable.h"

#ifndef _NDEBUG

template <class T>
void assume(const T &t) {
	if (t) {
		return;
	}
	assume_unreachable();
}

#else

#include <cassert>

#define assume assert

#endif //!_NDEBUG

#endif // ASSUME_H