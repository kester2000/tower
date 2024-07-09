
#include <bits/stdc++.h>

#include "tower.h"

using namespace std;

int main(int argc, char **argv)
{
    freopen("init.txt", "r", stdin);
    readInit();
    auto ret = spfa();
    freopen("idlist.txt", "w", stdout);
    cout << ret.size() << endl;
    for (const auto &id : ret)
    {
        cout << id << ' ';
    }
    return 0;
}