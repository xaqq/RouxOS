#ifndef STRING_HH__
#define STRING_HH__

#include	"CString.hh"

class String {
public:
    String(const char* m = 0);
    String(const String& o);
    String(String&& o);

    virtual ~String(void);

    String&	append(const String& o);
    String&	append(const char* o);

    char		at(int i) const;
    String      from(int i) const;

    // Args...
    char*       cstr(void);
    const char*	cstr(void) const;
    size_t      count(void) const;

    char		operator[](int i) const;

    String	operator+(const char* o) const;
    String	operator+(const String& o) const;

    String& operator+=(char c);
    String&	operator+=(const char* o);
    String&	operator+=(const String& o);

    String&	operator=(const char* o);
    String&	operator=(const String& o);
    String&	operator=(String&& o);

    bool		operator==(const char* o) const;
    bool		operator==(const String& o) const;

    //  inline String&	arg(const String& a1)
    //  { const String *args[] = {&a1}; return multiArg(1, args); }
    inline String     arg(const String &a1, const String &a2)
    { const String* args[] = {&a1, &a2}; return multiArg(2, args); }

    inline String 	arg(const String& a1, const String& a2, const String& a3)
    { const String* args[] = {&a1, &a2, &a3}; return multiArg(3, args); }

    inline String 	arg(const String& a1, const String& a2, const String& a3,
                        const String& a4)
    { const String* args[] = {&a1, &a2, &a3, &a4}; return multiArg(4, args); }

    inline String 	arg(const String& a1, const String& a2, const String& a3,
                        const String& a4, const String& a5)
    { const String* args[] = {&a1, &a2, &a3, &a4, &a5}; return multiArg(5, args); }

    inline String 	arg(const String& a1, const String& a2, const String& a3,
                        const String& a4, const String& a5, const String& a6)
    { const String* args[] = {&a1, &a2, &a3, &a4, &a5, &a6}; return multiArg(6, args); }

    inline String 	arg(const String& a1, const String& a2, const String& a3,
                        const String& a4, const String& a5, const String& a6,
                        const String& a7)
    { const String* args[] = {&a1, &a2, &a3, &a4, &a5, &a6, &a7}; return multiArg(7, args); }

    inline String 	arg(const String& a1, const String& a2, const String& a3,
                        const String& a4, const String& a5, const String& a6,
                        const String& a7, const String& a8)
    { const String* args[] = {&a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8}; return multiArg(8, args); }

    inline String 	arg(const String& a1, const String& a2, const String& a3,
                        const String& a4, const String& a5, const String& a6,
                        const String& a7, const String& a8, const String& a9)
    { const String* args[] = {&a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9}; return multiArg(9, args); }


    static inline String	number(u8 n, int base)
    { char	buff[32] = {0}; unsignedToChar<u8>(n, base, buff); return String(buff); }
    static inline String	number(s8 n, int base)
    { char	buff[32] = {0}; signedToChar<s8>(n, base, buff); return String(buff); }
    static inline String	number(u16 n, int base)
    { char	buff[32] = {0}; unsignedToChar<u16>(n, base, buff); return String(buff); }
    static inline String	number(s16 n, int base)
    { char	buff[32] = {0}; signedToChar<s16>(n, base, buff); return String(buff); }
    static inline String	number(void* n, int base)
    { char	buff[32] = {0}; unsignedToChar<size_t>((size_t)n, base, buff); return String(buff); }
    static inline String	number(u32 n, int base)
    { char	buff[32] = {0}; unsignedToChar<u32>(n, base, buff); return String(buff); }
    static inline String	number(s32 n, int base)
    { char	buff[32] = {0}; signedToChar<s32>(n, base, buff); return String(buff); }

#if (_PTRSIZE == 64)
    String	number(u64 n, int base)
    { char	buff[64] = {0}; unsignedToChar<u64>(n, base, buff); return (buff); }
    String	number(s64 n, int base)
    { char	buff[64] = {0}; signedToChar<s64>(n, base, buff); return (buff); }
#endif

protected:

    String(size_t len);		// This is a hack for StaticString

    char		*data;
    size_t	len;

    String	multiArg(int argNbr, const String** args) const;
};

String	operator+(const char* o, const String& r);
String&	operator+=(const char* o,String& r);
bool	operator==(const char* o, const String& r);

#endif /* !STRING_HH__ */
