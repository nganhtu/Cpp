#include <bits/stdc++.h>

using namespace std;

int main()
{
    char s[1000];
    fgets(s, 1000, stdin);
    s[strlen(s)] = '\0';
    strcat_s(s, 1000, "\r\n\0");
    printf("\"%s\"", s);
    printf("%d\n", strlen(s));
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
