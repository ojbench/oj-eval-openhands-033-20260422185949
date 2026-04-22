#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include <cstddef>
#include <utility>

namespace sjtu {
/**
 * A minimal doubly-linked list similar to std::list used for testing.
 */
template <typename T> class list {
protected:
  struct node {
    T *data;
    node *prev;
    node *next;
    node() : data(nullptr), prev(this), next(this) {}
    explicit node(const T &v) : data(new T(v)), prev(nullptr), next(nullptr) {}
    ~node() { if (data) { delete data; data = nullptr; } }
  };

protected:
  node *sentinel; // circular sentinel node; sentinel->data == nullptr
  size_t n;

  node *insert(node *pos, node *cur) {
    // link cur before pos
    cur->next = pos;
    cur->prev = pos->prev;
    pos->prev->next = cur;
    pos->prev = cur;
    return cur;
  }

  node *erase(node *pos) {
    // unlink pos from the list; caller responsible for deleting node
    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;
    pos->prev = pos->next = nullptr;
    return pos;
  }

public:
  class const_iterator;
  class iterator {
  private:
    node *ptr;
    friend class list<T>;
    friend class const_iterator;
    explicit iterator(node *p) : ptr(p) {}

  public:
    iterator() : ptr(nullptr) {}
    iterator operator++(int) { iterator tmp = *this; if (ptr) ptr = ptr->next; return tmp; }
    iterator &operator++() { if (ptr) ptr = ptr->next; return *this; }
    iterator operator--(int) { iterator tmp = *this; if (ptr) ptr = ptr->prev; return tmp; }
    iterator &operator--() { if (ptr) ptr = ptr->prev; return *this; }

    T &operator*() const { return *(ptr->data); }
    T *operator->() const noexcept { return ptr->data; }

    bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }
    bool operator==(const const_iterator &rhs) const { return ptr == rhs.ptr; }
    bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
    bool operator!=(const const_iterator &rhs) const { return ptr != rhs.ptr; }
  };

  class const_iterator {
  private:
    const node *ptr;
    friend class list<T>;
    explicit const_iterator(const node *p) : ptr(p) {}

  public:
    const_iterator() : ptr(nullptr) {}
    const_iterator(const iterator &it) : ptr(it.ptr) {}
    const_iterator operator++(int) { const_iterator tmp = *this; if (ptr) ptr = ptr->next; return tmp; }
    const_iterator &operator++() { if (ptr) ptr = ptr->next; return *this; }
    const_iterator operator--(int) { const_iterator tmp = *this; if (ptr) ptr = ptr->prev; return tmp; }
    const_iterator &operator--() { if (ptr) ptr = ptr->prev; return *this; }

    const T &operator*() const { return *(ptr->data); }
    const T *operator->() const noexcept { return ptr->data; }

    bool operator==(const const_iterator &rhs) const { return ptr == rhs.ptr; }
    bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }
    bool operator!=(const const_iterator &rhs) const { return ptr != rhs.ptr; }
    bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
  };

  list() : sentinel(new node()), n(0) {}

  list(const list &other) : list() {
    for (auto it = other.cbegin(); it != other.cend(); ++it) push_back(*it);
  }

  virtual ~list() { clear(); delete sentinel; sentinel = nullptr; }

  list &operator=(const list &other) {
    if (this == &other) return *this;
    clear();
    for (auto it = other.cbegin(); it != other.cend(); ++it) push_back(*it);
    return *this;
  }

  const T &front() const { return *(sentinel->next->data); }
  const T &back() const { return *(sentinel->prev->data); }

  iterator begin() { return iterator(sentinel->next); }
  const_iterator cbegin() const { return const_iterator(sentinel->next); }

  iterator end() { return iterator(sentinel); }
  const_iterator cend() const { return const_iterator(sentinel); }

  virtual bool empty() const { return n == 0; }
  virtual size_t size() const { return n; }

  virtual void clear() {
    node *cur = sentinel->next;
    while (cur != sentinel) {
      node *nxt = cur->next;
      erase(cur);
      delete cur;
      cur = nxt;
    }
    sentinel->next = sentinel->prev = sentinel;
    n = 0;
  }

  virtual iterator insert(iterator pos, const T &value) {
    node *p = pos.ptr;
    node *cur = new node(value);
    insert(p, cur);
    ++n;
    return iterator(cur);
  }

  virtual iterator erase(iterator pos) {
    node *p = pos.ptr;
    node *nxt = p->next;
    erase(p);
    delete p;
    --n;
    return iterator(nxt);
  }

  void push_back(const T &value) { insert(iterator(sentinel), value); }
  void pop_back() { erase(iterator(sentinel->prev)); }
  void push_front(const T &value) { insert(iterator(sentinel->next), value); }
  void pop_front() { erase(iterator(sentinel->next)); }
};

} // namespace sjtu

#endif // SJTU_LIST_HPP
