using namespace std;
#include "server_header.h"

int main()
{
	tv.tv_sec = 0;
    tv.tv_usec = 80000;
    srand(time(NULL));
	int s;
	char packet[1000];
    fromlen=sizeof(saddr);
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
		perror("error:");
		exit(EXIT_FAILURE);
	}
    FD_SET(s, &rd);
    ip = (struct iphdr *)packet;
    rtlp = (struct rtlpheader *)&packet[20];
	memset(packet, 0, sizeof(packet));
	socklen_t *len = (socklen_t *)sizeof(saddr);

    //startconnection(s,packet);
    ip = (struct iphdr *)packet;
    rtlp = (struct rtlpheader *)&packet[20];
    ip->saddr = saddr.sin_addr.s_addr;
    ip->daddr = daddr.sin_addr.s_addr;
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
    ip->tot_len = htons(50);	/* 16 byte value */
    ip->frag_off = 0;		/* no fragment */
    ip->ttl = 64;			/* default value */
    ip->protocol = IPPROTO_RAW;	/* protocol at L4 */
    ip->check = 0;			/* not needed in iphdr */

    if (recvfrom(s, (char *)&packet, sizeof(packet), 0,(struct sockaddr *)&saddr, &fromlen) < 0)
		perror("packet receive error:");
    ip = (struct iphdr *)packet;
    rtlp = (struct rtlpheader *)&packet[20];
    printf("%d %d %d %d %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum,rtlp->rtlp_acknum,rtlp->rtlp_chksum);
    printf("Received request for a Connection\n" );

    if(rtlp->rtlp_type==SYN)  //Always true
    {
        swap(ip->saddr,ip->daddr);
        rtlp->rtlp_acknum=rtlp->rtlp_seqnum;
        last_seq_no=rtlp->rtlp_seqnum;
        rtlp->rtlp_seqnum=rand()%(300)+1;
        swap(rtlp->rtlp_srcport,rtlp->rtlp_destport);
        printf("%d %d %d %d %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum,rtlp->rtlp_acknum,rtlp->rtlp_chksum);
        rtlp->rtlp_type=10*SYN+ACK;
        while(1){
            if (sendto(s, (char *)packet, sizeof(packet), 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr))<0)
                perror("packet send error:");
            printf("Sent to client\n" );
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
        ip=(struct iphdr *)packet;
        rtlp=(struct rtlpheader *)&packet[20];
        if(rtlp->rtlp_type!=ACK) perror("Error:");
        printf("%d %d %d %d %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum,rtlp->rtlp_acknum,rtlp->rtlp_chksum);
        printf("Received from client\nThree way hand shake completed.\nConnection created successfully\n");
        last_seq_no=rtlp->rtlp_seqnum;
    }


    int bytes_received,kk=2;
    char response[20],temp[10];
    strcpy(response,"ECHO RES ");
    while(1){
        if (recvfrom(s, (char *)&packet, sizeof(packet), 0,(struct sockaddr *)&saddr, &fromlen) < 0)
            perror("packet receive error:");
        ip=(struct iphdr *)packet;
        rtlp=(struct rtlpheader *)&packet[20];
        printf("%d %d %d %d %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum,rtlp->rtlp_acknum,rtlp->rtlp_chksum);

        //receive_data(s,packet,&saddr);
        if(rtlp->rtlp_type==FIN){
            break;
            exit(EXIT_SUCCESS);
        }
        unsigned int zz=csum((unsigned short *)&packet[28], sizeof(struct rtlpheader));
        if(zz!=rtlp->rtlp_chksum){
            printf("equal nhi aaya\n" );
        }
        else{
            bytes_received=rtlp->rtlp_seqnum-last_seq_no;
            last_seq_no=rtlp->rtlp_seqnum;
            printf("bytes received=%d\n",bytes_received );
            cout<<"Received :";
            for(int i=40,j=0;j<bytes_received;i++,j++) printf("%c",packet[i] );
            cout<<endl;
            sprintf(temp,"%d",kk);
            int i,j;
            for(i=9,j=0;temp[j]!='\0';j++,i++) response[i]=temp[j];
            kk++;
            response[i]='\0';
            strcpy(packet+40,response);
            int size=0;
            for(int i=40;packet[i]!='\0';i++) size++;
            swap(rtlp->rtlp_seqnum,rtlp->rtlp_acknum);
            swap(ip->saddr,ip->daddr);
            swap(rtlp->rtlp_srcport,rtlp->rtlp_destport);
            cout<<"Sent :";
            for(int i=40,j=0;j<size;j++,i++) printf("%c", packet[i]);
            cout<<endl<<size<<endl;
            rtlp->rtlp_seqnum+=size;
            rtlp->rtlp_chksum=csum((unsigned short *)&packet[28],sizeof(struct rtlpheader));
            printf("%d %d %d %d %d\n",rtlp->rtlp_srcport,rtlp->rtlp_destport,rtlp->rtlp_seqnum,rtlp->rtlp_acknum,rtlp->rtlp_chksum);
            if(size==0) rtlp->rtlp_type=ACK;
            else rtlp->rtlp_type=10*ACK+DATA;
            if (sendto(s, (char *)packet, sizeof(packet), 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr))<0)
                perror("packet send error:");

            //send_data(s,packet,size);
        }
    }
    printf("Received termination request from client\n");
    rtlp->rtlp_acknum=rtlp->rtlp_seqnum;
    rtlp->rtlp_seqnum=501;
    swap(ip->saddr,ip->daddr);
    swap(rtlp->rtlp_srcport,rtlp->rtlp_destport);
    rtlp->rtlp_chksum=csum((unsigned short *)packet+28,sizeof(struct rtlpheader));
    rtlp->rtlp_type=10*FIN+ACK;
    while(1){
        if (sendto(s, (char *)packet, sizeof(packet), 0,(struct sockaddr *)&daddr, (socklen_t)sizeof(daddr))<0)
            perror("packet send error:");
        printf("Accepted termination request\n");
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
    if(rtlp->rtlp_type==ACK)   printf("Final ACK packet received...Connection Closed...\n");
    else printf("Final ACK packet not received\n");
}
