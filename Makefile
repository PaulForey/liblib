#For detecting the os:
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	TESTPAT=*_linux_test
	TESTSCRIPT=./test/runtests.sh
	TESTFLAGS=-ldl
	PLAT_LIBS=-ldl
	EXE_EXT=
	SO_EXT=.so
endif
ifeq ($(UNAME), MINGW32_NT-6.1)
	TESTPAT=*_test
	TESTSCRIPT=powershell -executionpolicy remotesigned -NoProfile ./test/runtests.ps1
	TESTFLAGS=
	PLAT_LIBS=
	EXE_EXE=.exe
	SO_EXT=.dll
endif


CFLAGS=-g -O2 -Wall -Wextra -Isrc -Iinclude $(OPTFLAGS)
LIBS=-Linclude -lbstr $(PLAT_LIBS) $(OPTLIBS)
PREFIX?=/usr/local

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wildcard test/$(TESTPAT).c)
TEST_OBJ=$(patsubst %.c,%.o,$(TEST_SRC))
TESTS=$(patsubst %.o,%,$(TEST_OBJ))

TARGET=build/liblib.a
SO_TARGET=$(patsubst %.a,%$(SO_EXT),$(TARGET))

# The Target Build
all: $(TARGET) $(SO_TARGET) tests

dev: CFLAGS += -DDEBUG
dev: all

#$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS) $(LIBS)

build:
	@mkdir -p build


# The Unit Tests
.PHONY: tests
tests: CFLAGS += $(TESTFLAGS)
tests: $(TESTS)

$(TESTS): $(TEST_OBJ)
	$(CC) -o $@ $(TEST_OBJ) $(TARGET) $(LIBS)
	@touch ./test/tests.log
	@TESTPAT=$(TESTPAT) $(TESTSCRIPT)

ifeq ($(UNAME), Linux)
memtest:
	VALGRIND="valgrind --track-origins=yes" $(MAKE)
endif
ifeq ($(UNAME), MINGW32_NT-6.1)
memtest:
	#DRMEMORY="C:\Program Files (x86)\Dr. Memory\bin\drmemory.exe -batch " $(MAKE)
	DRMEMORY="drmemory -batch " $(MAKE)
endif

# The Cleaner
clean:
	rm -rf build 
	rm -f $(OBJECTS) $(wildcard test/*.exe test/*.o)
	rm -f test/tests.log
#find . -name "*.gc*" -exec rm {} \;
#rm -rf 'find . -name "*.dSYM" -print'

# The Install
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The Checker
BADFUNCS='[^_.>a-zA-Z0-9] (str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions:
	@grep $(BADFUNCS) $(SOURCES) || true
