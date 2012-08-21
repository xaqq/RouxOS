#include	"KBase.hh"
#include	"StaticString.hh"
#include	"PrintEngine.hh"
#include	"CString.hh"
#include	"BLMalloc.hh"

/*
** We get this using -D_PAGE_SIZE=`getconf PAGESIZE`
** There is no reason to change the make command line.
** However, because we're in Epitech, I perform a little useless check
*/
#if defined(_PAGE_SIZE) && (_PAGE_SIZE >= 1024) && (_PAGE_SIZE <= 65536)
# define _PAGE_SIZE_		(_PAGE_SIZE)
#else
# define _PAGE_SIZE_		(4096U)
#endif

#if defined(_ALIGN) && (_ALIGN >= 16) && (_ALIGN <= 32)
# define _ALIGN_		(_ALIGN)
#else
# define _ALIGN_		(16U)
#endif

/*
** Structures
** We use the "Bondary tag" way, as described here:
** ftp://ftp.cs.utexas.edu/pub/garbage/allocsrv.ps (bedtime reading!!11)
** This define a header before the block, and a footer, both containing
** size of the block. this allow super fast freed block merging.
**
** sizeof(t_freedList) == sizeof(t_header)
*/
struct	s_header {
    size_t	prev_s;
    size_t	curr_s;
};

struct s_freedList {
    void*	prev;
    void*	next;
};

/*
** As our pointers are at least 2 bytes aligned, we can use
** the LSB to store a single boolean. What about used/unused ?
*/
# define TVAL(val)	(val & ~FREED_FLAG)
# define IS_FREED(val)	(val & FREED_FLAG)
# define FREED_FLAG	(1 << 0)

# define ALIGN(v)	(((v) % _ALIGN_) ? ((v) + _ALIGN_ - (v) % _ALIGN_) : (v))

# define NEXT_HEADER(w)	((char*)(w) + TVAL((w)->curr_s) + sizeof(t_header))
# define PREV_HEADER(w)	((char*)(w) - TVAL((w)->prev_s) - sizeof(t_header))

# define HEAP_SIZE		(minfos.start - minfos.end)
# define MINIMUM_BLOCK_SIZE	(ALIGN(sizeof(t_header) + sizeof(t_freedList)))

BLMalloc            hack = BLMalloc(KERNEL_HEAP_BOT, KERNEL_HEAP_TOP);
MemoryAllocator		*memAlloc = &hack;

BLMalloc::BLMalloc(size_t bottom, size_t top) : lock(0)
{
    t_header	*h;
    t_freedList	*f;

    assert(bottom < top);

    h = minfos.start = minfos.dibbs = (t_header*)bottom;
    minfos.end = (t_header*)top;
    minfos.end -= 1;
    h->prev_s = 0;
    h->curr_s = (HEAP_SIZE - (2 * sizeof(t_header))) | FREED_FLAG;
    h = (t_header*)(((char*)minfos.end) - sizeof(t_header));
    h->prev_s = (HEAP_SIZE - (2 * sizeof(t_header))) | FREED_FLAG;
    h->curr_s = 0;
    f = (t_freedList*)minfos.dibbs + 1;
    f->prev = 0;
    f->next = 0;
}

/* ****************************************************************************** */
/* ****************************************************************************** */
/* ********************************  MALLOC  ************************************ */
/* ****************************************************************************** */
/* ****************************************************************************** */

// Used in realloc
int	BLMalloc::relink_pointer(t_freedList *f)
{
    t_freedList	*next;
    t_freedList	*prev;

    next = (t_freedList*)(f->next);
    prev = (t_freedList*)(f->prev);
    if (prev)
        prev->next = next;
    if (next)
        next->prev = prev;
    return (42);
}

