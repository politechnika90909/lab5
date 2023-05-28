#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define M_DATA 1
#define M_END 2
#define M_RESULT 3

struct message {
	long mtype;
	int data;
};

int main() {
	key_t key = ftok(".", 'A');
	int msgid = msgget(key, IPC_CREAT | 0666);
	struct message msg;
	int numbers[] = {10, 20, 30, 40, -1}; 
	
	for (int i = 0; numbers[i] != -1; i++) {
		msg.mtype = M_DATA;
		msg.data = numbers[i];
		msgsnd(msgid, &msg, sizeof(struct message) - sizeof(long), 0);
		sleep(2);
	}
	
	msg.mtype = M_END;
	msgsnd(msgid, &msg, sizeof(struct message) - sizeof(long), 0);
	struct message result_msg;
	msgrcv(msgid, &result_msg, sizeof(struct message) - sizeof(long), M_RESULT, 0);
	printf("Klient: Otrzymano wynik sumowania od serwera: %d\n", result_msg.data);

	return 0;
}

// gcc -c serwer26.c -o serwer.o; gcc -o serwer serwer.o; gcc -c klient26.c -o klient.o; gcc -o klient klient.o; ./serwer & sleep 1; ipcs -q; ./klient
