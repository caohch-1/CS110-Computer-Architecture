using namespace __detail;
using namespace std;

// return data
template<typename T>
typename list_row_elem_iter<T>::reference list_row_elem_iter<T>::operator*() {
    return inner->data;
}

// return data address
template<typename T>
typename list_row_elem_iter<T>::pointer list_row_elem_iter<T>::operator->() {
    return &(inner->data);
}

// post increment
template<typename T>
list_row_elem_iter<T> &list_row_elem_iter<T>::operator++() {
    inner = inner->right;
    return *this;
}

// pre increment
template<typename T>
list_row_elem_iter<T> list_row_elem_iter<T>::operator++(int) {
    list_row_elem_iter<T> ret_val(this->inner);
    ++(*this);
    return ret_val;
}

// post decrement
template<typename T>
list_row_elem_iter<T> &list_row_elem_iter<T>::operator--() {
    inner = inner->left;
    return *this;
}

// pre decrement
template<typename T>
list_row_elem_iter<T> list_row_elem_iter<T>::operator--(int) {
    list_row_elem_iter<T> ret_val(this->inner);
    --(*this);
    return ret_val;
}

// judge equal by inner
template<typename T>
bool list_row_elem_iter<T>::operator==(const list_row_elem_iter &other) {
    return inner == other.inner;
}

// judge unequal by inner
template<typename T>
bool list_row_elem_iter<T>::operator!=(const list_row_elem_iter &other) {
    return inner != other.inner;
}

// return data
template<typename T>
typename list_col_elem_iter<T>::reference list_col_elem_iter<T>::operator*() {
    return inner->data;
}

// return data address
template<typename T>
typename list_col_elem_iter<T>::pointer list_col_elem_iter<T>::operator->() {
    return &(inner->data);
}

// post increment
template<typename T>
list_col_elem_iter<T> &list_col_elem_iter<T>::operator++() {
    inner = inner->down;
    return *this;
}

// pre increment
template<typename T>
list_col_elem_iter<T> list_col_elem_iter<T>::operator++(int) {
    list_col_elem_iter<T> ret_val(*this);
    ++(*this);
    return ret_val;
}

// post decrement
template<typename T>
list_col_elem_iter<T> &list_col_elem_iter<T>::operator--() {
    inner = inner->up;
    return *this;
}

// pre decrement
template<typename T>
list_col_elem_iter<T> list_col_elem_iter<T>::operator--(int) {
    list_col_elem_iter<T> ret_val(*this);
    --(*this);
    return ret_val;
}

// judge equal by inner
template<typename T>
bool list_col_elem_iter<T>::operator==(const list_col_elem_iter &other) {
    return inner == other.inner;
}

// judge unequal by inner
template<typename T>
bool list_col_elem_iter<T>::operator!=(const list_col_elem_iter &other) {
    return inner != other.inner;
}

// get iter of leftmost 
template<typename T>
typename list_row_iter<T>::iterator list_row_iter<T>::begin() {
    return iterator(left);
}

// get iter of rightmost->right
template<typename T>
typename list_row_iter<T>::iterator list_row_iter<T>::end() {
    return iterator((right == nullptr) ? nullptr : right->right);
}

// get riter of rightmost
template<typename T>
typename list_row_iter<T>::reverse_iterator list_row_iter<T>::rbegin() {
    iterator rightmost(right);
    return reverse_iterator(rightmost);
}

// get riter of leftmost->left
template<typename T>
typename list_row_iter<T>::reverse_iterator list_row_iter<T>::rend() {
    iterator before_leftmost(((left == nullptr) ? nullptr : left->left));
    return reverse_iterator(before_leftmost);
}

// get obj 
template<typename T>
typename list_row_iter<T>::reference list_row_iter<T>::operator*() {
    return *this;
}

// get obj_ptr
template<typename T>
typename list_row_iter<T>::pointer list_row_iter<T>::operator->() {
    return this;
}

// post increment
template<typename T>
list_row_iter<T> &list_row_iter<T>::operator++() {
    this->left = (this->left->down == nullptr) ? nullptr : this->left->down;
    this->right = (this->right->down == nullptr) ? nullptr : this->right->down;
    return *this;
}

// pre increment
template<typename T>
list_row_iter<T> list_row_iter<T>::operator++(int) {
    list_row_iter<T> ret_val(this->left, this->right);
    ++(*this);
    return ret_val;
}

