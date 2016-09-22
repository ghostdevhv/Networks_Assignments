#include <bits/stdc++.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

using namespace std;
// RTLP header's structure
struct rtlpheader {
	unsigned int rtlp_chksum;
	unsigned short int rtlp_type;
 	unsigned short int rtlp_srcport;
 	unsigned short int rtlp_destport;
 	unsigned int rtlp_seqnum;
 	unsigned int rtlp_acknum;
};

struct sockaddr_in daddr,saddr;
struct timeval tv;
struct iphdr *ip;
struct rtlpheader *rtlp;
int client_seqnum,server_seqnum;
unsigned int fromlen;
fd_set rd, wr, er;


#define DEST "10.117.11.131"
#define SRC "10.117.11.102"

#define port_src 23456
#define port_dest 23466

#define SYN 1
#define FIN 2
#define ACK 3
#define DATA 4

unsigned int csum(unsigned short *buf, int nwords)
{
    unsigned long sum;
    for(sum=0; nwords>0; nwords--)
        sum += *buf++;
    sum = (sum >> 16) + (sum & 0xffff);
   	sum += (sum >> 16);
    return (unsigned int)(~sum);
}

void set_ip_and_rtlp(int s, char *packet)
{
	daddr.sin_family = AF_INET;
	saddr.sin_family = AF_INET;
	daddr.sin_port = 0; /* not needed in SOCK_RAW */
	saddr.sin_port = 0; /* not needed in SOCK_RAW */
	inet_pton(AF_INET, SRC, (struct in_addr *)&saddr.sin_addr.s_addr);
	inet_pton(AF_INET, DEST, (struct in_addr *)&daddr.sin_addr.s_addr);
	memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));
	memset(saddr.sin_zero, 0, sizeof(saddr.sin_zero));

	ip->ihl = 5;
	ip->version = 4;
	ip->tos = 0;
	ip->tot_len = htons(50);
	ip->frag_off = 0;
	ip->ttl = 64;
	ip->protocol = IPPROTO_RAW;
	ip->check = 0;
	ip->saddr = saddr.sin_addr.s_addr;
	ip->daddr = daddr.sin_addr.s_addr;

	rtlp->rtlp_srcport = htons(port_src);
	rtlp->rtlp_destport = htons(port_dest);
	rtlp->rtlp_seqnum = rand()%300 +1;

	client_seqnum = rtlp->rtlp_seqnum;
	rtlp->rtlp_acknum = 1000;
	rtlp->rtlp_chksum = csum((unsigned short *)&packet[28], sizeof(struct rtlpheader));
	fromlen = sizeof(daddr);
	rtlp->rtlp_type = SYN;
}

