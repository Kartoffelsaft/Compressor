#include "fileReader.h"

#include <fstream>
#include <algorithm>

std::string fileToString(char const * const dir)
{
    //is it copyright infringement for this to be so similar
    //to what I found on stack overflow?

    std::ifstream file{dir};

    return std::string{
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    };
}

std::vector<bool> stringToVecBool(std::string::const_iterator begin, std::string::const_iterator const end)
{
    std::vector<bool> out{};
    out.reserve((end - begin) * 8);

    //for each character append it's binary data to out
    for(; begin != end; begin++)
    {
        //leftmost bit is 1 to be iteratively shifted right
        //TODO: constexpr
        unsigned char bitmask = 1 << (sizeof(char) * 8 - 1);

        for(int i = 0; i < sizeof(char) * 8; i++)
        {
            out.push_back(*begin & bitmask);

            bitmask >>= 1;
        }
    }

    return out;
}

void printVecBool(std::vector<bool>::const_iterator begin, std::vector<bool>::const_iterator const end)
{
    //I got tired of writing this out when testing
    std::for_each(begin, end, [](bool b){
        printf("%c ", b? '1' : '0');        
    });
}
