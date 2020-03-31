#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

struct termios o_attr, n_attr;
struct termios chgattr(struct termios);

char time_buf[25];
void time_f(char *);

int open_port(char *, int);

int main(int argc, char *argv[])
{
    /* set non-blocking getchar() */
    tcgetattr(0, &o_attr);
    n_attr = chgattr(o_attr);
    tcsetattr(0, TCSANOW, &n_attr);

    int rcv_fd = open_port(argc == 3 ? argv[2] : "/dev/ttyS1", O_RDONLY);
    int snd_fd = open_port(argc == 3 ? argv[1] : "/dev/ttyS2", O_WRONLY);

    char snd_buf[257] = {0};
    char rcv_buf[257] = {0};
    unsigned char snd_head = 0;
    unsigned char rcv_head = 0;

    char key = 0;
    while (key != 27)
    {
        /* 
         * Always try reading 1 Byte from rcv_fd,
         * which indicates the size of a following string.
         * If succussful, accept all that follows.
         */
        if (rcv_head == 0)
            read(rcv_fd, &rcv_head, 1);
        else if (read(rcv_fd, rcv_buf, rcv_head) > -1)
        {
            time_f(time_buf);
            printf("[%s] RECV: %s\n", time_buf, rcv_buf);
            memset(rcv_buf, 0, sizeof(rcv_buf));
            rcv_head = 0;
        }

        /*
         * Echo and store console input in snd_buf,
         * and send message on reading '\n'.
         * Before writing content, its size is given in 1 Btye.
         */
        key = getchar();
        if (isprint(key))
        {
            putchar(key);
            snd_buf[strlen(snd_buf)] = key;
        }
        else if (key == '\n')
        {
            snd_head = strlen(snd_buf);
            write(snd_fd, &snd_head, 1);
            write(snd_fd, snd_buf, strlen(snd_buf));
            for (unsigned char i = 0; i < snd_head; ++i)
                putchar('\b');
            time_f(time_buf);
            printf("[%s] SENT: %s\n", time_buf, snd_buf);
            memset(snd_buf, 0, sizeof(snd_buf));
        }
    }

    /* restore console attrs */
    tcsetattr(0, TCSANOW, &o_attr);
    return (0);
}

struct termios chgattr(struct termios attr)
{
    attr.c_lflag &= ~ICANON;
    attr.c_lflag &= ~ECHO;
    attr.c_lflag &= ~ISIG;
    attr.c_cc[VMIN] = 0;
    attr.c_cc[VTIME] = 0;
    return attr;
}

int open_port(char *dev, int flag)
{
    int fd = -1;
    while (fd == -1)
    {
        fd = open(dev, flag | O_NONBLOCK);
        /* see fifo(4) */
        if (getchar() == 27)
            exit(-1);
    }
    //fcntl(fd, F_SETFL, O_NONBLOCK);
    return fd;
}

/* dump current time */
void time_f(char *buf)
{
    time_t t = time(NULL);
    char *s = ctime(&t);
    s[strlen(s) - 1] = '\0';
    strcpy(buf, s);
}
