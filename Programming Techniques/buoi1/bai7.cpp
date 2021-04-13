#include <stdio.h>
#include <stdlib.h>

int *a;
int n, tmp;

int main()
{
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    a = (int *)calloc(n, sizeof(int));

    for (int i = 0; i < n; i++)
        scanf("%d", a + i);

    printf("The input array is: \n");
    for (int i = 0; i < n; i++)
        printf("%d ", *(a + i));
    printf("\n");

    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (*(a + i) > *(a + j))
            {
                tmp = *(a + j);
                *(a + j) = *(a + i);
                *(a + i) = tmp;
            }
        }
    }

    printf("The sorted array is: \n");
    for (int i = 0; i < n; i++)
        printf("%d ", *(a + i));
    printf("\n");

    delete[] a;
    return 0;
}
