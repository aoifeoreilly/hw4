# Makefile for arith (Comp 40 Assignment 4)
# 
# Last updated: February 26th, 2025


############## Variables ###############

# The compiler being used
CC = gcc 

# Include path using Comp 40 .h files and CII interfaces
IFLAGS = -I/comp/40/build/include -I/usr/sup/cii40/include/cii

# Compile flags
CFLAGS = -g -std=gnu99 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

# Linking flags
LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64

# Libraries needed for linking
LDLIBS = -l40locality -larith40 -lnetpbm -lcii40 -lm -lrt

# Collect all .h files in your directory.
# This way, you can never forget to add
# a local .h file in your dependencies.
#
# This bugs Mark, who dislikes false dependencies, but
# he agrees with Noah that you'll probably spend hours 
# debugging if you forget to put .h files in your 
# dependency list.
INCLUDES = $(shell echo *.h)

############### Rules ###############

all: ppmdiff 40image

## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@


## Linking step (.o -> executable program)

ppmdiff: ppmdiff.o a2plain.o uarray2.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

40image: 40image.o compress_decompress.o uarray2b.o uarray2.o trimPPM.o convert_comp_rgb.o block_unblock.o DCT_transform.o bitpack.o pack_unpack.o read_write.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS) 

unit_tests: unit_tests.o uarray2b.o uarray2.o compress_decompress.o trimPPM.o convert_comp_rgb.o block_unblock.o DCT_transform.o bitpack.o pack_unpack.o read_write.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f ppmdiff 40image *.o

