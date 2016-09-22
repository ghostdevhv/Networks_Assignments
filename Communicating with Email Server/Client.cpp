 #include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h> // For the socket (), bind () etc. functions.
#include <netinet/in.h> // For IPv4 data struct..
#include <arpa/inet.h> // For inet_pton (), inet_ntop ().
#include <time.h>
using namespace std;

#define BUF_SIZE 1000

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[1000];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}

int main(int argc, char *argv[])
{
	int num,check,sfd,rst;
	sfd = socket(AF_INET, SOCK_STREAM,0);
	char buf[1000];

	if(sfd== -1)
	{
		perror("Client: Socket Error");
		exit(1);
	}
	printf("Socket fd = %d\n",sfd);

	struct sockaddr_in srv_addr, cli_addr; // Addresses of the server and the client.
    socklen_t addrlen = sizeof (struct sockaddr_in); // size of the addresses.

    // Clear the two addresses.
    memset (&srv_addr, 0, addrlen);

    // Assign values to the server address.
    srv_addr.sin_family = AF_INET; // IPv4.
	char from[1000],to[1000],message[1000],date[1000],username[1000],password[1000];
	char **tOkens;
	char *tOken;
	char name[1000];
	while(1){
		printf("Do you want to (1) Send mail or (2) Receive mails or (3) exit?\n");
		cin>>num;
		if(num==1){      // send
            srv_addr.sin_port = htons(25);
            rst = inet_pton (AF_INET, "10.5.30.131", &srv_addr.sin_addr);
            if (rst <= 0)
            {
                perror ("Client Presentation to network address conversion.\n");
                exit (1);
            }
            rst = connect (sfd, (struct sockaddr *) &srv_addr, addrlen);
            if (rst == -1)
            {
                perror ("Client: Connect failed.");
                exit (1);
            }
            memset(buf,0,sizeof(buf));
            //sleep(1);
            rst = recv(sfd,buf,BUF_SIZE,0);
            printf("%s\n", buf);
            strcpy(buf,"HELO garudaserver.com\r\n");
            //sleep(1);
            rst = send(sfd,buf,strlen(buf),0);
        	if (rst == -1)
        	{
        		perror ("Client: Receive failed");
        		exit (1);
        	}
            memset(buf,'\0',sizeof(buf));
            rst = recv(sfd,buf,BUF_SIZE,0);
            printf("%s\n",buf);

            labelA: printf("Mail from?\n");
			scanf("%s",from);
            memset(buf,'\0',sizeof(buf));
            sprintf(buf,"MAIL FROM: <%s>\r\n",from);
			rst = send(sfd,buf,strlen(buf),0);
            memset(buf,'\0',sizeof(buf));
			rst = recv(sfd,buf,BUF_SIZE,0);
            printf("%s\n",buf);
            if(buf[0]=='2' && buf[1]=='5' && buf[2]=='0');
            else{
                printf("Error\n");
                rst=send(sfd,"RSET\r\n",BUF_SIZE,0);
                rst = recv(sfd,buf,BUF_SIZE,0);
                printf("%s\n",buf);
                goto labelA;
            }
            printf("How many clients do you want to mail to?\n");
            cin>>num;
            for(int i=0;i<num;i++){
                printf("Enter the receiver #%d mail address\n",i+1);
                scanf("%s",to);
                memset(buf,'\0',sizeof(buf));
                sprintf(buf,"RCPT TO: <%s>\r\n",to);
                //usleep(100000);
    			rst = send(sfd,buf,strlen(buf),0);
                memset(buf,'\0',sizeof(buf));
    			rst = recv(sfd,buf,BUF_SIZE,0);  // 250 Ok
                printf("%s\n",buf );
                if(buf[0]=='2' && buf[1]=='5' && buf[2]=='0');
                else{
                    printf("Error\n");
                    memset(buf,'\0',sizeof(buf));
                    strcpy(buf,"RSET\r\n");
                    rst=send(sfd,"RSET\r\n",BUF_SIZE,0);
                    rst = recv(sfd,buf,BUF_SIZE,0);
                    printf("%s\n",buf);
                    goto labelA;
                }
            }

            memset(buf,'\0',sizeof(buf));
            strcpy(buf,"DATA\r\n");
			rst = send(sfd,buf,strlen(buf),0);
            memset(buf,'\0',sizeof(buf));
			rst = recv(sfd,buf,BUF_SIZE,0);  // 354 START MAIL INPUT
            printf("Enter sender's name:\n");
            char temp[1000];
            getchar();
            fgets(temp,900,stdin);
            temp[strlen(temp)-1]='\0';
            printf("from=%s\n",temp);
            sprintf(buf,"From: %s\r\n",temp);
			rst = send(sfd,buf,strlen(buf),0);
            memset(buf,'\0',sizeof(buf));
            sprintf(buf,"To: ");
            for(int i=0;i<num;i++){
                printf("Enter receiver #%d name:\n",i+1);
                fgets(temp,900,stdin);
                temp[strlen(temp)-1]='\0';
                printf("temp=%s\n",temp);
                strcat(buf,temp);
                if(num-i-1) strcat(buf,",");
            }
            strcat(buf,"\r\n");
			rst = send(sfd,buf,strlen(buf),0);
			string tempdate=currentDateTime();
			strcpy(date, tempdate.c_str());
			printf("%s\n", date);
            sprintf(buf,"\nDate: %s\r\n",date);
			// strcpy(buf,"Date: ");
			// strcat(buf,date);
            // usleep(100000);
			rst = send(sfd,buf,strlen(buf),0);

			memset(buf,'\0',sizeof(buf));
            printf("Enter Subject:\n");
            fgets(temp,900,stdin);
            temp[strlen(temp)-1]='\0';
            printf("temp=%s\n",temp);
			sprintf(buf,"Subject: %s\r\n",temp);
			//printf("%s\n", buf);
            // usleep(100000);
			rst = send(sfd,buf,strlen(buf),0);
            strcpy(buf,"\nContent:\r\n");
            rst = send(sfd,buf,strlen(buf),0);
			printf("Input Mail Body [end with .]\n");
            while(1){
                memset(message,'\0',sizeof(message));
                fgets(message,950,stdin);
                message[strlen(message)-1]='\0';
    			printf("message=%s\n", message);
                sprintf(buf,"%s\r\n",message);
    			rst = send(sfd,buf,strlen(buf),0);
                if(rst==-1){
                    printf("server write failed\n");
                    exit(1);
                }
                if(strcmp(message,".")==0) break;
            }
            printf("abc\n");
            memset(buf,'\0',sizeof(buf));
			rst = recv(sfd,buf,BUF_SIZE,0);
            printf("%s\n",buf);
            if(buf[0]=='2' && buf[1]=='5' && buf[2]=='0');
            else{
                printf("Error\n");
                rst=send(sfd,"RSET\r\n",BUF_SIZE,0);
                rst = recv(sfd,buf,BUF_SIZE,0);
                printf("%s\n",buf);
                goto labelA;
            }

            printf("Do you want to send more mails? 0 for No 1 for Yes\n");
            cin>>num;
            if(num==1) goto labelA;
            else{
                strcpy(buf,"QUIT\r\n");
            	rst = send(sfd,buf,BUF_SIZE,0);
            	rst = recv(sfd,buf,BUF_SIZE,0);
                printf("%s\n",buf);
                break;
            }
		}
		else if(num==2){       //receive
            srv_addr.sin_port = htons(110);
            rst = inet_pton(AF_INET,"10.5.30.131",&srv_addr.sin_addr);
            if (rst <= 0)
            {
                perror ("Client Presentation to network address conversion.\n");
                exit (1);
            }
            rst = connect (sfd, (struct sockaddr *) &srv_addr, addrlen);
            if (rst == -1)
            {
                perror ("Client: Connect failed.");
                exit (1);
            }
            rst=0;
            while(rst<=0){
                memset(buf,'\0',BUF_SIZE);
                rst = recv(sfd,buf,BUF_SIZE,0);
            }
            printf("%s\n",buf);
			printf("username?\n");
			scanf("%s",username);
            memset(buf,'\0',BUF_SIZE);
            sprintf(buf,"USER %s\r\n",username);
			rst = send(sfd,buf,strlen(buf),0);
            rst=0;
            while(rst<=0){
                memset(buf,'\0',BUF_SIZE);
                rst = recv(sfd,buf,BUF_SIZE,0);
            }
            printf("%s\n",buf);
			printf("password?\n");
			scanf("%s",password);
            memset(buf,'\0',BUF_SIZE);
            sprintf(buf,"PASS %s\r\n",password);
			rst = send(sfd,buf,strlen(buf),0);
            rst=0;
            while(rst<=0){
                memset(buf,'\0',BUF_SIZE);
                rst = recv(sfd,buf,BUF_SIZE,0);
            }
            printf("%s\n",buf);

            sprintf(buf,"LIST\r\n");
			rst = send(sfd,buf,strlen(buf),0);
            usleep(100);
            while(1){
                memset(buf,'\0',BUF_SIZE);
                rst = recv(sfd,buf,BUF_SIZE,0);
                printf("%s",buf);
                if(rst<BUF_SIZE-1) break;
            }
            labelC: printf("Which message do you want to read?\n");
            scanf("%d",&num);
            memset(buf,'\0',BUF_SIZE);
            sprintf(buf,"RETR %d\r\n",num);
			rst = send(sfd,buf,strlen(buf),0);
            //usleep(100);
            memset(buf,'\0',BUF_SIZE);
            printf("This is the mail you requested: \n");
            while(1){
                rst=0;
                memset(buf,'\0',BUF_SIZE);
                while(rst<=0) rst=recv(sfd,buf,BUF_SIZE,0);
                for(int i=0;i<BUF_SIZE;i++) cout<<buf[i];
                if(rst<BUF_SIZE) break;
            }
            printf("\nWant to recv more mails? 0 for No 1 for Yes\n");
            cin>>num;
            if(num==0){
                memset(buf,'\0',BUF_SIZE);
                strcpy(buf,"QUIT\r\n");
            	rst = send(sfd,buf,strlen(buf),0);
                memset(buf,'\0',BUF_SIZE);
            	rst = recv(sfd,buf,BUF_SIZE,0);
                printf("%s\n",buf);
                break;
            }
            else goto labelC;
		}
		else if(num==3) break;
		else{
			printf("chutiya h kya bc\n");
			exit(1);
		}
	}
    close(sfd);
}
