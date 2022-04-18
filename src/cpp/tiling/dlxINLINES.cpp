//
// Created by dshin on 3/23/22.
//

#include "dlx.h"

namespace dlx {

namespace detail {
inline TorusBuilder::TorusBuilder(const BoolMatrix &adj_matrix)
        : adj_matrix_(adj_matrix), n_rows_(adj_matrix.rows()), n_cols_(adj_matrix.cols()) {
    populate_node_map();
    set_root();
    link_headers();
    link_horizontally();
    link_vertically();
}

inline void TorusBuilder::populate_node_map() {
    for (col_index_t c = -1; c < n_cols_; ++c) {
        node_map_[std::make_pair(-1, c)] = new HeaderNode(-1, c);
    }

    for (row_index_t r = 0; r < n_rows_; ++r) {
        for (col_index_t c = 0; c < n_cols_; ++c) {
            if (!adj_matrix_(r, c)) continue;
            HeaderNode *header = static_cast<HeaderNode *>(node_map_.at(std::map_pair(-1, c)));
            node_map_[std::make_pair(r, c)] = new Node(r, c, header);
        }
    }
}

inline void TorusBuilder::set_root() {
    root_ = static_cast<HeaderNode *>(node_map_.at(std::make_pair(-1, -1)));
}

inline void TorusBuilder::link_headers() {
    Node *node = root_;
    for (col_index_t c = 0; c < n_cols_; ++c) {
        node = node->link_on_right(node_map_.at(std::make_pair(-1, c)));
    }
    node->link_on_right(root_);
}

inline void TorusBuilder::link_horizontally() {
    for (row_index_t r = 0; r < n_rows_; ++r) {
        Node *node0 = nullptr
        Node *node = nullptr;
        for (col_index_t c = 0; c < n_cols_; ++c) {
            if (!adj_matrix_(r, c)) continue;
            if (!node) {
                node0 = node_map_.at(std::make_pair(r, c));
                node = node0;
            } else {
                node = node->link_on_right(node_map_.at(std::make_pair(r, c)));
            }
        }
        if (node) {
            node->link_below(node0);
        }
    }
}

inline void TorusBuilder::link_vertically() {
    for (col_index_t c = 0; c < n_cols_; ++c) {
        HeaderNode *node0 = static_cast<HeaderNode *>(node_map_.at(-1, c));
        Node *node = node0;
        for (row_index_t r = 0; r < n_rows_; ++r) {
            if (!adj_matrix_(r, c)) continue;
            node = node->link_below(node_map_.at(std::make_pair(r, c)));
            node0->count++;
        }
        node->link_below(node0);
    }
}
}  // namespace detail

inline Node::Node(row_index_t ri, col_index_t ci, HeaderNode* hn)
        : row_index(ri)
        , col_index(ci)
        , top(hn)
        , left(this)
        , right(this)
        , above(this)
        , below(this) {}

inline Node* Node::link_on_right(Node* node) {
    this->right = node;
    node->left = this;
    return node;
}

inline Node* Node::link_below(Node* node) {
    this->below = node;
    node->above = this;
    return node;
}

inline bool Node::is_header() const {
    return row_index==-1;
}

inline bool Node::is_root() const {
    return col_index==-1;
}

inline HeaderNode::HeaderNode(row_index_t ri, col_index_t ci) : Node(ri, ci, self) {}

inline Torus::Torus(const BoolMatrix& adj_matrix) {
    detail::TorusBuilder builder(adj_matrix);
    root_ = builder.get_root();
}

inline void Torus::remove_row(Node* node) {
    if (node->is_header()) return;
    Node* node2 = node;
    do {
        node2->above->link_below(node2->below);
        node2->top->count--;
        node2 = node2->right;
    } while (node2 != node);
}

inline void Torus::restore_row(Node* node) {
    Node* node2 = node;
    do {
        node2->above->link_below(node2);
        node2->link_below(node2->below);
        node2->top->count++;
        node2 = node2->right;
    } while (node2 != node);
}

inline void Torus::remove_col(Node* node) {
    if (node->is_root()) return;
    Node* node2 = node;
    do {
        node2->left->link_on_right(node2->right);
        node2 = node2->above;
    } while (node2 != node);
}

inline void Torus::restore_col(Node* node) {
    Node* node2 = node;
    do {
        node2->left->link_on_right(node2);
        node2->link_on_right(node2->right);
        node2 = node2->above;
    } while (node2 != node);
}

inline bool computeExactCover(const BoolMatrix& adj_matrix, row_index_vec_t& solution) {
    H_Stack history;
    std::vector<int> solution;
    solution.reserve(M.number_of_rows());
    DLX(M, solution, history);
    solution.shrink_to_fit();
    return solution;
}

}  // namespace dlx
