#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"


int main(int argc, char *argv[]) {
  // read from stdin
  char tmp_c;
  // copy over
  char *cur_argv[MAXARG];
  int cur_argc = 0;
  for (int i=1; i<argc; i++) {
    cur_argv[cur_argc] = argv[i];
    cur_argc ++;
  }
  // start reading
  // every line from stdin get a buf[64]
  char buf[64];
  char *ptr = buf;
  int counter = 0;
  cur_argv[cur_argc] = ptr;
  cur_argc ++;
  while (read(0, &tmp_c, 1) != 0) {
    if (tmp_c == ' ') {
      *(ptr+counter) = '\0';
      counter++;
      cur_argv[cur_argc] = ptr+counter;
      cur_argc++;
    } else if (tmp_c == '\n') {
      *(ptr+counter) = '\0';
      cur_argv[cur_argc++] = 0;
      if (fork()==0){
        exec(argv[1], cur_argv);
      } else {
        wait(0);
        // rest
        cur_argc = argc;
        ptr = buf;
        counter = 0;
        cur_argv[cur_argc] = ptr;
      }
    } else {
       *(ptr+counter) = tmp_c;
       counter ++;
    }
  }
  exit(0);
}
