#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h> // For the socket () etc. functions.
#include <netinet/in.h> // For IPv4 data struct..
#include <string.h> // For memset.
#include <arpa/inet.h> // For inet_pton ().

int port_num = 23466;
#define BUF_SIZE 1000

void main () 
{
    int rst; // Return status of functions.
    int sfd; // Socket file descriptor.
    


    /***************** Create a socket *******************************/
    sfd = socket (AF_INET, SOCK_DGRAM, 0); /* AF_INET --> IPv4,
                * SOCK_DGRAM --> UDP Protocol, 0 --> for the protocol. */
    if (sfd == -1) 
    {
        perror ("Client_1.c socket error");
        exit (1);
    }
    printf ("Socket fd = %d\n", sfd);
        
    /****************** Send - receive messages **********************/
    // char msg[20] = "Hello";
    char msg[20] = {'\0'};
     msg[0]='H';    msg[1]='e';    msg[2]='l';    msg[3]='l';
     msg[4]='o'; /* Sent size remains 20 for both types of message.
    //     * Conclusion: NULL terminated strings are not treated
   //     * differently. */

    char msg_len = 20;
     /*int  flags   = 0 | MSG_DONTWAIT; /* Client doesn't wait even if 
        * server is not running.
        * The client will return with EAGAIN if the send-buffer is full.
        * */
    int flags = 0; /* Even Now the client doesn't wait even if server 
        * is not running. 
        * Now the client will wait if its send-buffer is full.
        * */
    
    struct sockaddr_in dest_addr; /* sockaddr_in because we are using 
            * IPv4. Type casted to struct sockaddr * at time of 
            * various system calls. */
            
    socklen_t addrlen = sizeof (struct sockaddr_in); 
        socklen_t sender_len;

    struct sockaddr_in sender_addr;
    
    // Initializing destination address.
    memset (&dest_addr, 0, addrlen); // Initializes address to zero.

    dest_addr.sin_family = AF_INET;  // Address is in IPv4 format.
    dest_addr.sin_port   = htons (port_num);  // Port number of the server.
    
    
    // rst = inet_pton (AF_INET, "127.0.0.1", &dest_addr.sin_addr);  Note
    //         * that third field should point to an in_addr (in6_addr). 
    // if (rst <= 0)
    // {
    //     perror ("Client Presentation to network address conversion.\n");
    //     exit (1);
    // }        
    
    /* Sending message to the server. */
    rst = sendto (sfd, msg, 20, flags, (struct sockaddr *) &dest_addr, 
                    sizeof (struct sockaddr_in)); /* Value of rst is 20,
        * on successful transmission; i.e. It has nothing to do with a 
        * NULL terminated string.
        */
    if (rst < 0)
    {
        perror ("Client: Sendto function call failed");
        exit (1);
    }
    else 
    {
        printf ("Sent data size = %d\n", rst);
    }


    while(1)
    {   
        char buf[1000];
        
        rst = recvfrom(sfd,buf,BUF_SIZE,flags,(struct sockaddr *) &sender_addr,&sender_len);

        printf ("%s\n", buf);   // initial msg
        
        memset(buf,'\0',sizeof(buf));
        scanf("%s",buf);        // UserName

        rst = sendto (sfd, buf, sizeof(buf), flags, (struct sockaddr *) &dest_addr, 
                        sizeof (struct sockaddr_in));  // Send Username
        if (rst < 0)
        {
            perror ("Client: Sendto function call failed");
            exit (1);
        }

        rst = recvfrom (sfd, buf, BUF_SIZE, flags, 
                        (struct sockaddr *) &sender_addr, // Recieve 
                        &sender_len);
        if (rst < 0)
        {
            perror ("Client: couldn't receive");
            exit (1);
        }
        
        printf ("%s\n", buf);

        if(strcmp(buf,"Incorrect Login ID or Password . Please try again")==0)
        {
            continue;
        }
        else
        {
            memset(buf,'\0',sizeof(buf));
            scanf("%s",buf);        // Password

            rst = sendto (sfd, buf, sizeof(buf), flags, (struct sockaddr *) &dest_addr, sizeof (struct sockaddr_in));   // Send Password
            if (rst < 0)
            {
                perror ("Client: Sendto function call failed");
                exit (1);
            }

            rst = recvfrom (sfd, buf, BUF_SIZE, flags, (struct sockaddr *) &sender_addr,&sender_len);
            if (rst < 0)
            {
                perror ("Client: couldn't receive");
                exit (1);
            }
    
            printf ("%s\n", buf);
            if(strcmp(buf,"Incorrect Login ID or Password . Please try again")==0)
            {
                continue;
            }
            else
            {
                char buf[BUF_SIZE];
                memset(buf,'\0',sizeof(buf));
                rst = recvfrom (sfd, buf, BUF_SIZE, flags, (struct sockaddr *) &sender_addr,&sender_len);
                printf("\n%s\n",buf);

                memset(buf,'\0',sizeof(buf));
                rst = recvfrom (sfd, buf, BUF_SIZE, flags, (struct sockaddr *) &sender_addr,&sender_len);
                printf("\n%s\n\n",buf);

                char buffered[9000];
                memset(buffered,'\0',sizeof(buffered));
                rst = recvfrom (sfd, buffered,sizeof(buffered), flags, (struct sockaddr *) &sender_addr,&sender_len);
                
                //printf("%s\n",buffered );
                int bufsize21 = 10000;
                int position21 = 0;
                char **tokens21 = malloc(sizeof(char)*bufsize21);
                char *token21;
                token21 = strtok(buffered,"|");

                while(token21!=NULL)
                {
                    tokens21[position21] = token21;
                    position21++;
                    token21 = strtok(NULL,"|");
                }
                tokens21[position21] = NULL;

                // for(int i=0;i<position21;i++)
                //     printf("%s\n",tokens21[i]);

                char headlines_212[900][900];
                char date_212[900][900];
                char time_212[900][900];
                memset(headlines_212,'\0',sizeof(headlines_212));

                memset(date_212,'\0',sizeof(date_212));

                memset(time_212,'\0',sizeof(time_212));

                
                
                
                for(int i=0;i<position21;i++)
                {
                    int headx = 0;
                    for(int j=0;j<strlen(tokens21[i]);j++)
                    {
                        if(tokens21[i][j]=='~')
                        {
                            break;
                        }
                        else if(tokens21[i][j]=='^')
                        {
                            headlines_212[i][headx++] = ' ';
                        }
                        else
                            headlines_212[i][headx++] = tokens21[i][j];
                    }
                    //printf("%s\n",headlines_212[i]);
                }
                
                for(int i=0;i<position21;i++)
                {
                    int datex = 0;
                    for(int j=0;j<strlen(tokens21[i]);j++)
                    {
                        if(tokens21[i][j]=='~')
                        {
                            for(int lol=j+1;lol<strlen(tokens21[i]);lol++)
                            {
                                if(tokens21[i][lol]=='`')
                                    break;
                                else
                                    date_212[i][datex++] = tokens21[i][lol];
                                        }
                                    }
                        else
                        {
                            continue;
                        }
                    }
                }

                for(int i=0;i<position21;i++)
                {
                    int timex = 0;
                    for(int j=0;j<strlen(tokens21[i]);j++)
                    {
                        if(tokens21[i][j]=='~')
                        {
                            for(int lol=j+1;lol<strlen(tokens21[i]);lol++)
                            {
                                if(tokens21[i][lol]=='`')
                                {
                                     for(int lol21=lol+1;lol21<strlen(tokens21[i]);lol21++)
                                     {
                                        time_212[i][timex++] = tokens21[i][lol21];
                                     }
                                }
                            }
                        }
                        else
                        {
                            continue;
                        }
                    }
                }

                //printf("%s\n", headlines_212[1]);
                printf("%-120s %11s %6s\n","Headline------","Date","Time" );
                for(int i=0;i<position21;i++)
                {

                    printf("%-120s %11s %6s\n",headlines_212[i],date_212[i],time_212[i]);
                }


                memset(buf,'\0',sizeof(buf));
                rst = recvfrom (sfd, buf, BUF_SIZE, flags, (struct sockaddr *) &sender_addr,&sender_len);
                printf("\n%s\n\n",buf);  // Non Academics

                memset(buffered,'\0',sizeof(buffered));
                rst = recvfrom (sfd, buffered,sizeof(buffered), flags, (struct sockaddr *) &sender_addr,&sender_len);
                //printf("%s\n",buffered );// Non academics


                int bufsize212 = 10000;
                int position212 = 0;
                char **tokens212 = malloc(sizeof(char)*bufsize212);
                char *token212;
                token212 = strtok(buffered,"|");

                while(token212!=NULL)
                {
                    tokens212[position212] = token212;
                    position212++;
                    token212 = strtok(NULL,"|");
                }
                tokens212[position212] = NULL;

                // for(int i=0;i<position212;i++)
                //     printf("%s\n",tokens212[i]);


                memset(headlines_212,'\0',sizeof(headlines_212));

                memset(date_212,'\0',sizeof(date_212));

                memset(time_212,'\0',sizeof(time_212));


                for(int i=0;i<position212;i++)
                {
                    int headx = 0;
                    for(int j=0;j<strlen(tokens212[i]);j++)
                    {
                        if(tokens212[i][j]=='~')
                        {
                            break;
                        }
                        else if(tokens212[i][j]=='^')
                        {
                            headlines_212[i][headx++] = ' ';
                        }
                        else
                            headlines_212[i][headx++] = tokens212[i][j];
                    }
                    //printf("%s\n",headlines_212[i]);
                }
                
                for(int i=0;i<position212;i++)
                {
                    int datex = 0;
                    for(int j=0;j<strlen(tokens212[i]);j++)
                    {
                        if(tokens212[i][j]=='~')
                        {
                            for(int lol=j+1;lol<strlen(tokens212[i]);lol++)
                            {
                                if(tokens212[i][lol]=='`')
                                    break;
                                else
                                    date_212[i][datex++] = tokens212[i][lol];
                            }
                        }
                        else
                        {
                            continue;
                        }
                    }
                }

                for(int i=0;i<position212;i++)
                {
                    int timex = 0;
                    for(int j=0;j<strlen(tokens212[i]);j++)
                    {
                        if(tokens212[i][j]=='~')
                        {
                            for(int lol=j+1;lol<strlen(tokens212[i]);lol++)
                            {
                                if(tokens212[i][lol]=='`')
                                {
                                     for(int lol21=lol+1;lol21<strlen(tokens212[i]);lol21++)
                                     {
                                        time_212[i][timex++] = tokens212[i][lol21];
                                     }
                                }
                            }
                        }
                        else
                        {
                            continue;
                        }
                    }
                }

                //printf("%s\n", headlines_212[1]);
                printf("%-120s %11s %6s\n","Headline------","Date","Time" );
                for(int i=0;i<position212;i++)
                {

                    printf("%-120s %11s %6s\n",headlines_212[i],date_212[i],time_212[i]);
                }

                char buf11[1000],date[1000];

                rst = recvfrom (sfd, buf11, BUF_SIZE, flags, (struct sockaddr *) &sender_addr,&sender_len);

                printf("\n%s :\n",buf11);
                scanf("%s",date);

                rst = sendto(sfd,date, sizeof(date),flags,(struct sockaddr *) &dest_addr, sizeof (struct sockaddr_in));

                if (rst < 0)
                {
                    perror ("Client: Sendto function call failed");
                    exit (1);
                }
                memset(buf11,'\0',sizeof(buf11));
                rst = recvfrom (sfd, buf11,sizeof(buf11), flags, (struct sockaddr *) &sender_addr,&sender_len);
                //printf("Hello\n");
                printf("%s ",buf11);

                memset(buf11,'\0',sizeof(buf11));
                rst = recvfrom (sfd, buf11,sizeof(buf11), flags, (struct sockaddr *) &sender_addr,&sender_len);
                printf("%s\n",buf11);

                memset(buf11,'\0',sizeof(buf11));
                rst = recvfrom (sfd, buf11,sizeof(buf11), flags, (struct sockaddr *) &sender_addr,&sender_len);
                printf("%s ",buf11);

                memset(buf11,'\0',sizeof(buf11));
                rst = recvfrom (sfd, buf11,sizeof(buf11), flags, (struct sockaddr *) &sender_addr,&sender_len);
                printf("%s\n",buf11);

            }
            
            break;
        }
    }
    

    /****************** Close ****************************************/
    rst = close (sfd); // Close the socket file descriptor.
    if (rst < 0)
    {
        perror ("Client close failed");
        exit (1);
    }
}