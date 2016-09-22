#include <bits/stdc++.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h> // For the socket (), bind () etc. functions.
#include <netinet/in.h> // For IPv4 data struct..
#include <arpa/inet.h> // For inet_pton (), inet_ntop ().

using namespace std;

// #define port_num 12364
#define BUF_SIZE 1000
#define EMAIL_DOMAIN "abc.com"
#define token_delimiters " :@"
#define Command_length 1000
#define EMAIL_DOMAIN2 "xyz.com"

int port_num,port_num1,port_num3;


void alagserver(char *sender_emailid, char *receiver_emailid, char *mailkadate, char *mailsubject, char *mailcontent)
{
	
	
    printf("Naya connection banega\n");
    int sfd1,rst1;
    sfd1 = socket(AF_INET, SOCK_STREAM,0);
    char buf1[1000];
    if(sfd1== -1)
    {
        perror("Client: Socket Error");
        exit(1);
    }
    printf("Socket fd = %d\n",sfd1);

    struct sockaddr_in srv_addr1, cli_addr1; // Addresses of the server and the client.
    socklen_t addrlen1 = sizeof (struct sockaddr_in); // size of the addresses.

    // Clear the two addresses.
    memset (&srv_addr1, 0, addrlen1);

    // Assign values to the server address.
    srv_addr1.sin_family = AF_INET; // IPv4.
    srv_addr1.sin_port   = htons (port_num1); // Port Number.

    rst1 = inet_pton (AF_INET, "10.117.11.131", &srv_addr1.sin_addr); /* To
                              * type conversion of the pointer here. */
    if (rst1 <= 0)
    {
        perror ("Client Presentation to network address conversion.\n");
        exit (1);
    }
    
    rst1 = connect (sfd1, (struct sockaddr *) &srv_addr1, addrlen1);
    if (rst1 == -1)
    {
        perror ("Client: Connect failed.");
        exit (1);
    }

    printf("Yayy Server connect ho gye\n");

    memset(buf1,'\0',sizeof(buf1));
    strcpy(buf1,"HELO: abc.com");
    usleep(100000);
    rst1 = send(sfd1,buf1,BUF_SIZE,0);

    memset(buf1,'\0',sizeof(buf1));
    rst1 = recv(sfd1,buf1,BUF_SIZE,0);
	if (rst1 == -1)
	{
		perror ("Client: Receive failed");
		exit (1);
	}
	if(strcmp(buf1,"250 OK")==0){
		printf("Connection Established\n");
	}
	else{
		printf("%s\n",buf1);
		exit(1);

	}

	printf("187\n");
	char from_x[100],to_x[100],message_x[1000],date_x[100],user_x[100],pass_x[100];

	char **tokens;
	char *token;
	char name_x[1000];

	usleep(100000);
	
	rst1 = send(sfd1,"1",BUF_SIZE,0);

	memset(from_x,'\0',sizeof(from_x));
	strcpy(from_x,sender_emailid);
	strcpy(buf1,"MAIL FROM: ");
	strcat(buf1,from_x);
    usleep(100000);
	rst1 = send(sfd1,buf1,BUF_SIZE,0);
	
	memset(buf1,'\0',sizeof(buf1));
	rst1 = recv(sfd1,buf1,BUF_SIZE,0);
	
	printf("206\n");

	memset(to_x,'\0',sizeof(to_x));
	strcpy(to_x,receiver_emailid);
	strcpy(buf1,"RCPT TO: ");
	strcat(buf1,to_x);
    usleep(100000);
	rst1 = send(sfd1,buf1,BUF_SIZE,0);

	memset(buf1,'\0',sizeof(buf1));
	rst1 = recv(sfd1,buf1,BUF_SIZE,0);
   
	printf("218\n");

	memset(buf1,'\0',sizeof(buf1));
	strcpy(buf1,"DATA");
    usleep(100000);
	rst1 = send(sfd1,buf1,BUF_SIZE,0);

	memset(buf1,'\0',sizeof(buf1));
	rst1 = recv(sfd1,buf1,BUF_SIZE,0); // 354 start mail input

	memset(buf1,'\0',sizeof(buf1));
	strcpy(buf1,"FROM: ");
	strcat(buf1,from_x);
    usleep(100000);
	rst1 = send(sfd1,buf1,BUF_SIZE,0);

	printf("234\n");

	memset(buf1,'\0',sizeof(buf1));
	strcpy(buf1,"TO: ");
	strcat(buf1,to_x);
    usleep(100000);
	rst1 = send(sfd1,buf1,BUF_SIZE,0);


	printf("243\n");
	
	memset(buf1,'\0',sizeof(buf1));
	// strcpy(buf1,"Date: ");
	strcat(buf1,mailkadate);
    usleep(100000);
	rst1 = send(sfd1,buf1,BUF_SIZE,0);

	
	memset(buf1,'\0',sizeof(buf1));
	strcpy(buf1,"Subject: Network");
	printf("%s\n", buf1);
    usleep(100000);
	rst1 = send(sfd1,buf1,BUF_SIZE,0);


	printf("259\n");

	memset(buf1,'\0',sizeof(buf1));
	strcpy(buf1,mailcontent);
	printf("sent message: %s\n",buf1);
    usleep(100000);
	rst1 = send(sfd1,buf1,BUF_SIZE,0);

	
	printf("268\n");	

	memset(buf1,'\0',sizeof(buf1)); // After content
	printf("Pahunch gya\n");
	strcpy(buf1,"#");
	printf("%s\n",buf1);
    usleep(100000);
	rst1 = send(sfd1,buf1,BUF_SIZE,0);
	printf("Bhej Diya\n");

	printf("278\n");
	memset(buf1,'\0',sizeof(buf1));
	rst1 = recv(sfd1,buf1,BUF_SIZE,0);
	if(strcmp(buf1,"250 OK")==0){
		printf("Message sent successfully\n");
	}

	printf("Finally\n");
	return;
}


