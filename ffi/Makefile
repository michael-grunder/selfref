
#➜  ffi git:(master) ✗ cc -std=c99 -c -O2 -fPIC -g -ggdb ffitest.c
#➜  ffi git:(master) ✗ cc -shared -Wl,-soname,libffitest.so -o libffitest.so ffitest.o

# ffi git:(master) ✗ cc -std=c99 -c -O2 -fPIC -g -ggdb ffitest.c
# ffi git:(master) ✗ cc -shared -Wl,-soname,libffitest.so -o libffitest.so ffitest.o

all:
	cc -std=c99 -c -O2 -fPIC -g -ggdb ffitest.c
	cc -shared -Wl,-soname,libffitest.so -o libffitest.so ffitest.o
	ar rcs libffitest.a ffitest.o
	cp libffitest.so libffitest.a ../target/debug/deps
	cp libffitest.so libffitest.a ../target/release/deps
	cc -O0 -g -ggdb -std=c99 ffitest.c use.c -ouse
#all:
#	cc -std=c99 -c -O2 -fPIC -g -ggdb ffitest.c
#	ar rcs libffitest.a ffitest.o
#	cc -ggdb -shared -Wl,-soname,libffitest.so -o libffitest.so
#clean:
#	rm -rf *.o *.so *.a
#
