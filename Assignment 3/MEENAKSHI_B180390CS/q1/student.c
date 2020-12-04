#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// structure for message queue
typedef struct mesg_buffer
{
    long mesg_type;
    double mark;
} message;

int main()
{

    key_t key;
    int msgqid;

    key = ftok("assg3", 65);
    msgqid = msgget(key, 0066 | IPC_CREAT);

    message stud[5];
    for (int i = 0; i < 5; i++)
    {
        msgrcv(msgqid, &stud[i], sizeof(stud[i]), i + 1, 0);
    }
    printf("Data Received.\n");
    // display the message
    printf("Student Data :\n");
    for (int i = 0; i < 5; i++)
    {
        printf("id = %ld, Mark = %f\n", stud[i].mesg_type, stud[i].mark);
    }

    msgctl(msgqid, IPC_RMID, NULL);
    return 0;
}