int main(int argc,char *argv[])
{
    port_num = atoi(argv[1]);
    port_num1 = atoi(argv[2]);
    port_num3 = atoi(argv[3]);


    /* port_num3 --> Jab mera server dusre server ke liye server ki tarah act kr rha hai*/
	/* port_num --> Jab mera server mere client se interact kr rha hai*/
	/* port_num1 --> jab mera server dusre server k liye client ki tarah act kr rha hai*/
    
    if(fork()==0)
    {

	    	/**************** Create a socket. *******************************/
	    int sfd; // Socket file descriptor.
	    int rst;
	    int cfd;
	 
	    sfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (sfd == -1)
	    {
	        perror ("Server: socket error");
	        exit (1);
	    }
	    printf ("Socket fd = %d\n", sfd);

	    /***************** Assign an address to the server **************/
	    struct sockaddr_in srv_addr, cli_addr; // Addresses of the server and the client.
	    socklen_t addrlen = sizeof(struct sockaddr_in); // size of the addresses.

	    // Clear the two addresses.
	    memset(&srv_addr, 0, addrlen);
	    memset(&cli_addr, 0, addrlen);

	    // Assign values to the server address.
	    srv_addr.sin_family = AF_INET; // IPv4.
	    srv_addr.sin_port   = htons(port_num); // Port Number.

	    rst = inet_pton(AF_INET, "10.117.11.15", &srv_addr.sin_addr); /* To
	                            * type conversion of the pointer here. */
	    if (rst <= 0)
	    {
	        //perror("Server Presentation to network address conversion.\n");
	        exit (1);
	    }

	    /***************** Bind the server to an address. **************/
	    rst = bind(sfd, (struct sockaddr *) &srv_addr, addrlen); /* Note
	        * the type casting of the pointer to the server's address. */
	    if (rst == -1)
	    {
	        perror ("Server: Bind failed");
	        exit (1);
	    }



	    /***************** listen (): Mark the socket as passive. *******/
	    printf ("Max connections allowed to wait: %d\n", SOMAXCONN);
	    rst = listen (sfd, SOMAXCONN);
	    if (rst == -1)
	    {
	        perror ("Server: Listen failed");
	        exit (1);
	    }
	    /*----------------------------------------------------------------------------------*/

	    /*---------------------------------------------------------------------------------------------*/

	/*---------------------------------------------------------------------------------------------------*/
		while(1)
	    {
	    	printf("%d\n",sfd);
	        cfd = accept(sfd, (struct sockaddr *) &cli_addr, &addrlen);
	                   /* Returns the file descriptor for the client.
	                    * Stores the address of the client in cli_addr. */
	        if (cfd == -1)
	        {
	            perror ("Server: Accept failed");
	            exit (1);
	        }

	        int b = fork();

	        if(b==0)
	        {
	        	//close(sfd);
		        char buf[BUF_SIZE] = {'\0'};
		        rst = recv(cfd,buf,1000,0);
		        if (rst <= 0)
		        {
		            perror ("Client: Receive failed");
		            close(cfd);
		            continue;
		        }

		        printf("%s\n",buf);
		        char temp[40];
		        int k=0;
		        if(buf[0]=='H' && buf[1]=='E' && buf[2]=='L' && buf[3]=='O')
		        {
		            for(int i=6;i<=strlen(buf);i++)
		            {
		                temp[k] = buf[i];
		                k++;
		            }
		            temp[k]='\0';

		            if(strcmp(temp,EMAIL_DOMAIN)==0)
		            {
		            	usleep(100000);
		                rst = send(cfd, "250 OK",7,0);
		                if(rst == -1)
		                {
		                    perror("Server write failed");
		                    exit(1);
		                }
		            }
		            else
		            {
		            	usleep(100000);
		                rst = send(cfd, "Connection Refused",19,0);
		                if(rst == -1)
		                {
		                    perror("Server write failed");

		                }
		                close(cfd);
		            }
		        }
		        else
		        {
		        	usleep(100000);
		            rst = send(cfd, "Connection Refused",19,0);
		            if(rst == -1)
		            {
		                perror("Server write failed");

		            }
		            close(cfd);
		        }

		        while(1)
		        {
		        	printf("****** JAI MATA DI ******\n");
	                memset(buf,'\0',sizeof(buf));
		            rst = recv(cfd,buf,1000,0);
	                printf("146: %s\n",buf);
		            if(strcmp(buf,"1")==0) // Client msg bhejega
		            {
		                // printf("Bhej BC\n");

		                memset(buf,'\0',sizeof(buf));
		                rst = recv(cfd,buf,1000,0);
	                    printf("153 %s\n",buf);
		                int bufsize = Command_length;
		                int position = 0;
		                char **tokens = (char**)malloc(sizeof(char*) * bufsize);
		                char *token;
		                token = strtok(buf,token_delimiters);

		                while(token!=NULL)
		                {
		                    tokens[position] = token;
		                    position++;
		                    token = strtok(NULL,token_delimiters);
		                }
		                tokens[position] = NULL;

		                for(int i=0;i<position;i++)
		                    cout<<tokens[i]<<endl;

		                char sender_emailid[1000];
		                char receiver_emailid[1000];

		                if(strcmp(tokens[3],EMAIL_DOMAIN)==0)
		                {
		                    printf("Mil gya\n");
		                    char temp1[100]; // Concate the Email ID
		                    memset(temp1,'\0',sizeof(temp));
		                    strcpy(temp1,tokens[2]);
		                    strcat(temp1,"@");
		                    strcat(temp1,tokens[3]);
		                    memset(sender_emailid,'\0',sizeof(sender_emailid));
		                    strcpy(sender_emailid,temp1);
		                    printf("Sender Email ID: %s\n",sender_emailid);
		                    rst = send(cfd, "250 OK",7,0);
		                    if(rst == -1)
		                    {
		                        perror("Server write failed");
		                        exit(1);
		                    }
		                    memset(buf,'\0',sizeof(buf));
		                    rst = recv(cfd,buf,1000,0);
		                    printf("193: %s\n",buf);

		                    int bufsize = Command_length;
		                    int position = 0;
		                    char **tokens = (char**)malloc(sizeof(char*) * bufsize);
		                    char *token;
		                    token = strtok(buf,token_delimiters);
		                    while(token!=NULL)
		                    {
		                        tokens[position] = token;
		                        position++;
		                        token = strtok(NULL,token_delimiters);
		                    }
		                    tokens[position] = NULL;

		                    for(int i=0;i<position;i++)
		                        cout<<tokens[i]<<endl;

		                    printf(" 276 Jai Mata Di\n");

		                    if(strcmp(tokens[3],EMAIL_DOMAIN)==0)
		                    {
		                        printf("Same Domain hai apne Bhai ka b :) 280\n");
		                        char temp1[100]; // Concate the Email ID
	                            memset(temp1,'\0',sizeof(temp1));
		                        strcpy(temp1,tokens[2]);
		                        strcat(temp1,"@");
		                        strcat(temp1,tokens[3]);
		                        memset(receiver_emailid,'\0',sizeof(receiver_emailid));
		                        strcpy(receiver_emailid,temp1);
		                        printf("Receiver EmailID: %s\n", receiver_emailid);
		                        rst = send(cfd, "250 OK",7,0);
		                        if(rst == -1)
		                        {
		                            perror(" 210 Server write failed");
		                            exit(1);
		                        }
		                        memset(buf,'\0',sizeof(buf));
		                        rst = recv(cfd,buf,1000,0);
		                        printf("%s\n",buf);
		                        
		                        FILE *ptr;
		                        char torr[1000]; // Create email.txt
		                        memset(torr,'\0',sizeof(torr));
		                        strcpy(torr,receiver_emailid);
		                        strcat(torr,".txt");
		                        printf("filename = %s\n",torr);
		                        printf("File Create ho Gyi\n");
		                      	ptr = fopen(torr,"a");

		                       	rst = send(cfd, "354 start mail input",21,0);
		                        if(rst == -1)
		                        {
		                            perror("Server write failed");
		                            exit(1);
		                        }

		                        memset(buf,'\0',sizeof(buf));
		                        rst = recv(cfd,buf,1000,0);

		                        char kahanseaaya[1000], kahankojaega[100];
		                        int k=0,ko=0;
		                        for(int i=6;i<strlen(buf);i++)
		                        {
		                            kahanseaaya[k] = buf[i];
		                            k++;
		                        }
		                        kahanseaaya[k] = '\0';
		                        printf("%s 321\n", kahanseaaya);

		                        fprintf(ptr, "Sender Email ID:  %s\n", kahanseaaya);

		                        memset(buf,'\0',sizeof(buf));
		                        rst = recv(cfd,buf,1000,0);

		                        for(int i=4;i<strlen(buf);i++)
		                        {
		                            kahankojaega[ko] = buf[i];
		                            ko++;
		                        }
		                        kahankojaega[ko] = '\0';
		                        printf("334 %s\n",kahankojaega );

		                        fprintf(ptr, "Receiver Email ID:  %s\n", kahankojaega);

		                        char mailkadate[20];
		                        int date=0;

		                        memset(buf,'\0',sizeof(buf));
		                        rst = recv(cfd,buf,1000,0);

		                        for(int i=6;i<strlen(buf);i++)
		                        {
		                            mailkadate[date] = buf[i];
		                            date++;
		                        }
		                        mailkadate[date] = '\0';
		                        printf("350 %s\n", mailkadate);
		                        fprintf(ptr, "Date: %s\n", mailkadate);


		                        char mailsubject[1000];
		                        int subject = 0;

		                        memset(buf,'\0',sizeof(buf));
		                        rst = recv(cfd,buf,1000,0);
	                            //rst=send(cfd,"abcd",1000,0);
		                        printf("%lu\n", strlen(buf));
		                        printf("360 %s....\n", buf);
		                        fprintf(ptr, "%s\n",buf );
	                            memset(buf,'\0',sizeof(buf));
		                        rst = recv(cfd,buf,1000,0);

		                        printf("366  %s\n", buf);
		                        fprintf(ptr, "Content: %s\n\n", buf);

		                        memset(buf,'\0',sizeof(buf));
		                        rst = recv(cfd,buf,1000,0);
		                        printf("%s\n",buf);
		                        printf("Pahunch gya\n");
		                        if(strcmp(buf,"#")==0)
		                        {
		                        	printf("Ghus gya\n");
		                        	usleep(100000);
		                            rst = send(cfd, "250 OK",7,0);
		                            if(rst == -1)
		                            {
		                                perror(" 210 Server write failed");
		                                exit(1);
		                            }
		                            fclose(ptr);
		                            printf("FIle closed\n");
		                        	continue;
		                        }

		                    }

		                    else
		                    {
		                    	if(strcmp(tokens[3],"xyz.com")!=0)
		                    	{
		                    		usleep(100000);
		                    		rst =send(cfd,"Invalid Domain",15,0);
		                    		continue;
		                    	}

		                    	char temp1[100]; // Concate the Email ID
	                            memset(temp1,'\0',sizeof(temp1));
		                        strcpy(temp1,tokens[2]);
		                        strcat(temp1,"@");
		                        strcat(temp1,tokens[3]);
		                        memset(receiver_emailid,'\0',sizeof(receiver_emailid));
		                        strcpy(receiver_emailid,temp1);
		                        printf("Receiver EmailID: %s\n", receiver_emailid);
		                        
		                        rst = send(cfd, "250 OK",7,0);
		                        if(rst == -1)
		                        {
		                            perror(" 210 Server write failed");
		                            exit(1);
		                        }
		                        
		                        memset(buf,'\0',sizeof(buf));
		                        rst = recv(cfd,buf,1000,0);
		                        printf("%s\n",buf);
		                       

		                       	rst = send(cfd, "354 start mail input",21,0);
		                        if(rst == -1)
		                        {
		                            perror("Server write failed");
		                            exit(1);
		                        }

		                        memset(buf,'\0',sizeof(buf));
		                        rst = recv(cfd,buf,1000,0);

		                        char kahanseaaya[1000], kahankojaega[100];
		                        int k=0,ko=0;
		                        for(int i=6;i<strlen(buf);i++)
		                        {
		                            kahanseaaya[k] = buf[i];
		                            k++;
		                        }
		                        kahanseaaya[k] = '\0';
		                        char jai[1000];
		                        memset(jai,'\0',sizeof(jai));
		                        strcpy(jai,"Sender Email ID: ");
		                        strcat(jai,kahanseaaya);
		                        memset(kahanseaaya,'\0',sizeof(kahanseaaya));
		                        strcpy(kahanseaaya,jai);

		                        
		                        memset(buf,'\0',sizeof(buf));
		                        rst = recv(cfd,buf,1000,0);

		                        for(int i=4;i<strlen(buf);i++)
		                        {
		                            kahankojaega[ko] = buf[i];
		                            ko++;
		                        }
		                        kahankojaega[ko] = '\0';
		                        printf("334 %s\n",kahankojaega );

		                        memset(jai,'\0',sizeof(jai));
		                        strcpy(jai,"Receiver Email ID: ");
		                        strcat(jai,kahankojaega);
		                        memset(kahankojaega,'\0',sizeof(kahankojaega));
		                        strcpy(kahankojaega,jai);

		                        char mailkadate[20];
		                        int date=0;
		                        memset(mailkadate,'\0',sizeof(mailkadate));
		                        memset(buf,'\0',sizeof(buf));
		                        rst = recv(cfd,buf,1000,0);

		                        strcpy(mailkadate,buf);


		                        char mailsubject[1000];
		                        char mailcontent[1000];
		                        int subject = 0;
		                        memset(mailsubject,'\0',sizeof(mailsubject));
		                        memset(buf,'\0',sizeof(buf));
		                        rst = recv(cfd,buf,1000,0);
	     						strcpy(mailsubject,buf);

		 						memset(mailcontent,'\0',sizeof(mailcontent));
	                            memset(buf,'\0',sizeof(buf));
		                        rst = recv(cfd,buf,1000,0);
		                        strcpy(mailcontent,buf);

		                        memset(buf,'\0',sizeof(buf));
		                        rst = recv(cfd,buf,1000,0);
		                        printf("%s\n",buf);
		                        printf("Pahunch gya\n");
		                        if(strcmp(buf,"#")==0)
		                        {
		                        	printf("Ghus gya\n");
		                        	usleep(100000);
		                            rst = send(cfd, "250 OK",7,0);
		                            if(rst == -1)
		                            {
		                                perror(" 210 Server write failed");
		                                exit(1);
		                            }
		                            
		                        }
								alagserver(sender_emailid,receiver_emailid,mailkadate,mailsubject,mailcontent);
		                    	continue;

		                    }



		                }
		                else
		                {
		                	usleep(100000);
		                    rst = send(cfd, "Invalid EmailID",16,0);
		                    if(rst == -1)
		                    {
		                        perror(" 215 Server write failed");

		                    }
		                    continue;
		                }


		            }

		            else if(strcmp(buf,"2")==0)	// Receiving Part
		            {
	                    memset(buf,'\0',sizeof(buf));
		                rst = recv(cfd,buf,1000,0);
		                printf("419 %s\n",buf);

		                int bufsize = Command_length;
		                int position = 0;
		                char **tokens = (char**)malloc(sizeof(char*) * bufsize);
		                char *token;
		                token = strtok(buf,token_delimiters);
		                printf("139\n");
		                while(token!=NULL)
		                {
		                    tokens[position] = token;
		                    position++;
		                    token = strtok(NULL,token_delimiters);
		                }
		                tokens[position] = NULL;
		                printf("147\n");
		                printf("%s\n", tokens[1]);
		                printf("%s\n",tokens[2]);

		                char my_username[100];
		                strcpy(my_username,tokens[1]);
		                strcat(my_username,"@");
		                strcat(my_username,tokens[2]);

		                printf("%s\n",my_username );
		                char check[1000];
		                strcpy(check,my_username);
		                printf("CHECK     %s\n",check );
		                if(strcmp(tokens[2],EMAIL_DOMAIN)==0)
		                {
		                    printf("150\n");
		                    rst = send(cfd, "OK",7,0);
		                    if(rst == -1)
		                    {
		                        perror("Server write failed");
		                        exit(1);
		                    }


		                    memset(buf,'\0',sizeof(buf));
		                    rst = recv(cfd,buf,1000,0);
		                    printf("\n%s\n",buf);

		                    char pass[1000];
		                    memset(pass,'\0',sizeof(pass));
		                    int pa = 0;
		                    for(int i=5;i<strlen(buf);i++)
		                    {
		                    	pass[pa] = buf[i];
		                    	pa++;
		                    }
		                    printf("pass   %s\n",pass);
		                    printf("%s\n", check);
		                    strcat(check,pass);
		                    printf("check %s\n",check );
		                }
		                else
		                {
		                	usleep(100000);
		                    rst = send(cfd, "Invalid EmailID",16,0);
		                    if(rst == -1)
		                    {
		                        perror("Server write failed");
		                        exit(1);
		                    }
		                    continue;
		                }



		                printf("Ye hai Data  %s\n", check);

		                FILE* fh;
		                fh = fopen("register.txt", "r");

		                if (fh == NULL){
		                    printf("HUlla\n");
		                    usleep(100000);
		                    rst = send(cfd, "You are not registered",23,0);
		                    printf("YOLO\n");
		                    if(rst == -1)
		                    {
		                        perror(" 215 Server write failed");


		                    }
		                    continue;
		                }

		                int flag = 0;
		                const size_t line_size = 300;
		                char* line = (char *)malloc(line_size);
		                while (fgets(line, line_size, fh) != NULL){
		                	printf("-----%s----",line);
		                  	line[strlen(line)-1] = '\0';
		                  	printf("-----%s  ", line);
		                  	printf("--------%s----- USernae",check);
		                   	if(strcmp(line,check)==0)
		                    {

		                        flag=1;
		                        break;
		                    }

		                }
		                free(line);
		                if(flag==0)
		                {
		                    printf("Lull\n");
		                    usleep(100000);
		                    rst = send(cfd, "You are not registered",23,0);
		                    if(rst == -1)
		                    {
		                        perror("215 Server write failed");


		                    }
		                    continue;
		                }
		                else
		                {
		                	usleep(100000);
		                    rst = send(cfd,"OK",9,0);
		                    if(rst == -1)
		                    {
		                        perror("215 Server write failed");
		                    }

		                    rst = recv(cfd,buf,BUF_SIZE,0);
		                    printf("%s\n",buf);

		                    strcat(my_username,".txt");
		                    FILE *ptr;
		                    printf("TXT FILE --> %s\n",my_username);
		                    ptr = fopen(my_username,"r");
		                    char ch;

		                    if( ptr == NULL )
		                    {
		                    	usleep(100000);
		                        rst = send(cfd, "No message",23,0);
		                        if(rst == -1)
		                        {
		                            perror("485 Server write failed");

		                        }
		                        continue;
		                    }
		                    printf("Hello\n");
		                    char content[1000];
		                    char temp[40][1000];
		                    memset(content,'\0',sizeof(content));
		                    memset(temp,'\0',sizeof(temp));

		                    int k=1,j=0;

		                    while((ch = fgetc(ptr))!= EOF )
		                    {
		                        if(ch!='#')
		                        {

		                            temp[k][j] = ch;
		                            j++;
		                        }
		                        else
		                        {
		                            k++;
		                            j=0;

		                        }
		                    }

		                    for(int i=1;i<k;i++)
		                    {
	                            printf("%s\n",temp[i]);
		                        char la[10];
		                        if(i==1)
		                        {
		                            sprintf(la,"%d",i);
		                            strcpy(content,la);
		                            strcat(content,". ");
		                            int l = strlen(temp[i]);
		                            sprintf(la,"%d",l);
		                            strcat(content,la);
		                            strcat(content," bytes");
		                        }
		                        else
		                        {
		                           sprintf(la,",%d",i);
		                            strcat(content,la);
		                            strcat(content,". ");
		                            int l = strlen(temp[i]);
		                            sprintf(la,"%d",l);
		                            strcat(content,la);
		                            strcat(content," bytes");
		                        }
		                    }


		                    printf("%s\n", content);
		                    usleep(100000);
		                    rst = send(cfd,content,1000,0);
		                    if(rst == -1)
		                    {
		                        perror("215 Server write failed");

		                    }

		                    rst = recv(cfd,buf,1000,0);

		                    printf("%s\n",buf);
		                    char chnum[100];
		                    int lol=0;
		                    for(int i=5;i<strlen(buf);i++)
		                    {
		                        chnum[lol] = buf[i];
		                        lol++;
		                    }
		                    chnum[lol] = '\0';
		                    int chnumber = atoi(chnum);
		                    printf("%d\n",chnumber);
		                    fclose(ptr);
	                        remove(my_username);
		                    for(int j=1;j<k;j++)
		                    {
		                    	printf("%s\n\n",temp[j]);
		                        FILE *ptr;
		                        ptr = fopen(my_username,"a");
		                        {
		                            if(j!=chnumber)
		                            {
	                                    strcat(temp[j],"#");
		                                fprintf(ptr, "%s\n",temp[j]);
		                            }
		                            else
		                            {
		                                char content1[1000];
		                                strcpy(content1,temp[j]);
		                                printf("%s\n",content1 );
		                                strcat(content1,"\n");
		                                usleep(100000);
		                                rst = send(cfd,content1,1000,0);
		                                if(rst == -1)
		                                {
		                                    perror("215 Server write failed");
		                                }
		                            }
		                        }
		                        fclose(ptr);
		                    }
		                    continue;
						}// Check kr liya properly Registration
		            }
		            else
		            {
		            	break;
		            }

		        }

		        memset(buf,'\0',sizeof(buf));
		        rst = recv(cfd,buf,BUF_SIZE,0);
		        printf("%s\n",buf);

		        if(strcmp(buf,"QUIT")==0)
		        {
		        	usleep(100000);
		        	memset(buf,'\0',sizeof(buf));
		        	strcpy(buf,"221 service closed");
		        	rst = send(cfd,buf,BUF_SIZE,0);
		        	printf("%s\n",buf);
		        	close(cfd);
		        }
	        }

	    }

    }	

    else {  // 

        int cfd1,sfd1,rst1;
        sfd1 = socket(AF_INET, SOCK_STREAM,0);
        char buf1[1000];
        if(sfd1== -1)
        {
            perror("Client: Socket Error");
            exit(1);
        }
        

        struct sockaddr_in srv_addr1, cli_addr1; // Addresses of the server and the client.
        socklen_t addrlen1 = sizeof (struct sockaddr_in); // size of the addresses.

        // Clear the two addresses.
        memset(&srv_addr1, 0, addrlen1);
        memset(&cli_addr1, 0, addrlen1);
        // Assign values to the server address.
        srv_addr1.sin_family = AF_INET; // IPv4.
        srv_addr1.sin_port   = htons (port_num3); // Port Number. 1st arg

        rst1 = inet_pton (AF_INET, "10.117.11.15", &srv_addr1.sin_addr); /* To
        * type conversion of the pointer here. */
        rst1 = bind(sfd1, (struct sockaddr *) &srv_addr1, addrlen1); /* Note
        * the type casting of the pointer to the server's address. */

        if (rst1 ==-1)
        {
            perror ("Client Presentation to network address conversion.\n");
            exit (1);
        }
        printf ("Max connections allowed to wait: %d\n", SOMAXCONN);
        rst1 = listen (sfd1, SOMAXCONN);
        if (rst1 == -1)
        {
            perror ("Server: Listen failed");
            exit (1);
        }

        while(1)
        {
            cfd1 = accept(sfd1, (struct sockaddr *) &cli_addr1, &addrlen1);
            if(cfd1==-1)
            {
                perror("Server: Accept Failed");
                exit(1);
            }
            //close(sfd1);
            char temp1[40];
            int k=0;
            memset(buf1,'\0',sizeof(buf1));
            rst1 = recv(cfd1,buf1,1000,0);
            if(buf1[0]=='H' && buf1[1]=='E' && buf1[2]=='L' && buf1[3]=='O')
            {
                for(int i=6;i<=strlen(buf1);i++)
                {
                    temp1[k] = buf1[i];
                    k++;
                }
                temp1[k]='\0';
                if(strcmp(temp1,EMAIL_DOMAIN2)==0)
                {
                    usleep(100000);
                    rst1 = send(cfd1, "250 OK",7,0);
                    if(rst1 == -1)
                    {
                        perror("Server write failed");
                        exit(1);
                    }
                }
                else
                {
                    usleep(100000);
                    rst1 = send(cfd1, "Connection Refused",19,0);
                    if(rst1 == -1)
                    {
                        perror("Server write failed");
                    }
                    close(cfd1);
                }
            }
            else
            {
                usleep(100000);
                rst1 = send(cfd1, "Connection Refused",19,0);
                if(rst1 == -1)
                {
                    perror("Server write failed");
                }
                close(cfd1);
            }
            //printf("****** JAI MATA DI ******\n");
            memset(buf1,'\0',sizeof(buf1));
            rst1 = recv(cfd1,buf1,1000,0);
            printf("146: %s\n",buf1);
            printf("lol\n");
            if(strcmp(buf1,"1")==0) // Client msg bhejega
            {
                memset(buf1,'\0',sizeof(buf1));
                rst1 = recv(cfd1,buf1,1000,0);
                printf("153 %s\n",buf1);
                int bufsize = Command_length;
                int position = 0;
                char **tokens = (char**)malloc(sizeof(char*) * bufsize);
                char *token;
                token = strtok(buf1,token_delimiters);
                while(token!=NULL)
                {
                    tokens[position] = token;
                    position++;
                    token = strtok(NULL,token_delimiters);
                }
                tokens[position] = NULL;
                for(int i=0;i<position;i++)
                cout<<tokens[i]<<endl;

                char sender_emailid[1000];
                char receiver_emailid[1000];
                char temp1[100];
                memset(temp1,'\0',sizeof(temp1));
                strcpy(temp1,tokens[2]);
                strcat(temp1,"@");
                strcat(temp1,tokens[3]);
                memset(sender_emailid,'\0',sizeof(sender_emailid));
                strcpy(sender_emailid,temp1);
                printf("Sender Email ID: %s\n",sender_emailid);
                usleep(100000);
                rst1 = send(cfd1, "250 OK",7,0);
                if(rst1 == -1)
                {
                    perror("Server write failed");
                    exit(1);
                }
                memset(buf1,'\0',sizeof(buf1));
                rst1 = recv(cfd1,buf1,1000,0);
                printf("193: %s\n",buf1);

                bufsize = Command_length;
                position = 0;
                tokens = (char**)malloc(sizeof(char*) * bufsize);
                token = strtok(buf1,token_delimiters);
                while(token!=NULL)
                {
                    tokens[position] = token;
                    position++;
                    token = strtok(NULL,token_delimiters);
                }
                tokens[position] = NULL;

                for(int i=0;i<position;i++)
                cout<<tokens[i]<<endl;

                printf(" 276 Jai Mata Di\n");
                memset(temp1,'\0',sizeof(temp1));
                strcpy(temp1,tokens[2]);
                strcat(temp1,"@");
                strcat(temp1,tokens[3]);
                memset(receiver_emailid,'\0',sizeof(receiver_emailid));
                strcpy(receiver_emailid,temp1);
                printf("Receiver EmailID: %s\n", receiver_emailid);
                usleep(100000);
                rst1 = send(cfd1, "250 OK",7,0);
                if(rst1 == -1)
                {
                    perror(" 210 Server write failed");
                    exit(1);
                }
                memset(buf1,'\0',sizeof(buf1));
                rst1 = recv(cfd1,buf1,1000,0);

                FILE *ptr;
                char torr[1000]; // Create email.txt
                memset(torr,'\0',sizeof(torr));
                strcpy(torr,receiver_emailid);
                strcat(torr,".txt");
                printf("filename = %s\n",torr);
                printf("File Create ho Gyi\n");
                ptr = fopen(torr,"a");

                rst1 = send(cfd1, "354 start mail input",21,0);
                if(rst1 == -1)
                {
                    perror("Server write failed");
                    exit(1);
                }

                memset(buf1,'\0',sizeof(buf1));
                rst1 = recv(cfd1,buf1,1000,0);

                char kahanseaaya[1000], kahankojaega[100];
                int k=0,ko=0;
                for(int i=6;i<strlen(buf1);i++)
                {
                    kahanseaaya[k] = buf1[i];
                    k++;
                }
                kahanseaaya[k] = '\0';
                printf("%s 321\n", kahanseaaya);

                fprintf(ptr, "Sender Email ID:  %s\n", kahanseaaya);

                memset(buf1,'\0',sizeof(buf1));
                rst1 = recv(cfd1,buf1,1000,0);

                for(int i=4;i<strlen(buf1);i++)
                {
                    kahankojaega[ko] = buf1[i];
                    ko++;
                }
                kahankojaega[ko] = '\0';
                printf("334 %s\n",kahankojaega );

                fprintf(ptr, "Receiver Email ID:  %s\n", kahankojaega);

                char mailkadate[20];
                int date=0;

                memset(buf1,'\0',sizeof(buf1));
                rst1 = recv(cfd1,buf1,1000,0);

                for(int i=6;i<strlen(buf1);i++)
                {
                    mailkadate[date] = buf1[i];
                    date++;
                }
                mailkadate[date] = '\0';
                printf("350 %s\n", mailkadate);
                fprintf(ptr, "Date: %s\n", mailkadate);


                char mailsubject[1000];
                int subject = 0;

                memset(buf1,'\0',sizeof(buf1));
                rst1 = recv(cfd1,buf1,1000,0);
                //rst=send(cfd,"abcd",1000,0);
                printf("%lu\n", strlen(buf1));
                printf("360 %s....\n", buf1);
                fprintf(ptr, "%s\n",buf1);
                memset(buf1,'\0',sizeof(buf1));
                rst1 = recv(cfd1,buf1,1000,0);

                printf("366  %s\n", buf1);
                fprintf(ptr, "Content: %s\n\n", buf1);

                memset(buf1,'\0',sizeof(buf1));
                rst1 = recv(cfd1,buf1,1000,0);
                printf("%s\n",buf1);
                printf("Pahunch gya\n");
                printf("Ghus gya\n");
                usleep(100000);
                rst1 = send(cfd1, "250 OK",7,0);
                if(rst1 == -1)
                {
                    perror(" 210 Server write failed");
                    exit(1);
                }
                fclose(ptr);
                printf("FIle closed\n");
                close(cfd1);
                continue;
            }

            else if(strcmp(buf1,"2")==0)	// Receiving Part
            {
                memset(buf1,'\0',sizeof(buf1));
                rst1 = recv(cfd1,buf1,1000,0);

                printf("419 %s\n",buf1);

                int bufsize = Command_length;
                int position = 0;
                char **tokens = (char**)malloc(sizeof(char*) * bufsize);
                char *token;
                token = strtok(buf1,token_delimiters);
                printf("139\n");
                while(token!=NULL)
                {
                    tokens[position] = token;
                    position++;
                    token = strtok(NULL,token_delimiters);
                }
                tokens[position] = NULL;
                printf("147\n");
                printf("%s\n", tokens[1]);
                printf("%s\n",tokens[2]);

                char my_username[100];
                strcpy(my_username,tokens[1]);
                strcat(my_username,"@");
                strcat(my_username,tokens[2]);

                printf("%s\n",my_username );
                char check[1000];
                strcpy(check,my_username);
                printf("CHECK     %s\n",check );

                usleep(100000);
                rst1 = send(cfd1, "OK",7,0);
                if(rst1 == -1)
                {
                    perror("Server write failed");
                    exit(1);
                }
                memset(buf1,'\0',sizeof(buf1));
                rst1 = recv(cfd1,buf1,1000,0);
                printf("\n%s\n",buf1);

                char pass[1000];
                memset(pass,'\0',sizeof(pass));
                int pa = 0;
                for(int i=5;i<strlen(buf1);i++)
                {
                    pass[pa] = buf1[i];
                    pa++;
                }
                printf("pass   %s\n",pass);
                printf("%s\n", check);
                strcat(check,pass);
                printf("check %s\n",check );

                printf("Ye hai Data  %s\n", check);

                FILE* fh;
                fh = fopen("register.txt", "r");

                if (fh == NULL){
                    printf("HUlla\n");
                    usleep(100000);
                    rst1 = send(cfd1, "You are not registered",23,0);
                    printf("YOLO\n");
                    if(rst1 == -1)
                    {
                        perror(" 215 Server write failed");
                    }
                    continue;
                }

                int flag = 0;
                const size_t line_size = 300;
                char* line = (char *)malloc(line_size);
                while (fgets(line, line_size, fh) != NULL){
                    //printf("-----%s----",line);
                    line[strlen(line)-1] = '\0';
                    //printf("-----%s  ", line);
                    //printf("--------%s----- USernae",check);
                    if(strcmp(line,check)==0)
                    {
                        flag=1;
                        break;
                    }
                }
                free(line);
                if(flag==0)
                {
                    printf("Lull\n");
                    usleep(100000);
                    rst1 = send(cfd1, "You are not registered",23,0);
                    if(rst1 == -1)
                    {
                        perror("215 Server write failed");
                    }
                    continue;
                }
                else
                {
                    usleep(100000);
                    rst1 = send(cfd1,"OK",9,0);
                    if(rst1 == -1)
                    {
                        perror("215 Server write failed");
                    }

                    rst1 = recv(cfd1,buf1,BUF_SIZE,0);
                    printf("%s\n",buf1);

                    strcat(my_username,".txt");
                    FILE *ptr;
                    printf("TXT FILE --> %s\n",my_username);
                    ptr = fopen(my_username,"r");
                    char ch;

                    if( ptr == NULL )
                    {
                        usleep(100000);
                        rst1 = send(cfd1, "No message",23,0);
                        if(rst1 == -1)
                        {
                            perror("485 Server write failed");
                        }
                        continue;
                    }
                    printf("Hello\n");
                    char content[1000];
                    char temp[40][1000];
                    memset(content,'\0',sizeof(content));
                    memset(temp,'\0',sizeof(temp));

                    int k=1,j=0;

                    while((ch = fgetc(ptr))!= EOF )
                    {
                        if(ch!='#')
                        {
                            temp[k][j] = ch;
                            j++;
                        }
                        else
                        {
                            k++;
                            j=0;
                        }
                    }

                    for(int i=1;i<k;i++)
                    {
                        printf("%s\n",temp[i]);
                        char la[10];
                        if(i==1)
                        {
                            sprintf(la,"%d",i);
                            strcpy(content,la);
                            strcat(content,". ");
                            int l = strlen(temp[i]);
                            sprintf(la,"%d",l);
                            strcat(content,la);
                            strcat(content," bytes");
                        }
                        else
                        {
                            sprintf(la,",%d",i);
                            strcat(content,la);
                            strcat(content,". ");
                            int l = strlen(temp[i]);
                            sprintf(la,"%d",l);
                            strcat(content,la);
                            strcat(content," bytes");
                        }
                    }


                    printf("%s\n", content);
                    usleep(100000);
                    rst1 = send(cfd1,content,1000,0);
                    if(rst1 == -1)
                    {
                        perror("215 Server write failed");
                        exit(1);
                    }
                    memset(buf1,'\0',sizeof(buf1));
                    rst1 = recv(cfd1,buf1,1000,0);

                    printf("%s\n",buf1);
                    char chnum[100];
                    int lol=0;
                    for(int i=5;i<strlen(buf1);i++)
                    {
                        chnum[lol] = buf1[i];
                        lol++;
                    }
                    chnum[lol] = '\0';
                    int chnumber = atoi(chnum);
                    printf("%d\n",chnumber);
                    remove(my_username);
                    for(int j=1;j<k;j++)
                    {
                        printf("%s\n\n",temp[j]);
                        FILE *ptr;
                        ptr = fopen(my_username,"a");
                        {
                            if(j!=chnumber)
                            {
                                strcat(temp[j],"#");
                                fprintf(ptr, "%s\n",temp[j]);
                            }
                            else
                            {
                                char content1[1000];
                                strcpy(content1,temp[j]);
                                printf("%s\n",content1 );
                                strcat(content1,"\n");
                                usleep(100000);
                                rst1 = send(cfd1,content1,1000,0);
                                if(rst1 == -1)
                                {
                                    perror("215 Server write failed");
                                    exit(1);
                                }
                            }
                        }
                        fclose(ptr);
                    }
                }
            }
            else  // terminate
            {
                break;
            }
        }
    }
}

