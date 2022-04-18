//
// Created by dshin on 3/24/22.
//

#pragma once

namespace util {

/*
 * A wrapper around boost::dynamic_bitset that provides 2D-access.
 */
class DynamicBitMatrix2D {

};

class AdjacencyMatrix {
public:
    AdjacencyMatrix(int n_rows, int n_cols);

    ~AdjacencyMatrix();

    void set(int row_index, int, col_index, bool value = true);

private:
    const int n_rows_;
    const int n_cols_;
    bool* mat_;
};

}  // namespace util

#include "AdjacencyMatrixINLINES.cpp"
