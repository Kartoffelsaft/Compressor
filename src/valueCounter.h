#ifndef VALUE_COUNTER
#define VALUE_COUNTER

#include <map>
#include <vector>

//maps how common a given T appears in str
template<typename T>
std::map<T, unsigned int> countValues(void const * const str, size_t const strSize);

//turns a map into a tuple
template<typename K, typename V>
std::vector<std::tuple<K, V>> mapToTuples(std::map<K, V> m);

#include "valueCounter.hpp"

#endif
