#pragma once

/* #ifdef __cplusplus */
/* extern "C" { */
/* #endif */

extern const char baseChar[];
extern const char baseCharMin[];

void*	bzero(void* ptr, size_t size);

void	*memcpy(void* to, const void* from, size_t size);
void	memset(void *ptr, char val, size_t size);

size_t	strlen(const char* str);
size_t	strcpy(char* to, const char* f);
size_t	strncpy(char* to, const char* from, size_t l);

char*	strdup(const char* f);
char*	strndup(const char* f, size_t l);

int	strcmp(const char* c1, const char* c2);
int	strncmp(const char* c1, const char* c2, size_t s);

/* #ifdef __cplusplus */
/* } */
/* #endif */

#ifdef __cplusplus
template<typename T>
size_t	signedToChar(T val, int base, char* buff);
template<typename T>
size_t	unsignedToChar(T val, int base, char* buff);
/* #else */
/* size_t	signedToChar(size_t val, int base, char* buff); */
/* size_t	unsignedToChar(size_t val, int base, char* buff); */
#endif