int	BLMalloc::add_allocable_mem(t_header **h, size_t size)
{
    t_header	*prev_h;

    // FIXME : Someday this will cause problems
    // while (42);
    failure("Heap overflow! No more allocable memory in heap...\n");

    size += MINIMUM_BLOCK_SIZE;
    prev_h = 0;
    if (IS_FREED(h[0]->prev_s)) {
        prev_h = (t_header*)PREV_HEADER(h[0]);
        size -= prev_h->curr_s;
    }
    // size = (size / HEAP_SIZE + 1) * HEAP_SIZE;
    // if ((char*)-1 == sbrk(size))
    //   return (-1);
    // minfos.dibbs = ((char*)minfos.dibbs == (char*)minfos.end) ?
    //   ((t_header*)((char*)minfos.dibbs + size)) : (minfos.dibbs);
    // minfos.end = ((t_header*)((char*)minfos.end + size));
    // minfos.end->curr_s = 0;
    // h[0]->curr_s = size - sizeof(t_header);
    // minfos.end->prev_s = h[0]->curr_s;
    // unlock(&lock);
    // this->free((char*)h[0] + sizeof(t_header));
    // lock(&lock);
    // if (prev_h)
    //   h[0] = prev_h;
    return (42);
}

void	*BLMalloc::set_used_space(t_header *h, size_t size)
{
    t_header	*n_h = (t_header*)0x0;

    if (TVAL(h->curr_s) >= (size + sizeof(t_header) + sizeof(t_freedList)))
    {
        n_h = (t_header*)(((char*)h) + (TVAL(h->curr_s) - size));
        n_h->prev_s = (h->curr_s - (size + sizeof(t_header))) | FREED_FLAG;
        n_h->curr_s = size;
        h->curr_s = n_h->prev_s;
        ((t_header*)(((char*)n_h) + sizeof(t_header) + size))->prev_s = size;
        unlock(&lock);
        return (n_h + 1);
    }
    unlock(&lock);
    return (0);
}

void	*BLMalloc::perfect_match(t_header *h, t_freedList *f)
{
    if (h == minfos.dibbs)
    {
        if (((t_freedList*)((char*)(minfos.dibbs) + sizeof(t_header)))->next == 0)
            minfos.dibbs = minfos.end;
        else
            minfos.dibbs = (t_header*)((char*)((t_freedList*)((char*)minfos.dibbs
                                                              + sizeof(t_header)))->next
                                       - sizeof(t_header));
    }
    h->curr_s &= ~FREED_FLAG;
    ((t_header*)NEXT_HEADER(h))->prev_s = h->curr_s;
    relink_pointer(f);
    unlock(&lock);
    return (f);
}

// Unlock in perfect_match and set_used_space.
void		*BLMalloc::malloc(size_t size)
{
    t_header	*h;
    t_freedList	*f;
    CMTYPE	type;

    if (!size)
        return (0);
    ::lock(&lock);
    size = ALIGN(size);
    h = minfos.dibbs;
    f = (h->curr_s) ? ((t_freedList*)(minfos.dibbs + 1)) : (0);

    if (None == (type = get_usable_chunk(&h, &f, size)))
        if (-1 == add_allocable_mem(&h, size) && unlock(&lock))
            return (0);

    if (type == Perfect)
        return (perfect_match(h, f));
    return (set_used_space(h, size));
}


/* ****************************************************************************** */
/* ****************************************************************************** */
/* *********************************  CALLOC  *********************************** */
/* ****************************************************************************** */
/* ****************************************************************************** */

/*
** calloc() allocates memory for an array of nmemb elements of size bytes
** each and returns a pointer to the allocated memory.  The memory is set
** to zero.  If nmemb or size is 0, then calloc() returns either NULL, or
** a unique pointer value  that  can  later  be  successfully  passed  to
** free().
*/
void	*BLMalloc::calloc(size_t nmemb, size_t size)
{
    char		*ptr;
    size_t	t;
    size_t	c;

    c = 0;
    t = nmemb * size;
    ptr = (char*)malloc(t);
    while (c < t)
        ptr[c++] = 0;
    return (ptr);
}


/* ******************************************************************************** */
/* ******************************************************************************** */
/* *********************************  REALLOC  ************************************ */
/* ******************************************************************************** */
/* ******************************************************************************** */

void	*BLMalloc::realloc_relocate(t_header *c_h, size_t size)
{
    void		*ptr;
    void		*toReturn;

    ptr = (char*)c_h + sizeof(t_header);
    ::unlock(&lock);
    toReturn = malloc(size);
    ::lock(&lock);
    memcpy(toReturn, ptr, c_h->curr_s);
    unlock(&lock);
    this->free(ptr);
    ::lock(&lock);
    return (toReturn);
}

