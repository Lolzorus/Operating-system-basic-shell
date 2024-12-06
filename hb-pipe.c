#include <stdio.h>
#include <unistd.h>
#include <assert.h>

/* hb−pipe.c

Fabriquer un pipe, y ecrire et y lire des donnees.
*/

int hbpipe(){
int fd[2];
int t;
char buf[5];

t = pipe(fd);
assert ( t >= 0);

t = write(fd[1], "foo", 3);
assert ( t == 3);

t = write(fd[1], " et bar\n", 8);
assert ( t == 8);

t = read(fd[0], buf, sizeof buf);
assert ( t >= 0);

buf[t] = 0;
printf( "lu %d octets : %s\n", t, buf);

return 0;

}