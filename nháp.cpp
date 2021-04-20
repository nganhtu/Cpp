#include <bits/stdc++.h>

using namespace std;

int main()
{
    char s[1000];
    fgets(s, 1000, stdin);

    printf("%d\n", strlen(s));
    s[strlen(s) - 1] = '\0';
    printf("%d\n", strlen(s));
    strcat_s(s, 1000, "\r\n\0");
    printf("%d\n", strlen(s));
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
