#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
    // P1 - reads an array of size 5
    // P2 - calculates avg, std deviation 
    // P3 - analyses temperatures with avg, sd; categorizes locns
    // P1 - actuates temperature
    float arr[5];
    int i;
    int cat[5];
    float newTemp[5];
    printf("Enter temperatures:\n");
    for (i=0; i<5; i++) {
        scanf("%f", &arr[i]);
    }

    if( mkfifo("/tmp/myfifo2", 0666) == -1) {
        if (errno != EEXIST) {
            return 1;
        }
    }
    if( mkfifo("/tmp/myfifo3", 0666) == -1) {
        if (errno != EEXIST) {
            return 1;
        }
    }
    if( mkfifo("/tmp/myfifocat", 0666) == -1) {
        if (errno != EEXIST) {
            return 1;
        }
    }
    


    int fifoArr2 = open("/tmp/myfifo2", O_WRONLY);
    if (fifoArr2 == -1) {
        return 2;
    }
    int fifoArr3 = open("/tmp/myfifo3", O_WRONLY);
    if (fifoArr3 == -1) {
        return 2;
    }
    
    if( write(fifoArr2, arr, 5*sizeof(float)) == -1) {
        return 2;
    }
    if( write(fifoArr3, arr, 5*sizeof(float)) == -1) {
        return 2;
    }
    close(fifoArr2);
    close(fifoArr3);
    
    // printf("Opening...\n");
    int fifoCat = open("/tmp/myfifocat", O_RDONLY);
    if (fifoCat == -1) {
        return 2;
    }
    if(read(fifoCat, cat, 5*sizeof(int)) == -1) {
        return 1;
    }
    close(fifoCat);
  
    // for (i=0; i<5; i++) {
    //     printf("%d ", cat[i]);
    // }
    // printf("\n");

    for (i=0; i<5; i++) {
            if(cat[i] == 0)
                newTemp[i] = arr[i];
            else if (cat[i] == 1)
                newTemp[i] = arr[i]-3;
            else if (cat[i] == 2)
                newTemp[i] = arr[i]-1.5;
            else if (cat[i] == 3)
                newTemp[i] = arr[i] +2;
            else if (cat[i] == 4)
                newTemp[i] = arr[i] +2.5;
        }

    printf("Revised Temperatures:\n");
    for (i=0; i<5; i++) {
        printf("L%d: %f\n", i+1, newTemp[i]);
    }   

    
    return 0;
}