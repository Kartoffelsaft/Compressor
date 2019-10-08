#include <stdio.h>
#include <map>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>

#include "./valueCounter.h"
#include "./fileReader.h"
#include "./huffTree.h"
#include "./fileOutput.h"

template<typename typeOfSize>
void compress(std::string const file, int const chunkSize)
{
    auto str = fileToString(file.c_str());

    auto t = mapToTuples(
        countValues<typeOfSize>(str.c_str(), str.length())
    );

    auto tree = HuffTree<typeOfSize>::init(t);
    auto treeMap = tree->getReverseMap();

    auto compressedFinalData = tree->serialize();
    auto compressedFileData = mapString(str.c_str(), str.length(), treeMap);

    compressedFinalData.insert(compressedFinalData.end(), compressedFileData.begin(), compressedFileData.end());

    std::string outputFileData{};
    outputFileData.push_back((char)(log2(chunkSize) - 2));
    auto compressedFinalDataString = mapVectorboolToString(compressedFinalData);
    outputFileData.insert(outputFileData.end(), compressedFinalDataString.begin(), compressedFinalDataString.end());

    std::ofstream fileStream{"./test.cmp"};
    fileStream << outputFileData;

    delete tree;
}

template<typename typeOfSize>
void decompress(std::vector<bool> data)
{
    std::vector<bool>::const_iterator pData = data.begin() + 7;

    auto tree = HuffTree<typeOfSize>::deserialize(pData);
    auto decompressed = tree->decompressData(pData, data.end());

    std::ofstream fileStream{"./test.dcmp"};
    fileStream << decompressed;

    delete tree;
}

void decompress(std::string const file)
{
    auto str = fileToString(file.c_str());

    auto data = stringToVecBool(str.begin() + 1, str.end());

    switch(str[0])
    {
        default:
        case 1:
            decompress<unsigned char>(data);
            break;
        case 2:
            decompress<unsigned short>(data);
            break;
        case 3:
            decompress<unsigned int>(data);
            break;
        case 4:
            decompress<unsigned long>(data);
            break;
    }
}

int main(int argc, char** argv)
{
    enum {
        none,
        compressFile,
        compressChunkSize,
        decompressFile,
    } argReadMode{compressFile};

    std::string file{};
    int chunkSize{8};
    bool decompressing{false};

    for(auto arg = argv + 1; arg < argv + argc; arg++)
    {
        if((*arg)[0] == '-')
        {
            switch((*arg)[1])
            {
                case 'c':
                    argReadMode = compressFile;
                    decompressing = false;
                    continue;
                case 'C':
                    argReadMode = compressChunkSize;
                    continue;
                case 'd':
                    argReadMode = decompressFile;
                    decompressing = true;
                    continue;
            }
        }

        switch(argReadMode)
        {
            case compressFile:
                file = *arg;
                break;
            case compressChunkSize:
                chunkSize = atoi(*arg);
                break;
            case decompressFile:
                file = *arg;
                break;
            default:
                break;
        }
        argReadMode = none;
    }

    if(file.size() == 0)
    {
        printf("Needs a file to compress or decompress.\n");
        return 1;
    }
    
    if(!decompressing)
    {
        switch(chunkSize)
        {
            case 8:
                compress<unsigned char>(file, chunkSize);
                break;
            case 16:
                compress<unsigned short>(file, chunkSize);
                break;
            case 32:
                compress<unsigned int>(file, chunkSize);
                break;
            case 64:
                compress<unsigned long>(file, chunkSize);
                break;

            default:
                printf("A chunk size of %u is invalid. valid numbers are: 8, 16, 32, 64\n", chunkSize);
                return 2;
        }
    }
    else
    {decompress(file);}

    return 0;
}
