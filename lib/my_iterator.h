#include <iterator>
#include <memory>

#include "node.h"

template <typename T, size_t NodeMaxSize>
class my_iterator {
    template <typename, size_t, typename>
    friend class unrolled_list;

   public:
    using iterator_category = std::bidirectional_iterator_tag;
    typedef T* pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::ptrdiff_t difference_type;

    inline my_iterator() : ptr(nullptr), current(0){};
    inline my_iterator(node<T, NodeMaxSize>* obj) : ptr(obj), current(0){};
    inline my_iterator(node<T, NodeMaxSize>* obj, size_t index) : ptr(obj), current(index){};
    inline my_iterator& operator=(T* p) {
        this->ptr = p;
        return *this;
    }
    inline my_iterator(const my_iterator& other) : ptr(other.ptr), current(other.current) {}
    ~my_iterator() = default;
    inline bool operator!=(const my_iterator& other) const { return !(this->operator==(other)); }
    inline bool operator==(const my_iterator& other) const {
        return ptr == other.ptr && current == other.current;
    }
    inline reference operator*() const { return ptr->at(current); }
    inline pointer operator->() const { return &(ptr->at(current)); }
    my_iterator operator++(int);
    my_iterator operator--(int);
    my_iterator& operator++();
    my_iterator& operator--();
    my_iterator& operator+(size_t n);
    my_iterator& operator-(size_t n);
    difference_type operator-(const my_iterator&);

   private:
    node<T, NodeMaxSize>* ptr;
    size_t current;
};

template <typename T, size_t NodeMaxSize>
class my_const_iterator {
    template <typename, size_t, typename>
    friend class unrolled_list;

   public:
    using iterator_category = std::bidirectional_iterator_tag;
    typedef const T* pointer;
    typedef const T& reference;
    typedef const T& const_reference;
    typedef std::ptrdiff_t difference_type;

    inline my_const_iterator() : ptr(nullptr){};
    inline my_const_iterator(const node<T, NodeMaxSize>* obj) : ptr(obj){};
    inline my_const_iterator(const node<T, NodeMaxSize>* obj, size_t index)
        : ptr(obj), current(index){};

    inline my_const_iterator& operator=(T* p) {
        this->ptr = p;
        return *this;
    }
    inline my_const_iterator(const my_const_iterator& other)
        : ptr(other.ptr), current(other.current) {}
    ~my_const_iterator() = default;
    inline bool operator!=(const my_const_iterator& other) const {
        return !(this->operator==(other));
    }
    inline bool operator==(const my_const_iterator& other) const {
        return ptr == other.ptr && current == other.current;
    }
    inline reference operator*() const { return ptr->at(current); }
    inline pointer operator->() const { return &(ptr->at(current)); }
    my_const_iterator operator++(int);
    my_const_iterator operator--(int);
    my_const_iterator& operator++();
    my_const_iterator& operator--();
    my_const_iterator& operator+(size_t n);
    my_const_iterator& operator-(size_t n);
    difference_type operator-(const my_const_iterator&);

   private:
    const node<T, NodeMaxSize>* ptr;
    size_t current;
};

template <typename T, size_t NodeMaxSize>
class my_reverse_iterator {
    template <typename, size_t, typename>
    friend class unrolled_list;

   public:
    typedef T* pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::ptrdiff_t difference_type;
    using iterator_category = std::bidirectional_iterator_tag;

    inline my_reverse_iterator() : ptr(nullptr){};
    inline my_reverse_iterator(node<T, NodeMaxSize>* obj) : ptr(obj){};
    inline my_reverse_iterator(node<T, NodeMaxSize>* obj, size_t index)
        : ptr(obj), current(index){};

    inline my_reverse_iterator& operator=(T* p) {
        this->ptr = p;
        return *this;
    }
    inline my_reverse_iterator(const my_reverse_iterator& other)
        : ptr(other.ptr), current(other.current) {}
    ~my_reverse_iterator() = default;
    inline bool operator!=(const my_reverse_iterator& other) const {
        return !(this->operator==(other));
    }
    inline bool operator==(const my_reverse_iterator& other) const {
        return ptr == other.ptr && current == other.current;
    }
    inline reference operator*() const { return ptr->at(current); }
    inline pointer operator->() const { return &(ptr->at(current)); }
    my_reverse_iterator operator++(int);
    my_reverse_iterator operator--(int);
    my_reverse_iterator& operator++();
    my_reverse_iterator& operator--();
    my_reverse_iterator& operator-(size_t n);
    my_reverse_iterator& operator+(size_t n);
    difference_type operator-(const my_reverse_iterator&);

