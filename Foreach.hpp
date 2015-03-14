#ifndef CRANK_FOREACH_HPP
#define CRANK_FOREACH_HPP

// Preprocessor trick to allow nested loops
#define CRANK_PP_CAT_IMPL(a, b) a ## b
#define CRANK_PP_CAT(a, b) CRANK_PP_CAT_IMPL(a, b)
#define CRANK_ID(identifier) CRANK_PP_CAT(auroraDetail_, identifier)
#define CRANK_LINE_ID(identifier) CRANK_PP_CAT(CRANK_ID(identifier), __LINE__)

// Macro to emulate C++11 range-based for loop
// Instead of for (decl : rnge) you write FOREACH(decl, range) as in the following example
//
// std::vector<int> v = ...;
// FOREACH(int& i, v)
// {
//     i += 2;
// }
#define FOREACH(declaration, container)                                                                                                             \
    if (bool CRANK_LINE_ID(broken) = false) {} else                                                                                                 \
    for (auto CRANK_LINE_ID(itr) = (container).begin(); CRANK_LINE_ID(itr) != (container).end() && !CRANK_LINE_ID(broken); ++CRANK_LINE_ID(itr))    \
    if (bool CRANK_LINE_ID(passed) = false) {} else                                                                                                 \
    if (CRANK_LINE_ID(broken) = true, false) {} else                                                                                                \
    for (declaration = *CRANK_LINE_ID(itr); !CRANK_LINE_ID(passed); CRANK_LINE_ID(passed) = true, CRANK_LINE_ID(broken) = false)

#endif // CRANK_FOREACH_HPP
