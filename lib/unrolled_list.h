#include <initializer_list>
#include <list>

#include "my_iterator.h"

template <typename T, size_t NodeMaxSize = 10, typename Allocator = std::allocator<T>>
class unrolled_list {
   public:
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t difference_type;
    typedef size_t size_type;
    typedef my_iterator<T, NodeMaxSize> iterator;
    typedef my_const_iterator<T, NodeMaxSize> const_iterator;
    typedef my_reverse_iterator<T, NodeMaxSize> reverse_iterator;
    typedef my_const_reverse_iterator<T, NodeMaxSize> const_reverse_iterator;
    typedef Allocator allocator_type;

    typedef typename std::allocator_traits<Allocator>::template rebind_alloc<node<T, NodeMaxSize>>
        allocatorNode;

    unrolled_list();
    unrolled_list(const T&, Allocator&);
    unrolled_list(const T&, const size_t&, Allocator&);
    unrolled_list(const size_t&, const T&);
    unrolled_list(iterator&, iterator&);
    unrolled_list(iterator&, iterator&, Allocator&);
    unrolled_list(std::list<T>::iterator begin, std::list<T>::iterator end, const Allocator& al);
    unrolled_list(const std::initializer_list<T>&);
    unrolled_list(const Allocator&);
    unrolled_list(unrolled_list&&, const Allocator&);
    unrolled_list(const unrolled_list&);

    ~unrolled_list();

    unrolled_list& operator=(const unrolled_list<T, NodeMaxSize, Allocator>&);

    inline iterator begin() { return iterator(head, 0); }
    inline iterator end() { return iterator(tail, tail->end); }
    inline const_iterator begin() const { return const_iterator(head, 0); }
    inline const_iterator end() const { return const_iterator(tail, tail->end); }
    inline const_iterator cbegin() const { return const_iterator(head, 0); }
    inline const_iterator cend() const { return const_iterator(tail, tail->end); }
    inline reverse_iterator rbegin() { return reverse_iterator(tail, tail->end - 1); }
    inline reverse_iterator rend() { return reverse_iterator(head, -1); }
    inline const_reverse_iterator rbegin() const {
        return const_reverse_iterator(tail, tail->end - 1);
    }
    inline const_reverse_iterator rend() const { return const_reverse_iterator(head, -1); }
    inline const_reverse_iterator crbegin() const {
        return const_reverse_iterator(tail, tail->end - 1);
    }
    inline const_reverse_iterator crend() const { return const_reverse_iterator(head, -1); }

    inline bool operator==(const unrolled_list<T, NodeMaxSize, Allocator>& rhs) const {
        return std::equal(this->cbegin(), this->cend(), rhs.cbegin(), rhs.cend());
    }
    inline bool operator!=(const unrolled_list<T, NodeMaxSize, Allocator>& rhs) const {
        return !(this == rhs);
    }

    void swap(const unrolled_list<T, NodeMaxSize, Allocator>&);
    inline size_t size() { return capacity; }
    inline size_t max_size() { return NodeMaxSize * node_capacity; }
    inline bool is_empty() { return *this == unrolled_list<T, NodeMaxSize, Allocator>(); }
    inline bool empty() { return *this == unrolled_list<T, NodeMaxSize, Allocator>(); }
    inline allocator_type get_allocator() const { return alloc; }

    iterator insert(iterator it, T value);
    iterator insert(iterator it, size_t n, T value);
    iterator insert(iterator point, iterator begin, iterator end);
    iterator insert(iterator point, std::initializer_list<T> init);
    iterator erase(iterator point) noexcept;
    iterator erase(iterator begin, iterator end) noexcept;

    iterator insert(const_iterator it, T value);
    iterator insert(const_iterator it, size_t n, T value);
    iterator insert(const_iterator point, iterator begin, iterator end);
    iterator insert(const_iterator point, std::initializer_list<T> init);
    iterator erase(const_iterator point) noexcept;
    iterator erase(const_iterator begin, const_iterator end) noexcept;

    void clear() noexcept;
    void assign(const_iterator, const_iterator) noexcept;
    void assign(std::initializer_list<T>) noexcept;
    void assign(size_t, T) noexcept;