   private:
    node<T, NodeMaxSize>* ptr;
    size_t current;
};

template <typename T, size_t NodeMaxSize>
class my_const_reverse_iterator {
    template <typename, size_t, typename>
    friend class unrolled_list;

   public:
    typedef const T* pointer;
    typedef const T& reference;
    typedef const T& const_reference;
    typedef std::ptrdiff_t difference_type;
    using iterator_category = std::bidirectional_iterator_tag;

    inline my_const_reverse_iterator() : ptr(nullptr){};
    inline my_const_reverse_iterator(const node<T, NodeMaxSize>* obj) : ptr(obj){};
    inline my_const_reverse_iterator(const node<T, NodeMaxSize>* obj, size_t index)
        : ptr(obj), current(index){};

    inline my_const_reverse_iterator& operator=(T* p) {
        this->ptr = p;
        return *this;
    }
    inline my_const_reverse_iterator(const my_const_reverse_iterator& other)
        : ptr(other.ptr), current(other.current) {}
    ~my_const_reverse_iterator() = default;
    inline bool operator!=(const my_const_reverse_iterator& other) const {
        return !(this->operator==(other));
    }
    inline bool operator==(const my_const_reverse_iterator& other) const {
        return ptr == other.ptr && current == other.current;
    }
    inline reference operator*() const { return ptr->at(current); }
    inline pointer operator->() const { return &(ptr->at(current)); }
    my_const_reverse_iterator operator++(int);
    my_const_reverse_iterator operator--(int);
    my_const_reverse_iterator& operator++();
    my_const_reverse_iterator& operator--();
    my_const_reverse_iterator& operator-(size_t n);
    my_const_reverse_iterator& operator+(size_t n);
    difference_type operator-(const my_const_reverse_iterator&);

   private:
    const node<T, NodeMaxSize>* ptr;
    size_t current;
};

template <typename T, size_t NodeMaxSize>
struct std::iterator_traits<my_iterator<T, NodeMaxSize>> {
    typedef std::ptrdiff_t difference_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
};
template <typename T, size_t NodeMaxSize>
struct std::iterator_traits<my_const_iterator<T, NodeMaxSize>> {
    typedef std::ptrdiff_t difference_type;
    typedef const T value_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
};
template <typename T, size_t NodeMaxSize>
struct std::iterator_traits<my_reverse_iterator<T, NodeMaxSize>> {
    typedef std::ptrdiff_t difference_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
};
template <typename T, size_t NodeMaxSize>
struct std::iterator_traits<my_const_reverse_iterator<T, NodeMaxSize>> {
    typedef std::ptrdiff_t difference_type;
    typedef const T value_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
};

template <typename T, size_t NodeMaxSize>
my_iterator<T, NodeMaxSize> my_iterator<T, NodeMaxSize>::operator++(int) {
    my_iterator<T, NodeMaxSize> temp = *this;
    if (current < ptr->end || !ptr->next) {
        ++current;
    } else {
        ptr = ptr->next;
        current = 0;
    }
    return temp;
}
template <typename T, size_t NodeMaxSize>
my_iterator<T, NodeMaxSize> my_iterator<T, NodeMaxSize>::operator--(int) {
    my_iterator<T, NodeMaxSize> temp = *this;
    if (current > 0 || !ptr->prev) {
        --current;
    } else {
        ptr = ptr->prev;
        current = ptr->end;
    }
    return temp;
}
template <typename T, size_t NodeMaxSize>
my_iterator<T, NodeMaxSize>& my_iterator<T, NodeMaxSize>::operator++() {
    if (current < ptr->end - 1 || !ptr->next) {
        ++current;
    } else {
        ptr = ptr->next;
        current = 0;
    }
    return *this;
}
template <typename T, size_t NodeMaxSize>
my_iterator<T, NodeMaxSize>& my_iterator<T, NodeMaxSize>::operator--() {
    if (current > 0 || !ptr->prev) {
        --current;
    } else {
        ptr = ptr->prev;
        current = ptr->end;
    }
    return *this;
}
template <typename T, size_t NodeMaxSize>
my_iterator<T, NodeMaxSize>::difference_type my_iterator<T, NodeMaxSize>::operator-(
    const my_iterator& other) {
    my_iterator<T, NodeMaxSize> temp = *this;
    size_t cnt = 1;
    while (temp != other) {
        ++temp;
        ++cnt;
    }
    return cnt;
}

