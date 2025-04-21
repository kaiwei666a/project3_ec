#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define MAX_PATH 128

void test_basic_symlink() {
    printf(1, "=== Testing basic symlink functionality ===\n");
    
    // Create a test file
    int fd = open("test2.txt", O_CREATE | O_WRONLY);
    if(fd < 0) {
        printf(1, "Error: failed to create test file\n");
        return;
    }
    write(fd, "Hello World", 11);
    close(fd);

    // Create a symlink
    if(symlink("test2.txt", "link2.txt") < 0) {
        printf(1, "Error: failed to create symlink\n");
        return;
    }

    // Read through symlink
    char buf[20];
    fd = open("link2.txt", O_RDONLY);
    if(fd < 0) {
        printf(1, "Error: failed to open symlink\n");
        return;
    }
    int n = read(fd, buf, sizeof(buf));
    close(fd);
    if(n != 11) {
        printf(1, "Error: wrong content length\n");
        return;
    }
    buf[n] = 0;
    printf(1, "Content via symlink: %s\n", buf);

    printf(1, "Basic symlink test passed!\n\n");
}

void test_nofollow() {
    printf(1, "=== Testing O_NOFOLLOW flag ===\n");
    
    // Try to open symlink with O_NOFOLLOW
    int fd = open("link2.txt", O_RDONLY | O_NOFOLLOW);
    if(fd < 0) {
        printf(1, "Error: should be able to open symlink with O_NOFOLLOW\n");
        return;
    }
    close(fd);

    printf(1, "O_NOFOLLOW test passed!\n\n");
}

void test_recursive() {
    printf(1, "=== Testing recursive symlinks ===\n");
    
    // Create recursive symlinks
    if(symlink("link2b.txt", "link2a.txt") < 0) {
        printf(1, "Error: failed to create link2a\n");
        return;
    }
    if(symlink("link2a.txt", "link2b.txt") < 0) {
        printf(1, "Error: failed to create link2b\n");
        return;
    }

    // Try to open recursive symlink
    int fd = open("link2a.txt", O_RDONLY);
    if(fd >= 0) {
        printf(1, "Error: should not be able to open recursive symlink\n");
        close(fd);
        return;
    }

    printf(1, "Recursive symlink test passed!\n\n");
}

void test_hardlink_vs_symlink() {
    printf(1, "=== Testing hardlink vs symlink ===\n");
    
    // Create a hardlink
    if(link("test2.txt", "hardlink2.txt") < 0) {
        printf(1, "Error: failed to create hardlink\n");
        return;
    }

    // Get inode numbers
    struct stat st1, st2, st3;
    if(stat("test2.txt", &st1) < 0 || 
       stat("hardlink2.txt", &st2) < 0 ||
       lstat("link2.txt", &st3) < 0) {
        printf(1, "Error: failed to stat files\n");
        return;
    }

    printf(1, "Original file inode: %d\n", st1.ino);
    printf(1, "Hardlink inode: %d\n", st2.ino);
    printf(1, "Symlink inode: %d\n", st3.ino);

    if(st1.ino != st2.ino) {
        printf(1, "Error: hardlink should have same inode as original\n");
        return;
    }
    if(st1.ino == st3.ino) {
        printf(1, "Error: symlink should have different inode\n");
        return;
    }

    printf(1, "Hardlink vs symlink test passed!\n\n");
}

void test_file_operations() {
    printf(1, "=== Testing file operations on symlinks ===\n");
    
    // Try to unlink the symlink
    if(unlink("link2.txt") < 0) {
        printf(1, "Error: failed to unlink symlink\n");
        return;
    }

    // Verify original file still exists
    int fd = open("test2.txt", O_RDONLY);
    if(fd < 0) {
        printf(1, "Error: original file should still exist\n");
        return;
    }
    close(fd);

    printf(1, "File operations test passed!\n\n");
}

int main() {
    test_basic_symlink();
    test_nofollow();
    test_recursive();
    test_hardlink_vs_symlink();
    test_file_operations();

    // Cleanup
    unlink("test2.txt");
    unlink("hardlink2.txt");
    unlink("link2a.txt");
    unlink("link2b.txt");

    printf(1, "All symlink tests completed!\n");
    exit();
} 