    void push_back(const T&);
    void push_front(const T&);
    void pop_back() noexcept;
    void pop_front() noexcept;
    inline T& front() { return head->front(); }
    inline T& back() { return tail->back(); }
    inline const T& front() const { return head->front(); }
    inline const T& back() const { return tail->back(); }

   private:
    node<T, NodeMaxSize>* head;
    node<T, NodeMaxSize>* tail;

    allocatorNode alloc;
    size_t capacity;
    size_t node_capacity;
};

template <typename T, size_t NodeMaxSize, typename Allocator>
unrolled_list<T, NodeMaxSize, Allocator>::unrolled_list() : alloc() {
    capacity = 0;
    node_capacity = 1;
    head = alloc.allocate(1);
    std::allocator_traits<allocatorNode>::construct(alloc, head);
    tail = head;
}
template <typename T, size_t NodeMaxSize, typename Allocator>
unrolled_list<T, NodeMaxSize, Allocator>::unrolled_list(const T& value, Allocator& al) : alloc(al) {
    capacity = 1;
    node_capacity = 1;
    head = alloc.allocate(1);
    std::allocator_traits<allocatorNode>::construct(alloc, head, value);
    tail = head;
}
template <typename T, size_t NodeMaxSize, typename Allocator>
unrolled_list<T, NodeMaxSize, Allocator>::unrolled_list(const T& value, const size_t& n,
                                                        Allocator& al)
    : alloc(al) {
    capacity = n;
    node_capacity = 1;
    head = alloc.allocate(1);
    std::allocator_traits<allocatorNode>::construct(alloc, head, n, value);
    tail = head;
    while (tail->next) {
        tail = tail->next;
        ++node_capacity;
    }
}
template <typename T, size_t NodeMaxSize, typename Allocator>
unrolled_list<T, NodeMaxSize, Allocator>::unrolled_list(const size_t& n, const T& value) : alloc() {
    capacity = n;
    node_capacity = 1;

    node<T, NodeMaxSize>* temp = alloc.allocate(1);
    std::allocator_traits<allocatorNode>::construct(alloc, temp);
    tail = temp;
    head = temp;
    size_t len = n;

    if (len > NodeMaxSize / 2) {
        (temp, value, NodeMaxSize / 2);
    } else {
        std::allocator_traits<allocatorNode>::construct(alloc, temp, len, value);
    }
    len -= NodeMaxSize / 2;

    while (len > 0) {
        temp = alloc.allocate(1);
        if (len > NodeMaxSize / 2) {
            std::allocator_traits<allocatorNode>::construct(alloc, temp, value, NodeMaxSize / 2);
        } else {
            std::allocator_traits<allocatorNode>::construct(alloc, temp, len, value);
        }
        tail->link_forward(temp);
        tail = temp;
        len -= NodeMaxSize / 2;
    }
}
template <typename T, size_t NodeMaxSize, typename Allocator>
unrolled_list<T, NodeMaxSize, Allocator>::unrolled_list(iterator& begin, iterator& end) : alloc() {
    capacity = 0;
    node_capacity = 1;
    head = alloc.allocate(1);
    std::allocator_traits<allocatorNode>::construct(alloc, head);
    tail = head;
    while (begin != end) {
        tail->push_back(*begin);
        ++capacity;
        if (tail->next) {
            tail = tail->next;
            ++node_capacity;
        }
        ++begin;
    }
}
template <typename T, size_t NodeMaxSize, typename Allocator>
unrolled_list<T, NodeMaxSize, Allocator>::unrolled_list(iterator& begin, iterator& end,
                                                        Allocator& al)
    : alloc(al) {
    capacity = 0;
    node_capacity = 1;
    head = alloc.allocate(1);
    std::allocator_traits<allocatorNode>::construct(alloc, head);
    tail = head;
    while (begin != end) {
        try {
            tail->push_back(*begin);
            ++capacity;
        } catch (std::runtime_error) {
            this->~unrolled_list();
            throw std::runtime_error("");
        }
        if (tail->next) {
            tail = tail->next;
            ++node_capacity;
        }
        ++begin;
    }
}
template <typename T, size_t NodeMaxSize, typename Allocator>
unrolled_list<T, NodeMaxSize, Allocator>::unrolled_list(std::list<T>::iterator begin,
                                                        std::list<T>::iterator end,
                                                        const Allocator& al)
    : alloc(al) {
    capacity = 0;
    node_capacity = 1;
    head = alloc.allocate(1);
    std::allocator_traits<allocatorNode>::construct(alloc, head);
    node<T, NodeMaxSize>* temp = head;
    tail = head;

    while (begin != end) {
        if (temp->end == NodeMaxSize / 2) {
            temp = alloc.allocate(1);
            std::allocator_traits<allocatorNode>::construct(alloc, temp);
            ++node_capacity;
            tail->link_forward(temp);
            tail = temp;
        }
        try {
            std::construct_at(tail->arr + tail->end, *begin);
        } catch (std::runtime_error) {
            this->~unrolled_list();
            throw std::runtime_error("");
        }
        ++tail->end;
        ++begin;
        ++capacity;
    }
}
template <typename T, size_t NodeMaxSize, typename Allocator>
unrolled_list<T, NodeMaxSize, Allocator>::unrolled_list(const std::initializer_list<T>& il)
    : alloc() {
    capacity = 0;
    node_capacity = 1;
    head = alloc.allocate(1);
    std::allocator_traits<allocatorNode>::construct(alloc, head);
    tail = head;
    iterator begin = il.begin();
    iterator end = il.end();
    while (begin != end) {
        tail->push_back(*begin);
        ++capacity;
        if (tail->next) {
            tail = tail->next;
            ++node_capacity;
        }
        ++begin;
    }
}
template <typename T, size_t NodeMaxSize, typename Allocator>
unrolled_list<T, NodeMaxSize, Allocator>::unrolled_list(const Allocator& al) : alloc(al) {
    capacity = 0;
    node_capacity = 1;
    head = alloc.allocate(1);
    std::allocator_traits<allocatorNode>::construct(alloc, head);
    tail = head;
}
template <typename T, size_t NodeMaxSize, typename Allocator>
unrolled_list<T, NodeMaxSize, Allocator>::unrolled_list(unrolled_list&& other, const Allocator& al)
    : alloc(al) {
    capacity = 0;
    node_capacity = 1;
    head = alloc.allocate(1);
    std::allocator_traits<allocatorNode>::construct(alloc, head);
    tail = head;
    iterator begin = other.begin();
    iterator end = other.end();
    while (begin != end) {
        tail->push_back(*begin);
        ++capacity;
        if (tail->next) {
            tail = tail->next;
            ++node_capacity;
        }
        ++begin;
    }
}
template <typename T, size_t NodeMaxSize, typename Allocator>
unrolled_list<T, NodeMaxSize, Allocator>::unrolled_list(const unrolled_list& other) : alloc() {
    capacity = other.capacity;
    node_capacity = other.node_capacity;
    head = alloc.allocate(1);
    std::allocator_traits<allocatorNode>::construct(alloc, head);
    tail = head;
    iterator begin = other.begin();
    iterator end = other.end();
    while (begin != end) {
        tail->push_back(*begin);
        if (tail->next) {
            tail = tail->next;
        }
        ++begin;
    }
}
template <typename T, size_t NodeMaxSize, typename Allocator>
unrolled_list<T, NodeMaxSize, Allocator>::~unrolled_list() {
    node<T, NodeMaxSize>* temp = tail->prev;
    while (temp != head && temp) {
        std::allocator_traits<allocatorNode>::destroy(alloc, tail);
        alloc.deallocate(tail, 1);
        tail = temp;
        temp = temp->prev;
    }
    std::allocator_traits<allocatorNode>::destroy(alloc, head);
    alloc.deallocate(head, 1);
    head = nullptr;
    tail = nullptr;
}

