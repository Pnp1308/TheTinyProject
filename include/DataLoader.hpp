#ifndef DATALOADER_HPP
#define DATALOADER_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "Vector.hpp"
#include "Matrix.hpp"

struct DataSet {
    Matrix* A;  // Feature matrix (rows × 6)
    Vector* b;  // Target vector (rows × 1)
};

DataSet loadData(const std::string& filename) {
    std::vector<std::vector<double>> features;
    std::vector<double> targets;

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;

        // Skip vendor name and model name
        ss >> token; // vendor
        ss >> token; // model

        std::vector<double> row(6);
        for (int i = 0; i < 6; ++i) {
            ss >> row[i];
        }

        double prp;
        ss >> prp;      // PRP (target)
        ss >> token;    // ERP (ignore)

        features.push_back(row);
        targets.push_back(prp);
    }

    int nRows = features.size();
    int nCols = 6;
    
    Matrix* A = new Matrix(nRows, nCols);
    Vector* b = new Vector(nRows);

    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            (*A)(i + 1, j + 1) = features[i][j]; // 1-based indexing
        }
        (*b)(i + 1) = targets[i];
    }

    return { A, b };
}

#endif
