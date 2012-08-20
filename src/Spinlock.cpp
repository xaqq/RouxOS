/*
** type __sync_lock_test_and_set (type *ptr, type value, ...)
** Writes value into ptr and return prev value of ptr.
*/
void	lock(int *_l)
{
  while (__sync_lock_test_and_set(_l, 1));
}

/*
** void __sync_lock_release (type *ptr, ...)
** Write 0 into ptr.
*/
int	unlock(int *_l)
{
  __sync_lock_release(_l);
  return (42);
}
