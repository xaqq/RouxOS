#include	"KBase.hh"
#include	"CString.hh"
#include	"String.hh"
#include	"Maths.hh"
#include    "Map.hh"
#include    "PrintEngine.hh"
#include    "StaticString.hh"
#include	"MemoryManager.hh"

String::String(const char* m)
{
    if (!m || !m[0]) {
        data = 0;
        len = 0;
    } else {
        len = strlen(m);
        data = strndup(m, len);
    }
}
String::String(const String& o)
{
    if (!o.count()) {
        data = 0;
        len = 0;
    } else {
        len = o.count();
        data = strndup(o.cstr(), len);
    }
}

String::String(size_t l) : len(l)                                           {}

String::String(String&& o) : data(o.data), len(o.len)	{ o.data = 0; o.len = 0; }

String::~String(void)                                   { if (data) {kprint("Deleting data...\n"); free(data); kprint("Deleted\n");}}

String&		String::operator=(const char* o)
{
    size_t	olen = strlen(o);

    if (olen > len) {
        free(data);
        data = strndup(o, olen);
        len = olen;
    } else {
        strncpy(data, o, olen);
        bzero(data + olen, len - olen);
        len = olen;
    }
    return (*this);
}

String&		String::operator=(const String& o)
{
    size_t	olen = o.count();

    if (olen > len) {
        free(data);
        data = strndup(o.cstr(), olen);
        len = olen;
    } else {
        strncpy(data, o.cstr(), olen);
        bzero(data + olen, len - olen);
        len = olen;
    }
    return (*this);
}

String&		String::operator=(String&& o)
{
    data = o.data;
    len = o.len;
    o.data = 0;
    o.len = 0;
    return (*this);
}


String&		String::append(const String& o)
{
    size_t	olen = o.count();

    data = (char*)realloc(data, olen + len);
    strncpy(data + len, o.cstr(), olen);
    len += olen;
    return *this;
}
String&		String::append(const char* o)
{
    if (!o || !o[0])
        return *this;
    size_t	olen = strlen(o);
    data = (char*)realloc(data, olen + len);
    strncpy(data + len, o, olen);
    len += olen;
    return *this;
}

char		String::at(int i) const		{ return (data[i]); }
String		String::from(int i) const	{ return String(data + i); }

char*		String::cstr(void)		{ return strdup(data); }
const char*	String::cstr(void) const	{ return data; }

size_t		String::count(void) const	{ return len; }

char		String::operator[](int i) const		{ return data[i]; }

String	String::operator+(const char* o) const
{ return (String(*this).append(o)); }
String	String::operator+(const String& o) const
{ return String(*this).append(o); }
String&	String::operator+=(char c)
{ char t[] = {c, 0}; return this->append(t); }
String&	String::operator+=(const char* o)
{ kprint("STFU1\n"); return this->append(o); }
String&	String::operator+=(const String& o)
{ kprint("STFU2\n"); return this->append(o); }
bool	String::operator==(const char* o) const
{ return (strcmp(data, o)); }
bool	String::operator==(const String& o) const
{ return (strcmp(data, o.cstr())); }


String	operator+(const char* o, const String& r)
{ return (r + o); }
String&	operator+=(const char* o,String& r)
{ return r.append(o); }
bool	operator==(const char* o, const String& r)
{ return (r == o); }


static int getEscape(const char *uc, size_t *pos, size_t len, int maxNumber = 999)
{
    size_t i =	*pos;

    ++i;
    if (i < len && uc[i] == 'L')
        ++i;
    if (i < len) {
        int escape = uc[i] - '0';
        if ((size_t)escape >= 10U)
            return -1;
        ++i;
        while (i < len) {
            int digit = uc[i] - '0';
            if ((size_t)digit >= 10U)
                break;
            escape = (escape * 10) + digit;
            ++i;
        }
        if (escape <= maxNumber) {
            *pos = i;
            return escape;
        }
    }
    return -1;
}

String	String::multiArg(int numArgs, const String **args) const
{
    String         result;
    Map<int, int>  numbersUsed;
    const size_t   end = len - 1;
    int            lastNumber = -1;
    size_t         i = 0;

    assert(len);
    // populate the numbersUsed map with the %n's that actually occur in the string
    while (i < len) {
        if (data[i] == '%') {
            int number = getEscape(data, &i, len);
            //kprint(StaticString("Find %").append(StaticString::number(number)).append("\n"));
            if (number != -1) {
                numbersUsed.insert(number, -1);
                assert(numbersUsed.end().key() == number);
                continue;
            }
        }
        ++i;
    }

    // assign an argument number to each of the %n's
    Map<int, int>::Iterator j = numbersUsed.begin();
    int arg = 0;
    while (j.isValid() && arg < numArgs) {
        *j = arg++;
        lastNumber = j.key();
        ++j;
    }

    // sanity
    if (numArgs > arg) {
        kprint(StaticString("String::arg: ").append(StaticString::number(numArgs - arg))
               .append(" argument(s) missing in '").append(data).append("'\n"));
        numArgs = arg;
    }

    //PRINTREG(esp);
    //memAlloc->showAllocatedMemory();
    i = 0;
    while (i < len) {
        if (data[i] == '%' && i != end) {
            kprint("FFS\n");
            int number = getEscape(data, &i, len, lastNumber);
            int arg = numbersUsed[number].value();
            if (number != -1 && arg != -1) {
                result += *args[arg];
                continue;
            }
        }
        result += data[i++];
    }
    kprint("Kikou\n");
    return result;
}
