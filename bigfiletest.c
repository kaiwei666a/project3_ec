#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main()
{
  int fd;
  char buf[512];
  int i;
  int total_blocks = 0;

  // Initialize buffer with some data
  for(i = 0; i < 512; i++)
    buf[i] = i % 256;

  // Create a new file
  fd = open("bigfile", O_CREATE | O_WRONLY);
  if(fd < 0){
    printf(1, "bigfile: cannot open bigfile for writing\n");
    exit();
  }

  // Write 16,523 blocks (maximum allowed)
  for(i = 0; i <= 16523; i++){
    int n = write(fd, buf, 512);
    if(n != 512){
      printf(1, "bigfile: write error at block %d (wrote %d bytes)\n", i, n);
      printf(1, "Total blocks written: %d\n", total_blocks);
      exit();
    }
    total_blocks++;
  }

  close(fd);
  printf(1, "Successfully wrote %d blocks\n", total_blocks);

  // Try to write one more block (should fail)
  fd = open("bigfile", O_WRONLY);
  if(fd < 0){
    printf(1, "bigfile: cannot open bigfile for writing\n");
    exit();
  }

  printf(1, "Attempting to write block %d (should fail)...\n", total_blocks);
  int n = write(fd, buf, 512);
  if(n != 512){
    printf(1, "bigfile: write error at block %d (expected failure)\n", total_blocks);
    printf(1, "Final total blocks: %d\n", total_blocks);
  } else {
    total_blocks++;
    printf(1, "bigfile: unexpected success at block %d\n", total_blocks-1);
    printf(1, "Final total blocks: %d\n", total_blocks);
  }

  close(fd);
  exit();
} 