#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <ctype.h> 
#include <unistd.h> 


int main()
{
    // unique key 
    key_t key = ftok("shmfile1",65); 
    // identifier 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
    // to attach
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
        sleep(1);
    }while(*flag_p2 == 0); 

    if ( *alnum >= *spc )
        *strength = 0;
    else if ( *alnum < *spc )
        *strength = 1;

    // printf("alnum: %d, special: %d\n", *alnum, *spc);
    // printf("Strength: %d\n", *strength);
    *flag_p3 = 1;
  
    
    //detach  
    shmdt(base); 
    shmctl(shmid,IPC_RMID,NULL); 
    return 0;
}