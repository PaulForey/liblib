#include <liblib.h>
#include <bstrlib.h>
#include "dbg.h"
#include "minunit.h"

#define LIB_FILE "test/libtest.dll"

LibraryHandle* lib = NULL;

char* test_max()
{
    int rc = 0;
    int res = 0;
    FunctionHandle* f = NULL;
    bstring funcname = bfromcstr("max");

    rc = openlib(lib, bfromcstr(LIB_FILE));
    mu_assert(rc == 0, "Failed to open library.");
    debug("Library opened!");

    rc = openfunc(lib, f, funcname);
    mu_assert(rc == 0, "Failed to find function.");
    debug("Function opened!");

    res = f->func(2,3);
    printf("Result: %i", res);

    closefunc(f);
    closelib(lib);

    return NULL;
}

char* all_tests() {
    mu_suite_start();
    mu_run_test(test_max);
    return NULL;
}

RUN_TESTS(all_tests);

