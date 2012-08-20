#include	"KBase.hh"
#include	"CString.hh"
#include	"Maths.hh"
#include	"StaticString.hh"

StaticString::StaticString(const char* m) : String(strlen(m))
{ data = d; strcpy(data, m); }
StaticString::StaticString(const String& o) : String(o.count())
{ data = d; strcpy(data, o.cstr()); }

StaticString&	StaticString::append(const String& o)
{
  size_t	cpy = MIN(o.count(), STATICSTRING_BUFFERSIZE);
  strncpy(this->d + len, o.cstr(), cpy);
  len += cpy;
  return (*this);
}

StaticString&	StaticString::append(const char* o)
{
  int cpy = strlen(o);
  cpy = MIN(cpy, STATICSTRING_BUFFERSIZE);
  strncpy(this->d + len, o, cpy);
  len += cpy;
  return (*this);
}

StaticString	StaticString::from(int i) const
{ return StaticString(data + i); }

StaticString	StaticString::number(u8 n, int base)
{
  char	buff[32] = {0}; 

  unsignedToChar<u8>(n, base, buff);
  return (buff);
}

StaticString	StaticString::number(s8 n, int base)
{
  char	buff[32] = {0}; 

  signedToChar<s8>(n, base, buff);
  return (buff);
}

StaticString	StaticString::number(u16 n, int base)
{
  char	buff[32] = {0}; 

  unsignedToChar<u16>(n, base, buff);
  return (buff);
}

StaticString	StaticString::number(s16 n, int base)
{
  char	buff[32] = {0}; 

  signedToChar<s16>(n, base, buff);
  return (buff);
}

StaticString	StaticString::number(void* n, int base)
{
  char	buff[32] = {0}; 

  unsignedToChar<size_t>((size_t)n, base, buff);
  return (buff);
}

StaticString	StaticString::number(u32 n, int base)
{
  char	buff[32] = {0}; 

  unsignedToChar<u32>(n, base, buff);
  return (buff);
}

StaticString	StaticString::number(s32 n, int base)
{
  char	buff[32] = {0}; 

  signedToChar<s32>(n, base, buff);
  return (buff);
}

#if (_PTRSIZE == 64)
StaticString	StaticString::number(u64 n, int base)
{
  char	buff[64] = {0}; 

  unsignedToChar<u64>(n, base, buff);
  return (buff);
}

StaticString	StaticString::number(s64 n, int base)
{
  char	buff[64] = {0}; 

  signedToChar<s64>(n, base, buff);
  return (buff);
}

#endif
