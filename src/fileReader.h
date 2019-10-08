#pragma once

#include <string>
#include <vector>

std::string fileToString(char const * const dir);
std::vector<bool> stringToVecBool(std::string::const_iterator begin, std::string::const_iterator const end);
void printVecBool(std::vector<bool>::const_iterator begin, std::vector<bool>::const_iterator const end);
