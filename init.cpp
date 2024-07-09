
#include <bits/stdc++.h>

#include "tower.h"

using namespace std;

int main()
{
    freopen("init.txt", "w", stdout);
    char buff[65536];
    FILE *file = fopen("input.txt", "r");
    fread(buff, 1, 65536, file);
    fclose(file);
    char *p = strstr(buff, "body");
    if (p == 0)
    {
        cout << "no body" << endl;
        return 0;
    }
    p += 7;
    const char *start = p;
    while (p && *p && *p != '"')
        p++;
    *p = '\0';
    init(start);
    return 0;
}