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
	int sum = 0;

	while (1) {
		msgrcv(msgid, &msg, sizeof(struct message) - sizeof(long), 0, 0);
		if (msg.mtype == M_DATA) {
			printf("serwer : otrzymano wartosc od klienta %d\n", msg.data);
			sum += msg.data;
		}
		else if (msg.mtype == M_END) {
			struct message result_msg;
			result_msg.mtype = M_RESULT;
			result_msg.data = sum;
			msgsnd(msgid, &result_msg, sizeof(struct message) - sizeof(long), 0);
			break;
		}
	}

	printf("Serwer: Suma liczb to %d - odsylam do klienta\n", sum);

return 0;
}