// post decrement
template<typename T>
list_row_iter<T> &list_row_iter<T>::operator--() {
    this->left = (this->left->up == nullptr) ? nullptr : this->left->up;
    this->right = (this->right->up == nullptr) ? nullptr : this->right->up;
    return *this;
}

// pre decrement
template<typename T>
list_row_iter<T> list_row_iter<T>::operator--(int) {
    list_row_iter<T> ret_val(this->left, this->right);
    --(*this);
    return ret_val;
}

// judge equal by both left and right
template<typename T>
bool list_row_iter<T>::operator==(const list_row_iter &other) {
    return (this->left == other.left) && (this->right == other.right);
}

// judge unequal by both left and right
template<typename T>
bool list_row_iter<T>::operator!=(const list_row_iter &other) {
    return (this->left != other.left) || (this->right != other.right);
}

// get iter of top
template<typename T>
typename list_col_iter<T>::iterator list_col_iter<T>::begin() {
    return iterator(up);
}

// get iter of bottom->down
template<typename T>
typename list_col_iter<T>::iterator list_col_iter<T>::end() {
    return iterator(((down == nullptr) ? nullptr : down->down));
}

// get riter of bottom
template<typename T>
typename list_col_iter<T>::reverse_iterator list_col_iter<T>::rbegin() {
    iterator bottom(down);
    return reverse_iterator(bottom);
}

// get riter of top->up
template<typename T>
typename list_col_iter<T>::reverse_iterator list_col_iter<T>::rend() {
    iterator above_top(((up == nullptr) ? nullptr : up->up));
    return reverse_iterator(above_top);
}

// get obj 
template<typename T>
typename list_col_iter<T>::reference list_col_iter<T>::operator*() {
    return *this;
}

// get obj_ptr
template<typename T>
typename list_col_iter<T>::pointer list_col_iter<T>::operator->() {
    return this;
}

// post increment
template<typename T>
list_col_iter<T> &list_col_iter<T>::operator++() {
    this->up = (this->up->right == nullptr) ? nullptr : this->up->right;
    this->down = (this->down->right == nullptr) ? nullptr : this->down->right;
    return *this;
}

// pre increment
template<typename T>
list_col_iter<T> list_col_iter<T>::operator++(int) {
    list_col_iter<T> ret_val(this->up, this->down);
    ++(*this);
    return ret_val;
}

// post decrement
template<typename T>
list_col_iter<T> &list_col_iter<T>::operator--() {
    this->up = (this->up->left == nullptr) ? nullptr : this->up->left;
    this->down = (this->down->left == nullptr) ? nullptr : this->down->left;
    return *this;
}

// pre decrement
template<typename T>
list_col_iter<T> list_col_iter<T>::operator--(int) {
    list_col_iter<T> ret_val(this->up, this->down);
    --(*this);
    return ret_val;
}

// judge equal by both up and down
template<typename T>
bool list_col_iter<T>::operator==(const list_col_iter &other) {
    return (this->up == other.up) && (this->down == other.down);
}

// judge unequal by both up and down
template<typename T>
bool list_col_iter<T>::operator!=(const list_col_iter &other) {
    return (this->up != other.up) && (this->down != other.down);
}

// get iter of top row
template<typename T>
typename doubll2d<T>::row_iter doubll2d<T>::row_begin() {
    return row_iter(up_left, up_right);
}

// get iter of bottom row->down 
template<typename T>
typename doubll2d<T>::row_iter doubll2d<T>::row_end() {
    list_node<T> *l = (down_left == nullptr) ? nullptr : down_left->down;
    list_node<T> *r = (down_right == nullptr) ? nullptr : down_right->down;
    return row_iter(l, r);
}

// get riter of bottom row
template<typename T>
typename doubll2d<T>::row_riter doubll2d<T>::row_rbegin() {
    row_iter row_bottom(down_left, down_right);
    return row_riter(row_bottom);
}

// get riter of top row->up
template<typename T>
typename doubll2d<T>::row_riter doubll2d<T>::row_rend() {
    list_node<T> *l = (up_left == nullptr) ? nullptr : up_left->up;
    list_node<T> *r = (up_right == nullptr) ? nullptr : up_right->up;
    row_iter row_above_top(l, r);
    return row_riter(row_above_top);
}

