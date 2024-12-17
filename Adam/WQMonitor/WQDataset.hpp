#pragma once

#include <vector>
#include <string>
#include "WQSample.hpp"

using namespace std;

class WQDataset {
public:

    WQDataset(){}
    WQDataset(const string& filename) {loadData(filename);}
    void loadData(const string&);
    int size() const { return data.size(); }
    WQSample operator[](int index) const { return data.at(index); }


private:
    vector<WQSample> data;
    void checkDataExists() const;
};