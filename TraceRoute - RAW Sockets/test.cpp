#include <bits/stdc++.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <sys/time.h>
#include <iomanip>

using namespace std;

#define SRC "10.5.16.227"

struct sockaddr_in daddr;
struct iphdr *ip;
struct icmphdr *icmp;
fd_set rd,wr,er;

unsigned short csum (unsigned short *buf, int nwords)
{
    unsigned long sum;
    for (sum = 0; nwords > 0; nwords--)
      sum += *buf++;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return ~sum;
}

int main (int argc, char *argv[])
{
    if (argc != 2)
    {
      printf ("need destination for tracert\n");
      exit (0);
    }

    int sfd, hop = 0, one = 1;
    int checksum;
    char packet[1000];
    char packet_recv[1000];

    if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
      perror("error:");
      exit(EXIT_FAILURE);
    }
    
    struct timeval it;
    struct timeval fi;

    const int *val = &one;
    if (setsockopt (sfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("Cannot set HDRINCL!\n");

    ip = (struct iphdr *) packet;

    char name[4096] = {0};
    int flag = 0;
    
    if(argv[1][0]>'0' && argv[1][0]<'9')
    {

    }
    else
    {
        strcpy(name,argv[1]);
        flag = 1;
        struct hostent *he;
        struct in_addr **addr_list;
        if ((he = gethostbyname(argv[1])) == NULL) {  // get the host info
            herror("gethostbyname");
            return 2;
        }
        addr_list = (struct in_addr **)he->h_addr_list;
        strcpy(argv[1],inet_ntoa(*addr_list[0]));
    }

    daddr.sin_port = 0;
    daddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], (struct in_addr *)&daddr.sin_addr.s_addr);

    if (flag == 1)
        printf("\ntraceroute to %s (%s), 30 hops max, 28 byte packets\n",name,argv[1]);
    else
        printf("\ntraceroute to %s (%s), 30 hops max, 28 byte packets\n",argv[1],argv[1]);

    while (1)
    {
        fd_set master;
        fd_set read_fds;
        FD_SET(sfd,&master);
        struct timeval tm;
        tm.tv_sec = 5;
        tm.tv_usec = 0;
        int fd_max;
        fd_max = sfd;
        int flag1=0;
        char t_d1[10],t_d2[10],t_d3[10];

        ip->ihl = 5;
        ip->version = 4;
        ip->tos = 0;
        ip->tot_len = 20 + 8;
        ip->frag_off = 0;
        ip->ttl = hop;
        ip->protocol = IPPROTO_ICMP;
        inet_pton (AF_INET, "10.5.16.215", &(ip->saddr));
        inet_pton (AF_INET, argv[1], &(ip->daddr));
        ip->check = 0;

        icmp = (struct icmphdr *)&packet[20];

        icmp->type = ICMP_ECHO;
        icmp->code = 0;
        icmp->checksum = 0;
        icmp->un.echo.id = 0;
        icmp->un.echo.sequence = hop + 1;
        icmp->checksum = csum ((unsigned short *)&packet[20], 4);

        double time_diff1,time_diff2,time_diff3;
        gettimeofday(&it,NULL);

        if (sendto(sfd, (char *)packet, sizeof(struct iphdr) + sizeof(struct icmphdr), 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr))<0)
              perror("packet send error:");

        struct sockaddr_in addr2;
        socklen_t len = sizeof (struct sockaddr_in);

        FD_ZERO(&rd);
        FD_ZERO(&wr);
        FD_ZERO(&er);
        FD_SET(sfd,&rd);

        read_fds = master;
        if (select(fd_max+1,&read_fds,NULL,NULL,&tm)==-1)
        {
          perror("select");
          exit(0);
        }
        memset(packet_recv,'\0',sizeof(packet_recv));

        if(FD_ISSET(sfd,&read_fds))
        {
            recvfrom(sfd, (char *)&packet_recv, sizeof(packet_recv), 0,(struct sockaddr *)&addr2, &len);
                      
            gettimeofday(&fi,NULL);
            time_diff1 = (double)(fi.tv_sec-it.tv_sec) * 1000 + (double)(fi.tv_usec-it.tv_usec)/1000;
        }
        else
        {
            flag1 =1;       
            time_diff1 = -1;
        }

        gettimeofday(&it,NULL);
        if (sendto(sfd, (char *)packet, sizeof(struct iphdr) + sizeof(struct icmphdr), 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr))<0)
            perror("packet send error:");

        FD_ZERO(&rd);
        FD_ZERO(&wr);
        FD_ZERO(&er);
        FD_SET(sfd,&rd);

        read_fds = master;
        if (select(fd_max+1,&read_fds,NULL,NULL,&tm)==-1)
        {
          perror("select");
          exit(0);
        }

        memset(packet_recv,'\0',sizeof(packet_recv));

        if(FD_ISSET(sfd,&read_fds))
        {
            if (recvfrom(sfd, (char *)&packet_recv, sizeof(packet_recv), 0,(struct sockaddr *)&addr2, &len) < 0)
                perror("packet receive error:");
            gettimeofday(&fi,NULL);
            time_diff2 = (double)(fi.tv_sec-it.tv_sec) * 1000 + (double)(fi.tv_usec-it.tv_usec)/1000;
        }
        else
        {
          time_diff2 = -1;
        }

        gettimeofday(&it,NULL);
        if (sendto(sfd, (char *)packet, sizeof(struct iphdr) + sizeof(struct icmphdr), 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr))<0)
            perror("packet send error:");

        FD_ZERO(&rd);
        FD_ZERO(&wr);
        FD_ZERO(&er);
        FD_SET(sfd,&rd);

        read_fds = master;
        if (select(fd_max+1,&read_fds,NULL,NULL,&tm)==-1)
        {
          perror("select");
          exit(0);
        }

        memset(packet_recv,'\0',sizeof(packet_recv));
        if(FD_ISSET(sfd,&read_fds))
        {
            if (recvfrom(sfd, (char *)&packet_recv, sizeof(packet_recv), 0,(struct sockaddr *)&addr2, &len) < 0)
                perror("packet receive error:");
            gettimeofday(&fi,NULL);
            time_diff3 = (double)(fi.tv_sec-it.tv_sec) * 1000 + (double)(fi.tv_usec-it.tv_usec)/1000;
        }
        else
        {
          time_diff3 = -1;
        }

        if(time_diff1 == -1)
          sprintf(t_d1,"*");
        else
        {
          sprintf(t_d1,"%0.3f ms",time_diff1);
        }

        if(time_diff2 == -1)
          sprintf(t_d2,"*");
        else
        {
          sprintf(t_d2,"%0.3f ms",time_diff2);
        }

        if(time_diff3 == -1)
          sprintf(t_d3,"*");
        else
        {
          sprintf(t_d3,"%0.3f ms",time_diff3);
        }

        icmp = (struct icmphdr *)&packet_recv[20];
        
        if (icmp->type != 0)
        {    
            printf("-----------> %d\n",icmp->type );
            if(hop)
            {
                struct hostent *he;
                struct in_addr ipv4addr;
                inet_pton(AF_INET, inet_ntoa (addr2.sin_addr), &ipv4addr);
                if((he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET)) == NULL)
                {
                    cout << setw(2)  <<  hop  <<  setw(30)  <<  inet_ntoa (addr2.sin_addr)  <<  setw(20)  <<  inet_ntoa (addr2.sin_addr)  <<  setw(15)  <<  t_d1  <<  setw(15)  <<  t_d2  <<  setw(15)  <<  t_d3  <<  endl;
                    //printf ("%d %s (%s)  %s  %s  %s\n", hop, inet_ntoa (addr2.sin_addr),inet_ntoa (addr2.sin_addr),t_d1,t_d2,t_d3);
                }
                else
                {
                    cout << setw(2) << hop << setw(30) << he->h_name << setw(20) << inet_ntoa (addr2.sin_addr) << setw(15) << t_d1 << setw(15) << t_d2 << setw(15) << t_d3 << endl;
                    //printf ("%d %s (%s)  %s  %s  %s\n", hop, he->h_name,inet_ntoa (addr2.sin_addr),t_d1,t_d2,t_d3);
                }
                
                if(icmp->type == 3)
                {
                    printf("Destination Unreachable\n");
                    exit(0);
                }
            }
        }
        else if(flag1==0)
        {
            printf("-----------> %d\n",icmp->type );
            struct hostent *he;
            struct in_addr ipv4addr;
            inet_pton(AF_INET, inet_ntoa (addr2.sin_addr), &ipv4addr);
            if((he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET)) == NULL)
            {
                cout << setw(2) << hop << setw(30) << inet_ntoa (addr2.sin_addr) << setw(20) << inet_ntoa (addr2.sin_addr) << setw(15) << t_d1 << setw(15) << t_d2 << setw(15) << t_d3 << endl;
                //printf ("%d %s (%s)  %s  %s  %s\n", hop, inet_ntoa (addr2.sin_addr),inet_ntoa (addr2.sin_addr),t_d1,t_d2,t_d3);
            }
            else
            {
                cout << setw(2) << hop << setw(30) << he->h_name << setw(20) << inet_ntoa (addr2.sin_addr) << setw(15) << t_d1 << setw(15) << t_d2 << setw(15) << t_d3 << endl;
                //printf ("%d %s (%s)  %s  %s  %s\n", hop, he->h_name,inet_ntoa (addr2.sin_addr),t_d1,t_d2,t_d3);
            }
            if(icmp->type == 3)
            {
                printf("Destination Unreachable\n");
                exit(0);
            }
            exit (0);
        }
        else
        {
            printf("-----------> %d\n",icmp->type );
            cout << setw(2) << hop << setw(5)<<t_d1 << setw(5) << t_d2 << setw(5) << t_d3 << endl;
            //printf ("%d %s  %s  %s\n", hop,t_d1,t_d2,t_d3);
        }
        
        hop++;

        if(hop>30)
            break;
    }

    return 0;
}