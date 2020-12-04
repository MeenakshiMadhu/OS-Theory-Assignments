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
    int fifoArr = open("/tmp/myfifo2", O_RDONLY);
    if (fifoArr ==-1) {
        return 1;
    }
    // printf("Opening...");
    
    int i;
    if(read(fifoArr, &arr, 5*sizeof(float)) == -1) {
            return 1;
        }
    // printf("Received\n");
    close(fifoArr);

    // for (i=0; i<5; i++) {
    //     printf("%f ", arr[i]);
    // }
    // printf("\n");

    float avg = 0;
    double sd = 0;
    for (i=0; i<5; i++) {
        avg += arr[i];
    }
    avg /= 5;

    for (i=0; i<5; i++) {
        double k;
        k = (arr[i]-avg) * (arr[i]-avg);
        sd += k;
    }

    sd /= 5;
    sd = sqrt(sd);
    // printf("Avg: %f\nStd Dev: %lf\n", avg, sd);

// --------------Average fifo---------------- 
    if( mkfifo("/tmp/myfifoavg", 0666) == -1) {
        if (errno != EEXIST) {
            printf("Could not make fifo file\n");
            return 1;
        }
    } 
    int fifoAvg = open("/tmp/myfifoavg", O_WRONLY);
    if (fifoAvg == -1) {
        printf("Could not open fifofile\n");
        return 2;
    }
    if( write(fifoAvg, &avg, sizeof(float)) == -1) {
        return 2;
    }
    close(fifoAvg);

//-------------SD fifo-----------------------------
if( mkfifo("/tmp/myfifosd", 0666) == -1) {
        if (errno != EEXIST) {
            printf("Could not make fifo file\n");
            return 1;
        }
    } 
    // printf("Opening...\n");
    int fifoSD = open("/tmp/myfifosd", O_WRONLY);
    if (fifoSD == -1) {
        printf("Could not open fifofile\n");
        return 2;
    }
    if( write(fifoSD, &sd, sizeof(double)) == -1) {
        return 2;
    }
    close(fifoSD);



    // printf("Avg: %f\nStd Dev: %lf\n", avg, sd);
    return 0;
}