template <typename T, size_t NodeMaxSize, typename Allocator>
unrolled_list<T, NodeMaxSize, Allocator>& unrolled_list<T, NodeMaxSize, Allocator>::operator=(
    const unrolled_list<T, NodeMaxSize, Allocator>& other) {
    if (this != &other) {
        this = other;
    }
    return this;
}
template <typename T, size_t NodeMaxSize, typename Allocator>
void unrolled_list<T, NodeMaxSize, Allocator>::swap(
    const unrolled_list<T, NodeMaxSize, Allocator>& other) {
    std::swap(this->head, other->head);
    std::swap(this->tail, other->tail);
    if (std::allocator_traits<Allocator>::propagate_on_container_swap) {
        swap(this->alloc, other->alloc);
    }
}

template <typename T, size_t NodeMaxSize, typename Allocator>
my_iterator<T, NodeMaxSize> unrolled_list<T, NodeMaxSize, Allocator>::insert(
    my_iterator<T, NodeMaxSize> it, T value) {
    if (it.ptr->end == NodeMaxSize) {
        ++node_capacity;
        node<T, NodeMaxSize>* bufer = alloc.allocate(1);
        std::allocator_traits<allocatorNode>::construct(alloc, bufer);
        it.ptr->thread_forward(bufer);
        while (tail->next) {
            tail = tail->next;
        }
        if (it.current > it.ptr->end) {
            it.ptr = it.ptr->next;
            it.current -= NodeMaxSize / 2 + 1;
        }
    }
    std::reverse(it.ptr->arr + it.current, it.ptr->arr + it.ptr->end);
    std::reverse(it.ptr->arr + it.current, it.ptr->arr + it.ptr->end + 1);
    try {
        if constexpr (std::is_assignable_v<T, T>) {
            it.ptr->arr[it.current] = value;
        } else {
            std::construct_at(it.ptr->arr + it.current, value);
        }
    } catch (...) {
        std::reverse(it.ptr->arr + it.current, it.ptr->arr + it.ptr->end + 1);
        std::reverse(it.ptr->arr + it.current, it.ptr->arr + it.ptr->end);
        throw std::runtime_error("Failure at insert");
    }
    ++it.ptr->end;
    ++capacity;
    return it;
}
template <typename T, size_t NodeMaxSize, typename Allocator>
my_iterator<T, NodeMaxSize> unrolled_list<T, NodeMaxSize, Allocator>::insert(
    my_iterator<T, NodeMaxSize> it, size_t n, T value) {
    node<T, NodeMaxSize>* bufer = alloc.allocate(1);
    for (size_t i = 0; i < it.ptr->end - it.current - 1; ++i) {
        try {
            if constexpr (std::is_assignable_v<T, T>) {
                bufer->arr[i] = it.ptr->arr[it.current + i + 1];
            } else {
                std::construct_at(bufer->arr + it.current + i + 1, *((i + 1) + it));
            }
        } catch (...) {
            throw std::runtime_error("Failure at insert");
        }
    }
    bufer->end = it.ptr->end - it.current - 1;
    end = it.current + 1;
    node_capacity += 2;
    bufer->link_forward(it->ptr->next);

    node<T, NodeMaxSize>* forward = alloc.allocate(1);
    node<T, NodeMaxSize>* temp_head = forward;

    size_t len = n;

    if (len > NodeMaxSize / 2) {
        try {
            std::allocator_traits<allocatorNode>::construct(alloc, forward, NodeMaxSize / 2, value);
        } catch (...) {
            throw std::runtime_error("Failure at constructing node while inserting");
        }
    } else {
        try {
            std::allocator_traits<allocatorNode>::construct(alloc, forward, len, value);
        } catch (...) {
            throw std::runtime_error("Failure at constucting node while inserting");
        }
    }
    len -= NodeMaxSize / 2;

    while (len > 0) {
        node<T, NodeMaxSize>* back = forward;
        forward = alloc.allocate(1);
        if (len > NodeMaxSize / 2) {
            try {
                std::allocator_traits<allocatorNode>::construct(alloc, forward, NodeMaxSize / 2,
                                                                value);
            } catch (...) {
                throw std::runtime_error("Failure at constructing node while inserting");
            }
        } else {
            try {
                std::allocator_traits<allocatorNode>::construct(alloc, forward, len, value);
            } catch (...) {
                throw std::runtime_error("Failure at constucting node while inserting");
            }
        }
        back->link_forward(forward);
        len -= NodeMaxSize / 2;
        ++node_capacity;
    }
    it->ptr->link_forward(temp_head);
    forward->link_forward(bufer);
    capacity += n;
    while (tail->next) {
        tail = tail->next;
    }

    return iterator(forward, forward->end);
}
template <typename T, size_t NodeMaxSize, typename Allocator>
my_iterator<T, NodeMaxSize> unrolled_list<T, NodeMaxSize, Allocator>::insert(
    my_iterator<T, NodeMaxSize> point, my_iterator<T, NodeMaxSize> begin,
    my_iterator<T, NodeMaxSize> end) {
    node<T, NodeMaxSize>* bufer = alloc.allocate(1);
    my_iterator<T, NodeMaxSize> p(point);
    for (size_t i = 0; i < point.ptr->end - point.current - 1; ++i) {
        try {
            if constexpr (std::is_assignable_v<T, T>) {
                bufer->arr[i] = point.ptr->arr[point.current + i + 1];
            } else {
                std::construct_at(bufer->arr + point.current + i + 1, point.ptr->arr[point.current + i + 1]);
            }
        } catch (...) {
            throw std::runtime_error("Failure at insert");
        }
    }
    bufer->end = point.ptr->end - point.current - 1;
    end = ++point;
    node_capacity += 2;
    bufer->link_forward(point.ptr->next);

    node<T, NodeMaxSize>* temp_tail = alloc.allocate(1);
    node<T, NodeMaxSize>* temp_head = temp_tail;
    node<T, NodeMaxSize>* temp = temp_tail;
    std::allocator_traits<allocatorNode>::construct(alloc, temp_tail);

    while (begin != end) {
        ++capacity;
        if (temp_tail->end == NodeMaxSize / 2) {
            temp_tail = alloc.allocate(1);
            std::allocator_traits<allocatorNode>::construct(alloc, temp_tail);
            ++node_capacity;
            temp->link_forward(temp_tail);
        }
        try {
            if constexpr (std::is_assignable_v<T, T>) {
                temp_tail->arr[temp_tail->end] = *begin;
            } else {
                std::construct_at(temp_tail->arr + temp_tail->end, *begin);
            }
        } catch (...) {
            --capacity;
            throw std::runtime_error("Failure at insert");
        }
        ++begin;
        ++temp_tail->end;
    }

    point.ptr->link_forward(temp_head);
    temp_tail->link_forward(bufer);
    while (tail->next) {
        tail = tail->next;
    }

    return iterator(temp_tail, temp_tail->end);
}
template <typename T, size_t NodeMaxSize, typename Allocator>
my_iterator<T, NodeMaxSize> unrolled_list<T, NodeMaxSize, Allocator>::insert(
    my_iterator<T, NodeMaxSize> point, std::initializer_list<T> init) {
    return insert(point, init.begin(), init.end());
}
template <typename T, size_t NodeMaxSize, typename Allocator>
my_iterator<T, NodeMaxSize> unrolled_list<T, NodeMaxSize, Allocator>::erase(
    my_iterator<T, NodeMaxSize> point) noexcept {
    --capacity;
    iterator res(point->erase(point.cur, point.cur));
    while (point.ptr != res.ptr) {
        --node_capacity;
        point.ptr = point.ptr->next;
    }
    return res;
}
template <typename T, size_t NodeMaxSize, typename Allocator>
my_iterator<T, NodeMaxSize> unrolled_list<T, NodeMaxSize, Allocator>::erase(
    my_iterator<T, NodeMaxSize> begin, my_iterator<T, NodeMaxSize> end) noexcept {
    while (begin != end) {
        if (begin.ptr != end.ptr) {
            if (begin.cur == 0) {
                capacity -= begin.ptr->end;
                --node_capacity;
                iterator temp(begin);
                ++begin;
                temp.ptr->prev->link_forward(temp.ptr->next);
                temp.ptr->next = nullptr;
                alloc.destroy(temp);
            } else {
                begin.ptr->erase(begin);
                ++begin;
            }
        } else {
            begin.ptr->erase(begin.current, end.current);
        }
        node<T, NodeMaxSize>* bufer = alloc.allocate(1);
        std::allocator_traits<allocatorNode>::construct(alloc, bufer);
        tail->thread_forward(bufer);
        tail = bufer;
    }
}

