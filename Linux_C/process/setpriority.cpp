#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int which, prio;
  id_t who;
  if (argc < 4 || strchr("pgu", argv[1][0]) == NULL)
    std::cout << "argv err" << std::endl;
  which = (argv[1][0] == 'p') ? PRIO_PROCESS
                              : (argv[1][0] == 'g') ? PRIO_PGRP : PRIO_USER;
// who = atoi(argv[2]);
    who = getpid();
  prio = atoi(argv[3]);
  if (setpriority(which, who, prio) == -1) {
    std::cout << "setpriority err" << std::endl;
    exit(0);
  }
  errno = 0;
  prio = getpriority(which, who);
  if (prio < 0 && errno != 0) {
    std::cout << "getpriority err" << std::endl;
    exit(0);
  }
  std::cout << "Nice Value = " << prio << std::endl;
  return 0;
}