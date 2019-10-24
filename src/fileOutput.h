#ifndef FILE_OUTPUT_H
#define FILE_OUTPUT_H

#include <string>
#include <map>
#include <vector>

//applies the map for each string and returns them appended together
template<typename T>
std::vector<bool> mapString(char const * const str, size_t const strlen, std::map<T, std::vector<bool>> map);

//reverse of stringToVecBool
//TODO: rename
std::string mapVectorboolToString(std::vector<bool> const vecBool);

#include "./fileOutput.hpp"

#endif
