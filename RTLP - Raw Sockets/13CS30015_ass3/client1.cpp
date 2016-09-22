#include "client_header.h"

using namespace std;
int main()
{
	int s;
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
		perror("error:");
		exit(EXIT_FAILURE);
	}
	tv.tv_sec = 0;
    tv.tv_usec = 80000;
	srand(time(NULL));
	char packet[1000] ;

	ip = (struct iphdr *)packet;
	rtlp = (struct rtlpheader *)&packet[20];




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
		//sleep(10);
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

	//set_ip_and_rtlp(s,packet);
	//start_handshake(s,packet);
	/* Normal Interaction Starts */

	int kk = 1;
	char rtlp_buffer[10000] = {"ECHO REQ "};
	char temp[100];
	int i;
	while(1)
	{
		int n;
		printf("Want to terminate the sesion. Press 1 for Yes | Press 2 for No\n");
		scanf("%d",&n);
		if(n==1)
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
			close(s);
			exit(EXIT_SUCCESS); /// Terminate the session
		}
		else if (n==2)
		{
			strcpy(packet+40,rtlp_buffer);
			sprintf(temp,"%d",kk++);
			int j=0;

			for(i=49;temp[j]!='\0';i++)
				packet[i] = temp[j++];
			packet[i] = '\0';

			for(i=40;packet[i]!='\0';i++)
			{
				rtlp->rtlp_seqnum ++;
			}
			rtlp->rtlp_chksum = csum((unsigned short *)&packet[28], sizeof(struct rtlpheader));
			rtlp->rtlp_type = DATA;

			//send_data(s,packet);

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

			unsigned int checker = csum((unsigned short *)&packet[28], sizeof(struct rtlpheader));
			ip = (struct iphdr *)packet;
			rtlp = (struct rtlpheader *)&packet[20];

			if(checker == rtlp->rtlp_chksum)
			{
				printf("\nSuccessfully Receive the Acknowledgement\n");
				printf("%d  |  %d  | %d   |  %d  | %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum, rtlp->rtlp_acknum,rtlp->rtlp_chksum);

				if(rtlp->rtlp_type==ACK)
				{
					continue;
				}
				else if(rtlp->rtlp_type==10*ACK+DATA)
				{

					int c = 0;
					int d = rtlp->rtlp_seqnum - server_seqnum;
					server_seqnum = rtlp->rtlp_seqnum;

					for(int i=40;packet[i]!='\0';i++)
					{
						c++;
					}
					if(c==d)
					{
						for(int i=40;packet[i]!='\0';i++)
							printf("%c", packet[i]);
						printf("\n\n");
						swap(rtlp->rtlp_srcport,rtlp->rtlp_destport);
						swap(ip->saddr,ip->daddr);
						swap(rtlp->rtlp_seqnum,rtlp->rtlp_acknum);

					}
				}
			}
			else
			{
				printf("Packet Apna ni hai :/ \n");
			}
		}
		else continue;
	}

	/*-----------------Terminaton Part Starts----------------------------*/

	exit(EXIT_SUCCESS);
}
