// code from http://www.linuxhowtos.org/C_C++/socket.htm

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

int encrypt(char* argv[3], char encrypted[9]);

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char* 	hostname = argv[1];
	char* 	port = argv[2];
	FILE *fp;

	char buffer[256];
	char bufferTemp[256];

	char* temp = (char *) malloc(sizeof(char) * 9);
	char* temp2 = (char *) malloc(sizeof(char) * 9);
	char* pass[3];
	char encryptbuffer[256];
	char encrypttemp[9];
	int count = 0;

	if (argc != 5) 
	{
	   fprintf(stderr,"usage: %s hostname port key text_file\n", argv[0]);
	   exit(0);
	}

	/* set up port */
	portno = atoi(port);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");

	/* set up server with ip */
	server = gethostbyname(hostname);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	/* make connection to server */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");

	/* get a local message to send */
	fp = fopen(argv[4],"r");
	bzero(buffer,256);
	bzero(bufferTemp,256);
	while(fgets(bufferTemp,sizeof(bufferTemp),fp) != NULL) {
		sprintf(buffer,"%s%s",buffer,bufferTemp);
	}
	fclose(fp);


	//printf("input: %s\n", buffer);


	while(strlen(buffer) %4 != 0) {
        	sprintf(buffer,"%s~",buffer);
    	}
    	while(count < strlen(buffer)) {
        	temp[count%4] = buffer[count];
        	count++;

        	if(count%4 == 0) {
			printf("char=%c%c%c%c\n", temp[0], temp[1], temp[2], temp[3]);
            		sprintf(temp2,"%02X%02X%02X%02X", temp[0], temp[1], temp[2], temp[3]);
			printf("hex=%02X%02X%02X%02X\n", temp[0], temp[1], temp[2], temp[3]);
            		pass[1] = temp2;
            		pass[2] = argv[3];
            		encrypt(pass, encrypttemp);
			printf("encrypt=%s\n",encrypttemp);
            		sprintf(encryptbuffer, "%s%s", encryptbuffer, encrypttemp);
        	}
    	}





	printf("output: %s\n", encryptbuffer);

	n = write(sockfd,encryptbuffer,strlen(encryptbuffer));
	if (n < 0) 
		 error("ERROR writing to socket");


	/* wait for return message */
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if (n < 0) 
		 error("ERROR reading from socket");
	printf("%s\n",buffer);

	close(sockfd);

	return 0;
}





int encrypt(char* argv[3], char encrypted[9])
{
    int             i;
    int            *encrypt = (int *) malloc(sizeof(int) * 4);
    int            *key = (int *) malloc(sizeof(int) * 4);
    char            convert_hex[3];
    int             temp, temp2;

    
    convert_hex[2] = '\0';
    for (i = 0; i < 4; i++) {
	convert_hex[0] = argv[1][i * 2];
	convert_hex[1] = argv[1][i * 2 + 1];
	encrypt[i] = (int) strtol(convert_hex, NULL, 16);
	convert_hex[0] = argv[2][i * 2];
	convert_hex[1] = argv[2][i * 2 + 1];
	key[i] = (int) strtol(convert_hex, NULL, 16);
    }

    for (i = 0; i < 4; i++) {
	encrypt[i] = encrypt[i] ^ key[i];
    }

    temp = encrypt[0];
    temp2 = encrypt[1];
    encrypt[0] = encrypt[2];
    encrypt[2] = temp;
    encrypt[1] = encrypt[3];
    encrypt[3] = temp2;
    for (i = 0; i < 4; i++) {
	encrypt[i] = encrypt[i] ^ key[i];
    }
    sprintf(encrypted," %02x%02x%02x%02x", encrypt[0], encrypt[1], encrypt[2], encrypt[3]);
    return 0;
}
