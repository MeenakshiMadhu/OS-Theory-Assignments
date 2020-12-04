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

	marks msgmarks[5];
	grades msggrades[5];

	// msgrcv to receive message
	for (i = 0; i < 5; i++)
	{
		msgrcv(msgqid, &msgmarks[i], sizeof(msgmarks[i]), 10, 0);
	}

	// display the message
	printf("Data Received.\n");

	printf("Marks Data: \n");
	for (i = 0; i < 5; i++)
	{
		printf("id = %d, Mark = %f\n",i+1, msgmarks[i].mark);
	}

	//calculate average
	marks average;
	average.mesg_type = 101;
	for (i = 0; i < 5; i++)
	{
		average.mark += msgmarks[i].mark;
	}
	average.mark /= 5;

	for (i = 0; i < 5; i++)
	{
		// msggrades[i].mesg_type = 100;
		if (msgmarks[i].mark <= 19)
		{
			msggrades[i].mesg_type = 100;
			msggrades[i].grade = 'E';
		}
		else if (msgmarks[i].mark <= 29)
		{
			msggrades[i].mesg_type = 100;
			msggrades[i].grade = 'B';
		}
		else if (msgmarks[i].mark <= 39)
		{
			msggrades[i].mesg_type = 100;
			msggrades[i].grade = 'A';
		}
		else if (msgmarks[i].mark <= 50)
		{
			msggrades[i].mesg_type = 100;
			msggrades[i].grade = 'S';
		}
		else
		{
			msggrades[i].mesg_type = 100;
			msggrades[i].grade = 'Z';
		}
	}

	printf("Grade Data:\n");
	for (i = 0; i < 5; i++)
	{
		printf("id = %d, Grade = %c\n", i+1,msggrades[i].grade);
	}
	printf("Average : %f\n", average.mark);

	for (i = 0; i < 5; i++)
	{
		msgsnd(msgqid, &msggrades[i], sizeof(msggrades[i]), 0);
	}
	msgsnd(msgqid, &average, sizeof(average), 0);
	printf("Data Sent to CI.\n");

	for (i = 0; i < 5; i++)
		msgmarks[i].mesg_type = i + 1;
	for (i = 0; i < 5; i++)
		msgsnd(msgqid, &msgmarks[i], sizeof(msgmarks[i]), 0);

	printf("Data Sent to students.\n");

	// msgctl(msgqid, IPC_RMID, NULL);

	return 0;
}