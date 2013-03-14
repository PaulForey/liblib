#include <stdio.h>
#include <stdarg.h>
#include "dbg.h"
#include "bstrlib.h"

#ifdef _WIN32
#include <windows.h>
#define FUNC HMODULE
#endif // _WIN32

#ifdef __linux__
#include <dlfcn.h>
#define FUNC void*
#endif // __linux__

typedef struct LibraryHandle {
#ifdef _WIN32
    HMODULE lib;
#elif defined __unix__
    void* lib;
#endif
    bstring libname;
} LibraryHandle;

/*
typedef struct FunctionHandle {
#ifdef _WIN32
    FARPROC func;
#elif defined __unix__
    void* func;
#endif
    bstring funcname;
} FunctionHandle;
*/
/*
 * function:    openlib
 * arguments:   LibraryHandle* h, bstring libname
 * return-code: 0 on success, 1 on failure
 */
int openlib(LibraryHandle* h, bstring libname);

/*
 * function:    openfunction
 * arguments:   LibraryHandle* h, FunctionHandle* f, bstring funcname
 * return-code: 0 on success, 1 on failure
 */
int openfunc(LibraryHandle* h, FunctionHandle* f, bstring funcname);

/*
 * function:    closefunction
 * arguments:   FunctionHandle* f
 */
void closefunc(FunctionHandle* f);

/*
 * function:    closelib
 * arguments:   LibraryHandle* h
 * return-code: 0 on success, 1 on failure
 */
int closelib(LibraryHandle* h);
    
