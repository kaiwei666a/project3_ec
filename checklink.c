#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  struct stat st;

  if (stat("original.txt", &st) == 0)
    printf(1, "original.txt: type=%d, inum=%d\n", st.type, st.ino);
  else
    printf(1, "stat failed on original.txt\n");

  if (lstat("link.txt", &st) == 0)
    printf(1, "link.txt: type=%d, inum=%d\n", st.type, st.ino);
  else
    printf(1, "lstat failed on link.txt\n");

  exit();
}