template <typename T, size_t NodeMaxSize>
my_reverse_iterator<T, NodeMaxSize> my_reverse_iterator<T, NodeMaxSize>::operator++(int) {
    my_reverse_iterator<T, NodeMaxSize> temp = *this;
    if (current > 0 || !ptr->prev) {
        --current;
    } else {
        ptr = ptr->prev;
        current = ptr->end;
    }
    return temp;
}
template <typename T, size_t NodeMaxSize>
my_reverse_iterator<T, NodeMaxSize> my_reverse_iterator<T, NodeMaxSize>::operator--(int) {
    my_reverse_iterator<T, NodeMaxSize> temp = *this;
    if (current < ptr->end - 1 || !ptr->next) {
        ++current;
    } else {
        ptr = ptr->next;
        current = 0;
    }
    return temp;
}
template <typename T, size_t NodeMaxSize>
my_reverse_iterator<T, NodeMaxSize>& my_reverse_iterator<T, NodeMaxSize>::operator++() {
    if (current > 0 || !ptr->prev) {
        --current;
    } else {
        ptr = ptr->prev;
        current = ptr->end;
    }
    return *this;
}
template <typename T, size_t NodeMaxSize>
my_reverse_iterator<T, NodeMaxSize>& my_reverse_iterator<T, NodeMaxSize>::operator--() {
    if (current < ptr->end - 1 || !ptr->next) {
        ++current;
    } else {
        ptr = ptr->next;
        current = 0;
    }
    return *this;
}
template <typename T, size_t NodeMaxSize>
my_reverse_iterator<T, NodeMaxSize>::difference_type my_reverse_iterator<T, NodeMaxSize>::operator-(
    const my_reverse_iterator& other) {
    my_reverse_iterator<T, NodeMaxSize> temp = *this;
    size_t cnt = 1;
    while (temp != other) {
        ++temp;
        ++cnt;
    }
    return cnt;
}
template <typename T, size_t NodeMaxSize>
my_reverse_iterator<T, NodeMaxSize>& my_reverse_iterator<T, NodeMaxSize>::operator-(size_t n) {
    my_reverse_iterator res(*this);
    for (size_t i = 0; i < n; ++i) {
        ++res;
    }
    return res;
}
template <typename T, size_t NodeMaxSize>
my_reverse_iterator<T, NodeMaxSize>& my_reverse_iterator<T, NodeMaxSize>::operator+(size_t n) {
    my_reverse_iterator res(*this);
    for (size_t i = 0; i < n; ++i) {
        --res;
    }
    return res;
}

template <typename T, size_t NodeMaxSize>
my_const_iterator<T, NodeMaxSize> my_const_iterator<T, NodeMaxSize>::operator++(int) {
    my_const_iterator<T, NodeMaxSize> temp = *this;
    if (current < ptr->end - 1 || !ptr->next) {
        ++current;
    } else {
        ptr = ptr->next;
        current = 0;
    }
    return temp;
}
template <typename T, size_t NodeMaxSize>
my_const_iterator<T, NodeMaxSize> my_const_iterator<T, NodeMaxSize>::operator--(int) {
    my_const_iterator<T, NodeMaxSize> temp = *this;
    if (current > 0 || !ptr->prev) {
        --current;
    } else {
        ptr = ptr->prev;
        current = ptr->end;
    }
    return temp;
}