// get iter of leftmost col
template<typename T>
typename doubll2d<T>::col_iter doubll2d<T>::col_begin() {
    return col_iter(up_left, down_left);
}

// get iter of rightmost col->right 
template<typename T>
typename doubll2d<T>::col_iter doubll2d<T>::col_end() {
    list_node<T> *u = (up_right == nullptr) ? nullptr : up_right->right;
    list_node<T> *d = (down_right == nullptr) ? nullptr : down_right->right;
    return col_iter(u, d);
}

// get riter of rightmost col
template<typename T>
typename doubll2d<T>::col_riter doubll2d<T>::col_rbegin() {
    col_iter col_rightmost(up_right, down_right);
    return col_riter(col_rightmost);
}

// get riter of leftmost col->left
template<typename T>
typename doubll2d<T>::col_riter doubll2d<T>::col_rend() {
    list_node<T> *u = (up_left == nullptr) ? nullptr : up_left->left;
    list_node<T> *d = (down_left == nullptr) ? nullptr : down_left->left;
    col_iter col_leftmost(u, d);
    return col_riter(col_leftmost);
}


template<typename T>
template<typename input_iter>
typename doubll2d<T>::row_iter doubll2d<T>::insert_row(doubll2d<T>::row_iter cursor, input_iter begin, input_iter end) {
    // calculate input data length 
    size_t len = 0;
    input_iter temp_input = begin;
    while (temp_input != end) {
        len++;
        temp_input++;
    }
    temp_input = begin;

    // case1:
    // begin equals to end and empty
    // case2:
    // cusor is null iter and not empty
    if ((this->dim_col == 0 && this->dim_row == 0 && begin == end)
        || (cursor == row_iter(nullptr, nullptr) && this->dim_col != 0 && this->dim_row != 0))
        return this->row_end();

    // create new row
    // copy the input data
    // if len < dim padding with default value
    // if len > dim only copy len values
    // also need to connect each other
    size_t new_len = (this->dim_col == 0) ? len : this->dim_col;
    list_node<T> *new_nodes[new_len];
    T default_value = T();
    for (size_t i = 0; i < new_len; i++) {
        if (i < len) {
            new_nodes[i] = new list_node<T>(*temp_input);
            temp_input++;
        } else {
            new_nodes[i] = new list_node<T>(default_value);
        }
    }
    for (size_t i = 0; i < new_len; i++) {
        new_nodes[i]->left = (i != 0) ? (new_nodes[i - 1]) : nullptr;
        new_nodes[i]->right = (i + 1 <= new_len - 1) ? (new_nodes[i + 1]) : nullptr;
    }

    // case3: empty list
    if (this->dim_col == 0 && this->dim_row == 0) {
        // create padding row1 
        // it is down to the bottom
        // with default value
        // also need to connect with each other
        list_node<T> *padding_nodes1[new_len];
        for (size_t i = 0; i < new_len; i++) {
            padding_nodes1[i] = new list_node<T>(default_value);
        }
        for (size_t i = 0; i < new_len; i++) {
            padding_nodes1[i]->left = (i != 0) ? (padding_nodes1[i - 1]) : nullptr;
            padding_nodes1[i]->right = (i + 1 <= new_len - 1) ? (padding_nodes1[i + 1]) : nullptr;
        }
        // create padding row2 
        // it is up to the top
        // with default value
        // also need to connect with each other
        list_node<T> *padding_nodes2[new_len];
        for (size_t i = 0; i < new_len; i++) {
            padding_nodes2[i] = new list_node<T>(default_value);
        }
        for (size_t i = 0; i < new_len; i++) {
            padding_nodes2[i]->left = (i != 0) ? (padding_nodes2[i - 1]) : nullptr;
            padding_nodes2[i]->right = (i + 1 <= new_len - 1) ? (padding_nodes2[i + 1]) : nullptr;
        }
        // create padding node1
        // it is right to the rightmost new node 
        auto *padding_node1 = new list_node<T>(default_value);
        // create padding node2
        // it is left to the leftmost new node 
        auto *padding_node2 = new list_node<T>(default_value);
        // insert the whole data from begin to end
        // reset dim
        // connect padding rows with new nodes
        // connect 2 padding node with 2 side new nodes
        this->dim_col = new_len;
        this->dim_row = 1;
        for (size_t i = 0; i < this->dim_col; i++) {
            new_nodes[i]->up = padding_nodes2[i];
            new_nodes[i]->down = padding_nodes1[i];
            padding_nodes1[i]->up = new_nodes[i];
            padding_nodes2[i]->down = new_nodes[i];
        }
        new_nodes[new_len - 1]->right = padding_node1;
        new_nodes[0]->left = padding_node2;
        padding_node1->left = new_nodes[new_len - 1];
        padding_node2->right = new_nodes[0];
        //reset four corner
        this->up_left = new_nodes[0];
        this->down_left = new_nodes[0];
        this->up_right = new_nodes[this->dim_col - 1];
        this->down_right = new_nodes[this->dim_col - 1];
        // return iter of inserted row
        return row_iter(up_left, up_right);
    }

    // create padding node1
    // it is right to the rightmost new node 
    auto *padding_node1 = new list_node<T>(default_value);
    // create padding node2
    // it is left to the leftmost new node 
    auto *padding_node2 = new list_node<T>(default_value);
    // case4: cursor equals to row_end()
    if (cursor == this->row_end()) {
        // reset cursor to top row
        cursor = this->row_begin();
        // connect padding node1 with rightmost new node
        new_nodes[this->dim_col - 1]->right = padding_node1;
        padding_node1->left = new_nodes[this->dim_col - 1];
        padding_node1->down = cursor.end().inner;
        cursor.end().inner->up = padding_node1;
        // connect padding node2 with leftmost new node
        new_nodes[0]->left = padding_node2;
        padding_node2->right = new_nodes[0];
        padding_node2->down = cursor.begin().inner->left;
        cursor.begin().inner->left->up = padding_node2;
        // reset connection relationship
        // don't forget the existance of top padding row
        list_node<T> *temp_this = this->row_begin().begin().inner;
        for (size_t i = 0; i < this->dim_col; i++) {
            new_nodes[i]->up = temp_this->up;
            new_nodes[i]->down = temp_this;
            temp_this->up->down = new_nodes[i];
            temp_this->up = new_nodes[i];
            // forward
            temp_this = temp_this->right;
        }
        // reset dim
        // reset up corners
        this->dim_row++;
        this->up_left = new_nodes[0];
        this->up_right = new_nodes[this->dim_col - 1];
        // return iter of inserted row
        return row_iter(up_left, up_right);
    }

    // case5: become bottom row
    if (cursor.begin().inner == this->down_left) {
        // create padding node1
        // it is right to the rightmost new node 
        // connect padding node1 with rightmost new node
        new_nodes[this->dim_col - 1]->right = padding_node1;
        padding_node1->left = new_nodes[this->dim_col - 1];
        padding_node1->up = cursor.end().inner;
        cursor.end().inner->down = padding_node1;
        // create padding node2
        // it is left to the leftmost new node 
        // connect padding node2 with leftmost new node
        new_nodes[0]->left = padding_node2;
        padding_node2->right = new_nodes[0];
        padding_node2->up = cursor.begin().inner->left;
        cursor.begin().inner->left->down = padding_node2;
        // reset connection relationship
        // don't forget the existance of bottom padding row
        list_node<T> *temp_this = cursor.begin().inner;
        for (size_t i = 0; i < this->dim_col; i++) {
            new_nodes[i]->up = temp_this;
            new_nodes[i]->down = temp_this->down;
            temp_this->down->up = new_nodes[i];
            temp_this->down = new_nodes[i];
            // forward
            temp_this = temp_this->right;
        }
        //reset dim
        //reset bottom corners
        this->dim_row++;
        this->down_left = new_nodes[0];
        this->down_right = new_nodes[this->dim_col - 1];
        // return iter of inserted row
        return row_iter(new_nodes[0], new_nodes[this->dim_col - 1]);
    }

    // case6: common case
    // create padding node
    new_nodes[this->dim_col - 1]->right = padding_node1;
    padding_node1->left = new_nodes[this->dim_col - 1];
    padding_node1->up = cursor.end().inner;
    padding_node1->down = cursor.end().inner->down;
    cursor.end().inner->down->up = padding_node1;
    cursor.end().inner->down = padding_node1;

    new_nodes[0]->left = padding_node2;
    padding_node2->right = new_nodes[0];
    padding_node2->up = cursor.begin().inner;
    padding_node2->down = cursor.begin().inner->left->down;
    cursor.begin().inner->left->down->up = padding_node2;
    cursor.begin().inner->left->down = padding_node2;


    list_node<T> *temp_this = cursor.begin().inner;
    for (size_t i = 0; i < this->dim_col; i++) {
        new_nodes[i]->up = temp_this;
        new_nodes[i]->down = temp_this->down;
        temp_this->down->up = new_nodes[i];
        temp_this->down = new_nodes[i];

        temp_this = temp_this->right;
    }
    this->dim_row++;
    return row_iter(new_nodes[0], new_nodes[this->dim_col - 1]);

}

