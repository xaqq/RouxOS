#ifndef MEMORYMANAGER_HH__
#define MEMORYMANAGER_HH__

class MemoryAllocator {
public:
  virtual void*	malloc(size_t size)                 = 0;
  virtual void*	realloc(void *ptr, size_t size)		= 0;
  virtual void	free(void *ptr)                     = 0;
  virtual void*	calloc(size_t nmemb, size_t size)	= 0;
  virtual void	showAllocatedMemory(void)		{}
};

extern MemoryAllocator	*memAlloc;

inline void*	malloc(size_t s)            { return (memAlloc->malloc(s)); }
inline void*	realloc(void* p, size_t s)	{ return (memAlloc->realloc(p, s)); }
inline void     free(void* p)               { return (memAlloc->free(p)); }
inline void*	calloc(size_t n, size_t s)	{ return (memAlloc->calloc(n, s)); }

void	*operator	new(size_t size);
void	*operator	new[](size_t size);
void	operator	delete(void *p);
void	operator	delete[](void *p);


#endif /* !MEMORYMANAGER_HH__ */
