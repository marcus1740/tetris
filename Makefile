CXX := gcc
CXXFLAGS := -std=c11 -Wall -Wextra
SRCDIR := src
INCLUDEDIR := dependencies/include
LIBDIR := dependencies/lib
BUILDDIR := build
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
EXECUTABLE := program
LIBS := -lmingw32 -lSDL2main -lSDL2

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -I $(INCLUDEDIR) -L $(LIBDIR) $^ -o $@ $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(wildcard $(SRCDIR)/*.h)
	$(CXX) $(CXXFLAGS) -I $(INCLUDEDIR) -c $< -o $@

clean:
	$(RM) $(BUILDDIR)/*.o $(EXECUTABLE)

	