template <typename T, size_t NodeMaxSize, typename Allocator>
my_iterator<T, NodeMaxSize> unrolled_list<T, NodeMaxSize, Allocator>::insert(
    my_const_iterator<T, NodeMaxSize> it, T value) {
    ++capacity;
    if (it.ptr->end == NodeMaxSize) {
        ++node_capacity;
        node<T, NodeMaxSize>* bufer = alloc.allocate(1);
        std::allocator_traits<allocatorNode>::construct(alloc, bufer);
        it.ptr->thread_forward(bufer);
        while (tail->next) {
            tail = tail->next;
        }
        if (it.current > it.ptr->end) {
            it.ptr = it.ptr->next;
            it.current -= NodeMaxSize / 2 + 1;
        }
    }
    std::reverse(it.ptr->arr + it.current, it.ptr->arr + it.ptr->end);
    std::reverse(it.ptr->arr + it.current, it.ptr->arr + it.ptr->end + 1);
    try {
        if constexpr (std::is_assignable_v<T, T>) {
            it.ptr->arr[it.current] = value;
        } else {
            std::construct_at(it.ptr->arr + it.current, value);
        }
    } catch (...) {
        std::reverse(it.ptr->arr + it.current, it.ptr->arr + it.ptr->end + 1);
        std::reverse(it.ptr->arr + it.current, it.ptr->arr + it.ptr->end);
        throw std::runtime_error("Failure at insert");
    }
    ++it.ptr->end;
    return it;
}
template <typename T, size_t NodeMaxSize, typename Allocator>
my_iterator<T, NodeMaxSize> unrolled_list<T, NodeMaxSize, Allocator>::insert(
    my_const_iterator<T, NodeMaxSize> it, size_t n, T value) {
    node<T, NodeMaxSize>* bufer = alloc.allocate(1);
    for (size_t i = 0; i < it.ptr->end - it.current - 1; ++i) {
        try {
            if constexpr (std::is_assignable_v<T, T>) {
                bufer->arr[i] = it.ptr->arr[it.current + i + 1];
            } else {
                std::construct_at(bufer->arr + it.current + i + 1, *((i + 1) + it));
            }
        } catch (...) {
            throw std::runtime_error("Failure at insert");
        }
    }
    bufer->end = it.ptr->end - it.current - 1;
    end = it.current + 1;
    node_capacity += 2;
    bufer->link_forward(it->ptr->next);

    node<T, NodeMaxSize>* forward = alloc.allocate(1);
    node<T, NodeMaxSize>* temp_head = forward;

    size_t len = n;

    if (len > NodeMaxSize / 2) {
        try {
            std::allocator_traits<allocatorNode>::construct(alloc, forward, NodeMaxSize / 2, value);
        } catch (...) {
            throw std::runtime_error("Failure at constructing node while inserting");
        }
    } else {
        try {
            std::allocator_traits<allocatorNode>::construct(alloc, forward, len, value);
        } catch (...) {
            throw std::runtime_error("Failure at constucting node while inserting");
        }
    }
    len -= NodeMaxSize / 2;

    while (len > 0) {
        node<T, NodeMaxSize>* back = forward;
        forward = alloc.allocate(1);
        if (len > NodeMaxSize / 2) {
            try {
                std::allocator_traits<allocatorNode>::construct(alloc, forward, NodeMaxSize / 2,
                                                                value);
            } catch (...) {
                throw std::runtime_error("Failure at constructing node while inserting");
            }
        } else {
            try {
                std::allocator_traits<allocatorNode>::construct(alloc, forward, len, value);
            } catch (...) {
                throw std::runtime_error("Failure at constucting node while inserting");
            }
        }
        back->link_forward(forward);
        len -= NodeMaxSize / 2;
        ++node_capacity;
    }
    it->ptr->link_forward(temp_head);
    forward->link_forward(bufer);
    capacity += n;
    while (tail->next) {
        tail = tail->next;
    }

    return const_iterator(forward, forward->end);
}
template <typename T, size_t NodeMaxSize, typename Allocator>
my_iterator<T, NodeMaxSize> unrolled_list<T, NodeMaxSize, Allocator>::insert(
    my_const_iterator<T, NodeMaxSize> point, my_iterator<T, NodeMaxSize> begin,
    my_iterator<T, NodeMaxSize> end) {
    node<T, NodeMaxSize>* bufer = alloc.allocate(1);
    for (size_t i = 0; i < point->ptr->end - point->current - 1; ++i) {
        try {
            if constexpr (std::is_assignable_v<T, T>) {
                bufer->arr[i] = point.ptr->arr[point.current + i + 1];
            } else {
                std::construct_at(bufer->arr + point.current + i + 1, *((i + 1) + point));
            }
        } catch (...) {
            throw std::runtime_error("Failure at insert");
        }
    }
    bufer->end = point->ptr->end - point->current - 1;
    end = point.current + 1;
    node_capacity += 2;
    bufer->link_forward(point->ptr->next);

    node<T, NodeMaxSize>* temp_tail = alloc.allocate(1);
    node<T, NodeMaxSize>* temp_head = temp_tail;
    node<T, NodeMaxSize>* temp = temp_tail;
    std::allocator_traits<allocatorNode>::construct(temp_tail);

    while (begin != end) {
        ++capacity;
        if (temp_tail->end == NodeMaxSize / 2) {
            temp_tail = alloc.allocate(1);
            std::allocator_traits<allocatorNode>::construct(temp_tail);
            temp->link_forward(temp_tail);
            ++node_capacity;
        }
        try {
            if constexpr (std::is_assignable_v<T, T>) {
                temp_tail->arr[temp_tail->end] = *begin;
            } else {
                std::construct_at(temp_tail->arr + temp_tail->end, *begin);
            }
        } catch (...) {
            --capacity;
            throw std::runtime_error("Failure at insert");
        }
        ++begin;
        ++temp_tail->end;
    }

    point->ptr->link_forward(temp_head);
    temp_tail->link_forward(bufer);

    while (tail->next) {
        tail = tail->next;
    }

    return const_iterator(temp_tail, temp_tail->end);
}
template <typename T, size_t NodeMaxSize, typename Allocator>
my_iterator<T, NodeMaxSize> unrolled_list<T, NodeMaxSize, Allocator>::insert(
    my_const_iterator<T, NodeMaxSize> point, std::initializer_list<T> init) {
    return insert(point, init.begin(), init.end());
}
template <typename T, size_t NodeMaxSize, typename Allocator>
my_iterator<T, NodeMaxSize> unrolled_list<T, NodeMaxSize, Allocator>::erase(
    my_const_iterator<T, NodeMaxSize> point) noexcept {
    --capacity;
    iterator res(point->erase(point.cur, point.cur));
    while (point.ptr != res.ptr) {
        --node_capacity;
        point.ptr = point.ptr->next;
    }
    return res;
}
template <typename T, size_t NodeMaxSize, typename Allocator>
my_iterator<T, NodeMaxSize> unrolled_list<T, NodeMaxSize, Allocator>::erase(
    my_const_iterator<T, NodeMaxSize> begin, my_const_iterator<T, NodeMaxSize> end) noexcept {
    while (begin != end) {
        if (begin.ptr != end.ptr) {
            if (begin.cur == 0) {
                capacity -= begin.ptr->end;
                --node_capacity;
                iterator temp(begin);
                ++begin;
                temp.ptr->prev->link_forward(temp.ptr->next);
                temp.ptr->next = nullptr;
                alloc.destroy(temp);
            } else {
                begin.ptr->erase(begin);
                ++begin;
            }
        } else {
            begin.ptr->erase(begin.current, end.current);
        }
    }
}

