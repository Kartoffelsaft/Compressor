#ifndef HUFF_TREE_H
#define HUFF_TREE_H

#include <vector>
#include <tuple>
#include <stack>
#include <map>
#include <iterator>
#include <string>

template<typename T>
class HuffTree
{
public:
    static HuffTree<T>* init(std::vector<std::tuple<T, unsigned int>> tuples);
    static HuffTree<T>* deserialize(std::vector<bool>::const_iterator& serial);
    ~HuffTree();

    std::map<T, std::vector<bool>> getReverseMap() const;
    std::string decompressData(std::vector<bool>::const_iterator& pData, std::vector<bool>::const_iterator const pDataEnd) const;
    std::vector<bool> serialize() const;
    void print() const;

    bool operator==(HuffTree<T> const & other) const;
private:
    HuffTree<T>* a;
    // a is nullptr if b contains data
    union bType {
        T data;
        HuffTree<T>* branch;

        bType() = default;
        bType(T t): data(t){};
        bType(HuffTree<T>* h): branch(h){};
    } b;

    HuffTree(): a(nullptr), b(nullptr) {};
    HuffTree(T data): a(nullptr), b(data) {};
    HuffTree(HuffTree<T>* na, HuffTree<T>* nb): a(na), b(nb) {};

    void getReverseMap(std::map<T, std::vector<bool>>& m, std::vector<bool> path) const;
    T dereferenceData(std::vector<bool>::const_iterator& path) const;
    void serialize(std::vector<bool>& serial) const;
    void print(std::vector<bool> path) const;
};

#include "./huffTree.hpp"

#endif
