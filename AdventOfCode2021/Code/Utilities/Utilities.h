#pragma once

#include "Types.h"
#include "FullParsers.h"
#include "PartialParsers.h"

#include <algorithm>
#include <iterator>

template <class In, class Out, class Func>
inline void Transform(const In in, Out out, Func func)
{
	std::transform(in.cbegin(), in.cend(), std::back_inserter(out), func);
}

inline int ToInt(const std::string& str)
{
	return atoi(str.c_str());
}

inline int ToInt(const char* str)
{
	return atoi(str);
}
