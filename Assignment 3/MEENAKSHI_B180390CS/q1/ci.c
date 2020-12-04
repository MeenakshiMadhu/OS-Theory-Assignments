#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// structure for message queue
typedef struct marks
{
	long mesg_type;
	double mark;
} marks;

typedef struct grades
{
	long mesg_type;
	char grade;
} grades;

int main()
{
	key_t key;
	int msgqid, i;

	key = ftok("assg3", 65);
	msgqid = msgget(key, 0066 | IPC_CREAT);

	int inputmarks[5] = {10, 20, 30, 40, 50};
	marks msgmarks[5];
	for (i = 0; i < 5; i++)
	{
		msgmarks[i].mesg_type = 10;
		msgmarks[i].mark = inputmarks[i];
	}

	// msgsnd to send message
	for (i = 0; i < 5; i++)
	{
		msgsnd(msgqid, &msgmarks[i], sizeof(msgmarks[i]), 0);
	}

	printf("Marks data sent to TA. \n");
	printf("Waiting for grades and average... \n");

	// for (i = 0; i < 5; i++)
	// {
	// 	printf("mark = %d, id = %ld\n", msgmarks[i].mark, msgmarks[i].mesg_type);
	// }

	grades msggrades[5];
	marks average;
	// msgrcv to receive message
	for (i = 0; i < 5; i++)
	{
		msgrcv(msgqid, &msggrades[i], sizeof(msggrades[i]), 100, 0);
	}
	msgrcv(msgqid, &average, sizeof(average), 101, 0);

	printf("Data received. \n");
	for (i = 0; i < 5; i++)
	{
		printf("id = %d, Grade = %c\n", i+1 ,msggrades[i].grade);
	}
	printf("Average : %f\n", average.mark);

	// msgctl(msgqid, IPC_RMID, NULL);

	return 0;
}