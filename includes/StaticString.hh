#ifndef STATICSTRING_HH__
#define STATICSTRING_HH__

#include	"String.hh"

#define STATICSTRING_BUFFERSIZE		512

class StaticString : public String {
public:
  StaticString(const char* m = "");
  StaticString(const String& o);

  virtual ~StaticString(void)		{ data = 0; }

  StaticString&	append(const String& o);
  StaticString&	append(const char* o);

  StaticString	from(int i) const;

  static StaticString	number(u8, int base = 10);
  static StaticString	number(s8, int base = 10);
  static StaticString	number(u16, int base = 10);
  static StaticString	number(s16, int base = 10);
  static StaticString	number(u32, int base = 10);
  static StaticString	number(s32, int base = 10);

#if (_PTRSIZE == 64)
  static StaticString	number(u64, int base = 10);
  static StaticString	number(s64, int base = 10);
#endif

  static StaticString	number(void*, int base = 16);

protected:
  char		d[STATICSTRING_BUFFERSIZE];
};

#endif /* !STATICSTRING_HH__ */
