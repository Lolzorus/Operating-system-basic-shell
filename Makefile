CC = gcc
CFLAGS = -Wall

mon_shell: cn-decouper.c hb-pipe.c project.c
	$(CC) $(CFLAGS) cn-decouper.c hb-pipe.c project.c -o mon_shell	

clean:
	rm -f mon_shell
