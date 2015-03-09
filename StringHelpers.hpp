#ifndef CRANK_STRING_HELPERS
#define CRANK_STRING_HELPERS

#include <sstream>

// std::to_string do not work in MinGW so we need own implementation
template <typename T>
std::string toString(const T& value);

#include "StringHelpers.inl"
#endif // CRANK_STRING_HELPERS