// Called if the next area is freed and has enough space
void	*BLMalloc::merge_perfect_size(t_header *c_h, t_header *n_h, size_t size)
{
    t_header	*n_n_h;

    size = TVAL(c_h->curr_s) + TVAL(n_h->curr_s) + sizeof(t_header);
    n_n_h = (t_header*)((char*)n_h + TVAL(n_h->curr_s) + sizeof(t_header));
    c_h->curr_s = size;
    n_n_h->prev_s = size;
    relink_pointer((t_freedList*)((char*)n_h + sizeof(t_header)));
    return (char*)c_h + sizeof(t_header);
}


// La il faut voir si le bloc suivant est assez grand pour contenir
// l'upgrade de taille
// Il ne devrait pas y avoir de pb d'alignement
// Si il est assez grand, go memcpy, ca reste le plus simple.
void	*BLMalloc::realloc_upgrade(t_header *c_h, t_header *n_h, size_t size)
{
    t_freedList	*n_f;
    t_freedList	*new_f;
    t_header	*new_h;
    t_header	*n_n_h;

    if ((size >= (TVAL(c_h->curr_s) + TVAL(n_h->curr_s) - sizeof(t_header)) &&
         (size <= (TVAL(c_h->curr_s) + TVAL(n_h->curr_s) + sizeof(t_header)))))
        return (merge_perfect_size(c_h, n_h, size));
    if (size > (TVAL(c_h->curr_s) + TVAL(n_h->curr_s) + sizeof(t_header)))
        return (realloc_relocate(c_h, size));
    n_n_h = (t_header*)((char*)n_h + TVAL(n_h->curr_s) + sizeof(t_header));
    c_h->curr_s = TVAL(size);
    n_f = (t_freedList*)((char*)n_h + sizeof(t_header));
    new_h = (t_header*)((char*)c_h + TVAL(size) + sizeof(t_header));
    new_f = (t_freedList*)((char*)new_h + sizeof(t_header));
    new_f->prev = n_f->prev;
    new_f->next = n_f->next;
    new_h->prev_s = TVAL(size);
    new_h->curr_s = TVAL(n_h->curr_s) + TVAL(n_h->prev_s) - TVAL(size);
    new_h->curr_s |= FREED_FLAG;
    n_n_h->prev_s = new_h->curr_s;
    (new_f->prev) ? (((t_freedList*)new_f->prev)->next = new_f) : (c_h = c_h);
    (new_f->next) ? (((t_freedList*)new_f->next)->prev = new_f) : (c_h = c_h);
    return ((char*)c_h + sizeof(t_header));
}


// Il faut le decouper en 2 segments (Juste rajouter une structure
// en alignant bien les size.
// Ensuite tu passe la 2eme partie a free.
void	BLMalloc::realloc_reduce(t_header *c_h, t_header *n_h, size_t size)
{
    t_header	*new_h;

    if ((TVAL(c_h->curr_s) - size) < MINIMUM_BLOCK_SIZE)
        return;
    new_h = (t_header*)((char*)c_h + sizeof(t_header) + size);
    new_h->prev_s = size;
    new_h->curr_s = TVAL(c_h->curr_s) - size - sizeof(t_header);
    c_h->curr_s = size;
    new_h->curr_s |= FREED_FLAG;
    n_h->prev_s = new_h->curr_s;
    ::unlock(&lock);
    this->free((char*)new_h + sizeof(t_header));
    ::lock(&lock);
}

void	*BLMalloc::realloc(void *ptr, size_t size)
{
    t_header	*c_h;
    t_header	*n_h;

    kprint("Realloc call\n");
    if (ptr == 0)
        return (this->malloc(size));
    if (size == 0) {
        this->free(ptr);
        return (0);
    }
    ::lock(&lock);
    c_h = (t_header*)((char*)ptr - sizeof(t_header));
    n_h = (t_header*)((char*)ptr + c_h->curr_s);
    size = ALIGN(size);
    if (size < c_h->curr_s)
        realloc_reduce(c_h, n_h, size);
    else if (size > c_h->curr_s && IS_FREED(n_h->curr_s))
        ptr = realloc_upgrade(c_h, n_h, size);
    else if (size > c_h->curr_s)
        ptr = realloc_relocate(c_h, size);
    ::unlock(&lock);
    return (ptr);
}



