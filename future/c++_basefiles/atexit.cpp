/*
 * Needed under GCC (not VS)
 */

void *__dso_handle;

int __cxa_atexit(void (*destructor)(void *), void *arg, void *dso);
void __cxa_finalize(void *f);