template <typename T, size_t NodeMaxSize>
my_const_iterator<T, NodeMaxSize>& my_const_iterator<T, NodeMaxSize>::operator++() {
    if (current < ptr->end - 1 || !ptr->next) {
        ++current;
    } else {
        ptr = ptr->next;
        current = 0;
    }
    return *this;
}

template <typename T, size_t NodeMaxSize>
my_const_iterator<T, NodeMaxSize>& my_const_iterator<T, NodeMaxSize>::operator--() {
    if (current > 0 || !ptr->prev) {
        --current;
    } else {
        ptr = ptr->prev;
        current = ptr->end;
    }
    return *this;
}
template <typename T, size_t NodeMaxSize>
my_const_iterator<T, NodeMaxSize>::difference_type my_const_iterator<T, NodeMaxSize>::operator-(
    const my_const_iterator& other) {
    my_const_iterator<T, NodeMaxSize> temp = *this;
    size_t cnt = 1;
    while (temp != other) {
        ++temp;
        ++cnt;
    }
    return cnt;
}
template <typename T, size_t NodeMaxSize>
my_const_iterator<T, NodeMaxSize>& my_const_iterator<T, NodeMaxSize>::operator+(size_t n) {
    my_const_iterator res(*this);
    for (size_t i = 0; i < n; ++i) {
        ++res;
    }
    return res;
}
template <typename T, size_t NodeMaxSize>
my_const_iterator<T, NodeMaxSize>& my_const_iterator<T, NodeMaxSize>::operator-(size_t n) {
    my_const_iterator res(*this);
    for (size_t i = 0; i < n; ++i) {
        --res;
    }
    return res;
}

template <typename T, size_t NodeMaxSize>
my_const_reverse_iterator<T, NodeMaxSize> my_const_reverse_iterator<T, NodeMaxSize>::operator++(
    int) {
    my_const_reverse_iterator<T, NodeMaxSize> temp = *this;
    if (current > 0 || !ptr->prev) {
        --current;
    } else {
        ptr = ptr->prev;
        current = ptr->end;
    }
    return temp;
}
template <typename T, size_t NodeMaxSize>
my_const_reverse_iterator<T, NodeMaxSize> my_const_reverse_iterator<T, NodeMaxSize>::operator--(
    int) {
    my_const_reverse_iterator<T, NodeMaxSize> temp = *this;
    if (current < ptr->end - 1 || !ptr->next) {
        ++current;
    } else {
        ptr = ptr->next;
        current = 0;
    }
    return temp;
}
template <typename T, size_t NodeMaxSize>
my_const_reverse_iterator<T, NodeMaxSize>& my_const_reverse_iterator<T, NodeMaxSize>::operator++() {
    if (current > 0 || !ptr->prev) {
        --current;
    } else {
        ptr = ptr->prev;
        current = ptr->end;
    }
    return *this;
}
template <typename T, size_t NodeMaxSize>
my_const_reverse_iterator<T, NodeMaxSize>& my_const_reverse_iterator<T, NodeMaxSize>::operator--() {
    if (current < ptr->end - 1 || !ptr->next) {
        ++current;
    } else {
        ptr = ptr->next;
        current = 0;
    }
    return *this;
}
template <typename T, size_t NodeMaxSize>
my_const_reverse_iterator<T, NodeMaxSize>::difference_type
my_const_reverse_iterator<T, NodeMaxSize>::operator-(const my_const_reverse_iterator& other) {
    my_const_reverse_iterator<T, NodeMaxSize> temp = *this;
    size_t cnt = 1;
    while (temp != other) {
        ++temp;
        ++cnt;
    }
    return cnt;
}
template <typename T, size_t NodeMaxSize>
my_const_reverse_iterator<T, NodeMaxSize>& my_const_reverse_iterator<T, NodeMaxSize>::operator-(
    size_t n) {
    my_const_reverse_iterator res(*this);
    for (size_t i = 0; i < n; ++i) {
        ++res;
    }
    return res;
}
template <typename T, size_t NodeMaxSize>
my_const_reverse_iterator<T, NodeMaxSize>& my_const_reverse_iterator<T, NodeMaxSize>::operator+(
    size_t n) {
    my_const_reverse_iterator res(*this);
    for (size_t i = 0; i < n; ++i) {
        --res;
    }
    return res;
}