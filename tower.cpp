#include "tower.h"

#include <bits/stdc++.h>

using namespace std;
using Id = int;
using MapCode = string;
using IdSet = set<Id>;

const Id startId = 2;
const MapCode startCode = "ST";

struct Attr {
    int attack, defend, yellow, blue, red;
};

struct MonsterAttr {
    int hp, attack, defend, sp;
};

unordered_map<MapCode, MonsterAttr> monsterMp;
unordered_map<MapCode, string> nameMp;
vector<MapCode> id2MapCode;

#define N 13
static Id idMap[N][N];
static MapCode codeMap[N][N];
static Attr initAttr;
static int initHp;
static int initX, initY;

unordered_map<Id, IdSet> adjacent;

struct IdSetHash {
    size_t operator()(const IdSet &idSet) const
    {
        size_t seed = 0;
        for (const auto &value : idSet) {
            seed ^= hash<Id>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

unordered_map<IdSet, int, IdSetHash> dp;
unordered_set<IdSet, IdSetHash> inside;

static bool isWall(const MapCode &value) { return value == "a3"; }

static bool isRoad(const MapCode &value) { return value == "a0"; }

static void color(IdSet &ret, int flag[N][N], int i, int j)
{
    flag[i][j] = 1;
    static const int dx[] = {0, 0, -1, 1};
    static const int dy[] = {-1, 1, 0, 0};
    for (int k = 0; k < 4; k++) {
        int ii = i + dx[k];
        int jj = j + dy[k];
        if (ii >= 0 && ii < N && jj >= 0 && jj < N && !flag[ii][jj]) {
            MapCode &mapCode = codeMap[ii][jj];
            if (isRoad(mapCode)) {
                color(ret, flag, ii, jj);
            } else if (!isWall(mapCode)) {
                ret.insert(idMap[ii][jj]);
            }
        }
    }
}

static IdSet getAdjacent(int i, int j)
{
    IdSet ret;
    ret.clear();
    int flag[N][N] = {0};
    color(ret, flag, i, j);
    return ret;
}

static void directlyAdd(IdSet &idSet, Attr &attr, int &hp) {}

int init(const char *body)
{
    id2MapCode = {"roda", "wall", "ST"};
    nameMp = {
        {"ST", "起点"},   {"c0", "黄钥匙"}, {"c1", "蓝钥匙"}, {"c2", "红钥匙"}, {"c4", "剑"},   {"c6", "蓝宝石"},
        {"c7", "红宝石"}, {"d0", "红血瓶"}, {"d1", "蓝血瓶"}, {"h0", "黄门"},   {"h1", "蓝门"}, {"h2", "红门"},
    };
    int buffInt;
    char buff[256];
    stringstream ss(body);
    ss.getline(buff, 256);  // title
    ss.getline(buff, 256);  // $$
    ss >> initHp >> initAttr.attack >> initAttr.defend >> buffInt >> buffInt >> initAttr.yellow >> initAttr.blue >>
        initAttr.red >> buffInt >> initX >> initY;
    // cout << initHp << ' ' << initAttr.attack << ' ' << initAttr.defend << ' ' << initAttr.yellow << ' ' <<
    // initAttr.blue << ' ' << initAttr.red << endl;
    ss.getline(buff, 256);  // \n
    ss.getline(buff, 256);  // $$
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            ss >> codeMap[i][j];
        }
    }
    // for (int i = 0; i < N; i++)
    // {
    //     for (int j = 0; j < N; j++)
    //     {
    //         cout << gameMap[i][j] << ' ';
    //     }
    //     cout << endl;
    // }

    ss.getline(buff, 256);  // \n
    ss.getline(buff, 256);  // #
    ss.getline(buff, 256);  // $$
    while (strncmp(buff, "#", 1) != 0) {
        ss.getline(buff, 256);
        if (strncmp(buff, "mapcode=", 8) == 0) {
            string mapCode = buff + 8;
            while (*mapCode.rbegin() == '\r') mapCode.pop_back();
            MonsterAttr attr;
            ss.getline(buff, 256);  // prefix=
            ss.getline(buff, 256);  // name=
            string name = buff + 5;
            while (*name.rbegin() == '\r') name.pop_back();
            ss.getline(buff, 256);  // bfEvents=
            ss.getline(buff, 256);  // afEvents=
            ss.getline(buff, 256);  // hp=
            attr.hp = atoi(buff + 3);
            ss.getline(buff, 256);  // attack=
            attr.attack = atoi(buff + 7);
            ss.getline(buff, 256);  // defend=
            attr.defend = atoi(buff + 7);
            ss.getline(buff, 256);  // coins=
            ss.getline(buff, 256);  // exp=
            ss.getline(buff, 256);  // sp=
            attr.sp = atoi(buff + 3);
            // cout << mapCode << ' ' << attr.name << ' ' << attr.hp << ' ' << attr.attack << ' ' << attr.defend << ' '
            // << attr.sp << endl;
            nameMp[mapCode] = move(name);
            monsterMp.emplace(move(mapCode), move(attr));
        }
    }

    int top = 3;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            MapCode &code = codeMap[i][j];
            if (isRoad(code)) {
                idMap[i][j] = 0;
            } else if (isWall(code)) {
                idMap[i][j] = 1;
            } else {
                idMap[i][j] = top++;
                id2MapCode.push_back(codeMap[i][j]);
            }
        }
    }
    idMap[initX][initY] = startId;
    codeMap[initX][initY] = startCode;
    // for (int i = 0; i < N; i++)
    // {
    //     for (int j = 0; j < N; j++)
    //     {
    //         cout << idMap[i][j] << ' ';
    //     }
    //     cout << endl;
    // }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            MapCode &mapCode = codeMap[i][j];
            Id &id = idMap[i][j];
            if (!isWall(mapCode) && !isRoad(mapCode)) {
                adjacent[id] = getAdjacent(i, j);
                // cout << nameMp[mapCode] << ":";
                // for (const auto &i : adjacent[id])
                // {
                //     cout << nameMp[id2MapCode[i]] << " ";
                // }
                // cout << endl;
            }
        }
    }

    return 0;
}

// If NULL is passed in, it is directly output in std::out, otherwise it is output in buffer.
int spfa(char *buffer)
{
    // string str;
    // stringstream ss(str);

    IdSet initSet = {startId};
    directlyAdd(initSet, initAttr, initHp);
    queue<pair<IdSet, Attr>> queue;
    queue.emplace(initSet, initAttr);
    dp[initSet] = initHp;
    inside.insert(initSet);
    // cout << ss.str();
    // if (buffer)
    //     strcpy(buffer, ss.str().c_str());
    return 0;
}