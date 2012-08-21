#ifndef KBASE_HH__
# define KBASE_HH__

#ifndef _ENDIAN
# define _ENDIAN		0 // Little
#endif

#ifndef _PTRSIZE
# define _PTRSIZE		32
#endif

#define     STRINGIZE(x)    #x
#define     PASTTOKEN(x, y) x##y

#include	"Types.hh"
#include	"Debug.hh"		/* assert */

#define	MIN(x, y)		((x > y) ? (y) : (x))
#define	MAX(x, y)		((x < y) ? (y) : (x))
#define	ABS(x)			((x < 0) ? (-x) : (x))

#define KERNEL_TEXT_TOP		0x500000
#define KERNEL_TEXT_BOT		0x100000

#define KERNEL_STACK_TOP	0x1000000
#define KERNEL_STACK_BOT	KERNEL_TEXT_TOP

#define KERNEL_HEAP_TOP		0x2000000
#define KERNEL_HEAP_BOT		KERNEL_STACK_TOP

/*
 * We use Linux EXIT_(SUCCESS/FAILURE) values
 * The aim is also that programmers can if (fun()) { error_code }
 * Last but not least, some ABI define SUCCESS as != 0
 */
# define SUCCESS	0
# define FAILURE	(!SUCCESS)


/* ecrit un octet sur un port */
#define outb(port, value)       asm volatile ("outb %%al, %%dx" :: "d" (port), "a" (value))

/* ecrit un octet sur un port et marque une temporisation  */
#define outbp(port, value)      asm volatile ("outb %%al, %%dx; jmp 1f; 1:" :: "d" (port), "a" (value))

/* lit un octet sur un port */
#define inb(port, var)          asm volatile ("inb %%dx, %%al" : "=a" (var) : "d" (port))

/*
 * Concurrency.
 * Move in a separate header once we get something good.
 */
void	lock(int *);
int     unlock(int *);

#endif /* !__KBASE_HH__ */
