#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n;
    scanf("%d", &n);
    int *ptr = (int *)calloc(n, sizeof(int));
    for (int i = 0; i < n; ++i)
    {
        scanf("%d", ptr + i);
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = i; j < n; ++j)
        {
            for (int k = i; k <= j; ++k)
            {
                printf("%d ", *(ptr + k));
            }
            printf("\n");
        }
    }

    delete[] ptr;

    return 0;
}
