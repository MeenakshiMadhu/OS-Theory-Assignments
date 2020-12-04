#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h>
#include <string.h>
#include <unistd.h> 


int checkpw(char *pw) {

    if(strlen(pw) == 10)
        return 0;
    printf("Enter password of length 10\n");
    return 1;
}

int main()
{

    // unique key 
    key_t key = ftok("shmfile1",65); 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
    // attach to base address
    char *base = (char*) shmat(shmid,(void*)0,0); 
    
    char *pw = base;
    int *flag_p1 = base + 15;
    int *flag_p2 = flag_p1 + 1;
    int *flag_p3 = flag_p2 + 1;
    int *alnum = flag_p3 + 1;
    int *spc = alnum+1;
    int *strength = spc+1;

    *flag_p1 = 0;
    *flag_p2 = 0;
    *flag_p3 = 0;
    *alnum = 0;
    *spc = 0;
    *strength = 0;
 
    do {
        printf("Enter password: "); 
        scanf("%s", pw);
    } while(checkpw(pw));
    *flag_p1 = 1;

    while(*flag_p3 == 0) {
        sleep(1);
    }


    if(*strength == 1) 
        printf("Password Is Strong\n") ;
    else if(*strength == 0)
        printf("Password Is Weak\n") ;
    else 
        printf("Password strength unavailable\n") ; 

      
    //detach from shared memory  
    shmdt(pw); 
    return 0;
}