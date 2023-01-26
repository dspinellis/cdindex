SOURCES=src/main.c src/cdindex.c src/graph.c src/utility.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bin/cdindex

ifdef DEBUG
	CFLAGS=-g -O0
	CXXFLAGS=-g -O0
else
	CFLAGS=-O3
	CXXFLAGS=-O3
endif

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	mkdir -p bin
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.PHONY: clean test

clean:
	rm -f src/*.o $(EXECUTABLE)

# Regression test
test:
	bin/cdindex.exe | diff tests/bin.ok -
	python tests/tests.py | diff tests/py.ok -

