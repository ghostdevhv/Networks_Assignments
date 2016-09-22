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
	// argv[1] --> Portnumber
	// argv[2] --> IP
	// argv[3] --> Domain name
	int mera_port;
	mera_port=atoi(argv[1]);
    int check;
    if(argc==4)
    {
    	printf("Have two different accounts on two different servers ? Press 1 for Yes | Press 2 for No\n");
    	scanf("%d",&check);
    	if(check==1)
    	{
    		if((strcmp(argv[3],"xyz.com"))==0)
    		{
    			char temp1[1000];
				memset(temp1,'\0',sizeof(temp1));
				strcpy(temp1,"./client ");
				strcat(temp1,"1214 "); // Ye change krna hai
				strcat(temp1,"10.117.11.15 ");
				strcat(temp1,"abc.com ");
				strcat(temp1,"c");
	    		int a;
				if((a=fork())==0)
				{
					execl("/usr/bin/xterm", "/usr/bin/xterm", "-e", "bash", "-c",temp1, (void*)NULL);
				}
				else
				{
				}
    		}
    		else
    		{
    			char temp1[1000];
				memset(temp1,'\0',sizeof(temp1));
				strcpy(temp1,"./client ");
				strcat(temp1,"1239 "); // ye change krna hai
				strcat(temp1,"10.117.11.131 ");
				strcat(temp1,"xyz.com ");
				strcat(temp1,"c");

	    		int a;

				if((a=fork())==0)
				{
					execl("/usr/bin/xterm", "/usr/bin/xterm", "-e", "bash", "-c",temp1, (void*)NULL);

				}
				else
				{

				}

    		}

    	}
    	else
    	{
    		//continue;
    	}
    }
    printf("Use this terminal to access your account on %s server\n",argv[3]);
	int sfd,rst;
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
    srv_addr.sin_port   = htons (atoi(argv[1])); // Port Number.
    rst = inet_pton (AF_INET, argv[2], &srv_addr.sin_addr); /* To
                              * type conversion of the pointer here. */
    if (rst <= 0)
    {
        perror ("Client Presentation to network address conversion.\n");
        exit (1);
    }



    /***************** Connect to the server ************************/
    rst = connect (sfd, (struct sockaddr *) &srv_addr, addrlen);
    if (rst == -1)
    {
        perror ("Client: Connect failed.");
        exit (1);
    }

    char lol[1000];
    memset(lol,'\0',sizeof(lol));
    strcpy(lol,"HELO: ");
    strcat(lol,argv[3]);
	strcpy(buf,lol);
    usleep(100000);
	rst = send(sfd,buf,BUF_SIZE,0);
	rst = recv(sfd,buf,BUF_SIZE,0);
	if (rst == -1)
	{
		perror ("Client: Receive failed");
		exit (1);
	}
	if(strcmp(buf,"250 OK")==0){
		printf("Connection Established\n");
	}
	else{
		printf("%s\n",buf);
		exit(1);

	}

	char from[1000],to[1000],message[1000],date[1000],username[1000],password[1000];
	char **tokens;
	char *token;
	char name[1000];

	while(1){
		printf("Do you want to (1) Send mail or (2) Receive mails or (3) exit?\n");
		int num;
		cin>>num;
		if(num==1){ // send
			strcpy(buf,"1");
            usleep(100000);
			rst = send(sfd,buf,BUF_SIZE,0);
			printf("Mail from?\n");
			scanf("%s",from);
			strcpy(buf,"MAIL FROM: ");
			strcat(buf,from);
            usleep(100000);
			rst = send(sfd,buf,BUF_SIZE,0);
			rst = recv(sfd,buf,BUF_SIZE,0);
			if(strcmp(buf,"250 OK")!=0){
				perror("line 148");
				printf("%s\n",buf);
				continue;
			}
			printf("Mail to?\n");
			scanf("%s",to);
			strcpy(buf,"RCPT TO: ");
			strcat(buf,to);
            usleep(100000);
			rst = send(sfd,buf,BUF_SIZE,0);
			rst = recv(sfd,buf,BUF_SIZE,0);  // 250 OK

            strcpy(buf,"DATA");
            usleep(100000);
			rst = send(sfd,buf,BUF_SIZE,0);
			rst = recv(sfd,buf,BUF_SIZE,0);  // 354 START MAIL INPUT

			memset(message,'\0',sizeof(message));
			printf("Input Mail Body [end with #]\n");
			getchar();
			scanf("%[^#]", message);
			getchar();
			printf("%d",(int)strlen(message));


			strcpy(buf,"FROM: ");
			strcat(buf,from);
            usleep(100000);
			rst = send(sfd,buf,BUF_SIZE,0);
			strcpy(buf,"TO: ");
			strcat(buf,to);
            usleep(100000);
			rst = send(sfd,buf,BUF_SIZE,0);
			string tempdate=currentDateTime();
			strcpy(date, tempdate.c_str());
			printf("%s\n", date);
			strcpy(buf,"Date: ");
			strcat(buf,date);
            usleep(100000);
			rst = send(sfd,buf,BUF_SIZE,0);

			memset(buf,'\0',sizeof(buf));
			strcpy(buf,"Subject: Network");
			printf("%s\n", buf);
            usleep(100000);
			rst = send(sfd,buf,BUF_SIZE,0);


			printf("message %s\n", message);
			strcpy(buf,message);
			strcat(buf,"#");
			printf("sent message: %s\n",buf);
            usleep(100000);
			rst = send(sfd,buf,BUF_SIZE,0);

			printf("Pahunch gya\n");
			strcpy(buf,"#");
			printf("%s\n",buf);
            usleep(100000);
			rst = send(sfd,buf,BUF_SIZE,0);
			printf("Bhej Diya\n");

			rst = recv(sfd,buf,BUF_SIZE,0);
			if(strcmp(buf,"250 OK")==0){
				printf("Message sent successfully\n");
			}
			continue;
		}
		else if(num==2){   //receive
			strcpy(buf,"2");
            usleep(100000);
			rst = send(sfd,buf,BUF_SIZE,0);
			printf("username?\n");
			scanf("%s",username);
			strcpy(buf,"USER ");
			strcat(buf,username);
            usleep(100000);
			rst = send(sfd,buf,BUF_SIZE,0);
			rst = recv(sfd,buf,BUF_SIZE,0);
			if(strcmp(buf,"OK")!=0){
				perror("receive 219: ");
				printf("%s\n",buf );
				continue;
			}
			printf("password?\n");
			scanf("%s",password);
			strcpy(buf,"PASS ");
			strcat(buf,password);
            usleep(100000);
			rst = send(sfd,buf,BUF_SIZE,0);
			memset(buf,'\0',sizeof(buf));
			rst = recv(sfd,buf,BUF_SIZE,0);
			if(strcmp(buf,"You are not registered")==0){
				perror("receive 149: ");
				printf("%s\n",buf);
				continue;
			}

			strcpy(buf,"LIST");
            usleep(100000);
			rst = send(sfd,buf,BUF_SIZE,0);
			rst = recv(sfd,buf,BUF_SIZE,0);

			if(strcmp(buf,"No message")==0)
			{
				printf("No Unread Messages\n");
				continue;
			}

            int position = 0;
            char **tokens = (char**)malloc(sizeof(char*) * 1000);
            char *token;
            token = strtok(buf,",");
            printf("139\n");
            while(token!=NULL)
            {
                tokens[position] = token;
                position++;
                token = strtok(NULL,",");
            }
            tokens[position] = NULL;
			printf("You have following unread emails\n");
			for(int i=0;i<position;i++)
				printf("%s\n", tokens[i]);
			printf("Which one do you want to read?\n");
			int mailno;
			cin>>mailno;
			if(mailno>position){
				printf("chutiya h kya bc\n");
				continue;
			}
			else{
				strcpy(buf,"RETR ");
				char str[10];
				sprintf(str,"%d",mailno);
				strcat(buf,str);
                usleep(100000);
				rst = send(sfd,buf,BUF_SIZE,0);
				rst = recv(sfd,buf,BUF_SIZE,0);
				printf("This is the mail you requested: \n");
				printf("\n%s\n",buf);
			}
		}
		else if(num==3) break;
		else{
			printf("chutiya h kya bc\n");
			exit(1);
		}
	}
    memset(buf,'\0',sizeof(buf));
	strcpy(buf,"3");
    usleep(100000);
	rst = send(sfd,buf,BUF_SIZE,0);

    memset(buf,'\0',sizeof(buf));
	strcpy(buf,"QUIT");
    usleep(100000);
	rst = send(sfd,buf,BUF_SIZE,0);

	rst = recv(sfd,buf,BUF_SIZE,0);
	if(strcmp(buf,"221 service closed")==0){
		printf("BYE!\nConnection Terminated!\n");
		close(sfd);
		exit(1);
	}
	else perror("Error in termination: ");
}
