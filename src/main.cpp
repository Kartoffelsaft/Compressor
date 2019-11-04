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
void compress(
    char const * const file, 
    char const * const outputFile = "./test.cmp"
){
    //get file contents
    auto str = fileToString(file);

    //make a tuple that relates data to it's frequency
    auto dataFrequency = mapToTuples(
        countValues<typeOfSize>(str.c_str(), str.length())
    );

    //use the frequencies to make the tree
    auto tree = HuffTree<typeOfSize>::init(dataFrequency);
    //and an easy way to convert from decompressed data to compressed
    auto treeMap = tree->getReverseMap();

    //data to store the tree
    //the rest of the data will be appended to it
    auto compressedFinalData = tree->serialize();
    //data to store the file (compressed)
    auto compressedFileData = mapString(str.c_str(), str.length(), treeMap);

    //append, as stated will happen above
    compressedFinalData.insert(compressedFinalData.end(), compressedFileData.begin(), compressedFileData.end());

    std::string outputFileData{};
    //datum to tell the decompressor the chunk size
    outputFileData.push_back((char)(log2(sizeof(typeOfSize)) - 2));
    //put data into string to prepare it for the file
    auto compressedFinalDataString = mapVectorboolToString(compressedFinalData);
    outputFileData.insert(outputFileData.end(), compressedFinalDataString.begin(), compressedFinalDataString.end());

    std::ofstream fileStream{outputFile};
    fileStream << outputFileData;

    delete tree;
}

void compress(
    char const * const file,
    char const * const fileOutput,
    int const chunkSize
){
    switch(chunkSize)
    {
        case 8:
            compress<unsigned char>(file, fileOutput);
            break;
        case 16:
            compress<uint16_t>(file, fileOutput);
            break;
        case 32:
            compress<uint32_t>(file, fileOutput);
            break;
        case 64:
            compress<uint64_t>(file, fileOutput);
            break;

        default:
            printf("A chunk size of %i is invalid. valid numbers are: 8, 16, 32, 64\n", chunkSize);
    }
}

template<typename typeOfSize>
void decompress(
    std::vector<bool> data, 
    char const * const outputFile = "./test.dcmp"
){
    //get a bit-sized pointer to the data
    //I tried using auto, but it initializes to bit_iterator
    //I have no idea where the + 7 came from by the way
    std::vector<bool>::const_iterator pData = data.begin() + 7;

    //pull the tree out of the file
    //pData is mutated to just after the end of the tree data
    auto tree = HuffTree<typeOfSize>::deserialize(pData);
    //data just after the tree must be the compressed file contents
    auto decompressed = tree->decompressData(pData, data.end());

    //should be self explanatory
    std::ofstream fileStream{outputFile};
    fileStream << decompressed;

    delete tree;
}

void decompress(char const * const file, char const * const outputFile)
{
    auto str = fileToString(file);  //get file contents

    auto data = stringToVecBool(str.begin() + 1, str.end());    //convert file contents to binary string

    //match chunk size to appropriate function and call it
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
    enum { //A one use enum just to help with reading user arguments
        none,
        compressFile,
        compressChunkSize,
        decompressFile,
        outputFile,
    } argReadMode{compressFile};

    std::string file{};         //What file will be (de)compressed
    char const * fileOutput{};  //What the output file will be called
    int chunkSize{8};           //Size of chunks in bits (see README)
    bool decompressing{false};  //Whether the file will be decompressed

    //loop through the arguments
    for(auto arg = argv + 1; arg < argv + argc; arg++)
    {
        //Check for hyphenated arg (like -d or -C)
        //All results should continue to next arg
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

        //Set variables above based on arguments
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

    //Checks whether the user gave a file at all.
    if(file.size() == 0)
    {
        printf("Needs a file to compress or decompress.\n");
        return 1;
    }
    
    //Match chunk size to appropriate function and call it
    if(!decompressing)
    {compress(file.c_str(), fileOutput, chunkSize);}
    else
    {decompress(file.c_str(), fileOutput);}

    return 0;
}
