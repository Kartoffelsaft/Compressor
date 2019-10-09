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
void compress(std::string const file, int const chunkSize, char const * const outputFile = "./test.cmp")
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

    std::ofstream fileStream{outputFile};
    fileStream << outputFileData;

    delete tree;
}

template<typename typeOfSize>
void decompress(std::vector<bool> data, char const * const outputFile = "./test.dcmp")
{
    std::vector<bool>::const_iterator pData = data.begin() + 7;

    auto tree = HuffTree<typeOfSize>::deserialize(pData);
    auto decompressed = tree->decompressData(pData, data.end());

    std::ofstream fileStream{outputFile};
    fileStream << decompressed;

    delete tree;
}

void decompress(std::string const file, char const * const outputFile)
{
    auto str = fileToString(file.c_str());

    auto data = stringToVecBool(str.begin() + 1, str.end());

    switch(str[0])
    {
        default:
        case 1:
            decompress<unsigned char>(data, outputFile);
            break;
        case 2:
            decompress<uint16_t>(data, outputFile);
            break;
        case 3:
            decompress<uint32_t>(data, outputFile);
            break;
        case 4:
            decompress<uint64_t>(data, outputFile);
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
        outputFile,
    } argReadMode{compressFile};

    std::string file{};
    char const * fileOutput{};
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
                case 'o':
                    argReadMode = outputFile;
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
            case outputFile:
                fileOutput = *arg;
                break;
            case none:
                fprintf(stderr, "Warning: argument unused %s", *arg);
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
                compress<unsigned char>(file, chunkSize, fileOutput);
                break;
            case 16:
                compress<uint16_t>(file, chunkSize, fileOutput);
                break;
            case 32:
                compress<uint32_t>(file, chunkSize, fileOutput);
                break;
            case 64:
                compress<uint64_t>(file, chunkSize, fileOutput);
                break;

            default:
                printf("A chunk size of %i is invalid. valid numbers are: 8, 16, 32, 64\n", chunkSize);
                return 2;
        }
    }
    else
    {decompress(file, fileOutput);}

    return 0;
}
