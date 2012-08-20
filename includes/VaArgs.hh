#ifndef VAARGS_HH__
#define VAARGS_HH__

typedef void*	va_list;
//#define va_start(ap, last)	(ap = (void*)((char*)&last + sizeof(last)))
//#define va_arg(ap, type)	*((type*)ap += 1)
//#define va_copy(dest, src)	assert(false)
//#define va_end(ap)


// void va_start(va_list ap, last);
// type va_arg(va_list ap, type);
// void va_end(va_list ap);
// void va_copy(va_list dest, va_list src);

#endif /* !VAARGS_HH__ */
