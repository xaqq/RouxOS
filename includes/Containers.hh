#ifndef CONTAINERS_HH__
#define CONTAINERS_HH__

#include    "Types.hh"
#include    "Iterators.hh"

/*
 * Those are standart containers.
 * They must provide every access primitives. No more.
 *
 * We have two access primitives: index based and iterator based.
 * We should prefer iterator-based way.
 */

/* ********************************************************************* */
/* ***************************** DEQUE ********************************* */
/* ********************************************************************* */
template <typename T>
class DequeNode {
public:
    DequeNode(const T& v, DequeNode<T>* p = 0, DequeNode<T>* n = 0)
        : val(v), prev(p), next(n)	{}
    T               val;
    DequeNode<T>*   prev;
    DequeNode<T>*   next;
};

template <typename T>
class Deque : public HasMutableIterator<T, Deque, DequeNode> {
public:
    typedef MutableIterator<T, Deque, DequeNode>    Iterator;
    //typedef ConstantIterator<T, Deque, DequeNode>    ConstIterator;

    Deque(void)                 { _first = _mid = _last = 0; _size = 0; }
    Deque(const Deque& o)       { _first = _mid = _last = 0; _size = 0; this->append(o); }
    Deque(Deque&& o)
    {
        _first = o._first;
        _mid = o._mid;
        _last = o._last;
        _size = o._size;
        o._first = o._mid = o._last = o._size = 0;
    }

    T&          atIndex(size_t i)                   { return (findNodeAt(i)->val); }

    Deque<T>&   insertAt(size_t p, const T& val)    { insertBeforeNode(findNodeAt(p), val); return *this; }
    Deque<T>&   deleteAt(size_t pos)
    {
        if (!_size)
            failure("Trying to delete on an empty Deque\n");
        else if (pos >= _size)
            failure("Trying to delete above last Deque's element\n");
        else
            deleteNode(findNodeAt(pos));
        return *this;
    }

    // Iterator is still valid after (modified by class)
    Deque<T>&   insertAt(Iterator& it, const T& val)
    {
        insertBeforeNode(it._node, val);
        it._node = it._node->prev;
        return *this;
    }
    Deque<T>&   deleteAt(Iterator& it)
    {
        DequeNode<T>*   toDel = it._node;
        it._node = it._node->next;
        deleteNode(toDel);
        return *this;
    }

    Deque<T>&   append(const T& val)                { insertAfterNode(_last, val); return *this; }
    Deque<T>&   append(const Deque<T>& o)
    {
        DequeNode<T>*   other = o._first;
        while (other) {
            insertAfterNode(_last, other->val);
            other = other->next;
        }
        return *this;
    }

    virtual Iterator    begin(void)   { return Iterator(_first); }
    virtual Iterator    end(void)     { return Iterator(_last); }


    size_t      count(void) const                   { return _size; }

protected:
    DequeNode<T>*   _first;
    DequeNode<T>*   _mid;
    DequeNode<T>*   _last;
    size_t          _size;

    DequeNode<T>*  findNodeAt(size_t i)
    {
        DequeNode<T>*   n;
        size_t          realPos;
        bool            incr;

        if (i >= _size)
            return _last;
        if (i < _size / 4) {
            n = _first;
            incr = true;
            realPos = 0;
        } else if (i <= _size / 2) {
            n = _mid;
            incr = false;
            realPos = _size / 2;
        } else if (i <= (_size / 4) * 3) {
            n = _mid;
            incr = true;
            realPos = _size / 2;
        } else {
            n = _last;
            incr = false;
            realPos = _size;
        }
        while (realPos != i) {
            if (incr) {
                n = n->next;
                ++realPos;
            } else {
                n = n->prev;
                --realPos;
            }
        }
        return (n);
    }
    void        deleteNode(DequeNode<T>* n)
    {
        if (n == _first) {
            _first = _first->next;
            delete _first->prev;
            if (_size > 1)
                _first->prev = 0;
            else
                _first = _mid = _last = 0;
        } else if (n == _last) {
            _last = _last->prev;
            delete _last->next;
            _last->next = 0;
        } else {
            n->prev->next = n->next;
            n->next->prev = n->prev;
            delete n;
        }
        if ((_size > 1) && (_size % 2))
            _mid = _mid->prev;
    }
    void        insertBeforeNode(DequeNode<T>* n, const T& val)
    {
        if (!_size) {
            _first = _mid = _last = new DequeNode<T>(val);
        } else if (n == _first) {
            _first = _first->prev = new DequeNode<T>(val, 0, _first);
        } else {
            DequeNode<T>*  toInsert = new DequeNode<T>(val, n->prev, n);
            n->prev = toInsert;
            toInsert->prev->next = toInsert;
        }
        ++_size;
        if (_size > 2 && !(_size % 2))
            _mid = _mid->next;
    }
    void        insertAfterNode(DequeNode<T>* n, const T& val)
    {
        if (!_size) {
            _first = _mid = _last = new DequeNode<T>(val);
        } else if (n == _last) {
            _last =_last->next = new DequeNode<T>(val, _last, 0);
        } else {
            DequeNode<T>*   toInsert = new DequeNode<T>(val, n->prev, n);
            n->prev = toInsert;
            toInsert->prev->next = toInsert;
        }
        ++_size;
        if (_size > 2 && !(_size % 2))
            _mid = _mid->next;
    }
};

/* ********************************************************************* */
/* ***************************** PAIR ********************************** */
/* ********************************************************************* */
template<typename First, typename Second>
class Pair {
public:
    Pair(void)      {}
    Pair(const First& f, const Second& s)
        : _first(f), _second(s)   {}

    First&      first(void)     { return _first; }
    Second&     second(void)    { return _second; }

protected:
    First       _first;
    Second      _second;
};


#endif /* !CONTAINERS_HH__ */
