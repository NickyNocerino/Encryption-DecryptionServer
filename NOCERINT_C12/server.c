// code from http://www.linuxhowtos.org/C_C++/socket.htm

/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int decrypt(char* argv[3], char decrypted[9]);

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	char* filename = argv[3];
	FILE *fp;

	char* temp = (char *) malloc(sizeof(char) * 9);
	char* temp2 = (char *) malloc(sizeof(char) * 9);
	char* pass[3];
	char decryptbuffer[256];
	char decrypttemp[9];
	int count = 0;
	int tempcount=0;

	if (argc != 4) 
	{
		fprintf(stderr,"usage: %s port key text_file\n", argv[0]);
		exit(1);
	}

	/* set up socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");

	/* set up socket  and bind */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR on binding");

	/* Wait for client */
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
 	if (newsockfd < 0) 
		error("ERROR on accept");

	/* accept a message - Woken up when client connects */
	bzero(buffer,256);
	n = read(newsockfd,buffer,255);
	if (n < 0) 
		error("ERROR reading from socket");


	printf("input: %s\n", buffer);



	
    	while(count < strlen(buffer)) {
        	temp[tempcount] = buffer[count];
        	count++;
		tempcount++;

        	if(buffer[count]==' ')
		 {
			if(strlen(temp)==8)
			{
				sprintf(temp2,"%c%c%c%c%c%c%c%c", temp[0],temp[1], temp[2]					, temp[3],temp[4],temp[5],temp[6],temp[7]);
            			printf("hex=%s\n", temp2);
            			pass[1] = temp2;
            			pass[2] = argv[2];
            			decrypt(pass, decrypttemp);
				printf("decrypt=%s\n",decrypttemp);
            			sprintf(decryptbuffer, "%s%s", decryptbuffer, decrypttemp);
			}
			count++;
			tempcount=0;
        	}
    	}

	printf("output: %s\n", decryptbuffer);

	fp = fopen(filename,"w");
	fputs(decryptbuffer, fp);
       	fclose(fp);

	/* send a message back */
	n = write(newsockfd,"I got your message",18);
	if (n < 0) 
		error("ERROR writing to socket");

	/* shut down */
	close(newsockfd);
	close(sockfd);
	return 0; 
}

int decrypt(char* argv[3], char decrypted[9])
{
    int             i;
    int            *decrypt = (int *) malloc(sizeof(int) * 4);
    int            *key = (int *) malloc(sizeof(int) * 4);
    char            convert_hex[3];
    int             temp, temp2;

    
    convert_hex[2] = '\0';
    for (i = 0; i < 4; i++) {
	convert_hex[0] = argv[1][i * 2];
	convert_hex[1] = argv[1][i * 2 + 1];
	decrypt[i] = (int) strtol(convert_hex, NULL, 16);
	convert_hex[0] = argv[2][i * 2];
	convert_hex[1] = argv[2][i * 2 + 1];
	key[i] = (int) strtol(convert_hex, NULL, 16);
    }

    for (i = 0; i < 4; i++) {
	decrypt[i] = decrypt[i] ^ key[i];
    }

    temp = decrypt[0];
    temp2 = decrypt[1];
    decrypt[0] = decrypt[2];
    decrypt[2] = temp;
    decrypt[1] = decrypt[3];
    decrypt[3] = temp2;
    for (i = 0; i < 4; i++) {
	decrypt[i] = decrypt[i] ^ key[i];
    }	
    sprintf(decrypted," %c%c%c%c", decrypt[0], decrypt[1], decrypt[2], decrypt[3]);
    return 0;
}
