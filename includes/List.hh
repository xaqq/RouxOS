#ifndef LIST_HH__
#define LIST_HH__

#include    "Containers.hh"

//template <typename T>
//class List {
//public:
//    List(void) : _d(0)	{}
//    List(const List<T>& v)
//    {
//        Deque<T>   *p = v._d;
//        while (p != v._end)
//            this->append(p->val);
//    }
//    List& operator=(List& v)
//    {
//        Deque<T>   *p = v._d;
//        while (p != v._end)
//            this->append(p->val);
//    }

//    List(List&& v)
//    {
//        this->_d = v._d;
//        v._d = 0;
//    }
//    List& operator=(List&& v)
//    {
//        this->_d = v._d;
//        v._d = v._end = v._mid = 0;
//        return *this;
//    }

//    const T&	first(void) const	{ return (_d->val); }
//    const T&	last(void) const	{ return (_end->val); }

//    T&		at(size_t position)
//    {
//        size_t          currPos = 0;
//        ssize_t         incr = 1;
//        Deque<T>*  p = _d;

//        assert(position < _s);
//        while (currPos < position) {
//            p = p->next;
//            (incr) ? (++currPos) : (--currPos);
//        }
//        return (p->val);
//    }

//    List<T>&    append(const T& v)
//    {
//        if (!_end)
//            _d = _mid = _end = new Deque<T>(v);
//        else {
//            _end = _end->next = new Deque<T>(v, _end);
//            if ((_s > 2) && (_s % 2))
//                _mid = _mid->next;
//        }
//        ++_s;
//        return *this;
//    }

//    size_t	count(void) const	{ return _s; }

//    const T&	operator[](size_t p) const
//    { return this->at(p); }
//    T&		operator[](size_t p)
//    { return this->at(p); }

//    // List<T>	operator+(const T& o);
//    // List<T>	operator+(const List<T>& o);

//    List<T>&	operator+=(const T& o)              { return this->append(o); }
//    List<T>&	operator+=(const List<T>& o)
//    {
//        Deque<T>   *p = o._d;
//        while (p != o._end)
//            this->append(p->val);
//        return *this;
//    }

//protected:
//    Deque<T>*	_d;
//};

#endif /* !LIST_HH__ */