template<typename T>
template<typename input_iter>
typename doubll2d<T>::col_iter doubll2d<T>::insert_col(list_col_iter<T> cursor, input_iter begin, input_iter end) {
    // calculate input data length 
    size_t len = 0;
    input_iter temp_input = begin;
    while (temp_input != end) {
        len++;
        temp_input++;
    }
    temp_input = begin;

    // case1:
    // begin equals to end and empty
    // case2:
    // cusor is null iter and not empty
    if ((this->dim_col == 0 && this->dim_row == 0 && begin == end)
        || (cursor == col_iter(nullptr, nullptr) && this->dim_col != 0 && this->dim_row != 0))
        return this->col_end();

    // create new col
    // copy the input data
    // if len < dim padding with default value
    // if len > dim only copy len values
    // also need to connect each other
    size_t new_len = (this->dim_row == 0) ? len : this->dim_row;
    list_node<T> *new_nodes[new_len];
    T default_value = T();
    for (size_t i = 0; i < new_len; i++) {
        if (i < len) {
            new_nodes[i] = new list_node<T>(*temp_input);
            temp_input++;
        } else {
            new_nodes[i] = new list_node<T>(default_value);
        }
    }
    for (size_t i = 0; i < new_len; i++) {
        new_nodes[i]->up = (i != 0) ? new_nodes[i - 1] : nullptr;
        new_nodes[i]->down = (i + 1 <= new_len - 1) ? (new_nodes[i + 1]) : nullptr;
    }

    // case3: empty list
    if (this->dim_col == 0 && this->dim_row == 0) {
        // create padding col1 
        // it is right to the rightmost
        // with default value
        // also need to connect with each other
        list_node<T> *padding_nodes1[new_len];
        for (size_t i = 0; i < new_len; i++) {
            padding_nodes1[i] = new list_node<T>(default_value);
        }
        for (size_t i = 0; i < new_len; i++) {
            padding_nodes1[i]->up = (i != 0) ? (padding_nodes1[i - 1]) : nullptr;
            padding_nodes1[i]->down = (i + 1 <= new_len - 1) ? (padding_nodes1[i + 1]) : nullptr;
        }
        // create padding col2 
        // it is left to the leftmost
        // with default value
        // also need to connect with each other
        list_node<T> *padding_nodes2[new_len];
        for (size_t i = 0; i < new_len; i++) {
            padding_nodes2[i] = new list_node<T>(default_value);
        }
        for (size_t i = 0; i < new_len; i++) {
            padding_nodes2[i]->up = (i != 0) ? (padding_nodes2[i - 1]) : nullptr;
            padding_nodes2[i]->down = (i + 1 <= new_len - 1) ? (padding_nodes2[i + 1]) : nullptr;
        }
        // create padding node1
        // it is down to the bottom new node 
        auto *padding_node1 = new list_node<T>(default_value);
        // create padding node2
        // it is up to the top new node 
        auto *padding_node2 = new list_node<T>(default_value);
        // insert the whole data from begin to end
        // reset dim
        // connect padding cols with new nodes
        // connect 2 padding node with 2 side new nodes
        this->dim_row = new_len;
        this->dim_col = 1;
        for (size_t i = 0; i < this->dim_row; i++) {
            new_nodes[i]->left = padding_nodes2[i];
            new_nodes[i]->right = padding_nodes1[i];
            padding_nodes1[i]->left = new_nodes[i];
            padding_nodes2[i]->right = new_nodes[i];
        }
        new_nodes[new_len - 1]->down = padding_node1;
        new_nodes[0]->up = padding_node2;
        padding_node1->up = new_nodes[new_len - 1];
        padding_node2->down = new_nodes[0];
        //reset four corner
        this->up_left = new_nodes[0];
        this->down_left = new_nodes[this->dim_row - 1];
        this->up_right = new_nodes[0];
        this->down_right = new_nodes[this->dim_row - 1];
        // return iter of inserted col
        return col_iter(up_left, down_left);
    }

    // create padding node1
    // it is up to the top new node 
    auto *padding_node1 = new list_node<T>(default_value);
    // create padding node2
    // it is up to the top new node 
    auto *padding_node2 = new list_node<T>(default_value);
    // case4: cursor equals to col_end()
    if (cursor == this->col_end()) {
        // reset cursor to leftmost col
        cursor = this->col_begin();
        // connect padding node1 with bottom new node
        new_nodes[this->dim_row - 1]->down = padding_node1;
        padding_node1->up = new_nodes[this->dim_row - 1];
        padding_node1->right = cursor.end().inner;
        cursor.end().inner->left = padding_node1;
        // connect padding node1 with top new node
        new_nodes[0]->up = padding_node2;
        padding_node2->down = new_nodes[0];
        padding_node2->right = cursor.begin().inner->up;
        cursor.begin().inner->up->left = padding_node2;
        // reset connection relationship
        // don't forget the existance of leftmost padding col
        list_node<T> *temp_this = this->col_begin().begin().inner;
        for (size_t i = 0; i < this->dim_row; i++) {
            new_nodes[i]->left = temp_this->left;
            new_nodes[i]->right = temp_this;
            temp_this->left->right = new_nodes[i];
            temp_this->left = new_nodes[i];
            // forward
            temp_this = temp_this->down;
        }
        //reset dim
        //reset left corners
        this->dim_col++;
        this->up_left = new_nodes[0];
        this->down_left = new_nodes[this->dim_row - 1];
        // return iter of inserted col
        return col_iter(up_left, down_left);
    }

    // case5: become rightmost col
    if (cursor.begin().inner == this->up_right) {
        // create padding node1
        // it is up to the top new node 
        // connect padding node1 with bottom new node
        new_nodes[this->dim_row - 1]->down = padding_node1;
        padding_node1->up = new_nodes[this->dim_row - 1];
        padding_node1->left = cursor.end().inner;
        cursor.end().inner->right = padding_node1;
        // create padding node2
        // it is up to the top new node 
        // connect padding node1 with top new node
        new_nodes[0]->up = padding_node2;
        padding_node2->down = new_nodes[0];
        padding_node2->left = cursor.begin().inner->up;
        cursor.begin().inner->up->right = padding_node2;
        // reset connection relationship
        // don't forget the existance of rightmost padding col
        list_node<T> *temp_this = cursor.begin().inner;
        for (size_t i = 0; i < this->dim_row; i++) {
            new_nodes[i]->left = temp_this;
            new_nodes[i]->right = temp_this->right;
            temp_this->right->left = new_nodes[i];
            temp_this->right = new_nodes[i];
            // forward
            temp_this = temp_this->down;
        }
        //reset dim
        //reset right corners
        this->dim_col++;
        this->up_right = new_nodes[0];
        this->down_right = new_nodes[this->dim_row - 1];
        // return iter of inserted col
        return col_iter(new_nodes[0], new_nodes[this->dim_row - 1]);
    }

    // case6: common case
    // create padding node
    new_nodes[this->dim_row - 1]->down = padding_node1;
    padding_node1->up = new_nodes[this->dim_row - 1];
    padding_node1->left = cursor.end().inner;
    padding_node1->right = cursor.end().inner->right;
    cursor.end().inner->right->left = padding_node1;
    cursor.end().inner->right = padding_node1;

//    auto *padding_node2 = new list_node<T>(default_value);
    new_nodes[0]->up = padding_node2;
    padding_node2->down = new_nodes[0];
    padding_node2->left = cursor.begin().inner;
    padding_node2->right = cursor.begin().inner->up->right;
    cursor.begin().inner->up->right->left = padding_node2;
    cursor.begin().inner->up->right = padding_node2;

    list_node<T> *temp_this = cursor.begin().inner;
    for (size_t i = 0; i < this->dim_row; i++) {
        new_nodes[i]->left = temp_this;
        new_nodes[i]->right = temp_this->right;
        temp_this->right->left = new_nodes[i];
        temp_this->right = new_nodes[i];

        temp_this = temp_this->down;
    }
    this->dim_col++;
    return col_iter(new_nodes[0], new_nodes[this->dim_row - 1]);
}

