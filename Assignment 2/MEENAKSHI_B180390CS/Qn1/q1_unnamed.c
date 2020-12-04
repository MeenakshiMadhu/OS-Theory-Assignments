#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    float arr[5];
    //int cat[5];
    // int arraysize = sizeof(arr) / sizeof(int);
    int i;
    int childpidB, childpidC;
    // P1 = main process    - reads an array of size 5
    // P2 = firstchild B    - calculates avg, std deviation 
    // P3 = secondchild C   - analyses temperatures with avg, sd; categorizes locns
    // P1                   - actuates temperature

    printf("Enter temperatures L1, L2, L3, L4, L5\n");
        for (i=0; i<5; i++) {
            scanf("%f",&arr[i]);
        }


    int pipeAvg[2];      // pipeAvg[0] - read     pipeAvg[1] - write
    if (pipe(pipeAvg) == -1) {
        printf("An error occured with opening pipe 1\n");
        return 1;
    }
    int pipeSD[2];      
    if (pipe(pipeSD) == -1) {
        printf("An error occured with opening pipe 2\n");
        return 1;
    }
    int pipeCat[2];      
    if (pipe(pipeCat) == -1) {
        printf("An error occured with opening pipe 3\n");
        return 1;
    }


    childpidB = fork();
    if (childpidB <0) {
        printf("An error occured while forking first time\n");
        return 2;
    }
    //we get child process B
    if (childpidB == 0) {
        
        close(pipeAvg[0]);
        close(pipeSD[0]);
        close(pipeCat[0]);
        close(pipeCat[1]);
        
        //printf("In child process B \n");
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

        write(pipeAvg[1], &avg, sizeof(float));
        close(pipeAvg[1]);
        write(pipeSD[1], &sd, sizeof(double));
        close(pipeSD[1]);
        exit(0);
    }
    
    wait(NULL);

    childpidC = fork();
    if (childpidC <0) {
        printf("An error occured while forking second time\n");
        return 3;
    }
    //we get child process C
    if (childpidC == 0) {
        close(pipeAvg[1]);
        close(pipeSD[1]);
        close(pipeCat[0]);
        // printf("In child process C\n");
        // for (i=0; i<5; i++) {
        //     printf("%f ", arr[i]);
        // }
        // printf("\n");
        float avg;
        double sd;
        int cat[5];
        read(pipeAvg[0], &avg, sizeof(float));
        close(pipeAvg[0]);
        read(pipeSD[0], &sd, sizeof(double));
        close(pipeSD[0]);

// avg-sd-----avg--------avg+sd
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

        // printf("Categories: ");
        // for (i=0; i<5; i++) {
        //     printf("%d ", cat[i]);
        // }
        // printf("\n");

        // printf("Avg: %f\nStd Dev: %lf\n", avg, sd);

        write(pipeCat[1], &cat, 5*sizeof(int));
        close(pipeCat[1]);
        exit(0);
    }

    wait(NULL);

    // ----------------Main process----------------------------
    if (childpidB > 0 && childpidC > 0) {
        // printf("In main process\n");
        
        close(pipeAvg[0]);
        close(pipeAvg[1]);
        close(pipeSD[0]);
        close(pipeSD[1]);
        close(pipeCat[1]);
        
        float newTemp[5];
        int cat[5];

        read(pipeCat[0], &cat, 5*sizeof(int));
        close(pipeCat[0]);

        // printf("Categories: ");
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
    }

    return 0;
}