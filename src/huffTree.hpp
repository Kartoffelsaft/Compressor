#ifndef HUFF_TREE_HPP
#define HUFF_TREE_HPP

#include <deque>
#include <stack>
#include <cstdio>
#include <cstdlib>

#include "huffTree.h"

template<typename T>
HuffTree<T>* HuffTree<T>::init(std::vector<std::tuple<T, unsigned int>> tuples)
{
    std::sort( //sort by occurence
        tuples.begin(),
        tuples.end(),
        [](auto const& a, auto const& b){return std::get<1>(a) < std::get<1>(b);}
    );

    std::deque<std::tuple<HuffTree<T>*, unsigned int>> huffTrees{};

    for(auto tuple: tuples)
    {
        huffTrees.push_back({
            new HuffTree<T>(std::get<0>(tuple)), 
            std::get<1>(tuple)
        });
    }

    while(huffTrees.size() >= 2)
    {
        //connect up the two least weighted trees
        auto a = huffTrees.front();
        huffTrees.pop_front();
        auto b = huffTrees.front();
        huffTrees.pop_front();
        huffTrees.push_front({
            new HuffTree<T>(std::get<0>(a), std::get<0>(b)),
            std::get<1>(a) + std::get<1>(b)
        });

        //sort again so new tree falls in place
        //TODO: optimize to only focus on lower trees
        std::sort(
            huffTrees.begin(),
            huffTrees.end(),
            [](auto const& a, auto const& b){return std::get<1>(a) < std::get<1>(b);}
        );
    }

    return std::get<0>(huffTrees[0]);
}

template<typename T>
HuffTree<T>* HuffTree<T>::deserialize(std::vector<bool>::const_iterator& serial)
{
    //keep track of current path, 
    //so it is easy to go to parent node
    std::vector<HuffTree<T>*> backtrace{};
    auto current = new HuffTree<T>();

    do
    {
        //keep making tree go deeper (prefering left)
        //until a 0 is first hit
        while((bool)*serial)
        {
            backtrace.push_back(current);
            if(!current->a)
            {
                current->a = new HuffTree<T>();
                current = current->a;
            }
            else
            {
                current->b.branch = new HuffTree<T>();
                current = current->b.branch;
            }
            serial++;
        }
        serial++;

        //after hitting a 0, the next
        //sizeof(T)*8 bits must b a T
        T newdata{0};
        for(int i = 0; i < sizeof(T) * 8 - 1; i++)
        {
            newdata += (bool)*serial;
            newdata <<= 1;
            serial++;
        }
        newdata += (bool)*serial;
        serial++;

        //make a child node, prefering left,
        //with the new data
        if(!current->a)
        {current->a = new HuffTree<T>(newdata);}
        else
        {current->b.branch = new HuffTree<T>(newdata);}

        //keep following parent nodes
        //until a child node can be filled
        //or no parent nodes left
        while(current->b.branch && backtrace.size())
        {
            current = backtrace.back();
            backtrace.pop_back();
        }
    //if at the root node with both children,
    //then the tree must be complete
    }while(backtrace.size() || !current->b.branch);

    return current;
}

template<typename T>
HuffTree<T>::~HuffTree()
{
    //delete recursively
    if(a)
    {
        delete a;
        delete b.branch;
    }
}

template<typename T>
std::vector<bool> HuffTree<T>::serialize() const
{
    //call the mutable& version of the function
    std::vector<bool> serial{};
    serialize(serial);
    return serial;
}

template<typename T>
void HuffTree<T>::serialize(std::vector<bool>& serial) const
{
    //if it has children
    //add both of them to the serial
    //with 1's indicating depth
    if(a)
    {
        serial.push_back(a->a);
        a->serialize(serial);
        serial.push_back(b.branch->a);
        b.branch->serialize(serial);
    }
    //ones without children have data
    //so push the data
    else
    {
        T bitmask = 1ULL << (sizeof(T) * 8 - 1);

        while(bitmask)
        {
            serial.push_back((bool)(b.data & bitmask));

            bitmask >>= 1;
        }
    }
}

template<typename T>
std::string genericToString(T t)
{
    //bytewise-ify
    std::string out{};
    for(char* p = (char*)(&t); p < (char*)((&t)+1); p++)
    {out.push_back(*p);}
    return out;
}

//optimization for single byte
template<>
std::string genericToString<unsigned char>(unsigned char t)
{return std::string{(char)t};}

template<typename T>
std::string HuffTree<T>::decompressData(std::vector<bool>::const_iterator& pData, std::vector<bool>::const_iterator const pDataEnd) const
{
    std::string out{};

    while(pData < pDataEnd)
    {
        auto next = genericToString(dereferenceData(pData));
        out.insert(out.end(), next.begin(), next.end());
    }

    return out;
}

template<typename T>
T HuffTree<T>::dereferenceData(std::vector<bool>::const_iterator& path) const
{
    //if there are children, keep recursing
    if(a)
    {
        //0 is left, 1 is right
        return *path?
        b.branch->dereferenceData(++path)
            :
        a->dereferenceData(++path);
    }
    //collapse callstack on leaf node
    else
    {return b.data;}
}

template<typename T>
std::map<T, std::vector<bool>> HuffTree<T>::getReverseMap() const
{
    //call mutable& version of function
    std::map<T, std::vector<bool>> map{};
    std::vector<bool> path{};
    getReverseMap(map, path);

    return map;
}

template<typename T>
void HuffTree<T>::getReverseMap(std::map<T, std::vector<bool>>& map, std::vector<bool> path) const
{
    //maps the data in the leaf nodes 
    //with how to access them
    if(a)
    {
        path.push_back(0);
        a->getReverseMap(map, path);
        path.pop_back();
        path.push_back(1);
        b.branch->getReverseMap(map, path);
    }
    else
    {
        map[b.data] = path;
    }
}

template<typename T>
void HuffTree<T>::print() const
{print(std::vector<bool>{});}

template<typename T>
void HuffTree<T>::print(std::vector<bool> path) const
{
    //much like getReverseMap(), but prints instead of returns
    if(a)
    {
        path.push_back(0);
        a->print(path);
        path.pop_back();
        path.push_back(1);
        b.branch->print(path);
    }
    else
    {
        for(auto b: path)
        {printf("%c", b? 'r' : 'l');}
        printf("->%u\n", b.data);
        path.pop_back();
    }
}

template<typename T>
bool HuffTree<T>::operator==(HuffTree<T> const & other) const
{
    //tests if both trees serialize to the same thing
    //TODO: replace with tree scan
    auto as = this->serialize();
    auto bs = other.serialize();

    return std::equal(as.begin(), as.end(), bs.begin());
}

#endif
