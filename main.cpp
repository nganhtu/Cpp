#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 10002

inline bool isNum(char c)
{
    return (c >= '0' && c <= '9');
}

int main()
{
    int t;
    scanf("%d", &t);
    while (t--)
    {
        char s[MAX_LEN];
        fflush(stdin);
        scanf("%s", s);
        int i = 0, cntNum = 0;
        for (i; i < strlen(s); ++i)
        {
            if ((isNum(s[i]) && (!isNum(s[i + 1]) || i == strlen(s) - 1)))
            {
                cntNum++;
            }
        }

        int *arr = (int *)calloc(cntNum, sizeof(int));
        int it = 0;
        char strNum[MAX_LEN];
        strcpy(strNum, "");
        for (i = 0; i < strlen(s); ++i)
        {
            if (isNum(s[i]))
            {
                strncat(strNum, &s[i], 1);
                if (!isNum(s[i + 1]) || i == strlen(s) - 1)
                {
                    arr[it] = atoi(strNum);
                    strcpy(strNum, "");
                    it++;
                }
            }
        }

        int j;
        for (i = 0; i < cntNum - 1; ++i)
        {
            for (j = i + 1; j < cntNum; ++j)
            {
                if (arr[i] > arr[j])
                {
                    int tmp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = tmp;
                }
            }
        }

        for (i = 0; i < cntNum; ++i)
        {
            printf("%d ", arr[i]);
        }
        printf("\n");

        delete[] arr;
    }

    return 0;
}
