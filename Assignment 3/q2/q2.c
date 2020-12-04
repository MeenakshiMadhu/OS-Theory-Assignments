#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, m, i, j;
    // n = 5; //number of processes
    // m = 3; // number of resources
    printf("Enter no. of processes:  ");
    scanf("%d", &n);
    printf("Enter no. of resources:  ");
    scanf("%d", &m);

    int AllocTable[n][m];
    int MaxTable[n][m];
    int need[n][m];
    int Total[m];
    int avail[m];
    int safeseq[100];

    // int AllocTable[5][3] = {{0, 1, 0},  // P0    // Allocation Matrix
    //                         {2, 0, 0},  // P1
    //                         {3, 0, 2},  // P2
    //                         {2, 1, 1},  // P3
    //                         {0, 0, 2}}; // P4

    // int MaxTable[5][3] = {{7, 5, 3},  // P0    // MAX Matrix
    //                       {3, 2, 2},  // P1
    //                       {9, 0, 2},  // P2
    //                       {4, 2, 2},  // P3
    //                       {5, 3, 3}}; // P4

    printf("Enter allocation table:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &AllocTable[i][j]);

    printf("Enter maximum need table:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &MaxTable[i][j]);

    printf("Enter no. of instances for %d resources:\n", m);
    for (i = 0; i < m; i++)
        scanf("%d", &Total[i]);

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            need[i][j] = MaxTable[i][j] - AllocTable[i][j];
            // printf("%d ", need[i][j]);
        }
        // printf("\n");
    }

    for (j = 0; j < m; j++)
        avail[j] = 0;
    for (j = 0; j < m; j++)
        for (i = 0; i < n; i++)
        {
            avail[j] += AllocTable[i][j];
        }
    for (j = 0; j < m; j++)
    {
        avail[j] = Total[j] - avail[j];
        // printf("%d ", avail[j]);
    }

    printf("Enter sequence to be checked(press any non-numeric at the end of sequence):\n");
    int p;
    int flag = 0;
    while (scanf("%d", &p))
    {
        for (j = 0; j < m; j++)
        {
            if (avail[j] < need[p][j])
            {
                flag = 1;
                break;
            }
        }
        if (flag == 1)
        {
            printf("NOT SAFE!\n");
            exit(0);
        }
        for (j = 0; j < m; j++)
        {
            avail[j] += AllocTable[p][j];
        }
    }
    printf("SAFE!\n");

    return 0;
}