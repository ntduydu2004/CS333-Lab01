#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

void find(char *path, char *filename) {
    char buf[512];
    // Struct to store directory entries
    struct dirent de;
    struct stat st;
    // File descriptor
    int fd;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // check if the input path is a directory
    if (st.type == T_DIR) {
        // Copy the path to buffer
        strcpy(buf, path);
        // Point pointer to next position after the path
        char *p = buf + strlen(buf);
        // Append a slash (path/) and move to next element in buffer
        *p++ = '/';
        // Check if the byte read from file descriptor is equal to a dirent (means valid read)
        // Continue to loop till all data is read
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            // inum = 0 => Invalid inode
            // . => Current directory
            // .. => Parent directory
            // Only find into valid directory
            if (de.inum == 0 || 
                strcmp(de.name, ".") == 0 || 
                strcmp(de.name, "..") == 0) {
                    continue;
                }
            // Copy the name of directory to buffer, starting at the element pointed by p from above
            // Size move is same as DIRSiZ (14).
            memmove(p, de.name, DIRSIZ);
            // Append null terminator
            p[DIRSIZ] = 0;
            // Error handling
            if (stat(buf, &st) < 0) {
                fprintf(2, "find: cannot stat %s\n", buf);
                continue;
            }
            // If valid directory, recursive inside more
            if (st.type == T_DIR) {
                find(buf, filename);
            }
            // If encounter file and file name is matched, print the path and return.
            else if (st.type == T_FILE && strcmp(de.name, filename) == 0) {
                printf("%s\n", buf);
            }
        }
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("The number of arguments does not match\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
