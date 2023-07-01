#include "tower.h"

#include <bits/stdc++.h>

#define QUEUE_LOG

#ifdef QUEUE_LOG
#define LOG_INTERVAl 1000
static int queue_cnt = 0;
#endif

using namespace std;
using Id = int;
using MapCode = string;
using IdSet = set<Id>;
using State = pair<IdSet, int>;

const Id startId = 2;
const MapCode startCode = "ST";
const string bossName = "BOSS！";

struct Attr {
    int attack, defend, yellow, blue, red;
};

struct MonsterAttr {
    int hp, attack, defend, sp;
};

unordered_map<MapCode, string> nameMp = {
    {"ST", "起点"},   {"c0", "黄钥匙"}, {"c1", "蓝钥匙"}, {"c2", "红钥匙"}, {"c4", "剑"},
    {"c6", "蓝宝石"}, {"c7", "红宝石"}, {"d0", "红血瓶"}, {"d1", "蓝血瓶"}, {"h0", "黄门"},
    {"h1", "蓝门"},   {"h2", "红门"},   {"r3", "公主"},
};

unordered_map<MapCode, Attr> attrMp = {
    {"c0", {0, 0, 1, 0, 0}},  {"c1", {0, 0, 0, 1, 0}},  {"c2", {0, 0, 0, 0, 1}},
    {"c4", {10, 0, 0, 0, 0}}, {"c6", {0, 3, 0, 0, 0}},  {"c7", {3, 0, 0, 0, 0}},
    {"h0", {0, 0, -1, 0, 0}}, {"h1", {0, 0, 0, -1, 0}}, {"h2", {0, 0, 0, 0, -1}},
};

unordered_map<MapCode, int> hpMp = {{"d0", 75}, {"d1", 200}, {"r3", 500}};

unordered_map<MapCode, MonsterAttr> monsterMp;

vector<MapCode> id2MapCode = {"road", "wall", "ST"};

#define N 13
static Id idMap[N][N];
static MapCode codeMap[N][N];
static Attr initAttr;
static int initHp;
static int initX, initY;

unordered_map<Id, IdSet> adjacent;

