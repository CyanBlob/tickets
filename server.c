/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{

     int tickets[10] = {0};
     int x = 0;
     int y = 0;
     int i = 0;
     int j = 0;
     int tmpticket;

     srand(time(NULL));

     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     //while(1)
     //{
     	listen(sockfd,5);
     	clilen = sizeof(cli_addr);
     	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     	if (newsockfd < 0) 
        	  error("ERROR on accept");
     	bzero(buffer,256);
   
   //The program is now in an infinite loop, so it continues to run -Andrew  
   while(1)
   {
  
 	 n = read(newsockfd,buffer,255);
    	 if (n < 0) error("ERROR reading from socket");
    	 printf("Here is the message: %s\n",buffer);
    	 n = write(newsockfd,"I got your message",18);
    	 if (n < 0) error("ERROR writing to socket");
	
	     if (strcmp(buffer,"buy\n") == 0)
	     {
		    
		     printf("You are buying a ticket!\n");
		     
		    //Ensures that no duplicate numbers can be created, just in case -Andrew
		    while (1)
		    {
		         tmpticket = rand() % 89999 + 10000;
			 for (y = 0; y < 10; y++)
			 {
			     if (tmpticket == tickets[y])
			     {
			         //printf("DUPLICATE FOUND\n");
			         tmpticket = rand() % 89999 + 10000;
			         y = -1;
			      }

			  }
			  break;
		     }


		     for (x = 0; x < 10; x++)
		     {
	                  if(tickets[x] == 0)
			  {

		               tickets[x] = tmpticket;
			       printf("Your ticket number is: %d\n",tickets[x]);
			       break;
			  }
	
			 
		     } 
		     if(x >= 10)
			     printf("ERROR: We are out of tickets! Try again later!\n");
	     }
	     else if (strcmp(buffer,"exit\n") == 0)
		     return 0;

	     //Checks if the user entered "cancel" as the first 6 digits. Next, needs to use string manipulation to copy the 7th-12th characters into a seperate string, which will be the cancelled ticket number -Andrew
	     else if(strncmp(buffer, "cancel", 6) == 0)
	     {
		     printf("CANCEL\n");
	     }
	     else
		     printf("You are not buying a ticket!\n");
	     bzero(buffer,256);
     }
     close(newsockfd);
     close(sockfd);
     return 0; 
}
