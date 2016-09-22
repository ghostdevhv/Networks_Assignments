#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h> // For the socket (), bind () etc. functions.
#include <netinet/in.h> // For IPv4 data struct..
#include <string.h> // For memset.
#include <arpa/inet.h> // For inet_pton (), inet_ntop ().
#include <sys/un.h>
#include <dirent.h>
#include <fcntl.h>

int port_num = 23465;
int port_num1 = 23466;

#define BUF_SIZE 1000


void remove_string(char *str,int k)
{
	if(k==1)
	{
		FILE *ptr=fopen("/home/himanshu/Documents/Academics_Count.txt","r");
	    FILE *ptr2=fopen("/home/himanshu/Documents/tempfile.txt","w+");
	    char *temp=malloc(1000*sizeof(char));
	    while(fgets(temp,1000,ptr)!=NULL){
	        if(strcmp(temp,str)!=0){
	            fputs(temp,ptr2);
	        }
	    }
	    fclose(ptr);
	    ptr=fopen("/home/himanshu/Documents/Academics_Count.txt","w");
	    fseek(ptr2,0,SEEK_SET);
	    while(fgets(temp,1000,ptr2)!=NULL){
	        fputs(temp,ptr);
	    }
	    fclose(ptr);
	    fclose(ptr2);
	}
	else
	{
		FILE *ptr=fopen("/home/himanshu/Documents/Non-Academics_Count.txt","r");
	    FILE *ptr2=fopen("/home/himanshu/Documents/tempfile.txt","w+");
	    char *temp=malloc(1000*sizeof(char));
	    while(fgets(temp,1000,ptr)!=NULL){
	        if(strcmp(temp,str)!=0){
	            fputs(temp,ptr2);
	        }
	    }
	    fclose(ptr);
	    ptr=fopen("/home/himanshu/Documents/Non-Academics_Count.txt","w");
	    fseek(ptr2,0,SEEK_SET);
	    while(fgets(temp,1000,ptr2)!=NULL){
	        fputs(temp,ptr);
	    }
	    fclose(ptr);
	    fclose(ptr2);
	}
}
void main ()
{
    int rst; // Return status of functions.
    int cfd; // File descriptor for the client.
    int pid;
    int rs_fr;
    int count ;

    /**************** Create a socket. *******************************/
    int sfd; // Socket file descriptor.
    sfd = socket (AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        perror ("Server: socket error");
        exit (1);
    }
    printf ("Socket fd = %d\n", sfd);



    /***************** Assign an address to the server **************/
    struct sockaddr_in srv_addr, cli_addr; // Addresses of the server and the client.
    socklen_t addrlen = sizeof (struct sockaddr_in); // size of the addresses.

    // Clear the two addresses.
    memset (&srv_addr, 0, addrlen);
    memset (&cli_addr, 0, addrlen);

    // Assign values to the server address.
    srv_addr.sin_family = AF_INET; // IPv4.
    srv_addr.sin_port   = htons (21436); // Port Number.

    rst = inet_pton (AF_INET, "127.0.0.1", &srv_addr.sin_addr); /* To
                              * type conversion of the pointer here. */
    if (rst <= 0)
    {
        perror ("Server Presentation to network address conversion.\n");
        exit (1);
    }



    /***************** Bind the server to an address. **************/
    rst = bind (sfd, (struct sockaddr *) &srv_addr, addrlen); /* Note
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
    ///////////////////n UDP ///////////////////////////

    int sfd1; // Socket file descriptor.
    sfd1 = socket (AF_INET, SOCK_DGRAM, 0); /* AF_INET --> IPv4,
                * SOCK_STREAM --> TCP Protocol, 0 --> for the protocol. */
    if (sfd1 == -1)
    {
        perror ("Server: socket error");
        exit (1);
    }
    printf ("Socket fd = %d\n", sfd1);

    struct sockaddr_in srv_addr1, cl_addr1; // For sever and client addresses.
    socklen_t addrlen1 = sizeof (struct sockaddr_in);


    /* Clear the two addresses. */
    memset (&srv_addr1, 0, addrlen1);
    memset (&cl_addr1, 0, addrlen1);

    /* Assign a server address. */
    srv_addr1.sin_family = AF_INET; // IPv4
    srv_addr1.sin_port   = htons (port_num1);


    /* The servers IP address. */
    rst = inet_pton (AF_INET, "127.0.0.1", &srv_addr1.sin_addr);
    if (rst <= 0)
    {
        perror ("Server Presentation to network address conversion.\n");
        exit (1);
    }

    rst = bind (sfd1, (struct sockaddr *) &srv_addr1, addrlen);
    if (rst < 0)
    {
        perror ("Server: bind failed");
        exit (1);
    }

    int b = fork();

    if(b==0)
    {
    	while(1) /////////////////////////  ADMINISTRATOR /////////////////////////////////////////////////////////////////
    	{

	    	char buf[BUF_SIZE] = {'\0'};
	    	char time1[BUF_SIZE] = {'\0'};
	    	char date[BUF_SIZE] = {'\0'};

		    int b_recv   = 0; // Number of bytes received.

		    // Flags for recvfrom.
		    int flags = 0;

		    b_recv = recvfrom (sfd1, buf, BUF_SIZE, flags,
		                        (struct sockaddr *) &cl_addr1, &addrlen1);
		    if (b_recv == -1)
		    {
		        perror ("Server: recvfrom failed");
		        exit (1);
		    }

		    while(1)
		    {
		    	//printf("Hii\n");
			    rst = sendto (sfd1,"Hi Administrator :) !! Please enter your User Name",52, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
			    if (rst < 0)
			    {
			        perror ("Server: Couldn't send");
			        exit (1);
			    }

			    memset(buf,'\0',sizeof(buf));
			    b_recv = recvfrom (sfd1, buf, BUF_SIZE, flags,
			                        (struct sockaddr *) &cl_addr1, &addrlen1);	// recieve usename
			    if (b_recv == -1)
			    {
			        perror ("Server: recvfrom failed");
			        exit (1);
			    }

			    if(strcmp(buf,"Gattu")==0)
			    {
			    	rst = sendto (sfd1,"Please enter the Password",52, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));

				    if (rst < 0)
				    {
				        perror ("Server: Couldn't send");
				        exit (1);
				    }
				    memset(buf,'\0',sizeof(buf));
				    b_recv = recvfrom (sfd1, buf, BUF_SIZE, flags,
				                        (struct sockaddr *) &cl_addr1, &addrlen1);
				    if (b_recv == -1)
				    {
				        perror ("Server: recvfrom failed");
				        exit (1);
				    }
				    if(strcmp(buf,"Tuntun")==0)
				    {
				    	rst = sendto (sfd1,"Successfully Login !! Welcome Administrator",100, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
				    	if (rst < 0)
					    {
					        perror ("Server: Couldn't send");
					        exit (1);
					    }

				    	rst = sendto (sfd1,"You have these files on your server",100, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
				    	if (rst < 0)
					    {
					        perror ("Server: Couldn't send");
					        exit (1);
					    }

				    	rst = sendto (sfd1,"Academics ----------",100, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
				    	if (rst < 0)
					    {
					        perror ("Server: Couldn't send");
					        exit (1);
					    }

				    	DIR *dir;
						struct dirent *ent;
						char buffered[900];
						memset(buffered,'\0',sizeof(buffered));
						if ((dir = opendir ("/home/himanshu/Documents/Academics")) != NULL)
						{
						  	/* print all the files and directories within directory */
						  	while ((ent = readdir (dir)) != NULL)
						  	{

							  	if(strcmp(ent->d_name,".")!=0 && strcmp(ent->d_name,"..")!=0)
							  	{
							  		strcat(buffered,ent->d_name);
							  		strcat(buffered,"|");
							  		//printf("%s\n",buffered );
							  	}
						  	}
						  	closedir (dir);
						}
						//printf("%s\n",buffered );
						buffered[strlen(buffered)-1] = '\0';
						rst = sendto (sfd1,buffered,sizeof(buffered), flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
						if (rst < 0)
					    {
					        perror ("Server: Couldn't send");
					        exit (1);
					    }


					    rst = sendto (sfd1,"Non Academics ----------",100, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
				    	if (rst < 0)
					    {
					        perror ("Server: Couldn't send");
					        exit (1);
					    }

					    memset(buffered,'\0',sizeof(buffered));
						if ((dir = opendir ("/home/himanshu/Documents/NonAcademics")) != NULL)
						{
						  	/* print all the files and directories within directory */
						  	while ((ent = readdir (dir)) != NULL)
						  	{

							  	if(strcmp(ent->d_name,".")!=0 && strcmp(ent->d_name,"..")!=0)
							  	{
							  		strcat(buffered,ent->d_name);
							  		strcat(buffered,"|");
							  		//printf("%s\n",buffered );
							  	}
						  	}
						  	closedir (dir);
						}
						buffered[strlen(buffered)-1] = '\0';
						rst = sendto (sfd1,buffered,sizeof(buffered), flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
						if (rst < 0)
					    {
					        perror ("Server: Couldn't send");
					        exit (1);
					    }



					    rst = sendto (sfd1,"Enter date and time(24 hrs) in format DD-MM-YYYY|HH:MM to preserve all the news that have arrived after this date and time",200, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
					    if (rst < 0)
					    {
					        perror ("Server: Couldn't send");
					        exit (1);
					    }

					    memset(buf,'\0',sizeof(buf));

					    b_recv = recvfrom (sfd1, buf, BUF_SIZE, flags,
					                        (struct sockaddr *) &cl_addr1, &addrlen1);
					    if (b_recv == -1)
					    {
					        perror ("Server: recvfrom failed");
					        exit (1);
					    }

					    int des =0;
					    memset(date,'\0',sizeof(date));	// For date
					    memset(time1,'\0',sizeof(time1)); // For time

					    char got_string[BUF_SIZE] = {'\0'}; // Final numbered string
					    long long int got_final_number;	// Final number
					    int got_index = 0;
					    for(int i=0;i<strlen(buf);i++)
					    {
					    	if(buf[i]=='|')
					    	{
					    		for(int j = i+1;j<strlen(buf);j++)
					    		{
					    			time1[des++] = buf[j];
					    		}
					    		break;
					    	}
					    	else
					    	{
					    		date[i] = buf[i];
					    	}
					    }
					    for(int i=0;i<strlen(date);i++)
					    {
				    		if(date[i]=='-')
				    			continue;
				    		else
				    			got_string[got_index++] = date[i];
				    	}
				    	for(int i=0;i<strlen(time1);i++)
				    	{
				    		if(time1[i]==':')
				    			continue;
				    		else
				    			got_string[got_index++] = time1[i];
				    	}
				    	got_string[got_index] = '\0';	// Time entered by adminsitrator in string format


					    FILE *fp;
					    char check[200];
					    char got_date[100]; // Date of entries in Academic_Count.txt
					    char got_time[100]; // Time of entries in Academic_Count.txt

					    char final_string[100]; // Numbered string of entries
					    //printf("%s\n",got_string );
					    char Academic[100][200];
					    char Non_Academic[100][200];
					    int non_acad = 0;
					    int acad = 0;

					    fp = fopen("/home/himanshu/Documents/Academics_Count.txt","r");

					    while(fgets(check,sizeof(check),fp))
					    {
					    	int got = 0,got1 = 0;
					    	int final_index = 0;
					    	for(int i=0;i<strlen(check);i++)
					    	{
					    		if(check[i]=='~')
					    		{
					    			for(int j=i+1;j<strlen(check);j++)
					    			{
					    				if(check[j]!='`')
					    				{
					    					got_date[got++] = check[j];
					    				}
					    				else
					    					break;
					    			}
					    		}
					    		// If we entered the .txt extension
					    		else if (check[i]=='`')
					    		{
					    			for(int j=i+1;j<strlen(check);j++)
					    				got_time[got1++] = check[j];
					    		}
					    		else
					    			continue;
					    	}

					    	for(int i=0;i<strlen(got_date);i++)
					    	{
					    		if(got_date[i]=='-')
					    			continue;
					    		else
					    			final_string[final_index++] = got_date[i];
					    	}

					    	for(int i=0;i<strlen(got_time);i++)
					    	{
					    		if(got_time[i]==':')
					    			continue;
					    		else
					    			final_string[final_index++] = got_time[i];
					    	}

					    	int eptr = strcmp(got_string,final_string);
					    	if(eptr>0)
					    	{
					    		strcpy(Academic[acad],check);
					    		acad++;
					    	}
					    	else
					    	{

					    	}
					    	memset(final_string,'\0',sizeof(final_string));
					    	memset(got_date,'\0',sizeof(got_date));
					    	memset(got_time,'\0',sizeof(got_time));

				    	}

						fclose(fp);
						int fakar = 0;
						char lakar[20];
						int rows = sizeof(Academic)/sizeof(Academic[0]);
						for(int i=0;i<rows;i++)
						{
							if(Academic[i]==NULL || strcmp(Academic[i],"")==0)
							{
								//rst = sendto (sfd1,"Academic Folder is already empty",100, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
							}
							else
							{
								//write lock here

								int ret;
								char pathre[] = "/home/himanshu/Documents/Academics/";
								strcat(pathre,Academic[i]);
								if(pathre[strlen(pathre)-1]=='\n')
									pathre[strlen(pathre)-1] = '\0';
								struct flock fl = {F_WRLCK,SEEK_SET,0,0,0};
								fl.l_pid=getpid();
								fl.l_type=F_WRLCK;
								int fd;
								if((fd=open(pathre,O_RDWR))==-1)
								{
									perror("open"); //exit(1);
								}
								if (fcntl(fd, F_SETLK, &fl) == -1) 
								{
							        perror("fcntl");
							        //exit(1);
							    }
							    else
							    {
							    	//printf("Locked\n");
							    }
								//close(fd);
								//sleep(10);
								FILE *fagot;
								//fagot = fopen("/home/himanshu/Documents/Academics_Count.txt","w");
								ret = remove(pathre);
								//int gattu_tuntun = 0;
								if(ret==0)
								{
									
									remove_string(Academic[i],1);
									fakar++;
									
									
								}
								else
								{
							
									rst = sendto (sfd1,"Error : Unable to delete the files from the Academics folder",100, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
								}
								close(fd);
								

							}
						}
						//printf("HII %d \n", fakar);
						
						sprintf(lakar,"%d",fakar);
						rst = sendto (sfd1,lakar,100, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
						rst = sendto (sfd1,"Files deleted Successfully from the Academics folder",100, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
						memset(check,'\0',sizeof(check));
						fp = fopen("/home/himanshu/Documents/Non-Academics_Count.txt","r");
					    while(fgets(check,sizeof(check),fp))
					    {
					    	int got = 0,got1 = 0;
					    	int final_index = 0;
					    	for(int i=0;i<strlen(check);i++)
					    	{
					    		if(check[i]=='~')
					    		{
					    			for(int j=i+1;j<strlen(check);j++)
					    			{
					    				if(check[j]!='`')
					    				{
					    					got_date[got++] = check[j];
					    				}
					    				else
					    					break;
					    			}
					    		}
					    		else if (check[i]=='`')
					    		{
					    			for(int j=i+1;j<strlen(check);j++)
					    				got_time[got1++] = check[j];
					    		}
					    		else
					    			continue;
					    	}

					    	for(int i=0;i<strlen(got_date);i++)
					    	{
					    		if(got_date[i]=='-')
					    			continue;
					    		else
					    			final_string[final_index++] = got_date[i];
					    	}
					    	for(int i=0;i<strlen(got_time);i++)
					    	{
					    		if(got_time[i]==':')
					    			continue;
					    		else
					    			final_string[final_index++] = got_time[i];
					    	}
					    	//printf("%s\n", final_string);
					    	int eptr = strcmp(got_string,final_string);
					    	if(eptr>0)
					    	{
					    		strcpy(Non_Academic[non_acad],check);
					    		non_acad++;
					    	}
					    	else
					    	{

					    	}
					    	memset(final_string,'\0',sizeof(final_string));
					    	memset(got_date,'\0',sizeof(got_date));
					    	memset(got_time,'\0',sizeof(got_time));

				    	}
						fclose(fp);
						int rows1 = sizeof(Academic)/sizeof(Academic[0]);
						int gattu_tuntun = 0;
						for(int i=0;i<rows1;i++)
						{
							if(Non_Academic[i]==NULL || strcmp(Non_Academic[i],"")==0)
							{

								//rst = sendto (sfd1,"Non Academic Folder is empty",100, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
								break;
							}
							else
							{
								//printf("Hi\n");
								//write lock here
								int ret;
								char pathre[] = "/home/himanshu/Documents/NonAcademics/";
								strcat(pathre,Non_Academic[i]);
								if(pathre[strlen(pathre)-1]=='\n')
									pathre[strlen(pathre)-1] = '\0';
								struct flock fl = {F_WRLCK,SEEK_SET,0,0,0};
								fl.l_pid=getpid();
								fl.l_type=F_WRLCK;
								int fd;
								if((fd=open(pathre,O_RDWR))==-1)
								{
									perror("open"); //
									exit(1);
								}
								if (fcntl(fd, F_SETLK, &fl) == -1) {
							        perror("fcntl");
							        //exit(1);
							    }
								//sleep(10);
								ret = remove(pathre);
								
								printf("\n");
								if(ret==0)
								{
								
									remove_string(Non_Academic[i],2);
									gattu_tuntun++;
																	
								}
								else
								{
									
									rst = sendto (sfd1,"Error : Unable to delete the files from the Academics folder",100, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
								}
								//remove lock here
								close(fd);
								//removed lock

								
							}
						}
						//printf("YOLO %d\n",gattu_tuntun );
						//memset(lakar,'\0',20);
						
						sprintf(lakar,"%d",gattu_tuntun);
						rst = sendto (sfd1,lakar,100, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
						rst = sendto (sfd1,"Files deleted Successfully from the Non Academics folder",100, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));
						exit(0);

					}

				    else
				    {
				    	rst = sendto (sfd1,"Incorrect Login ID or Password . Please try again",100, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));

					    if (rst < 0)
					    {
					        perror ("Server: Couldn't send");
					        exit (1);
					    }

					    continue;
				    }


		    	}
			    else
			    {
			    	rst = sendto (sfd1,"Incorrect Login ID or Password . Please try again",100, flags, (struct sockaddr *) &cl_addr1, sizeof (struct sockaddr_in));

				    if (rst < 0)
				    {
				        perror ("Server: Couldn't send");
				        exit (1);
				    }

				    continue;
			    }

	    	}
		}
	}

    else
    {
    	while(1)/////////////////////// READER OR REPORTER ///////////////////////////////
	    {

		    /***************** accept (): Waiting for connections ************/
		    cfd = accept (sfd, (struct sockaddr *) &cli_addr, &addrlen);
		       /* Returns the file descriptor for the client.
		        * Stores the address of the client in cli_addr. */
		    if (cfd == -1)
		    {
		        perror ("Server: Accept failed");
		        exit (1);
		    }

		    if((pid=fork()) ==0)
		    {
		    	close(sfd);
		    	char buf[BUF_SIZE] = {'\0'};
		    	int Rea_Rep;
		    	char send_data[BUF_SIZE] = {'\0'};

		    	rst = send(cfd, "Specify Whether you are Reporter or Reader | Press 1 for Reporter or Press 2 for Reader",89,0);
		    	if(rst == -1)
		    	{
		    		perror("Server write failed");
		    		exit(1);
		    	}

		    	rst = recv(cfd,buf,2,0); // Reader or Reporter

		    	Rea_Rep = atoi(buf);

		    	if(Rea_Rep==1) /////////////////////// REPORTER ///////////////////////////////////////
		    	{
		    		rst = send (cfd, "In which Group you want to add news | Press 1 for Academic or Press 2 for Non-Academic",87,0); // Send choices
		    		if (rst == -1)
				    {
				        perror ("Server write failed");
				        exit (1);
				    }

				    rst = recv(cfd,buf,2,0); // Receive Choices  1->Academic 2->Non-Academic
				    int ans = atoi(buf);



				    if(ans==1)
				    {
				    	FILE *fp,*fp1,*fp2,*rahul;
				    	char *flag = "0";
				    	char *line = NULL;
				    	size_t len = 0;
				    	char read;

				    	//rst = send (cfd, "You have these news ready!!",34,0);

				    	rst = send (cfd, "Please choose the right index",34,0); // Send choices

			    		if (rst == -1)
					    {
					        perror ("Server write failed");
					        exit (1);
					    }
					    memset(buf,'\0',sizeof(buf));
					    rst = recv(cfd,buf,BUF_SIZE,0); 		// Receive headline

						//printf("\n%s\n",buf );

						if(buf[strlen(buf)-1]=='\n')
							buf[strlen(buf)-1] = '\0';
						buf[strlen(buf)] = '\0';

					    if (rst == -1)
					    {
					        perror ("Server recieve failed");
					        exit (1);
					    }
				    	DIR *dir;
						struct dirent *ent;
						int mymood = 0;
						if ((dir = opendir ("/home/himanshu/Documents/Academics")) != NULL)
						{
						  	/* print all the files and directories within directory */
						  	while ((ent = readdir (dir)) != NULL)
						  	{
						  		if (strcmp(ent->d_name,buf)==0)
						  		{
						  			mymood = 1;
						  			break;
						  		}

						  	}
						  	closedir (dir);
						}

						if(mymood==1)
						{
							rst = send (cfd, "File already exists :/",34,0); // Send choices
						}
						else
						{
							rst = send (cfd, "Peace",34,0);
							fp = fopen("/home/himanshu/Documents/Academics_Count.txt","a");
							int results =fputs(buf,fp);
							results = fputs("\n",fp);
							if(results == EOF)
							{
								perror("File write failed");
							}
							fclose(fp);

							char a;

							char path2[2000] = "/home/himanshu/Documents/Academics/";
							strcat(path2,buf);


							fp = fopen(path2,"w");

							char content[900000];
							rst = recv(cfd,content,900000,0);
							//printf("%s\n", content);
							int results1 = fputs(content,fp);
							if(results1 == EOF)
							{
								perror("Failes to write");
							}
							fclose(fp);

							rst = send (cfd, "File Successfully Uploaded on Server",37,0);

						}

					}

						///// ERROR /////////

				    else
				    {
				    	FILE *fp,*fp1,*fp2;
				    	char *flag = "0";
				    	char *line = NULL;
				    	size_t len = 0;
				    	char read;
				    	rst = send (cfd, "Please choose the right index",34,0); // Send choices

			    		if (rst == -1)
					    {
					        perror ("Server write failed");
					        exit (1);
					    }

					    memset(buf,'\0',sizeof(buf));
					    rst = recv(cfd,buf,BUF_SIZE,0); 		// Receive headline

					    if (rst == -1)
					    {
					        perror ("Server receive failed");
					        exit (1);
					    }
					    //printf("\n%s\n",buf);	// Got the headline Yayy

					   	if(buf[strlen(buf)-1]=='\n')
							buf[strlen(buf)-1] = '\0';
						buf[strlen(buf)] = '\0';

					    if (rst == -1)
					    {
					        perror ("Server recieve failed");
					        exit (1);
					    }

					    DIR *dir;
						struct dirent *ent;
						int mymood = 0;
						if ((dir = opendir ("/home/himanshu/Documents/NonAcademics")) != NULL)
						{
						  	/* print all the files and directories within directory */
						  	while ((ent = readdir (dir)) != NULL)
						  	{
						  		if (strcmp(ent->d_name,buf)==0)
						  		{
						  			mymood = 1;
						  			break;
						  		}

						  	}
						  	closedir (dir);
						}

					    if(mymood==1)
					    {
					    	rst = send (cfd, "File already exists :/",23,0); // Send choices
					    }
					    else
					    {
					    	rst = send (cfd, "Peace",6,0);
					    	fp = fopen("/home/himanshu/Documents/Non-Academics_Count.txt","a");
							int results =fputs(buf,fp);
							results = fputs("\n",fp);

							if(results == EOF)
							{
								perror("File write failed");
							}
							fclose(fp);

							char a;

							char path2[2000] = "/home/himanshu/Documents/NonAcademics/";
							strcat(path2,buf);


							fp = fopen(path2,"w");

							char content[900000];
							rst = recv(cfd,content,900000,0);
							//printf("%s\n", content);
							int results1 = fputs(content,fp);
							if(results1 == EOF)
							{
								perror("Failes to write");
							}
							fclose(fp);
							rst = send (cfd, "File Successfully Uploaded on Server",37,0);
							///// ERROR /////////
					    }

		    		}
		    	}


		    	else	////////////////////////////////////////// READER ///////////////////////////////////////////////
		    	{
		    		char headlines[BUF_SIZE] = {'\0'};
		    		rst = send (cfd, "Press 1 for Academic / Press 2 for Non-Academic", 47,0); // Send choices
		    		if (rst == -1)
				    {
				        perror ("Server write failed");
				        exit (1);
				    }
				    rst = recv(cfd,buf,2,0); // Receive Choices
				    int ans = atoi(buf);
				    int index = 0;

				    if(ans==1)		// Getting the headlines "Academic"
		   			{
						int count = 0;
						char ch;
						FILE *fp;
						fp = fopen("/home/himanshu/Documents/Academics_Count.txt","r");
						while((ch=fgetc(fp))!=EOF)
						{
							if(ch=='\n')
							{
								headlines[count] = '|';
								count++;
								index++;
							}
							else
							{
								headlines[count] = ch;
								count++;
							}

						}
						headlines[count] = '\0';
		   			}

		   			else
		   			{
		   				int count = 0;
						char ch;
						FILE *fp;
						fp = fopen("/home/himanshu/Documents/Non-Academics_Count.txt","r");
						while((ch=fgetc(fp))!=EOF)
						{
							if(ch=='\n')
							{
								headlines[count] = '|';
								index++;
								count++;
							}
							else
							{
								headlines[count] = ch;
								count++;
							}

						}
						headlines[count] = '\0';
		   			}
		   			headlines[strlen(headlines)-1] = '\0';
		   			//printf("%s\n",headlines);
	   				rst = send (cfd,headlines,strlen(headlines)+1,0);	// Send the headlines

	   				memset(buf,'\0',sizeof(buf));

	   				rst = recv(cfd,buf,2,0); // Receive Headline's Index
		   			int head_index = atoi(buf);

		   			memset(buf,'\0',sizeof(buf));
		   			int buf_index = 0;
		   			int check_sep = 0;
		   			int flag = 0;
		   			// printf("%d\n", head_index);
		   			// printf("%d\n", index);

		   			index = index-1;
		   			if(head_index>index+1)
		   			{
		   				rst = send (cfd,"Ni mila",8,0);	// Send the flag
		   				//printf("ho\n");
		   				//exit(1);
		   			}

		   			else
		   			{
		   				//printf("ho\n");
		   				//printf("%d\n", index);
		   				rst = send (cfd,"Mil gya",8,0);
		   				if(head_index-index == 1)
			   			{
			   				for(int i=0;i<strlen(headlines);i++)
			   				{
			   					if(headlines[i]=='|')
			   						break;
			   					else
			   						buf[buf_index++] = headlines[i];
			   				}
			   			}

			   			else
			   			{
			   				for(int i = strlen(headlines)-1;i>=0;i--)
			   				{
			   					if(headlines[i]=='|')
			   					{
			   						check_sep++;
			   						if(check_sep==head_index)
			   						{
			   							for(int j= i+1 ;j<strlen(headlines);j++)
			   							{
			   								if(headlines[j]=='|')
			   								{
			   									break;
			   									flag = 1;
			   								}
			   								else
			   									buf[buf_index++] = headlines[j];
			   							}
			   						}
			   					}
			   					if(flag==1)
			   						break;
			   				}
			   			}
						//apply lock here

						struct flock fl = {F_WRLCK,SEEK_SET,0,0,0};
					    fl.l_pid=getpid();
						fl.l_type=F_RDLCK;
						int fd;
						// got read lock
			   			//printf("%s\n",buf);
			   			FILE *headline_content;
			   			char ch;
			   			int sent = 0;
			   			char path [2000] = "/home/himanshu/Documents/";
			   			if(ans==1)
			   			{
			   				strcat(path,"Academics/");
			   				strcat(path,buf);
			   				headline_content = fopen(path,"r");
			   				if(headline_content == NULL )
						   	{
						      perror("Error while opening the file.\n");
						      exit(EXIT_FAILURE);
						   	}
							fd=fileno(headline_content);
							if (fcntl(fd, F_SETLKW, &fl) == -1) {
						        perror("fcntl");
						        exit(1);
						    }
						    //sleep(10);
						   	while((ch = fgetc(headline_content))!= EOF )
						   	{
						   		send_data[sent++] = ch;
						   	}
							//fclose(headline_content);
			   			}
			   			else
			   			{
			   				strcat(path,"NonAcademics/");
			   				strcat(path,buf);
			   				//printf("%s\n",path);
			   				headline_content = fopen(path,"r");
							fd=fileno(headline_content);
							if (fcntl(fd, F_SETLKW, &fl) == -1) {
						        perror("fcntl");
						        exit(1);
						    }
			   				if(headline_content == NULL )
						   	{
						      perror("Error while opening the file.\n");
						      exit(EXIT_FAILURE);
						   	}
						   	//sleep(10);
						   	while((ch = fgetc(headline_content))!= EOF )
						   	{
						   		send_data[sent++] = ch;
						   	}
							//fclose(headline_content);
			   			}
			   			//printf("Hello\n");
			   			//printf("%s\n",send_data );
			   			rst = send (cfd,send_data,strlen(send_data)+1,0);	// Send the headlines_content
						// remove lock here
						fclose(headline_content);
						// removed lock
			   		}
		   		}
		    	close(cfd);
		    	exit(1);

			}
			else
			{
			close(cfd);
				//exit (1);
			}

		}

    }

}
