#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAXLINE 1024
#define PORTNUM 3600

struct input {
	char buf[MAXLINE];
	int num;
};

int main(int argc, char** argv)
{
	struct sockaddr_in serveraddr;
	struct input inputBuf;
	int server_sockfd;
	int client_len;
	char num[MAXLINE];

	memset((void*)&inputBuf, 0x00, sizeof(struct input));
	printf("input string : \n");
	read(0, (void *)&inputBuf.buf, sizeof(inputBuf.buf));
	//printf("input number : \n");
	read(0, (void *)&num, MAXLINE);
	inputBuf.num = atoi(num);
	time_t t;
	struct tm *tm;

	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		perror("socket error:");
		return 1;
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(PORTNUM);

	client_len = sizeof(serveraddr);

	if ((connect(server_sockfd, (struct sockaddr*)&serveraddr, client_len)) == -1)
	{
		perror("connect error:");
		return 1;
	}

	if ((write(server_sockfd, (void*)&inputBuf, sizeof(struct input))) <= 0)
	{
		perror("write error:");
		return 1;
	}

	while (1)
	{
		if ((read(server_sockfd, (void*)&inputBuf, sizeof(struct input))) <= 0)
		{
			perror("read error:");
			return 1;
		}
		t=time(NULL);
		tm = localtime(&t);
		printf("%s %s\n", inputBuf.buf, asctime(tm));
	}
	close(server_sockfd);
	return 0;
}