template<typename T>
typename doubll2d<T>::row_iter doubll2d<T>::delete_row(row_iter cursor) {
    /*cursor is row_end()*/
    if (cursor == this->row_end()) return this->row_end();
    
    /*only row*/
    if (dim_row == 1) {
//        cout << "delete row only "<<i<<endl;
        // free
        list_node<T> *temp1 = cursor.begin().inner;
        list_node<T> *temp2;

        for (size_t i = 0; i < this->dim_col; i++) {
            temp2 = temp1->right;
            delete temp1->up;
            delete temp1->down;
            if (i == dim_col - 1) delete temp1->right;
            if (i == 0) delete temp1->left;
            delete temp1;
            temp1 = temp2;
        }


        // reset
        this->dim_row = 0;
        this->dim_col = 0;
        this->up_left = nullptr;
        this->up_right = nullptr;
        this->down_left = nullptr;
        this->down_right = nullptr;
        return row_iter(up_left, up_right);
    }

    /*first row*/
    if (cursor.begin().inner == this->up_left) {
//        cout << "delete row top\n";
        list_node<T> *temp = cursor.begin().inner;
        list_node<T> *ret_val_head = cursor.begin().inner->down;
        list_node<T> *ret_val_tail;
        for (size_t i = 0; i < this->dim_col; i++) {
            if (i == this->dim_col - 1) ret_val_tail = temp->down;
            temp->down->up = temp->up;
            temp->up->down = temp->down;
            temp = temp->right;
        }

        // free
        list_node<T> *temp1 = cursor.begin().inner;
        list_node<T> *temp2;
        for (size_t i = 0; i < this->dim_col; i++) {
            temp2 = temp1->right;
            if (i == dim_col - 1) delete temp1->right;
            if (i == 0) delete temp1->left;
            delete temp1;
            temp1 = temp2;
        }

        this->dim_row--;
        this->up_left = ret_val_head;
        this->up_right = ret_val_tail;
        return row_iter(ret_val_head, ret_val_tail);
    }

    /*bottom row*/
    if (cursor.begin().inner == this->down_left) {
//        cout << "delete row bottom\n";
        list_node<T> *temp = cursor.begin().inner;
        list_node<T> *ret_val_head = cursor.begin().inner->up;
        list_node<T> *ret_val_tail;
        for (size_t i = 0; i < this->dim_col; i++) {
            if (i == this->dim_col - 1) ret_val_tail = temp->up;
            temp->up->down = temp->down;
            temp = temp->right;
        }

        // free
        list_node<T> *temp1 = cursor.begin().inner;
        list_node<T> *temp2;
        for (size_t i = 0; i < this->dim_col; i++) {
            temp2 = temp1->right;
            if (i == 0) delete temp1->left;
            if (i == dim_col - 1) delete temp1->right;
            delete temp1;
            temp1 = temp2;
        }

        this->dim_row--;
        this->down_left = ret_val_head;
        this->down_right = ret_val_tail;
        return row_iter(ret_val_head, ret_val_tail);
    }

    /*common case*/
//    cout << "delete row common\n";
    list_node<T> *temp = cursor.begin().inner;
    list_node<T> *ret_val_head = cursor.begin().inner->up;
    list_node<T> *ret_val_tail;
    for (size_t i = 0; i < this->dim_col; i++) {
        if (i == this->dim_col - 1) ret_val_tail = temp->up;
        temp->down->up = temp->up;
        temp->up->down = temp->down;
        temp = temp->right;
    }

    // free
    list_node<T> *temp1 = cursor.begin().inner;
    list_node<T> *temp2;
    for (size_t i = 0; i < this->dim_col; i++) {
        temp2 = temp1->right;
        if (i == 0) delete temp1->left;
        if (i == dim_col - 1) delete temp1->right;
        delete temp1;
        temp1 = temp2;
    }

    this->dim_row--;
    return row_iter(ret_val_head, ret_val_tail);
}

