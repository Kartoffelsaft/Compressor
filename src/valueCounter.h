#ifndef VALUE_COUNTER
#define VALUE_COUNTER

#include <map>
#include <vector>

template<typename T>
std::map<T, unsigned int> countValues(void const * const str, size_t const strSize);

template<typename K, typename V>
std::vector<std::tuple<K, V>> mapToTuples(std::map<K, V> m);

#include "valueCounter.hpp"

#endif
