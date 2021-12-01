#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include <string>

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