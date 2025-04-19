#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main() {
  int fd;


  fd = open("original.txt", O_CREATE | O_RDWR);
  write(fd, "RealContent\n", 12);
  close(fd);


  if (symlink("original.txt", "link.txt") < 0) {
    printf(1, "symlink failed\n");
    exit();
  }


  fd = open("link.txt", O_RDONLY);
  char buf[100];
  read(fd, buf, 12);
  buf[12] = '\0';
  printf(1, "Content via symlink: %s\n", buf);
  close(fd);


  fd = open("link.txt", O_RDONLY | O_NOFOLLOW);
  if (fd < 0)
    printf(1, "open with O_NOFOLLOW failed\n");
  else
    printf(1, "O_NOFOLLOW open succeeded (fd=%d)\n", fd);
  close(fd);

  exit();
}
