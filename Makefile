CC=g++
CFLAGS=-std=c++17 -Wall -Wextra -O2

SRCDIR=src
SRCS = $(wildcard $(SRCDIR)/*.cpp)
# SRCS = main.cpp command.cpp client.cpp
# OBJS = $(SRCS:.cpp=.o)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

TARGET=todo
OBJDIR=build

LIBS=-lfmt -lsqlite3

all: $(TARGET)

# Link objects to binary
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Compile .cpp to .o
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
