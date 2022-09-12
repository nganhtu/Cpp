#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    const char *s[] = {"tom", "jerry", "spike"};

    const char **ptr = s;
    printf("%d\n", ptr);
    printf("%s\n", *ptr + strlen(*ptr) + 1);

    printf("%d\n%d\n%d\n", &s[0], &s[1], &s[2]);
    printf("%d\n", s + 1);

    printf("%d\n", 'c');

    char res[] = "";
    strcat(res, "uwu");

    string str = "";

    str += "dsafsdf";
    str += to_string(12345);
    cout << str;

    return 0;
}
