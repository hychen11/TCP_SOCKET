# Targets for the build
TARGETS = client server

# Compiler to use
GCC = g++

# Default target
all: $(TARGETS)

# Phony targets for make commands that don't correspond to files
.PHONY: all clean

# Clean up build artifacts
clean:
	rm -f *.o $(TARGETS)

# Build the client program
client: client.cpp
	$(GCC) -o $@ $<

# Build the server program
server: server.cpp
	$(GCC) -o $@ $<
