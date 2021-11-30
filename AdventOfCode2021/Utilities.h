#pragma once

#include <vector>
#include <algorithm>
#include <iterator>

template <class In, class Out, class Func>
inline void Transform(const In in, Out out, Func func)
{
	std::transform(in.cbegin(), in.cend(), std::back_inserter(out), func);
}