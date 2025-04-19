#include "types.h"
#include "user.h"
#include "fcntl.h"

#define BUF_SIZE 128

int main() {
    int fd;
    char buffer[BUF_SIZE];

    // 创建文件
    fd = open("testfile", O_CREATE | O_RDWR);
    if (fd < 0) {
        printf(2, "Failed to create file\n");
        exit();
    }

    // 写入内容
    write(fd, "Hello", 5);  // "Hello" 占据 offset 0~4

    // Seek 到文件头并读取
    lseek(fd, 0);
    read(fd, buffer, 5);
    buffer[5] = '\0';
    printf(1, "\n[TEST 1] Read after lseek(0): %s\n", buffer);  // 应为 Hello

    // Seek 向后偏移2字节（从 offset 5 变为 7）
    lseek(fd, 2); // 注意：当前 offset 是 5，+2 后应为 7
    // int new_offset = tell(fd); // 如未实现 tell() 可注释此行
    // printf(1, "[TEST 2] Offset after lseek(2): %d\n", new_offset);

    // 写入新内容在偏移7处
    write(fd, "XYZ", 3);  // 文件大概为 H e l l o \0 \0 X Y Z

    // 回到开头读取整段数据
    lseek(fd, -10); // 假设当前 offset 为 10，返回开头；或者 lseek(fd, -10) 视实际偏移调整
    int n = read(fd, buffer, 15);
    printf(1, "[TEST 3] Read entire file (may contain \\0):\n");
    for (int i = 0; i < n; i++) {
        if (buffer[i] == 0)
            printf(1, "\\0");
        else
            printf(1, "%c", buffer[i]);
    }
    printf(1, "\n");

    // Seek 超过文件尾部（稀疏写入测试）
    lseek(fd, 1000); // 偏移到1000
    write(fd, "Z", 1); // 写入一个字符
    printf(1, "[TEST 4] Wrote to offset 1000 with 'Z'\n");

    close(fd);
    exit();
}
