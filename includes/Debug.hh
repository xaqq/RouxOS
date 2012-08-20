#ifndef DEBUG_HH__
#define DEBUG_HH__

#define assert(cond)		((cond) ? ((void)(cond)) :		\
    (assert_failed(__PRETTY_FUNCTION__, __FILE__, \
    __LINE__, #cond)))
#define failure(msg)        (assert_failed(__PRETTY_FUNCTION__, __FILE__, __LINE__, msg))

__attribute__((noreturn)) void	assert_failed(const char* fname, const char* file,
                                              int line, const char* cond);

#define dumpReg(reg, var)	asm("movl %%" #reg " ,%0" : "=r"(var));

#define PRINTREG(rg)		do { size_t rxg; dumpReg(rg, rxg);	\
    kprint(StaticString(#rg": 0x").append(StaticString::number(rxg, 16)).append("\n")); \
    } while (0)

#endif /* !DEBUG_HH__ */
