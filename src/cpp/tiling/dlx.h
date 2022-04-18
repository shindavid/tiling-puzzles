//
// Created by dshin on 3/23/22.
//
#pragma once

#include <map>
#include <utility>
#include <vector>

#include <Eigen/Core>

namespace dlx {

using row_index_t = int;
using col_index_t = int;
using row_index_vec_t = std::vector<row_index_t>;
using BoolMatrix = Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>;
class HeaderNode;

namespace detail {
class TorusBuilder {
public:
    TorusBuilder(const BoolMatrix &adj_matrix);
    HeaderNode* get_root() const { return root_; }

private:
    void populate_node_map();
    void set_root();
    void link_headers();
    void link_horizontally();
    void link_vertically();

    using node_map_t = std::map<std::pair<int, int>, Node*>;

    const BoolMatrix &adj_matrix_;
    node_map_t node_map_;
    const int n_rows_;
    const int n_cols_;
    HeaderNode *root_;
};
}  // namespace detail

struct Node {
    Node(row_index_t ri, col_index_t ci, HeaderNode* hn);
    Node* link_on_right(Node* node);
    Node* link_below(Node* node);
    bool is_header() const;
    bool is_root() const;

    row_index_t row_index;
    col_index_t col_index;
    HeaderNode* top;
    Node* left;
    Node* right;
    Node* above;
    Node* below;
};

struct HeaderNode : public Node {
    HeaderNode(row_index_t ri, col_index_t ci);
    int count = 0;
};

enum ItemType {
    kRow,
    kCol
};

struct WorkItem {
    Node* node;
    ItemType type;
};

class Torus {
public:
    Torus(const BoolMatrix& adj_matrix);
    void remove_row(Node* node);
    void restore_row(Node* node);
    void remove_col(Node* node);
    void restore_col(Node* node);

private:
    HeaderNode* root_;
};

bool computeExactCover(const BoolMatrix& adj_matrix, row_index_vec_t& solution);

}  // namespace dlx

#include "dlxINLINES.cpp"

