#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
int encrypt(int argc, char *argv[],char* encrypted);
int main (int argc, char *argv[])
{
    int i;
    int j;
    char [10000]message;
    char [10000]encrypted;
    char [9]encrypttemp;
    char [500]filetemp;
    char [9] temp;
    char [9] temphex;
    char [9] key;
    FILE* input;
    int count=0;
    key=argv[2];
    char [3]pass[9]
    input=fopen(argv[1],"r");
    if (argc != 5) 
	{
	   fprintf(stderr,"usage %s hostname port key textfile.txt\n", argv[0]);
	   exit(0);
	}
    int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
    char buffer[256];
    portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	
		server = gethostbyname(argv[1]);
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
    while()
    {
        fgets(filetemp,500,input);
        sprintf(message,"%s%s",message,filetemp);
    }
    fclose(input);
    while(message %4 !=0)
    {
        sprintf(message,"%s~",message);
    }
    while(count<strlen(message))
    {
        temp[count%4]=message[count];
        count++;
        if(count%4==0)
        {
            sprintf(temp2,"%02x%02x%02x%02x",temp[0],temp[1],temp[2],temp[3]);
            pass[1]=temp2;
            pass[2]=key;
            encrypted(pass,encrypttemp);
            sprintf(encrypted,"%s%s"encrypted,encrypttemp);
        }
    }
    n = write(sockfd,encrypted,strlen(encrypted));
    if (n < 0) 
		 error("ERROR writing to socket");
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if (n < 0) 
		 error("ERROR reading from socket");
	printf("%s\n",buffer);

	close(sockfd);

	return 0;
}
int encrypt(char [3]argv[9],char [9]encrypted)
{
    int             i;
    int            *encrypt = (int *) malloc(sizeof(int) * 4);
    int            *key = (int *) malloc(sizeof(int) * 4);
    char            convert_hex[3];
    int             temp,
                    temp2;
   // if (argc != 3 && strlen(argv[1]) != 8 && strlen(argv[2]) != 8) {
	//printf("usage: program.exe message_as_hex_8digitskey_as_hex_8digits\n");
	//exit(-1);
    }
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
    printf("xor1 %02x%02x%02x%02x\n", encrypt[0], encrypt[1], encrypt[2], encrypt[3]);	// means 
											// print 
											// with 
											// 2 
											// digits
    temp = encrypt[0];
    temp2 = encrypt[1];
    encrypt[0] = encrypt[2];
    encrypt[2] = temp;
    encrypt[1] = encrypt[3];
    encrypt[3] = temp2;
    printf("swap %02x%02x%02x%02x\n", encrypt[0], encrypt[1], encrypt[2], encrypt[3]);	// means 
											// print 
											// with 
											// 2 
											// digits
    for (i = 0; i < 4; i++) {
	encrypt[i] = encrypt[i] ^ key[i];
    }
    printf("xor2 %02x%02x%02x%02x\n", encrypt[0], encrypt[1], encrypt[2], encrypt[3]);	// means 
											// print 
											// with 
											// 2 
											// digits
    sprintf(encrypted," %c%cx%cx%cx", encrypt[0], encrypt[1], encrypt[2], encrypt[3]);	// means 
													// print 
													// with 
													// 2 
													// digits
    return 0;
}
void error(const char *msg)
{
	perror(msg);
	exit(0);
}