template <typename T, size_t NodeMaxSize, typename Allocator>
void unrolled_list<T, NodeMaxSize, Allocator>::clear() noexcept {
    std::allocator_traits<allocatorNode>::destroy(alloc, head);
    std::allocator_traits<allocatorNode>::construct(alloc, head);
    tail = head;
}
template <typename T, size_t NodeMaxSize, typename Allocator>
void unrolled_list<T, NodeMaxSize, Allocator>::assign(const_iterator begin,
                                                      const_iterator end) noexcept {
    std::allocator_traits<allocatorNode>::destroy(alloc, head);
    std::allocator_traits<allocatorNode>::construct(alloc, head);
    tail = head;
    while (begin != end) {
        tail->push_back(*begin);
        if (tail->next) {
            tail = tail->next;
        }
        ++begin;
    }
}
template <typename T, size_t NodeMaxSize, typename Allocator>
void unrolled_list<T, NodeMaxSize, Allocator>::assign(std::initializer_list<T> init) noexcept {
    this->assign(init.begin(), init.end());
}
template <typename T, size_t NodeMaxSize, typename Allocator>
void unrolled_list<T, NodeMaxSize, Allocator>::assign(size_t n, T value) noexcept {
    std::allocator_traits<allocatorNode>::destroy(alloc, head);
    std::allocator_traits<allocatorNode>::construct(alloc, head, n, value);
    tail = head;
    while (tail->next != nullptr) {
        tail = tail->next;
    }
}
template <typename T, size_t NodeMaxSize, typename Allocator>
void unrolled_list<T, NodeMaxSize, Allocator>::push_back(const T& value) {
    if (tail->end == NodeMaxSize) {
        node<T, NodeMaxSize>* bufer = alloc.allocate(1);
        std::allocator_traits<allocatorNode>::construct(alloc, bufer);
        tail->thread_forward(bufer);
        tail = bufer;
        ++node_capacity;
    }
    if constexpr (std::is_assignable_v<T, T>) {
        try {
            tail->arr[tail->end] = value;
        } catch (...) {
            throw std::runtime_error("");
        }
    } else {
        try {
            std::construct_at(tail->arr + tail->end, value);
        } catch (std::runtime_error) {
            throw std::runtime_error("");
        }
    }
    ++capacity;
    ++tail->end;
}
template <typename T, size_t NodeMaxSize, typename Allocator>
void unrolled_list<T, NodeMaxSize, Allocator>::push_front(const T& value) {
    if (head->end == NodeMaxSize) {
        node<T, NodeMaxSize>* bufer = alloc.allocate(1);
        std::allocator_traits<allocatorNode>::construct(alloc, bufer);
        head->thread_back(bufer);
        ++node_capacity;
        while (tail->next) {
            tail = tail->next;
        }
    }
    *head >> 1;
    ++head->end;
    if constexpr (std::is_assignable_v<T, T>) {
        try {
            head->arr[0] = value;
        } catch (...) {
            *head << 1;
            --head->end;
            throw std::runtime_error("");
        }
    } else {
        try {
            std::construct_at(head->arr, value);
        } catch (...) {
            *head << 1;
            --head->end;
            throw std::runtime_error("");
        }
    }
    ++capacity;
}
template <typename T, size_t NodeMaxSize, typename Allocator>
void unrolled_list<T, NodeMaxSize, Allocator>::pop_back() noexcept {
    while (tail->end == 0) {
        if (tail->prev) {
            node<T, NodeMaxSize>* temp = tail;
            tail = tail->prev;
            temp->prev->link_forward(temp->next);
            temp->next = nullptr;
        } else {
            return;
        }
    }
    tail->pop_back();

    while (tail->next != nullptr) {
        tail = tail->next;
    }
    while (head->prev != nullptr) {
        head = head->prev;
    }
}
template <typename T, size_t NodeMaxSize, typename Allocator>
void unrolled_list<T, NodeMaxSize, Allocator>::pop_front() noexcept {
    while (head->end == 0) {
        if (head->next) {
            node<T, NodeMaxSize>* temp = head;
            head = head->next;
            temp->prev->link_forward(temp->next);
            temp->next = nullptr;
            std::allocator_traits<allocatorNode>::destroy(alloc, temp);
        } else {
            return;
        }
    }
    head->pop_front();

    while (tail->next != nullptr) {
        tail = tail->next;
    }

    while (head->prev != nullptr) {
        head = head->prev;
    }
}