/* ******************************************************************************** */
/* ******************************************************************************** */
/* *********************************   FREE   ************************************* */
/* ******************************************************************************** */
/* ******************************************************************************** */

/*
** We assume that the chunk we receive is valid.
** If not... Well, shit will happen !
*/
/*
** Two case: dibbs is after, or dibbs is before
** If dibbs is after, we set the pointer to dibbs
** and set dibbs to the current chunk
** Dibbs is ALWAYS on the first item. So prev = 0;
** If dibbs is before, we insert curr in the middle of the double
** linked list.
*/
void	BLMalloc::assign_ptr(t_freedList *curr)
{
    t_freedList	*d_free;

    d_free = (t_freedList*)((char*)minfos.dibbs + sizeof(t_header));
    if ((char*)minfos.dibbs == (char*)minfos.end) {
        minfos.dibbs = (t_header*)((char*)curr - sizeof(t_header));
        return;
    }
    if ((char*)d_free > (char*)curr) {
        curr->prev = 0;
        curr->next = d_free;
        d_free->prev = curr;
        minfos.dibbs = (t_header*)((char*)curr - sizeof(t_header));
        return;
    }
    while (d_free->next && (((char*)d_free->next) < ((char*)curr)))
        d_free = (t_freedList*)(d_free->next);
    if (d_free->next)
        ((t_freedList*)(d_free->next))->prev = curr;
    curr->next = d_free->next;
    d_free->next = curr;
    curr->prev = d_free;
}

/*
** When we merge two areas (called "top" and "bot"), we need to:
** - change top header (Adding bot size + sizeof header
** - change bot footer as well
** - EVENTUALLY update dibbs (Assume adresses grows up)
** - EVENTUALLY copy top pointer from bot to top.
** IF: pointer != 0, copy next (not prev!!!11)
** PS: Actually this case is ONLY when "merge_prev_area(next)"
** NB: (top + 1) == ((char*)top + sizeof(t_header))
*/
int	BLMalloc::merge_prev_area(t_header **bot_)
{
    t_header	*top;
    t_header	*bot;

    bot = bot_[0];
    if (!TVAL(bot->prev_s) || !IS_FREED(bot->prev_s))
        return (0);
    top = (t_header*)(PREV_HEADER(bot));
    top->curr_s = (TVAL(top->curr_s) + TVAL(bot->curr_s) + sizeof(t_header));
    top->curr_s |= FREED_FLAG;
    ((t_header*)(NEXT_HEADER(bot)))->prev_s = top->curr_s;
    if (minfos.dibbs > top)
        minfos.dibbs = top;
    bot_[0] = top;
    bot->curr_s = 0;
    bot->prev_s = 0;
    return (42);
}

int	BLMalloc::merge_next_area(t_header *top)
{
    t_header	*bot;
    t_freedList	*b_f;
    t_freedList	*t_f;

    bot = (t_header*)(NEXT_HEADER(top));
    t_f = (t_freedList*)((char*)top + sizeof(t_header));
    b_f = (t_freedList*)((char*)bot + sizeof(t_header));
    if (!TVAL(bot->curr_s) || !IS_FREED(bot->curr_s))
        return (0);
    top->curr_s = (TVAL(top->curr_s) + TVAL(bot->curr_s) + sizeof(t_header))
            | FREED_FLAG;
    ((t_header*)(NEXT_HEADER(bot)))->prev_s = top->curr_s;
    if (minfos.dibbs > top)
        minfos.dibbs = top;
    t_f->next = b_f->next;
    if (b_f->next)
        ((t_freedList*)b_f->next)->prev = t_f;
    if (t_f->prev == 0 && (top != minfos.dibbs)) {
        t_f->prev = ((t_freedList*)(bot + 1))->prev;
        ((t_freedList*)(b_f->prev))->next = (top + 1);
    }
    return (42);
}

