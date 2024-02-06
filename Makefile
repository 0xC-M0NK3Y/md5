PROG_MD5   = bin/md5
LIB_MD5    = lib/md5.a

LIB_SRC    = md5.c
LIB_OBJ    = $(addprefix build/, $(LIB_SRC:.c=.o))

PROG_SRC   = main.c
PROG_OBJ   = $(addprefix build/, $(PROG_SRC:.c=.o))

CFLAGS     = -Wall -Wextra
CC         = gcc
AR         = ar rcs

all: $(LIB_MD5) $(PROG_MD5)

debug: CFLAGS += -D_DEBUG_
debug: $(LIB_MD5) $(PROG_MD5)

$(PROG_MD5): $(LIB_MD5) $(PROG_OBJ)
	mkdir -p $(shell dirname $(PROG_MD5))
	$(CC) $(CFLAGS) $(PROG_OBJ) $(LIB_MD5) -o $(PROG_MD5)
$(LIB_MD5): $(LIB_OBJ)
	mkdir -p $(shell dirname $(LIB_MD5))
	$(AR) $(LIB_MD5) $(LIB_OBJ)

build/%.o: %.c
	mkdir -p $(shell dirname $(@))
	$(CC) $(CFLAGS) -c $(<) -o $(@) -masm=intel

clean:
	rm -rf build/*
fclean: clean
	rm -rf $(PROG_MD5)
	rm -rf $(LIB_MD5)
re: fclean all

.phony: all clean fclean re
