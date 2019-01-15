CC = gcc
CFLAGS+= -ggdb3 #-DDISK_NAME=$(DISK)

c clean:
	@$(RM)	`find -maxdepth 1 -executable  -type f`
