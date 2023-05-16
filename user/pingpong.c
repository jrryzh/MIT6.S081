#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
        int p[2];
        char buf[1];

        pipe(p);
        write(p[1], "abcdefg\n", 1);

        int pid = fork();
        if (pid == 0) {
                read(p[0], buf, 1);
                fprintf(0, "%d: received ping\n", getpid());
                write(p[1], buf, 1);
                close(p[0]);
                close(p[1]);
                exit(0);
        } else {
                wait((int *) 0);
                close(p[1]);
                read(p[0], buf, 1);
                fprintf(0, "%d: received pong\n", getpid());
        }
        exit(0);
}
