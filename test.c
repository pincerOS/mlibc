#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
    const char *header = "argv: ";
    ssize_t header_written = 0;
    ssize_t header_len = strlen(header);
    while (header_written < header_len)
    {
        ssize_t bytes_written = pwrite(1, header + header_written, header_len - header_written, 0);
        if (bytes_written < 0)
        {
            exit(1);
        }
        header_written += bytes_written;
    }

    ssize_t arg_written = 0;
    ssize_t arg_len = strlen(argv[0]);
    while (arg_written < arg_len)
    {
        ssize_t bytes_written = pwrite(1, argv[0] + arg_written, arg_len - arg_written, 0);
        if (bytes_written < 0)
        {
            exit(1);
        }
        arg_written += bytes_written;
    }
    pwrite(1, "\n", 1, 0);

    const char *footer = "argc is exit code\n";
    ssize_t footer_written = 0;
    ssize_t footer_len = strlen(footer);
    while (footer_written < footer_len)
    {
        ssize_t bytes_written = pwrite(1, footer + footer_written, footer_len - footer_written, 0);
        if (bytes_written < 0)
        {
            exit(1);
        }
        footer_written += bytes_written;
    }

    exit(argc);

    int fd = openat(3, "test.txt", 0, 0, NULL);

    char buf[100];
    ssize_t nbytes = pread(fd, buf, sizeof(buf), 0);
    ssize_t total_written = 0;
    while (total_written < nbytes)
    {
        ssize_t bytes_written = pwrite(1, buf + total_written, nbytes - total_written, 0);
        if (bytes_written < 0)
        {
            exit(1);
        }
        total_written += bytes_written;
    }
    close(fd);

    char *mapping = mmap(NULL, 4096, 0, 1 << 1, -1, 0);
    mapping[0] = 'A';
    munmap(mapping, 4096);

    // I don't think file backed mappings are 100% functional right now, but let's at least check that I can map and unmap
    int fd2 = openat(3, "test2.txt", 0, 0, NULL);
    char *mapped_file = mmap(NULL, 4096, 0, 1 << 1, fd2, 0);
    munmap(mapped_file, 4096);
    close(fd2);

    int fd3 = openat(3, "test.txt", 0, 0, NULL);
    int fd4 = dup3(fd3, fd3 + 1, 0);
    if (fd4 != fd3 + 1) {
        exit(1);
    }
    int fd5 = dup2(fd3, fd3 + 2);
    if (fd5 != fd3 + 2) {
        exit(1);
    }
    close(fd3);
    close(fd4);
    close(fd5);

    exit(0);
}