struct StatetHash {
    size_t operator()(const State &state) const
    {
        size_t seed = 0;
        for (const auto &value : state.first) {
            seed ^= hash<Id>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        seed ^= hash<Id>{}(state.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

unordered_map<State, int, StatetHash> dp;
unordered_map<State, vector<Id>, StatetHash> path;
unordered_set<State, StatetHash> inside;

static int bestHp = 0;
static State bestState;

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

static bool touch(Id id, State &state, Attr &attr, int &hp)
{
    const MapCode &mapCode = id2MapCode[id];
    if (hpMp.count(mapCode)) {
        hp += hpMp[mapCode];
    } else if (attrMp.count(mapCode)) {
        const Attr &other = attrMp[mapCode];
        attr.attack += other.attack;
        attr.defend += other.defend;
        attr.yellow += other.yellow;
        attr.blue += other.blue;
        attr.red += other.red;
    } else {
        MonsterAttr monster = monsterMp[mapCode];
        if (monster.sp == 21) {
            for (const auto &u : state.first) {
                if (!(u == startId || hpMp.count(id2MapCode[u]) || attrMp.count(id2MapCode[u]))) {
                    monster.attack++;
                    monster.defend++;
                }
            }
        }
        int heroAttack = attr.attack - attr.attack * state.second / 100;
        int heroDefend = attr.defend - attr.defend * state.second / 100;
        int heroDamage = heroAttack - monster.defend;
        if (heroDamage <= 0) return false;
        int monsterDamage;
        if (monster.sp == 2) {
            monsterDamage = monster.attack;
        } else if (monster.sp == 3) {
            heroDamage = 1;
        } else if (monster.sp == 4) {
            monsterDamage = (monster.attack - heroDefend) * 2;
        } else if (monster.sp / 10000 == 1) {
            monsterDamage = monster.attack + heroDefend * ((monster.sp % 10000) / 100.0) - heroDefend;
        } else {
            monsterDamage = monster.attack - heroDefend;
        }
        if (monsterDamage < 0) monsterDamage = 0;
        hp -= ((monster.hp - 1) / heroDamage) * monsterDamage;
        if (monster.sp / 10000 == 4) {
            state.second = monster.sp % 10000;
        } else {
            state.second = 0;
        }
    }
    if (hp <= 0 || attr.yellow < 0 || attr.blue < 0 || attr.red < 0) {
        return false;
    }
    state.first.insert(id);
    return true;
}

static vector<Id> add(State &state, Attr &attr, int &hp)
{
    vector<Id> ret;
    // need to remove some map codes in some maps
    // unordered_set<MapCode> addSet = {"c0", "c1", "c2", "c4", "c6", "c7", "d0", "d1"};
    unordered_set<MapCode> addSet = {"c0", "c1", "c2", "c4", "c7", "d0", "d1"};
    queue<Id> nxtQueue;
    for (const auto &u : state.first) {
        for (const auto &v : adjacent[u]) {
            nxtQueue.push(v);
        }
    }
    while (!nxtQueue.empty()) {
        Id &id = nxtQueue.front();
        nxtQueue.pop();
        if (state.first.count(id)) {
            continue;
        } else if (addSet.count(id2MapCode[id])) {
            bool flag = touch(id, state, attr, hp);
            assert(flag);
            ret.push_back(id);
            for (const auto &v : adjacent[id]) {
                nxtQueue.push(v);
            }
        }
    }
    return ret;
}

int init(const char *body)
{
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
            cout << mapCode << ' ' << name << ' ' << attr.hp << ' ' << attr.attack << ' ' << attr.defend << ' '
                 << attr.sp << endl;
            nameMp[mapCode] = move(name);
            monsterMp[mapCode] = move(attr);
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
                id2MapCode.emplace_back(codeMap[i][j]);
            }
        }
    }
    idMap[initX][initY] = startId;
    codeMap[initX][initY] = startCode;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << codeMap[i][j] << ' ';
        }
        cout << endl;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << idMap[i][j] << '\t';
        }
        cout << endl;
    }

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
    queue<pair<State, Attr>> queue;
    State state = {{startId}, 0};
    Attr attr = initAttr;
    int hp = initHp;
    vector<Id> addRet = add(state, attr, hp);
    dp[state] = hp;
    path[state].clear();
    for (auto &addId : addRet) {
        path[state].emplace_back(addId);
    }
    inside.insert(state);
    queue.emplace(move(state), move(attr));

    while (!queue.empty()) {
#ifdef QUEUE_LOG
        if (queue_cnt % LOG_INTERVAl == 0) {
            cout << "queue_cnt: " << queue_cnt << " queue.size(): " << queue.size() << endl;
        }
        ++queue_cnt;
#endif
        auto t = queue.front();
        queue.pop();
        inside.erase(t.first);
        IdSet nxtSet;
        for (const auto &u : t.first.first) {
            for (const auto &v : adjacent[u]) {
                if (!t.first.first.count(v)) {
                    nxtSet.insert(v);
                }
            }
        }
        for (const auto &id : nxtSet) {
            State state = t.first;
            Attr attr = t.second;
            int hp = dp[state];
            vector<Id> addRet;
            if (touch(id, state, attr, hp)) {
                if (nameMp[id2MapCode[id]] != bossName) {
                    addRet = add(state, attr, hp);
                }
                if (hp > dp[state]) {
                    dp[state] = hp;
                    path[state] = path[t.first];
                    path[state].emplace_back(id);
                    for (auto &addId : addRet) {
                        path[state].emplace_back(addId);
                    }
                    if (nameMp[id2MapCode[id]] != bossName) {
                        if (!inside.count(state)) {
                            inside.insert(state);
                            queue.emplace(move(state), move(attr));
                        }
                    } else {
                        if (bestHp < hp) {
                            bestHp = hp;
                            bestState = state;
                            cout << bestHp << endl;
                            for (const auto &id : path[bestState]) {
                                cout << id << ' ';
                            }
                            cout << endl;
                        }
                    }
                }
            }
        }
    }

    state = {{startId}, 0};
    attr = initAttr;
    hp = initHp;
    cout << hp << ' ' << attr.attack << ' ' << attr.defend << ' ' << attr.yellow << ' ' << attr.blue << ' ' << attr.red
         << endl;
    for (const auto &id : path[bestState]) {
        touch(id, state, attr, hp);
        cout << id << endl;
        cout << hp << ' ' << attr.attack << ' ' << attr.defend << ' ' << attr.yellow << ' ' << attr.blue << ' '
             << attr.red << endl;
    }
    cout << endl;

    // cout << ss.str();
    // if (buffer)
    //     strcpy(buffer, ss.str().c_str());
    return 0;
}