void		BLMalloc::free(void *ptr)
{
    t_header	*curr_h;
    t_header	*next_h;
    int		lookForPointers;

    if (!ptr)
        return;

    if (!(((char*)ptr > (char*)minfos.start) && ((char*)ptr < (char*)minfos.end))) {
        kprint(StaticString("Error: free() on out-of-scope pointer: 0x")
               .append(StaticString::number(ptr)).append("\n"));
        failure("Trying to free() an out-of-heap pointer\n");
    }
    //assert(((char*)ptr > (char*)minfos.start) && ((char*)ptr < (char*)minfos.end));

    lookForPointers = 0;
    curr_h = (t_header*)(((char*)ptr) - sizeof(t_header));
    next_h = (t_header*)(((char*)ptr) + TVAL(curr_h->curr_s));
    ::lock(&lock);
    ((t_freedList*)ptr)->prev = 0;
    ((t_freedList*)ptr)->next = 0;
    next_h->prev_s |= FREED_FLAG;
    curr_h->curr_s |= FREED_FLAG;
    if (merge_prev_area(&curr_h))
        ++lookForPointers;
    if (merge_next_area(curr_h))
        ++lookForPointers;
    if (!lookForPointers)
        assign_ptr(((t_freedList*)ptr));
    ::unlock(&lock);
}


/* ****************************************************************************** */
/* ****************************************************************************** */
/* *********************************   ALL   ************************************ */
/* ****************************************************************************** */
/* ****************************************************************************** */

/*
** - Si on trouve la taille exacte on retourne cash avec Perfect
** - Si on arrive a la fin de la liste on retourne None
** (Et on positionne f sur le dernier elem free)
** - Sinon on renvoit la plus petite taille trouvee.
*/
BLMalloc::CMTYPE	BLMalloc::get_usable_chunk(t_header **h, t_freedList **f, size_t size)
{
    t_header	*s_h;
    t_header	*tmp;

    s_h = 0;
    while (f[0])
    {
        tmp = (t_header*)((char*)f[0] - sizeof(t_header));
        if (TVAL(tmp->curr_s) == size)
            if ((h[0] = tmp) || 1)
                return (Perfect);
        if (TVAL(tmp->curr_s) >= (size + sizeof(t_header) + sizeof(t_freedList)))
            if (!s_h || (s_h->curr_s > tmp->curr_s))
                s_h = tmp;
        f[0] = (t_freedList*)(f[0]->next);
    }
    if (s_h)
    {
        h[0] = s_h;
        f[0] = ((t_freedList*)(char*)s_h + sizeof(t_header));
        return (Bigger);
    }
    f[0] = (t_freedList*)(minfos.end + 1);
    h[0] = minfos.end;
    return (None);
}

void *operator new(size_t size)		{ return malloc(size); }
void *operator new[](size_t size)	{ return malloc(size); }
void operator delete(void *p)		{ free(p); }
void operator delete[](void *p)		{ free(p); }


/* ****************************************************************************** */
/* ****************************************************************************** */
/* ********************************   DEBUG   *********************************** */
/* ****************************************************************************** */
/* ****************************************************************************** */

