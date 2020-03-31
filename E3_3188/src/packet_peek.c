#include <pcap/pcap.h>     /* libpcap */
#include <net/ethernet.h>  /* define struct ether_header & struct ether_addr */
#include <netinet/ip.h>    /* define struct iphdr & struct in_addr */
#include <arpa/inet.h>     /* define inet_ntoa() */
#include <netinet/ether.h> /* define ether_ntoa() */
#include <time.h>          /* define struct tm & strftime() */
#include <string.h>        /* define strcpy() */
#include <termios.h>       /* define struct termios */
struct termios init;       /* initial terminal i/o attributes */
void nblock();             /* set non-blocking keyboard input */
void restore();            /* restore terminal keyboard attributes */
char *dev;                 /* network interface name */
pcap_t *handle;            /* network interface handle */
struct ether_header *ehdr; /* ethernet packet header */
struct iphdr *ihdr;        /* ip packet header */
char smac[18];             /* source mac address in ASCII */
char dmac[18];             /* destination mac address in ASCII */
char sip[16];              /* source ip address in ASCII */
char dip[16];              /* destination ip address in ASCII */
struct timeval pkttv;      /* packet time stamp in struct timeval */
char tm_fmt[20];           /* packet time stamp in ASCII */
struct timeval lasttv;     /* previous time stamp */
bpf_u_int32 len;           /* total packet length since last time */
double rate;               /* bit rate in bps from last time */
#define MAXRATE 8388608    /* bit rate alert cap in bps */
FILE *fp;                  /* log file output */
/* handle packet */
void logger(unsigned char *user,
            const struct pcap_pkthdr *pkthdr,
            const unsigned char *packet)
{
    ehdr = (struct ether_header *)packet;
    if (ntohs(ehdr->ether_type) == ETHERTYPE_IP)
    {
        /* get packet time stamp in ASCII */
        pkttv = pkthdr->ts;
        strftime(tm_fmt, sizeof tm_fmt, "%Y-%m-%d %H:%M:%S", localtime(&pkttv.tv_sec));
        /* get packet mac address in ASCII */
        strcpy(smac, ether_ntoa((struct ether_addr *)ehdr->ether_shost));
        strcpy(dmac, ether_ntoa((struct ether_addr *)ehdr->ether_dhost));
        /* get packet ip address in ASCII */
        ihdr = (struct iphdr *)(packet + ETH_HLEN);
        strcpy(sip, inet_ntoa((struct in_addr){ihdr->saddr}));
        strcpy(dip, inet_ntoa((struct in_addr){ihdr->daddr}));
        fprintf(fp, "%s,%s,%s,%s,%s,%d\n", tm_fmt, smac, sip, dmac, dip, pkthdr->caplen);
        if (pkttv.tv_sec > lasttv.tv_sec)
        {
            rate = (double)len * 8.0 / 
                   ((double)(pkttv.tv_sec - lasttv.tv_sec) + 
                    (double)(pkttv.tv_usec - lasttv.tv_usec) / 1000000.0);
            lasttv = pkttv, len = 0;
            if (rate > MAXRATE)
            {
                printf("[%s][%s,%s] SEND %d bytes, current speed: %.2lf Kbps\n", tm_fmt, smac, sip, pkthdr->caplen, rate / 1024.0);
                printf("[%s][%s,%s] RECV %d bytes, current speed: %.2lf Kbps\n", tm_fmt, dmac, dip, pkthdr->caplen, rate / 1024.0);
            }
        }
        len += pkthdr->caplen;
    }
    if (getchar() == 'q')
        pcap_breakloop(handle);
}
int main()
{
    nblock();
    char errbuf[PCAP_ERRBUF_SIZE];
    /* find a network interface */
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL)
    {
        printf("Error finding device: %s\n", errbuf);
        return 1;
    }
    /* open device */
    handle = pcap_open_live(dev, BUFSIZ, 1, 0, errbuf);
    if (handle == NULL)
    {
        printf("Error opening device[%s]: %s\n", dev, errbuf);
        return 2;
    }
    printf("Opening device[%s] for capture, press 'q' to quit.\n\n", dev);
    /* open log file for writing */
    fp = fopen("log.csv", "a");
    /* initial time stamp */
    len = 0;
    gettimeofday(&lasttv, NULL);
    /* capture packets */
    pcap_loop(handle, 0, logger, NULL);
    /* closing device */
    pcap_close(handle);
    printf("\nDevice[%s] closed.\n", dev);
    restore();
    return 0;
}
void nblock()
{
    tcgetattr(0, &init);
    struct termios attr = init;
    attr.c_lflag &= ~ICANON;
    attr.c_lflag &= ~ECHO;
    attr.c_lflag &= ~ISIG;
    attr.c_cc[VMIN] = 0;
    attr.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &attr);
}
void restore()
{
    tcsetattr(0, TCSANOW, &init);
}