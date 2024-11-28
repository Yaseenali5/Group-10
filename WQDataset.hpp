#pragma once

#include <vector>
#include <string>
#include "WQSample.hpp"

class WQDataset {
public:
    void loadData(const std::string& filename);

    const WQSample& operator[](size_t index) const { return data.at(index); }
    size_t size() const { return data.size(); }
    
    void clear();
    bool isEmpty() const { return data.empty(); }

private:
    std::vector<WQSample> data;

    void checkDataExists() const;
};