CC = gcc # select the c compiler
CFLAGS = -Wall -I/usr/include/ -I. # directory of the system's libraries
LDFLAGS = -lncurses # specify the system libraries that we are using
DEPS = conway.h # dependencies of "our" headder files (not from the system)
OBJ = main.o test5.o # targets

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

conway: $(OBJ)
	gcc  -o $@ $^  $(LDFLAGS) $(CFLAGS)

clean:
	rm -f $(OBJ) conway
