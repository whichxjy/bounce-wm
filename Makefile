CFLAGS = -Wall

all:
	$(CC) $(CFLAGS) bounce-wm.c -lX11 -o bounce-wm

clean:
	rm -f bounce-wm