#ifndef FILE_OUTPUT_HPP
#define FILE_OUTPUT_HPP

#include "./fileOutput.h"

template<typename T>
std::vector<bool> mapString(char const * const str, size_t const strlen, std::map<T, std::vector<bool>> map)
{
    std::vector<bool> out{};
    out.reserve(strlen); //it won't be longer than the input string. If it is, then what was the point of this whole endevour?

    for(T const * p = (T*)str; p < (T*)(str+strlen); p++)
    {
        auto v = map[*p];
        out.insert(out.end(), v.begin(), v.end());
    }

    return out;
}

std::string mapVectorboolToString(std::vector<bool> const vecBool)
{
    std::string out{};
    out.reserve((vecBool.size() - 1)/8 + 2);

    char buffer{0};

    for(size_t i = 0; i < vecBool.size(); i++)
    {
        buffer += (bool)vecBool[i];
        if(!(i%8))
        {out.push_back(buffer);}
        buffer <<= 1;
    }

    buffer <<= 8 - vecBool.size() % 8;
    out.push_back(buffer);

    return out;
}

#endif
