#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n;
    scanf("%d", &n);

    // Allocate
    int **a = (int **)calloc(n, sizeof(int *)), **b = (int **)calloc(n, sizeof(int *));
    for (int i = 0; i < n; ++i)
    {
        *(a + i) = (int *)calloc(n, sizeof(int));
        *(b + i) = (int *)calloc(n, sizeof(int));
    }

    // Input
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            scanf("%d", *(a + i) + j);
        }
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            scanf("%d", *(b + i) + j);
        }
    }

    // Print sum
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            printf("%d ", *(*(a + i) + j) + *(*(b + i) + j));
        }
        printf("\n");
    }

    // Print product
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            int p = 0;
            for (int k = 0; k < n; ++k)
            {
                p += *(*(a + i) + k) * *(*(b + k) + j);
            }
            printf("%d ", p);
        }
        printf("\n");
    }

    // Free memory
    for (int i = 0; i < n; ++i)
    {
        delete[] * (a + i);
        delete[] * (b + i);
    }
    delete[] a;
    delete[] b;

    return 0;
}
