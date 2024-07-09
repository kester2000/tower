
#include <bits/stdc++.h>

#include "tower.h"

using namespace std;

int main(int argc, char **argv)
{
    freopen("init.txt", "r", stdin);
    readInit();
    freopen("idlist.txt", "r", stdin);
    int len, tmp;
    cin >> len;
    vector<Id> v;
    for (int i = 0; i < len; i++)
    {
        cin >> tmp;
        v.emplace_back(tmp);
    }
    cout << v.size() << endl;
    auto p = makePath(v);
    printPath(p);
    return 0;
}