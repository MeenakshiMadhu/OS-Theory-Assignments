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
    float arr[5];
    float avg;
    double sd;
    int cat[5];
    int fifoArr = open("/tmp/myfifo3", O_RDONLY);
    if (fifoArr ==-1) {
        return 1;
    }
    int fifoAvg = open("/tmp/myfifoavg", O_RDONLY);
    if (fifoAvg ==-1) {
        return 1;
    }
    int fifoSD = open("/tmp/myfifosd", O_RDONLY);
    if (fifoSD ==-1) {
        return 1;
    }

    int i;
    if(read(fifoArr, &arr, 5*sizeof(float)) == -1) {
            return 1;
        }
    if(read(fifoAvg, &avg, sizeof(float)) == -1) {
            return 1;
        }
    if(read(fifoSD, &sd, sizeof(double)) == -1) {
            return 1;
        }

    //printf("Avg: %f\nStd Dev: %lf\n", avg, sd);
    // for (i=0; i<5; i++) {
    //     printf("%f ", arr[i]);
    // }
    // printf("\n");
    close(fifoArr);
    close(fifoAvg);
    close(fifoSD);
    
    for(i=0; i<5; i++) {
            
            if (arr[i] == avg ) 
                cat[i] = 0;
            else if (arr[i] > (avg+sd))
                cat[i] = 1;
            else if (arr[i] > avg && arr[i] <= avg+sd)
                cat[i] =2;
            else if (arr[i] < avg && arr[i] >= avg-sd)
                cat[i] =3;
            else if (arr[i] < avg-sd)
                cat[i] = 4;
        }

    // for (i=0; i<5; i++) {
    //     printf("%d ", cat[i]);
    // }
    // printf("\n");


    int fifoCat = open("/tmp/myfifocat", O_WRONLY);
    if (fifoCat == -1) {
        return 2;
    }
    if( write(fifoCat, cat, 5*sizeof(int)) == -1) {
        return 2;
    }
    close(fifoCat);
    
    return 0;
}