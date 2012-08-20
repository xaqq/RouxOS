#ifndef ITERATORS_HH__
#define ITERATORS_HH__

// FIXME: _node->next --> _node->next() ?
template <typename Val,
          template <typename V> class Cont,
          template <typename V> class ContNode>
class MutableIterator {
public:
    // Only Container can construct the object, other must use copy-constructor.
    friend class Cont;

    // Provided for Iterator j = obj.begin()
    MutableIterator(void) : _node(0)                            {}
    MutableIterator&    operator=(const MutableIterator& o)     { this->_node = o._node; return *this; }

    // FIXME: Should be protected, but friend class fail?
    MutableIterator(ContNode<Val> *p)   { _node = p; }


    MutableIterator(const MutableIterator& o)   { this->_node = o._node; }
    virtual ~MutableIterator(void)              {}

    inline MutableIterator<Val, Cont, ContNode>&    operator++(void)    { return operator +=(1); }
    inline MutableIterator<Val, Cont, ContNode>&    operator+=(ssize_t off)
    {
        assert(_node != 0);
        if (off < 0)
            return (operator-=(-1 * off));
        for (size_t i = 0; i < (size_t)off; ++i)
            _node = _node->next;
        return *this;
    }

    inline MutableIterator<Val, Cont, ContNode>&    operator--(void)     { return operator -=(1); }
    inline MutableIterator<Val, Cont, ContNode>&    operator-=(ssize_t off)
    {
        assert(_node->prev != 0);
        if (off < 0)
            return (operator+=(-1 * off));
        for (size_t i = 0; i < (size_t)off; ++i)
            _node = _node->prev;
        return *this;
    }

    inline Val&         operator*(void)     { return _node->val; }

    inline bool         operator==(const MutableIterator& o)    { return (_node == o._node); }
    inline bool         operator!=(const MutableIterator& o)    { return (_node != o._node); }

    inline bool         atEnd(void) const           { return (_node->next == 0); }
    inline bool         atBeginning(void) const     { return (_node->prev == 0); }

    inline bool         isValid(void) const         { return (_node); }

    // FIXME: _node should be protected but MapMutableIterator can't access it.
//protected:
    ContNode<Val>*      _node;
};

template <typename Val,
          template <typename V> class Cont,
          template <typename V> class ContNode>
class HasMutableIterator {
public:
    virtual ~HasMutableIterator(void)              {}

    virtual MutableIterator<Val, Cont, ContNode>   begin(void) = 0;
    virtual MutableIterator<Val, Cont, ContNode>   end(void) = 0;
};


/* ********************************************************************* */
/* ************************* MAP ITERATORS ***************************** */
/* ********************************************************************* */

#ifdef MAP_HH__
#include    "Containers.hh"

template <typename Key, typename Value>
class MapMutableIterator: public MutableIterator<Pair<Key, Value>, Deque, DequeNode> {
public:
    MapMutableIterator(MutableIterator<Pair<Key, Value>, Deque, DequeNode>&& o)   { this->_node = o._node; }

    inline Key&     key(void)       { return this->_node->val.first(); }
    inline Value&   value(void)     { return this->_node->val.second(); }

    inline Value&   operator*(void)     { return this->_node->val.first(); }
};
#endif

#endif /* !ITERATORS_HH__ */
