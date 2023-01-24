SOURCES=src/main.c src/cdindex.c src/graph.c src/utility.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bin/cdindex

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	mkdir -p bin
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.PHONY: clean test

clean:
	rm -f src/*.o $(EXECUTABLE)

# Regression test
test:
	bin/cdindex.exe | diff tests/bin.ok -
	python tests/tests.py | diff tests/py.ok -

