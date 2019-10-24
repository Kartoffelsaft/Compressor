#pragma once

#include <map>
#include <type_traits>
#include <vector>

template<typename T>
std::map<T, unsigned int> countValues(void const * const str, size_t const strSize)
{
    std::map<T, unsigned int> mapCount;
    for(
        T const * p = (T*)str;
        p < (T*)((char*)str+strSize);   //too many casts. basically currentIterator < endIteratorB
        p++                             //jump foward sizeof(T)
    )
    {mapCount[*p]++;}

    return mapCount;
}

template<typename K, typename V>
std::vector<std::tuple<K, V>> mapToTuples(std::map<K, V> m)
{
    std::vector<std::tuple<K, V>> tuples;
    tuples.reserve(m.size());

    for(auto t : m)
    {tuples.push_back(t);}

    return tuples;
}

