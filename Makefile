OBJS = src/report.o src/event.o src/reduction.o src/accumulator.o

static: $(OBJS)
	mkdir -p lib
	ar rcs lib/libaser.a $(OBJS)

dynamic: $(OBJS)
	mkdir -p lib
	gcc -shared -Wl,-soname,lib/libaser.so.1 -o lib/libaser.so.1.0 $(OBJS)

all: static dynamic

src/%.o: src/%.c
	gcc -c -fPIC -Iinclude -o $@ $<

.PHONY: clean

clean:
	rm -f src/*.o lib/*
