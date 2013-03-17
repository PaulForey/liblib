#include "liblib.h"

int openlib(LibraryHandle* h, bstring libname)
{
    check(libname != NULL, "openlib supplied with invalid libname.");

    h = malloc(sizeof(LibraryHandle));
    check_mem(h);

    h->libname = bstrcpy(libname);

#ifdef _WIN32
    h->lib = LoadLibrary(TEXT(bdata(h->libname)));
    check(h->lib != NULL, "Failed to open the library %s", bdata(h->libname));
#elif defined __unix__
    h->lib = dlopen(bdata(h->libname), RTLD_NOW);
    check(h->lib != NULL, "Failed to open the library %s: %s",
            bdata(h->libname), dlerror());
#endif

    return 0;
error:
    return 1;
}

int openfunc(LibraryHandle* h, FunctionHandle* f, bstring funcname)
{
    check(funcname != NULL, "openfunc supplied with invalid funcname.");
    f = malloc(sizeof(FunctionHandle));
    check_mem(f);

    f->funcname = bstrcpy(funcname);

#ifdef _WIN32
    f->func = GetProcAddress(h->lib, bdata(f->funcname));
    check(f->func != NULL, "Did not find %s in the library: %s",
            bdata(f->funcname), bdata(h->libname));
#elif defined __unix__
    f->func = dlsym(h->lib, bdata(f->funcname));
    check(f->func != NULL, "Did not find %s in the library %s: %s",
            bdata(f->funcname), bdata(h->libname), dlerror());
#endif 

    return 0;
error:
    return 1;
}

void closefunc(FunctionHandle* f)
{
    if(f->funcname) bdestroy(f->funcname);
    if(f) free(f);
}

int closelib(LibraryHandle* h)
{
#ifdef _WIN32
    int rc = FreeLibrary(h->lib);
    check(rc != 0, "Failed to close %s", bdata(h->libname));
#elif defined __unix__
    int rc = dlclose(h->lib);
    check(rc == 0, "Failed to close %s: %s", bdata(h->libname), dlerror());
#endif

    if(h->libname) bdestroy(h->libname);
    if(h) free(h);

    return 0;
error:
    return 1;
}
    
