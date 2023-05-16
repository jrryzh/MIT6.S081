#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int inner_process(int leftp) {
  int d, n;
  int read_result;
  int p[2];
  pipe(p);

  read_result = read(leftp, &d, 4);
  if (read_result == 0) {
      close(leftp);
      close(p[0]);
      close(p[1]);
      exit(0);
    }

  int pid = fork();
  if (pid == 0) {
    fprintf(0, "prime %d\n", d);
    read_result = read(leftp, &n, 4);
    while (read_result != 0 && n != 0) {
      if (n % d != 0) {
        write(p[1], &n, 4);
      }
      read_result = read(leftp, &n, 4);
    }
    close(leftp);
    close(p[0]);
    close(p[1]);
    exit(0);
  } else {
    wait(0);
    close(leftp);
    close(p[1]);
    inner_process(p[0]);
    exit(0);
  }
}

int main() {
  // First process
  int p[2];

  pipe(p);
  for (int i=2; i <= 35; i++) {
    write(p[1], &i, 4);
  }
  close(p[1]);
  inner_process(p[0]);
  exit(0);
}
