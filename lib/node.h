#pragma once
#include <algorithm>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

template <typename T, size_t NodeMaxSize>
class node {
    template <typename, size_t, typename>
    friend class unrolled_list;
    template <typename, size_t>
    friend class my_iterator;
    template <typename, size_t>
    friend class my_const_iterator;
    template <typename, size_t>
    friend class my_reverse_iterator;
    template <typename, size_t>
    friend class my_const_reverse_iterator;

   public:
    node();
    node(const T& value, size_t n = 1);
    node(node<T, NodeMaxSize>*);
    ~node();
    T& at(const size_t&);
    const T& at(const size_t&) const;
    void link_forward(node*);
    void link_back(node*);
    inline void pop_back() {
        if (end > 0) end--;
    }
    inline void pop_front() {
        if (end > 0) *this << 1;
        --end;
    }
    void erase(size_t, size_t to = NodeMaxSize);
    T& back();
    T& front();
    void operator<<(size_t);
    void operator>>(size_t);
    node* next;
    node* prev;
    size_t end;
    void thread_forward(node*);
    void thread_back(node*);

   private:
    alignas(T) std::byte storage[NodeMaxSize * sizeof(T)];
    T* arr;
};

template <typename T, size_t NodeMaxSize>
node<T, NodeMaxSize>::node() {
    arr = reinterpret_cast<T*>(storage);
    next = nullptr;
    prev = nullptr;
    end = 0;
}
template <typename T, size_t NodeMaxSize>
node<T, NodeMaxSize>::node(const T& value, size_t len) {
    arr = reinterpret_cast<T*>(storage);
    end = 0;
    for (size_t i = 0; i < NodeMaxSize; i++) {
        if (len > 0) {
            arr[i] = value;
            std::construct_at(arr + i, value);
            ++end;
            --len;
        }
    }
}

template <typename T, size_t NodeMaxSize>
node<T, NodeMaxSize>::node(node<T, NodeMaxSize>* other) {  // should be complitetd
    arr = reinterpret_cast<T*>(storage);
    end = 0;
    for (size_t i = 0; i < NodeMaxSize / 2; ++i) {
        std::construct_at(arr + i, other->arr[NodeMaxSize / 2 + i]);
        ++end;
    }
    other->end = NodeMaxSize / 2;
}

template <typename T, size_t NodeMaxSize>
node<T, NodeMaxSize>::~node() {
    for (size_t i = 0; i < end; ++i) {
        std::destroy_at(arr + (sizeof(T) * i));
    }
    next = nullptr;
    prev = nullptr;
}

template <typename T, size_t NodeMaxSize>
T& node<T, NodeMaxSize>::at(const size_t& index) {
    return arr[index];
}
template <typename T, size_t NodeMaxSize>
const T& node<T, NodeMaxSize>::at(const size_t& index) const {
    return arr[index];
}
template <typename T, size_t NodeMaxSize>
void node<T, NodeMaxSize>::link_forward(node* other) {
    if (this && other) {
        this->next = other;
        other->prev = this;
    } else if (!this && other) {
        other->prev = nullptr;
    } else if (this && !other) {
        this->next = nullptr;
    }
}
template <typename T, size_t NodeMaxSize>
void node<T, NodeMaxSize>::link_back(node* other) {
    if (this && other) {
        this->prev = other;
        other->next = this;
    } else if (!this && other) {
        other->next = nullptr;
    } else if (this && !other) {
        this->prev = nullptr;
    }
}
template <typename T, size_t NodeMaxSize>
void node<T, NodeMaxSize>::operator<<(size_t shift) {
    shift %= NodeMaxSize;
    std::reverse(arr, arr + shift);
    std::reverse(arr + shift, arr + NodeMaxSize);
    std::reverse(arr, arr + NodeMaxSize);
}
template <typename T, size_t NodeMaxSize>
void node<T, NodeMaxSize>::operator>>(size_t shift) {
    shift %= NodeMaxSize;
    this->operator<<(NodeMaxSize - shift);
}

template <typename T, size_t NodeMaxSize>
void node<T, NodeMaxSize>::thread_forward(node* bufer) {
    for (size_t i = NodeMaxSize / 2; i < NodeMaxSize - 1; ++i) {
        if constexpr (std::is_assignable_v<T, T>) {
            bufer->arr[bufer->end] = arr[i + 1];
        } else {
            std::construct_at(bufer->arr + bufer->end, arr[i + 1]);
        }
        ++bufer->end;
    }
    end = NodeMaxSize / 2 + 1;

    bufer->link_forward(this->next);
    this->link_forward(bufer);
}
template <typename T, size_t NodeMaxSize>  // pay attention
void node<T, NodeMaxSize>::thread_back(node* bufer) {
    for (size_t i = NodeMaxSize / 2; i < NodeMaxSize; ++i) {
        if constexpr (std::is_assignable_v<T, T>) {
            bufer->arr[bufer->end] = arr[i];
        } else {
            std::construct_at(bufer->arr + bufer->end, arr[i]);
        }
        ++bufer->end;
    }
    end = (NodeMaxSize / 2);
    bufer->link_forward(this->next);
    this->link_forward(bufer);
}
template <typename T, size_t NodeMaxSize>
T& node<T, NodeMaxSize>::front() {
    return arr[0];
}
template <typename T, size_t NodeMaxSize>
T& node<T, NodeMaxSize>::back() {
    return arr[end - 1];
}
template <typename T, size_t NodeMaxSize>
void node<T, NodeMaxSize>::erase(size_t from, size_t to) {
    for (size_t i = to; i >= from; ++i) {
        std::destroy_at(arr[to]);
        --end;
    }
    if (to != NodeMaxSize) {
        std::reverse(arr + to + 1, arr + NodeMaxSize);
        std::reverse(arr + from, arr + NodeMaxSize);
    }
}
