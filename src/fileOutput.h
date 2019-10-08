#ifndef FILE_OUTPUT_H
#define FILE_OUTPUT_H

#include <string>
#include <map>
#include <vector>

template<typename T>
std::vector<bool> mapString(char const * const str, size_t const strlen, std::map<T, std::vector<bool>> map);

std::string mapVectorboolToString(std::vector<bool> const vecBool);

#include "./fileOutput.hpp"

#endif
