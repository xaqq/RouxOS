#ifndef __KERNEL_HH__
# define __KERNEL_HH__

/*
 * TODO: A pointer class for shared pointer + one for shared data
 *
 */

#include	"KBase.hh"	/* MUST be first include everywhere */
#include	"CString.hh"		/* strcpy, strlen... */

#include	"MemoryManager.hh"

#include	"Maths.hh"		/* oom, conversion... */

#include	"String.hh"		/* Class String */
#include	"StaticString.hh"	/* String that use static buffer */
#include	"Boot.hh"		/* namespace boot */
#include	"PrintEngine.hh"	/* Class kprint */

void	kmain(void*);

#endif /* !__KERNEL_HH__ */
