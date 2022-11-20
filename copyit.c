#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define FILELENGTH 100

void display_message(int s)
{
    printf("copyit: still copying...\n");
    alarm(1);
}

int main(int argc, char **argv)
{
    // open, creat, read, write, close, strerror, errno, exit, signal, alarm

    int fs;        // file source
    int fd;        // file destination
    int count = 0; // count the bytes
    int bytes = 1000;
    char buffer[bytes];
    char duplication;

    signal(SIGALRM, display_message);
    alarm(1);

    // check the number of args
    if (argc > 3)
    {
        printf("copyit: Too many arguments!\n");
        printf("usage: copyit <sourcefile> <targetfile>\n");
        exit(1);
    }
    else if (argc < 3)
    {
        printf("copyit: Too few arguments!\n");
        printf("usage: copyit <sourcefile> <targetfile>\n");
        exit(1);
    }

    // open the source file
    fs = open(argv[1], O_RDONLY);
    if (!fs)
    {
        printf("copyit: Couldn't open file %s: %s\n", argv[1], strerror(errno));
        exit(1);
    }

    // create the destination file
    fd = creat(argv[2], 0600);
    if (!fd)
    {
        printf("copyit: Couldn't create file %s: %s\n", argv[2], strerror(errno));
        exit(1);
    }

    // Continue loop until the entire file is read and copied to created file
    while (duplication != EOF)
    {
        // read source file
        int num = read(fs, buffer, bytes);
        if (num == EOF)
        {
            printf("copyit: Couldn't read file %s: %s", argv[1], strerror(errno));
            exit(1);
        }
        else if (num == 0)
        {
            duplication = EOF;
        }
        // write to destination file
        num = write(fd, buffer, num);
        if (num == EOF)
        {
            printf("copyit: Couldn't write to file %s: %s", argv[2], strerror(errno));
            exit(1);
        }

        // print the message if it takes more than 1 second
        pause();
        count += num;
    }

    printf("\ncopyit: Copied %d bytes from file %s to %s.\n", count, argv[1], argv[2]);

    // close both the files
    close(fs);
    close(fd);

    return 0;
}
