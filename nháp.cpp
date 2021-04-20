#include <bits/stdc++.h>

using namespace std;

int main()
{
    char s[1000];
    fgets(s, 1000, stdin);     // "0123456789"
    printf("%d\n", strlen(s)); // 11
    s[strlen(s) - 1] = '\0';
    printf("%d\n", strlen(s)); // 10
    strcat_s(s, 1000, "\r\n\0");
    printf("%d\n", strlen(s)); // 12
    printf("\"%s\"\n", s);
    if (strstr(s, "\r\n") != NULL)
    {
        printf("s has \\r\\n.\n");
    }
    else
    {
        printf("s has not \\r\\n.\n");
    }

    return 0;
}