/* Dump all memory area reserved */
void		BLMalloc::showAllocatedMemory(void)
{
    t_header	*it;
    void		*curr;
    void		*next;


    kprint(StaticString("Heap: 0x")
           .append(StaticString::number((char*)minfos.start))
           .append(" - 0x")
           .append(StaticString::number(((char*)minfos.end + sizeof(t_header))))
           .append("\n"));
    it = (t_header*)minfos.start;
    kprint(StaticString("Dibbs: 0x").append(StaticString::number((char*)minfos.dibbs)).append("\n"));
    while (it->curr_s)
    {
        curr = (char*)it + sizeof(t_header);
        next = (char*)it + TVAL(it->curr_s) + sizeof(t_header);

        kprint(StaticString("Curr: 0x").append(StaticString::number((char*)curr)).append("\n"));
        kprint(StaticString("Next: 0x").append(StaticString::number((char*)next)).append("\n"));
        kprint(StaticString("Val: 0x").append(StaticString::number(TVAL(it->curr_s))).append("\n"));
        while (42);


        if (!(it->curr_s & FREED_FLAG)) {
            kprint("Freed\n");
            kprint(StaticString("0x").append(StaticString::number(curr)).append(" - 0x")
                   .append(StaticString::number(next)).append(" : ")
                   .append(StaticString::number(it->curr_s)).append(" octets\n"));
        } else {
            kprint("Not Freed\n");
// FIXME: Wrong pointer (?????)
// Pointers must be 0x1000000 - 0x2000000 and one -the first- was 0xDFFFF8
// ---- Uncommenting the next commented line make processor bug.
            kprint(StaticString("0x").append(StaticString::number(curr))
                   .append(" - 0x").append(StaticString::number(next))
                   .append(" : ").append(StaticString::number(TVAL(it->curr_s)))
                   .append(" octets freed - next : 0x")
                   .append(StaticString::number(((t_freedList*)(curr))->prev))
                   .append(" - prev : 0x")
                   .append(StaticString::number(((t_freedList*)(curr))->next))
                   .append("\n"));
        }
        it = (t_header*)(NEXT_HEADER(it));
    }
}


size_t		BLMalloc::check_linked_list(void)
{
    t_freedList	*p;
    t_freedList	*prev;
    size_t	count;

    count = 0;
    prev = 0;
    p = (t_freedList*)(minfos.dibbs + 1);
    kprint(StaticString("Check start from {0x")
           .append(StaticString::number((size_t)minfos.dibbs, 16)).append("}\n"));
    while ((char*)p->next != 0)
    {
        if (!(IS_FREED(((t_header*)((char*)p - sizeof(t_header)))->curr_s)))
            kprint(StaticString("[").append(StaticString::number(++count))
                   .append("] Error: {0x")
                   .append(StaticString::number((size_t)p, 16))
                   .append("} isn't freed! prev is {0x")
                   .append(StaticString::number((size_t)p->prev, 16))
                   .append("}, from {")
                   .append(StaticString::number((size_t)prev, 16)).append("}\n"));
        if (prev != p->prev)
            kprint(StaticString("[").append(StaticString::number(++count))
                   .append("] Error at {0x")
                   .append(StaticString::number((size_t)p, 16))
                   .append("}, prev is {0x")
                   .append(StaticString::number((size_t)p->prev, 16))
                   .append("}, expected {")
                   .append(StaticString::number((size_t)prev, 16)).append("}\n"));
        prev = p;
        p = (t_freedList*)(p->next);
    }
    if (prev != p->prev)
        kprint(StaticString("[").append(StaticString::number(++count))
               .append("]Error at {0x").append(StaticString::number((size_t)p, 16))
               .append("}, prev is {0x")
               .append(StaticString::number((size_t)p->prev, 16))
               .append("}, expected {").append(StaticString::number((size_t)prev, 16))
               .append("}\n"));
    kprint(StaticString("Check end at {")
           .append(StaticString::number((size_t)p, 16))
           .append("}, ")
           .append(StaticString::number(count))
           .append(" error(s)\n"));
    return (count);
}

size_t		BLMalloc::check_header(void)
{
    t_header	*curr;
    t_header	*next;
    size_t	count;

    count = 0;
    curr = minfos.dibbs;
    kprint("Checking headers...\n");
    while (TVAL(curr->curr_s))
    {
        next = (t_header*)NEXT_HEADER(curr);
        if (curr->curr_s != next->prev_s)
            kprint(StaticString("Error [")
                   .append(StaticString::number(++count))
                   .append("]: chunk {0x")
                   .append(StaticString::number((size_t)curr, 16))
                   .append("} set curr size at {0x")
                   .append(StaticString::number((size_t)curr->curr_s, 16))
                   .append("} while {0x")
                   .append(StaticString::number((size_t)next, 16))
                   .append("} have prev size of {")
                   .append(StaticString::number((size_t)next->prev_s, 16))
                   .append("}\n"));
        curr = next;
    }
    kprint(StaticString("Headers check done, ").append(StaticString::number(count)
                                                       .append(" error(s)\n")));
    return (count);
}
