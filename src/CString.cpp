#include	"KBase.hh"
#include	"CString.hh"
#include	"Maths.hh"
#include	"MemoryManager.hh"

const char baseChar[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			 'A', 'B', 'C', 'D', 'E', 'F'};
const char baseCharMin[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			    'a', 'b', 'c', 'd', 'e', 'f'};

void*	bzero(void* ptr, size_t size)
{
  for (size_t i = 0; i < size; ++i)
    ((char*)ptr)[i] = 0;
  return (ptr);
}

void	*memcpy(void* to, const void* from, size_t size)
{
  size_t	i;

  for (i = 0; i < size; ++i)
    ((char*)to)[i] = ((char*)from)[i];
  return (to);
}

size_t	strlen(const char* str)
{
  size_t	i = 0;

  assert(str != 0);
  while (str[i]) ++i;
  return (i);
}

void	memset(void *ptr, char val, size_t size)
{
  size_t	i;

  for (i = 0; i < size; i++)
    ((char*)ptr)[i] = val;
}

size_t	strcpy(char* to, const char* f)
{
  size_t	i;

  for (i = 0; f[i]; ++i)
    to[i] = f[i];
  return (i);
}

size_t	strncpy(char* to, const char* from, size_t l)
{
  size_t	i;

  for (i = 0; from[i] && i < l; ++i)
    to[i] = from[i];
  return (i);
}
  
char*	strdup(const char* f)
{
  size_t	s = strlen(f);
  char*		p = (char*)malloc(s);

  return ((char*)memcpy(p, f, s));
}

char*	strndup(const char* f, size_t s)
{
  char*		p = (char*)malloc(s);
  return ((char*)memcpy(p, f, s));
}

int	strcmp(const char* c1, const char* c2)
{
  size_t	s1 = strlen(c1);
  size_t	s2 = strlen(c2);
  size_t	sg = MIN(s1, s2);
  int		v = strncmp(c1, c2, sg);

  return ((v) ? (v) : ((sg == s1) ? (1) : (-1)));
}

int	strncmp(const char* c1, const char* c2, size_t s)
{
  for (size_t i = 0; i < s; ++i) {
    if (c1 - c2)
      return (c1 - c2);
  }
  return (0);
}


template<typename T>
size_t	signedToChar(T n, int base, char* bf)
{
  int		off = 0;
  if (n < 0) { bf[0] = '-'; n *= -1; ++off; }
  size_t	sc = orderOfMagnitude(n, base);
  size_t	disc = pow(base, sc - 1);

  for (int i = sc; i > 0; --i) {
    bf[sc - i + off] = baseChar[n / disc];
    n %= disc;
    disc /= base;
  }
  bf[sc + off] = 0;
  return (sc);
}

template<typename T>
size_t	unsignedToChar(T n, int base, char* bf)
{
  size_t	sc = orderOfMagnitude(n, base);
  size_t	disc = pow(base, sc - 1);

  for (int i = sc; i > 0; --i) {
    bf[sc - i] = baseChar[n / disc];
    n %= disc;
    disc /= base;
  }
  bf[sc] = 0;
  return (sc);
}

template size_t	signedToChar(s8 n, int base, char* bf);
template size_t	unsignedToChar(u8 n, int base, char* bf);
template size_t	signedToChar(s16 n, int base, char* bf);
template size_t	unsignedToChar(u16 n, int base, char* bf);
template size_t	signedToChar(s32 n, int base, char* bf);
template size_t	unsignedToChar(u32 n, int base, char* bf);
#if _PTRSIZE == 64
template size_t	signedToChar(s64 n, int base, char* bf);
template size_t	unsignedToChar(u64 n, int base, char* bf);
#endif
