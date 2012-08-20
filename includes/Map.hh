#ifndef MAP_HH__
#define MAP_HH__

#include    "Types.hh"
#include    "Iterators.hh"
#include    "Containers.hh"

//#include    "PrintEngine.hh"
//#include    "StaticString.hh"

template<typename Key, typename Value>
class Map {
public:
    typedef MapMutableIterator<Key, Value>  Iterator;

    Map(void) : items(new Deque<Pair<Key, Value> >) {}
    Map(const Map<Key, Value>& o) : items(new Deque<Pair<Key, Value> >(o.items))  {}
    Map(const Map<Key, Value>&& o)  { items = o.items; o.items = 0; }

    Iterator    begin(void)         { return Iterator(items->begin()); }
    Iterator    end(void)           { return Iterator(items->end()); }

    Iterator    operator[](const Key& k)
    {
        Iterator    i = this->begin();
        while (i.isValid()) {
            if (i.key() == k)
                break;
            ++i;
        }
        return i;
    }

    Map<Key, Value>&    insert(const Key& k, const Value& v)
    {
        Iterator    i = this->begin();

        while (i.isValid()) {
            if (i.key() >= k)
                break;
            ++i;
        }
        if (i.isValid() && i.key() == k)
            *i = v;
        else if (i.isValid())
            items->insertAt(i, Pair<Key, Value>(k, v));
        else
            items->append(Pair<Key, Value>(k, v));
        return *this;
    }

    size_t              count(void) const   { return items->count(); }

protected:
    Deque<Pair<Key, Value> >*  items;
};

#endif /* !MAP_HH__ */

