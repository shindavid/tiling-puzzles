//
// Created by dshin on 3/24/22.
//

#include "AdjacencyMatrix.h"

namespace util {

inline AdjacencyMatrix::AdjacencyMatrix(int n_rows, int n_cols)
: n_rows_(n_rows)
, n_cols_(n_cols)
{
    mat_ = new bool[n_rows*n_cols];
}

inline AdjacencyMatrix::~AdjacencyMatrix() {
    delete[] mat_;
}

inline void AdjacencyMatrix::set(int row_index, int, col_index, bool value = true);

}  // namespace util