void start_handshake(int s, char *packet)
{
	daddr.sin_family = AF_INET;
	saddr.sin_family = AF_INET;
	daddr.sin_port = 0; /* not needed in SOCK_RAW */
	saddr.sin_port = 0; /* not needed in SOCK_RAW */
	inet_pton(AF_INET, SRC, (struct in_addr *)&saddr.sin_addr.s_addr);
	inet_pton(AF_INET, DEST, (struct in_addr *)&daddr.sin_addr.s_addr);
	memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));
	memset(saddr.sin_zero, 0, sizeof(saddr.sin_zero));

	ip->ihl = 5;
	ip->version = 4;
	ip->tos = 0;
	ip->tot_len = htons(50);
	ip->frag_off = 0;
	ip->ttl = 64;
	ip->protocol = IPPROTO_RAW;
	ip->check = 0;
	ip->saddr = saddr.sin_addr.s_addr;
	ip->daddr = daddr.sin_addr.s_addr;

	rtlp->rtlp_srcport = htons(port_src);
	rtlp->rtlp_destport = htons(port_dest);
	rtlp->rtlp_seqnum = rand()%300 +1;

	client_seqnum = rtlp->rtlp_seqnum;
	rtlp->rtlp_acknum = 1000;
	rtlp->rtlp_chksum = csum((unsigned short *)&packet[28], sizeof(struct rtlpheader));
	fromlen = sizeof(daddr);
	rtlp->rtlp_type = SYN;
	while(1){

		printf("%d  |  %d  | %d   |  %d  | %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum, rtlp->rtlp_acknum,rtlp->rtlp_chksum);

		if (sendto(s, (char *)packet, sizeof(packet), 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr))<0)
			perror("packet send error:");
		else
		{
			printf("Successfully Sent a SYN Packet to Server\n");
			printf("%d  |  %d  | %d   |  %d  | %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum, rtlp->rtlp_acknum,rtlp->rtlp_chksum);
		}
		// sleep(10);
		FD_ZERO(&rd);
	    FD_ZERO(&wr);
	    FD_ZERO(&er);
		FD_SET(s, &rd);
		int err = select(s + 1, &rd, NULL, NULL, &tv);
		if(err == -1)
		{
			perror("Error in select");
			exit(EXIT_FAILURE);
		}
		if(FD_ISSET(s,&rd)){
			if (recvfrom(s, (char *)&packet, sizeof(packet), 0,(struct sockaddr *)&saddr, &fromlen) < 0)
			{
				perror("packet receive error:");
				continue;
			}
			else
			{
				printf("\nSuccessfully Connected with Server\n");
				ip = (struct iphdr *)packet;
				rtlp = (struct rtlpheader *)&packet[20];
				printf("%d  |  %d  | %d   |  %d  | %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum, rtlp->rtlp_acknum,rtlp->rtlp_chksum);
				server_seqnum = rtlp->rtlp_seqnum;
				break;
			}
		}
	}

	if(rtlp->rtlp_acknum == client_seqnum && rtlp->rtlp_seqnum>=0 && rtlp->rtlp_type ==10*SYN+ACK)
	{
		printf("\nSend an Acknowledgement\n");
		swap(rtlp->rtlp_srcport,rtlp->rtlp_destport);
		swap(ip->saddr,ip->daddr);
		rtlp->rtlp_acknum = rtlp->rtlp_seqnum;
		rtlp->rtlp_seqnum = client_seqnum;
		rtlp->rtlp_type =  ACK;
		printf("%d  |  %d  | %d   |  %d  | %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum, rtlp->rtlp_acknum,rtlp->rtlp_chksum);
		if (sendto(s, (char *)packet, sizeof(packet), 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr))<0)
			perror("packet send error:");
		printf("\n\n\n");

	}
	else
		printf("Server refused\n");
}

void send_data(int s, char *packet)
{
	while(1){
		if (sendto(s, (char *)packet, sizeof(packet), 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr))<0)
			perror("packet send error:");
		else
		{
			printf("Successfully Sent a Packet Containing data\n");
			printf("%d  |  %d  | %d   |  %d  | %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum, rtlp->rtlp_acknum,rtlp->rtlp_chksum);

			for(int i=40;i<sizeof(packet);i++)
				printf("%c",packet[i]);
		}
		FD_ZERO(&rd);
	    FD_ZERO(&wr);
	    FD_ZERO(&er);
		FD_SET(s, &rd);
		int err = select(s + 1, &rd, NULL, NULL, &tv);
		if(err == -1)
		{
			perror("Error in select");
			exit(EXIT_FAILURE);
		}
		if(FD_ISSET(s,&rd)){
			if (recvfrom(s, (char *)&packet, sizeof(packet), 0,(struct sockaddr *)&saddr, &fromlen) < 0)
			{
				perror("packet receive error:");
				continue;
			}
			else break;
		}
	}
}

void terminate(int s, char *packet)
{
	rtlp->rtlp_srcport = htons(port_src);
	rtlp->rtlp_destport = htons(port_dest);
	rtlp->rtlp_seqnum = 500;
	rtlp->rtlp_acknum = -1;
	client_seqnum = rtlp->rtlp_seqnum;
	rtlp->rtlp_type = FIN;
	while(1){
		if (sendto(s, (char *)packet, sizeof(packet), 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr))<0)
			perror("packet send error:");
		else
		{
			printf("Successfully Sent a FIN Packet to Server\n");
			printf("%d  |  %d  | %d   |  %d  | %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum, rtlp->rtlp_acknum,rtlp->rtlp_chksum);
		}
		FD_ZERO(&rd);
	    FD_ZERO(&wr);
	    FD_ZERO(&er);
		FD_SET(s, &rd);
		int err = select(s + 1, &rd, NULL, NULL, &tv);
		if(err == -1)
		{
			perror("Error in select");
			exit(EXIT_FAILURE);
		}
		if(FD_ISSET(s,&rd)){
			if (recvfrom(s, (char *)&packet, sizeof(packet), 0,(struct sockaddr *)&saddr, &fromlen) < 0)
			{
				perror("packet receive error:");
				continue;
			}
			else break;
		}
	}

	if(rtlp->rtlp_type == 10*FIN+ACK)
	{
		printf("\nSuccessfully Received FIN + ACK packet from Server\n");
		ip = (struct iphdr *)packet;
		rtlp = (struct rtlpheader *)&packet[20];
		printf("%d  |  %d  | %d   |  %d  | %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum, rtlp->rtlp_acknum,rtlp->rtlp_chksum);
		server_seqnum = rtlp->rtlp_seqnum;
	}

	if(rtlp->rtlp_acknum == client_seqnum && rtlp->rtlp_seqnum>=301 && rtlp->rtlp_type == 10*FIN+ACK)
	{
		printf("\nSend a Final Acknowledgement to Server \n");
		swap(rtlp->rtlp_srcport,rtlp->rtlp_destport);
		swap(ip->saddr,ip->daddr);
		rtlp->rtlp_acknum = rtlp->rtlp_seqnum;
		rtlp->rtlp_seqnum = client_seqnum;
		rtlp->rtlp_type = ACK;
		printf("%d  |  %d  | %d   |  %d  | %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum, rtlp->rtlp_acknum,rtlp->rtlp_chksum);
		if (sendto(s, (char *)packet, sizeof(packet), 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr))<0)
			perror("packet send error:");
		printf("\n\n");

	}
	else	printf("Server refused\n");
	printf("************ Connection Closed *************\n");
}