template<typename T>
typename doubll2d<T>::col_iter doubll2d<T>::delete_col(col_iter cursor) {
    /*cursor is col_end()*/
    if (cursor == this->col_end()) return col_end();

    /*only col*/
    if (dim_col == 1) {
//        cout << "delete col only\n";
        // free
        list_node<T> *temp1 = cursor.begin().inner;
        list_node<T> *temp2;

        for (size_t i = 0; i < this->dim_row; i++) {
            temp2 = temp1->down;
            delete temp1->left;
            delete temp1->right;
            if (i == dim_row - 1) delete temp1->down;
            if (i == 0) delete temp1->up;
            delete temp1;
            temp1 = temp2;
        }
        // reset
        this->dim_row = 0;
        this->dim_col = 0;
        this->up_left = nullptr;
        this->up_right = nullptr;
        this->down_left = nullptr;
        this->down_right = nullptr;
        return col_iter(up_left, down_left);
    }

    /*first col*/
    if (cursor.begin().inner == this->up_left) {
//        cout << "delete col leftmost\n";
        list_node<T> *temp = cursor.begin().inner;
        list_node<T> *ret_val_head = cursor.begin().inner->right;
        list_node<T> *ret_val_tail;
        for (size_t i = 0; i < this->dim_row; i++) {
            if (i == this->dim_row - 1) ret_val_tail = temp->right;
            temp->right->left = temp->left;
            temp->left->right = temp->right;
            temp = temp->down;
        }
        // free
        list_node<T> *temp1 = cursor.begin().inner;
        list_node<T> *temp2;
        for (size_t i = 0; i < this->dim_row; i++) {
            temp2 = temp1->down;
            if (i == dim_row - 1) delete temp1->down;
            if (i == 0) delete temp1->up;
            delete temp1;
            temp1 = temp2;
        }

        this->dim_col--;
        this->up_left = ret_val_head;
        this->down_left = ret_val_tail;
        return col_iter(ret_val_head, ret_val_tail);
    }


    /*rightmost col*/
    if (cursor.begin().inner == this->up_right) {
//        cout << "delete col rightmost\n";
        list_node<T> *temp = cursor.begin().inner;
        list_node<T> *ret_val_head = cursor.begin().inner->left;
        list_node<T> *ret_val_tail;
        for (size_t i = 0; i < this->dim_row; i++) {
            if (i == this->dim_row - 1) ret_val_tail = temp->left;
            temp->left->right = temp->right;
            temp = temp->down;
        }

        // free
        list_node<T> *temp1 = cursor.begin().inner;
        list_node<T> *temp2;
        for (size_t i = 0; i < this->dim_row; i++) {
            temp2 = temp1->down;
            if (i == 0) delete temp1->up;
            if (i == dim_row - 1) delete temp1->down;
            delete temp1;
            temp1 = temp2;
        }

        this->dim_col--;
        this->up_right = ret_val_head;
        this->down_right = ret_val_tail;
        return col_iter(ret_val_head, ret_val_tail);
    }



    /*common case*/
//    cout << "delete col common\n";
    list_node<T> *temp = cursor.begin().inner;
    list_node<T> *ret_val_head = cursor.begin().inner->left;
    list_node<T> *ret_val_tail;
    for (size_t i = 0; i < this->dim_row; ++i) {
        if (i == this->dim_row - 1) ret_val_tail = temp->left;
        temp->right->left = temp->left;
        temp->left->right = temp->right;
        temp = temp->down;
    }

    // free
    list_node<T> *temp1 = cursor.begin().inner;
    list_node<T> *temp2;
    for (size_t i = 0; i < this->dim_row; i++) {
        temp2 = temp1->down;
        if (i == 0) delete temp1->up;
        if (i == dim_row - 1) delete temp1->down;
        delete temp1;
        temp1 = temp2;
    }

    this->dim_col--;
    return col_iter(ret_val_head, ret_val_tail);
}

template<typename T>
void doubll2d<T>::clear() {
    // free
    size_t repeat_time = this->dim_col;
    for (size_t i = 0; i < repeat_time; i++){
        this->delete_col(this->col_begin());
    }


    // reset
    this->dim_row = 0;
    this->dim_col = 0;
    this->up_left = nullptr;
    this->up_right = nullptr;
    this->down_left = nullptr;
    this->down_right = nullptr;
}

template<typename T>
template<class R>
R doubll2d<T>::reduce(std::function<R(R, const T &)> fn, R init) {
    R res = init;
    for (auto row = this->row_begin(); row != this->row_end(); ++row)
        for (auto &elem: row) {
            res = fn(res, elem);
        }
    return res;
}