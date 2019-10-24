#pragma once

#include <string>
#include <vector>

//extract file contents
std::string fileToString(char const * const dir);
//bitwise scan through string
std::vector<bool> stringToVecBool(std::string::const_iterator begin, std::string::const_iterator const end);
//print 0's and 1's from vector<bool>'s contents
void printVecBool(std::vector<bool>::const_iterator begin, std::vector<bool>::const_iterator const end);
