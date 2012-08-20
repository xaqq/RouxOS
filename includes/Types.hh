#ifndef __TYPES_HH__
# define __TYPES_HH__

#ifndef KBASE_HH__
#error "types.hh is not standalone. Include kbase.hh instead."
#endif

typedef unsigned char	u8;
typedef unsigned short	u16;
typedef unsigned int	u32;
typedef unsigned char	uchar;
typedef char			s8;
typedef short			s16;
typedef int             s32;

#if _PTRSIZE == 64
typedef long unsigned int	u64;
typedef long int            s64;
typedef u64                 ptr_t;
typedef u64                 size_t;
typedef s64                 ssize_t;

# else
typedef u32			ptr_t;
typedef u32			size_t;
typedef s32			ssize_t;
# endif

#endif /* !__TYPES_HH__ */
