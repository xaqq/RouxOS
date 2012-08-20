#include	"KBase.hh"
#include	"Maths.hh"

size_t		orderOfMagnitude(size_t n, int base)
{
  size_t f = base;
  size_t r = 1;

  while (n / f) {
    f *= base;
    ++r;
  }
  return (r);
}

static size_t	powImp(size_t val, size_t to, size_t init)
{
  if (1 == to)
    return (val);
  return (powImp(val * init, to - 1, init));
}

size_t		pow(size_t i, size_t to)
{
  if (to == 0)
    return (1);
  return (powImp(i, to, i));
}
