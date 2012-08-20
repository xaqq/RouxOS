#ifndef BLMALLOC_HH__
#define BLMALLOC_HH__

#include	"MemoryManager.hh"

/*
** We need a pointer to the first free place in our program
** It allow us to don't walk through our entire list every
** time we want a malloc.
** It must be updated :
** - Everytime we do a malloc IF there's no empty chunk before.
** - Everytime we free a pointer under this var
**
** We also need a pointer to the current break of the program.
** This struct is located in constructor.o object.
*/

class BLMalloc : public MemoryAllocator {
public:
  BLMalloc(size_t from, size_t to);
  
  virtual void*	malloc(size_t size);
  virtual void*	realloc(void *ptr, size_t size);
  virtual void	free(void *ptr);
  virtual void*	calloc(size_t nmemb, size_t size);
  virtual void	showAllocatedMemory(void);

protected:

  typedef struct s_freedList	t_freedList;
  typedef struct s_header       t_header;

  typedef enum {
    None = -1,
    Perfect = 0,
    Bigger = 1
  } CMTYPE;

  typedef struct s_mallocInfos {
    t_header	*start;
    t_header	*dibbs;
    t_header	*end;
  } t_mallocInfos;


  t_mallocInfos	minfos;
  int           lock;

  int		relink_pointer(BLMalloc::t_freedList *f);
  
  BLMalloc::CMTYPE
  get_usable_chunk(BLMalloc::t_header **h,
           BLMalloc::t_freedList **f, size_t size);

  /* Malloc */
  int	add_allocable_mem(BLMalloc::t_header **h, size_t size);
  void	*set_used_space(BLMalloc::t_header *h, size_t size);
  void	*perfect_match(BLMalloc::t_header *h,
               BLMalloc::t_freedList *f);

  /* realloc */
  void	*realloc_relocate(BLMalloc::t_header *c_h,
			  size_t size);
  void	*merge_perfect_size(BLMalloc::t_header *c_h,
                BLMalloc::t_header *n_h,
			    size_t size);
  void	*realloc_upgrade(BLMalloc::t_header *c_h,
             BLMalloc::t_header *n_h, size_t size);
  void	realloc_reduce(BLMalloc::t_header *c_h,
               BLMalloc::t_header *n_h, size_t size);

  /* Free */
  void	assign_ptr(BLMalloc::t_freedList *curr);
  int	merge_prev_area(BLMalloc::t_header **bot_);
  int	merge_next_area(BLMalloc::t_header *top);

  /* Debug */
  size_t	check_linked_list(void);
  size_t	check_header(void);
};


#endif /* !BLMALLOC_HH__ */
