# === Variables ===
CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -I.
LIBNAME = librobotitec_dr.a
LIBOBJ = robotitec_dr.o

# === Targets ===

all: main

# Compile the library object file
$(LIBOBJ): robotitec_dr.c robotitec_dr.h
	$(CC) $(CFLAGS) -c robotitec_dr.c -o $(LIBOBJ)

# Create static library from object
$(LIBNAME): $(LIBOBJ)
	$(AR) rcs $(LIBNAME) $(LIBOBJ)

# Build the final executable, linking with the static library
main: server.c $(LIBNAME)
	$(CC) $(CFLAGS) server.c -L. -lrobotitec_dr -o server

# Clean all generated files
clean:
	rm -f *.o *.a server
