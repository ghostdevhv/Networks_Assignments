#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h> // For the socket (), bind () etc. functions.
#include <netinet/in.h> // For IPv4 data struct..
#include <string.h> // For memset.
#include <arpa/inet.h> // For inet_pton (), inet_ntop ().
#include <dirent.h>


int port_num = 23465;
#define BUF_SIZE 2000

void main ()
{
    int rst; // Return status of functions.
    int cfd; // File descriptor for the client.
    char buf[BUF_SIZE] = {'\0'};     
    char headlines_print[BUF_SIZE] = {'\0'};
    char mochi[20000] = {'\0'};
    int Rea_Rep;

    int flag;
    while(1)
    {
        /**************** Create a socket. *******************************/
        int sfd; // Socket file descriptor.
        sfd = socket (AF_INET, SOCK_STREAM, 0); /* AF_INET --> IPv4,
                 * SOCK_STREAM --> TCP Protocol, 0 --> for the protocol. */
        if (sfd == -1) 
        {
            perror ("Client: socket error");
            exit (1);
        }
        printf ("Socket fd = %d\n", sfd);
        
        
        
        /***************** Assign an address of the server **************/
        struct sockaddr_in srv_addr, cli_addr; // Addresses of the server and the client.
        socklen_t addrlen = sizeof (struct sockaddr_in); // size of the addresses.
        
        // Clear the two addresses.
        memset (&srv_addr, 0, addrlen);
        
        // Assign values to the server address.
        srv_addr.sin_family = AF_INET; // IPv4.
        srv_addr.sin_port   = htons (21436); // Port Number.
        
        rst = inet_pton (AF_INET, "127.0.0.1", &srv_addr.sin_addr); /* To 
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
        
        
        
        /**************** Send-Receive messages ************************/
        rst = recv(sfd,buf,BUF_SIZE,0);
        if (rst == -1)
        {
            perror ("Client: Receive failed");
            exit (1);
        }
        printf("%s\n", buf);    // Check whether Reader or Reporter
        memset(buf,'\0',sizeof(buf));
        scanf("%s",buf);

        Rea_Rep = atoi(buf);
        rst = send(sfd,buf,strlen(buf)+1,0); // Send Reader or Reporter

        if(Rea_Rep==1) /////////// Reporter /////////////
        {
            memset(buf,'\0',sizeof(buf));
            rst = recv(sfd,buf,BUF_SIZE,0); // Academic or Non-Academic (Reporter)
            if (rst == -1)
            {
                perror ("Client: Receive failed");
                exit (1);
            }

            printf("%s\n",buf );    // Print the message
            memset(buf,'\0',sizeof(buf));
            scanf("%s",buf);    // Get the choice

            rst = send(sfd,buf,strlen(buf)+1,0);    // Send the choices 1 for Academic 2 for Non-Academic

           	printf("\nYou have these contents :\n\n");

           	char farzi[BUF_SIZE][BUF_SIZE];
           	memset(farzi,'\0',sizeof(farzi));
           	int farzi_index = 1;

           	DIR *dir;
			struct dirent *ent;
			if ((dir = opendir ("/home/himanshu/Documents/reporter")) != NULL) 
			{
			  	/* print all the files and directories within directory */
			  	while ((ent = readdir (dir)) != NULL) 
			  	{
			  	
				  	if(strcmp(ent->d_name,".")!=0 && strcmp(ent->d_name,"..")!=0)
				  	{
				  		strcpy(farzi[farzi_index++],ent->d_name);
				  		printf("%d : ",farzi_index-1);
				  		for(int i=0;i<strlen(ent->d_name);i++)
				  		{
				  			if(ent->d_name[i]=='^')
				  				printf(" ");
				  			else if (ent->d_name[i] =='~')
				  				break;
				  			else
				  				printf("%c",ent->d_name[i]);
			  			}
			  			printf("\n");
				  	}
			  	}
			  	closedir (dir);
			} 
			else 
			{
			  /* could not open directory */
			  perror ("");
			  exit(1);
			}

            memset(buf,'\0',sizeof(buf));
            rst = recv(sfd,buf,BUF_SIZE,0); // Right index
            if (rst == -1)
            {
                perror ("Client: Receive failed");
                exit (1);
            }

            printf("\n%s\n",buf );    // Print the headline message
            int news_index;
            memset(buf,'\0',sizeof(buf));
            scanf("%d",&news_index);
            //printf("%s\n",farzi[news_index]);
            strcpy(buf,farzi[news_index]);
            rst = send(sfd,buf,strlen(buf)+1,0);    // Send the headline
            
            if (rst == -1)
            {
                perror ("Server write failed");
                exit (1);
            }

            memset(buf,'\0',sizeof(buf));
            rst = recv(sfd,buf,BUF_SIZE,0);

            if(strcmp(buf,"File already exists :/")==0)
            {
            	printf("Error : File Already Exists\n");

            }
            else
            {
	            FILE *fp;
	            char ch;
	            int lkj = 0;
	            char path1[2000] = "/home/himanshu/Documents/reporter/" ;
	            strcat(path1,farzi[news_index]);
	            fp = fopen(path1,"r");
	            memset(buf,'\0',sizeof(buf));
	            
	            while((ch=fgetc(fp))!=EOF)
	            {
	                buf[lkj++] = ch;
	                
	            }
	            rst = send(sfd,buf,strlen(buf)+1,0);
	            fclose(fp);
                memset(buf,'\0',sizeof(buf));
                rst = recv(sfd,buf,BUF_SIZE,0);
                printf("%s\n",buf);

            }
            
    
        }
        else    /// Reader
        {
            memset(buf,'\0',sizeof(buf));
            rst = recv(sfd,buf,BUF_SIZE,0); // Academic or Non-Academic
            if (rst == -1)
            {
                perror ("Client: Receive failed");
                exit (1);
            }

            printf("\n%s\n",buf );    // Print the message
            memset(buf,'\0',sizeof(buf));
            scanf("%s",buf);    // Get the choice
            
           //printf("%s\n", buf1);
            rst = send(sfd,buf,strlen(buf)+1,0);    // Send the choices

            memset(buf,'\0',sizeof(buf));
            
            rst = recv (sfd, buf, BUF_SIZE, 0); // Recieve the headlines
           	
            buf[strlen(buf)-1] = '\0';
           //	printf("%s\n",buf );
            int k=0,cx=1;

            int bufsize = 1000;
            int position = 0;
            char **tokens = malloc(sizeof(char)*bufsize);
            char *token;
            token = strtok(buf,"|");

            while(token!=NULL)
            {
                tokens[position] = token;
                position++;
                token = strtok(NULL,"|");
            }
            tokens[position] = NULL;

            printf("\n");
            for(int i=position-1;i>=0;i--)
            {
            	printf("%d : ",cx++);

            	for(int j=0;j<strlen(tokens[i]);j++)
            	{
            		if(tokens[i][j] == '^')
            			printf(" ");
            		else if (tokens[i][j] == '~' || tokens[i][j] == '`')
            			break;
            		else
            			printf("%c",tokens[i][j]);
            	}
            	printf("\n");
            }

            memset(buf,'\0',sizeof(buf));
            printf("\nEnter the index of the Headline \n");
           
            scanf("%s",buf);
            rst = send(sfd,buf,strlen(buf)+1,0);    // Send the headline index

            char flaged[BUF_SIZE];
            memset(flaged,'\0',sizeof(flaged));
            rst = recv (sfd, flaged, BUF_SIZE, 0); // recieve the content
            //printf("%s\n",flaged );
            if(strcmp(flaged,"Ni mila")==0)
            {
            	printf("Error : Please Enter the right index\n");
            	//exit(0);
            }
            else
            {
            	memset(buf,'\0',sizeof(buf));
	            rst = recv (sfd, buf, BUF_SIZE, 0); // recieve the content
	            FILE *fds;
	            fds = fopen("/home/himanshu/Documents/temp.txt","w");
	            fprintf(fds, "%s", buf);
	            fclose(fds);
	            if(!fork())
	            {
	                //int rtv=execlp("gedit","gedit","/tmp/1.txt",(const char*) NULL);
	                execlp ("xterm", "xterm", "-hold","-e", "more", "/home/himanshu/Documents/temp.txt", NULL);
	                //int rattata = remove("/home/himanshu/Documents/temp.txt");

	                // if(rattata == 0) 
	                // {
	                //     printf("File deleted successfully");
	                // }
	                exit(0);
	            }
            }
            

        }
        
        /****************** Close ****************************************/
        rst = close (sfd); // Close the socket file descriptor.
        if (rst == -1)
        {
            perror ("Client close failed");
            exit (1);
        }

        printf("Press 1 for continue | Press 0 for terminate\n");
        scanf("%d",&flag);
        if(flag==0)
            break;
        else
            continue;
    }

    
    
}


