#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<time.h>

#define MAXLINE 1024
#define PORTNUM 3600

struct input {
	char buf[MAXLINE];
	int num;
};


union semun {
	int val;
};

int main(int argc, char **argv)
{
	int listen_fd, client_fd;
	pid_t pid1;
	socklen_t addrlen;
	int readn;
	struct sockaddr_in client_addr, server_addr;

	listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	memset((void *)&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORTNUM);
	bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen(listen_fd, 5);

	while (1)
	{
		addrlen = sizeof(client_addr);
		client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addrlen);
		pid1 = fork();

		if (pid1 > 0)
			close(client_fd);
		else if (pid1 == 0)
		{
			pid_t pid2;
			int shmid, semid, keyshm, keysem;
			void *shared_memory = NULL; 
			struct input inputBuf;
			union semun sem_union;
			struct sembuf semopen = { 0, -1, SEM_UNDO };
			struct sembuf semclose = { 0, 1, SEM_UNDO };

			close(listen_fd);

			srand((unsigned int)time(NULL));
			keyshm = rand();
			keysem = rand();

			pid2 = fork();

			if (pid2 > 0)
			{
				int str_len;
				char tmp;

				shmid = shmget((key_t)keyshm, sizeof(struct input), 0666 | IPC_CREAT);
				if (shmid == -1)
					return 1;
				semid = semget((key_t)keysem, 1, IPC_CREAT | 0666);
				if (semid == -1)
					return 1;
				shared_memory = shmat(shmid, NULL, 0);
				if (shared_memory == (void *)-1)
					return 1;
				sem_union.val = 0;

				semctl(semid, 0, SETVAL, sem_union);
				memset(&inputBuf, 0x00, sizeof(struct input));
				readn = read(client_fd, &inputBuf, sizeof(struct input));

				printf("Read Data %s(%d) : %s", inet_ntoa(client_addr.sin_addr), client_addr.sin_port, inputBuf.buf);
				close(client_fd);

				str_len = strlen(inputBuf.buf);
				inputBuf.buf[str_len - 1] = 0x00;

				while (1)
				{
					inputBuf.num++;
					tmp = inputBuf.buf[0];

					for (int i = 1; i < str_len - 1; i++)
					{
						inputBuf.buf[i - 1] = inputBuf.buf[i];
					}
					inputBuf.buf[str_len - 2] = tmp;

					memset(shared_memory, 0x00, sizeof(struct input));
					memcpy(shared_memory, &inputBuf, sizeof(struct input));
					sleep(1);
					if(semop(semid, &semopen, 1)==-1)
						return 1;
				}
			}
			else if (pid2 == 0)
			{
				shmid = shmget((key_t)keyshm, sizeof(struct input), 0666);
				semid = semget((key_t)keysem, 0, 0666);
				shared_memory = shmat(shmid, NULL, 0);

				while (1)
				{
					sleep(1);
					semop(semid, &semclose, 1);
					memset(&inputBuf, 0x00, sizeof(struct input));
					memcpy(&inputBuf, shared_memory, sizeof(struct input));

					write(client_fd, &inputBuf, sizeof(struct input));
				}
			}
		}
	}
	return 0;
}


