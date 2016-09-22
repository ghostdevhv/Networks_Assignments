#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <ifaddrs.h>
#include <math.h>

char dst_addr[15];
char src_addr[15];
char packet[1000];
char packet_recv[1000];
double time_dev[1000000];

#define ECHO_REPLY      0
#define DEST_UNREACH    3
#define REDIRECT        5
#define ECHO_REQUEST    8
#define TIME_EXCEEDED   11
#undef max
#define max(x,y) ((x) > (y) ? (x) : (y))

clock_t send_time,recv_time;

struct iphdr* ip;
struct icmphdr* icmphd;
struct sockaddr_in daddr, saddr, client_addr;
struct timeval start_time,end_time;

int sfd;
int sent_packets,received_packets;
double min_rtt,max_rtt,avg_rtt;

unsigned short csum (unsigned short *buf, int nwords)
{
    unsigned long sum;
    for (sum = 0; nwords > 0; nwords--)
      sum += *buf++;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return ~sum;
}

void set_icmppayload(){
	struct timeval *tt = (struct timeval *)&packet[28];
	gettimeofday(tt, NULL);
}

void handler(int signum){

	int i;
	double avg = avg_rtt/received_packets;
	double mean_dev;

	for(i = 1 ; i <= received_packets ; i++)
	{
		mean_dev += fabs(time_dev[i]-avg);
	}
	mean_dev /= received_packets;

	gettimeofday(&end_time, NULL);
	suseconds_t difference = 1000000*end_time.tv_sec + end_time.tv_usec -1000000*start_time.tv_sec - start_time.tv_usec;
	double diff = (double)(difference)/1000;
	int dif = (int)diff;
	printf("\n--- %s ping statistics ---\n",dst_addr);
	int packet_loss = sent_packets - received_packets;
	printf("%d packets transmitted, %d received, %d%% packet loss, time %dms\n",sent_packets,received_packets,(int)((((double)packet_loss)/sent_packets)*100),dif);
	if(min_rtt != 1000000000 && max_rtt != 0 && avg_rtt != 0)
	{
	    printf("rtt min/avg/max/mdev = %0.3f/%0.3f/%0.3f/%0.3f ms\n",min_rtt,avg,max_rtt,mean_dev);
	}
	close(sfd);
	exit(0);
}

void ping()
{
	max_rtt = 0;
	min_rtt = 1000000000;
	avg_rtt = 0;
	sent_packets = 0;
	received_packets = 0;

	int sequence_no = 1;
	struct icmphdr *icmphd2;

	signal(SIGINT,handler);

	ip = (struct iphdr *) packet;
	ip->ihl = 5;
	ip->version = 4;
	ip->tos = 0;
	ip->tot_len = 0;					/* 16 byte value */
	ip->frag_off = 0;					/* no fragment */
	ip->ttl = 48;				/* default value */
	ip->protocol = IPPROTO_ICMP;			/* protocol at L4 */
	ip->check = 0;
	inet_pton (AF_INET,src_addr, &(ip->saddr));
    inet_pton (AF_INET,dst_addr, &(ip->daddr));

	while(1){
	    sleep(1);
	    set_icmppayload();
	   
	   	icmphd = (struct icmphdr *)&packet[20];

		icmphd->type = ICMP_ECHO;
		icmphd->code = 0;
		icmphd->checksum = 0;
		icmphd->un.echo.id = 0;
		icmphd->un.echo.sequence = 1;
		icmphd->checksum = csum ((unsigned short *)&packet[20], 12);

	    struct timeval recv_time;
	    if(sendto(sfd, packet, 44, 0, (struct sockaddr *)&daddr, (socklen_t)sizeof(daddr)) < 0){
	          perror("Unable to send: ");
	    }
	    sent_packets++;

	    int nfds = 0;
	    fd_set rd, wr, er;
	    FD_ZERO(&rd);
	    FD_ZERO(&wr);
	    FD_ZERO(&er);
	    FD_SET(sfd, &rd);
	    nfds = max(nfds, sfd);
	    struct timeval tv;
	    tv.tv_sec = 10;
	    tv.tv_usec = 0;
	    int readyFDCount = select(nfds + 1, &rd, NULL, NULL, &tv);
	    if(readyFDCount == -1){
	          perror("Error in select");
	          exit(0);
	    }
	    if (FD_ISSET(sfd, &rd))
	    {

          	memset(packet_recv, 0, sizeof(packet_recv));
          	socklen_t fromlen = sizeof(client_addr);
          	int size = recvfrom(sfd, (char *)&packet_recv, sizeof(packet_recv), 0, (struct sockaddr *)&client_addr, &fromlen);
          	if (size < 0)
            	perror("packet receive error: ");
          	received_packets++;

          	struct timeval *tt = (struct timeval *) (packet_recv + 28);
          	gettimeofday(&recv_time, NULL);
          	suseconds_t difference = 1000000*recv_time.tv_sec + recv_time.tv_usec -1000000*(tt->tv_sec) - (tt->tv_usec);
          	double diff = (double)(difference)/1000;
          	icmphd2 = (struct icmphdr *) (packet_recv + 20);

          	if(diff > max_rtt)      
          		max_rtt = diff;
          	if(diff < min_rtt)      
          		min_rtt = diff;

          	avg_rtt += diff;
          	time_dev[received_packets] = diff;

          	//get_host_name(client_addr.sin_addr);

          	printf("%d bytes from %s : icmp_seq=%d ttl=48 time=%f ms\n",size, dst_addr, icmphd2->un.echo.sequence, diff);
          	
	    }
	    sequence_no++;
	}
}
int main(int argc, char* argv[])
{
	if (getuid() != 0)
	{
		fprintf(stderr, "%s: root priviledges needed\n", *(argv + 0));
		exit(EXIT_FAILURE);
	}

	if(argc!=3)
	{
		printf("Check Arguments\n");
		exit(EXIT_FAILURE);
	}

	strcpy(src_addr,argv[1]);
	strcpy(dst_addr,argv[2]);

	gettimeofday(&start_time, NULL);
	printf("Source address: %s\n", argv[1]);
	printf("Destination address: %s\n", argv[2]);

	int one = 1;

	if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	const int *val = &one;
    if (setsockopt (sfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    	printf ("Cannot set HDRINCL!\n");

	daddr.sin_port = 0;
    daddr.sin_family = AF_INET;
    saddr.sin_port = 0;
    saddr.sin_family = AF_INET;

    inet_pton(AF_INET, argv[2], (struct in_addr *)&daddr.sin_addr.s_addr);
    inet_pton(AF_INET, argv[1], (struct in_addr *)&saddr.sin_addr.s_addr);

    memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));
	memset(saddr.sin_zero, 0, sizeof(saddr.sin_zero));
    
    printf("PING %s (%s)\n",argv[2],argv[2]);
	
	ping();
	return 0;
}