#include <bits/stdc++.h>

#define QUEUE_LOG

#ifdef QUEUE_LOG
#define LOG_INTERVAl 10000
static int queue_cnt = 0;
static int q_cnt = 0;
#endif

using namespace std;
using Id = int;
using MapCode = string;
using IdSet = set<Id>;
using State = pair<IdSet, int>;

const Id startId = 2;
const MapCode startCode = "ST";
const string bossName = "BOSS！";

Id top = 3;

struct Attr
{
    int attack, defend, yellow, blue, red;
};

struct MonsterAttr
{
    int hp, attack, defend, sp;
};

struct PathNode
{
    State state;
    Attr attr;
    int hp;
    int x, y;
};

unordered_map<MapCode, string> nameMp = {
    {"ST", "起点"},
    {"c0", "黄钥匙"},
    {"c1", "蓝钥匙"},
    {"c2", "红钥匙"},
    {"c4", "剑"},
    {"c5", "盾"},
    {"c6", "蓝宝石"},
    {"c7", "红宝石"},
    {"d0", "红血瓶"},
    {"d1", "蓝血瓶"},
    {"h0", "黄门"},
    {"h1", "蓝门"},
    {"h2", "红门"},
    {"r3", "公主"},
};

unordered_map<MapCode, Attr> attrMp = {
    {"c0", {0, 0, 1, 0, 0}},
    {"c1", {0, 0, 0, 1, 0}},
    {"c2", {0, 0, 0, 0, 1}},
    {"c4", {10, 0, 0, 0, 0}},
    {"c5", {0, 10, 0, 0, 0}},
    {"c6", {0, 3, 0, 0, 0}},
    {"c7", {3, 0, 0, 0, 0}},
    {"h0", {0, 0, -1, 0, 0}},
    {"h1", {0, 0, 0, -1, 0}},
    {"h2", {0, 0, 0, 0, -1}},
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
static const int dx[] = {0, 0, -1, 1};
static const int dy[] = {-1, 1, 0, 0};

unordered_set<MapCode> addSet = {"c0", "c1", "c2", "d0", "d1", "r3"};
bool addAtk = true, addDef = true;

unordered_map<Id, IdSet> adjacent;

struct StatetHash
{
    size_t operator()(const State &state) const
    {
        size_t seed = 0;
        for (const auto &value : state.first)
        {
            seed ^= hash<Id>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        seed ^= hash<Id>{}(state.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

struct PathNodeHash
{
    size_t operator()(const PathNode &n) const
    {
        size_t seed = 0;
        seed ^= StatetHash{}(n.state) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<int>{}(n.attr.attack) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<int>{}(n.attr.defend) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<int>{}(n.attr.yellow) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<int>{}(n.attr.blue) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<int>{}(n.attr.red) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<int>{}(n.hp) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<int>{}(n.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<int>{}(n.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

struct PathNodeEq
{
    size_t operator()(const PathNode &a, const PathNode &b) const
    {
        return a.x == b.x && a.y == b.y && a.hp == b.hp && a.attr.attack == b.attr.attack &&
               a.attr.defend == b.attr.defend && a.attr.yellow == b.attr.yellow && a.attr.blue == b.attr.blue &&
               a.attr.red == b.attr.red && a.state == b.state;
    }
};

unordered_map<State, int, StatetHash> dp;
unordered_map<State, vector<Id>, StatetHash> path;
unordered_set<State, StatetHash> inside;

unordered_map<PathNode, std::pair<void *, int>, PathNodeHash, PathNodeEq> pathDp;

static int bestHp = 0;
static State bestState;

static bool isWall(const MapCode &value) { return value == "a3"; }

static bool isRoad(const MapCode &value) { return value == "a0"; }

static void color(IdSet &ret, int flag[N][N], int i, int j)
{
    flag[i][j] = 1;
    for (int k = 0; k < 4; k++)
    {
        int ii = i + dx[k];
        int jj = j + dy[k];
        if (ii >= 0 && ii < N && jj >= 0 && jj < N && !flag[ii][jj])
        {
            MapCode &mapCode = codeMap[ii][jj];
            if (isRoad(mapCode))
            {
                color(ret, flag, ii, jj);
            }
            else if (!isWall(mapCode))
            {
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
    if (hpMp.count(mapCode))
    {
        hp += hpMp[mapCode];
    }
    else if (attrMp.count(mapCode))
    {
        const Attr &other = attrMp[mapCode];
        attr.attack += other.attack;
        attr.defend += other.defend;
        attr.yellow += other.yellow;
        attr.blue += other.blue;
        attr.red += other.red;
    }
    else
    {
        MonsterAttr monster = monsterMp[mapCode];
        // 仇恨
        if (monster.sp == 21)
        {
            for (const auto &u : state.first)
            {
                if (!(u == startId || hpMp.count(id2MapCode[u]) || attrMp.count(id2MapCode[u])))
                {
                    monster.attack++;
                    monster.defend++;
                }
            }
        }
        // 模仿
        if (monster.sp == 10)
        {
            monster.attack = attr.attack;
            monster.defend = attr.defend;
        }
        else if (monster.sp == 11)
        {
            monster.attack = attr.attack;
        }
        else if (monster.sp == 12)
        {
            monster.defend = attr.defend;
        }
        int heroAttack = attr.attack - attr.attack * state.second / 100;
        int heroDefend = attr.defend - attr.defend * state.second / 100;
        int heroDamage = heroAttack - monster.defend;
        if (heroDamage <= 0)
            return false;
        int monsterDamage;
        if (monster.sp == 2)
        {
            // 魔攻
            monsterDamage = monster.attack;
        }
        else if (monster.sp == 3)
        {
            // 坚固
            heroDamage = 1;
            monsterDamage = monster.attack - heroDefend;
        }
        else if (monster.sp == 4)
        {
            // 2连击
            monsterDamage = (monster.attack - heroDefend) * 2;
        }
        else if (monster.sp == 5)
        {
            // 3连击
            monsterDamage = (monster.attack - heroDefend) * 3;
        }
        else if (monster.sp / 10000 == 1)
        {
            // 破甲
            monsterDamage = monster.attack + heroDefend * (monster.sp % 10000) / 100 - heroDefend;
        }
        else if (monster.sp / 10000 == 2)
        {
            // 反击
            monsterDamage = monster.attack + heroAttack * (monster.sp % 10000) / 100 - heroDefend;
        }
        else
        {
            monsterDamage = monster.attack - heroDefend;
        }
        if (monsterDamage < 0)
            monsterDamage = 0;
        hp -= ((monster.hp - 1) / heroDamage) * monsterDamage;
        if (monster.sp / 10000 == 4)
        {
            // 衰弱
            state.second = monster.sp % 10000;
        }
        else
        {
            state.second = 0;
        }
    }
    if (hp <= 0 || attr.yellow < 0 || attr.blue < 0 || attr.red < 0)
    {
        return false;
    }
    state.first.insert(id);
    return true;
}

static vector<Id> add(State &state, Attr &attr, int &hp)
{
    vector<Id> ret;
    std::queue<Id> nxtQueue;
    for (const auto &u : state.first)
    {
        for (const auto &v : adjacent[u])
        {
            nxtQueue.push(v);
        }
    }
    while (!nxtQueue.empty())
    {
        Id &id = nxtQueue.front();
        nxtQueue.pop();
        if (state.first.count(id))
        {
            continue;
        }
        else if (addSet.count(id2MapCode[id]))
        {
            bool flag = touch(id, state, attr, hp);
            assert(flag);
            ret.push_back(id);
            for (const auto &v : adjacent[id])
            {
                nxtQueue.push(v);
            }
        }
    }
    return ret;
}

static void nextline(const char **pp)
{
    while (strncmp(*pp, "\\n", 2))
    {
        *pp += 1;
    }
    *pp += 2;
}

static int readint(const char **pp)
{
    int r = 0;
    while (!(**pp >= '0' && **pp <= '9'))
        *pp += 1;
    while (**pp >= '0' && **pp <= '9')
    {
        r = r * 10 + **pp - '0';
        *pp += 1;
    }
    return r;
}

static MapCode readMapCode(const char **pp)
{
    MapCode r(*pp, *pp + 2);
    *pp += 3;
    return r;
}

int init(const char *body)
{
    int buffInt;
    const char *tmp;
    nextline(&body); // title
    nextline(&body); // $$
    initHp = readint(&body);
    initAttr.attack = readint(&body);
    initAttr.defend = readint(&body);
    buffInt = readint(&body);
    buffInt = readint(&body);
    initAttr.yellow = readint(&body);
    initAttr.blue = readint(&body);
    initAttr.red = readint(&body);
    buffInt = readint(&body);
    initX = readint(&body);
    initY = readint(&body);
    cout << initHp << ' ' << initAttr.attack << ' ' << initAttr.defend << ' ' << initAttr.yellow << ' ' << initAttr.blue
         << ' ' << initAttr.red << endl;
    nextline(&body); // \r\n
    nextline(&body); // $$
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            codeMap[i][j] = readMapCode(&body);
        }
        nextline(&body); // r\n
    }

    nextline(&body); // #
    nextline(&body); // $$
    while (*body != '#')
    {
        MonsterAttr attr;
        MapCode mapCode(body + 13, body + 15);
        nextline(&body);
        nextline(&body);
        tmp = body;
        nextline(&body);
        string name(tmp + 10, body - 4);
        name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
        nextline(&body);
        nextline(&body);
        body += 8;
        attr.hp = readint(&body);
        nextline(&body);
        body += 12;
        attr.attack = readint(&body);
        nextline(&body);
        body += 12;
        attr.defend = readint(&body);
        nextline(&body);
        nextline(&body);
        nextline(&body);
        body += 8;
        attr.sp = readint(&body);
        nextline(&body);
        nextline(&body);
        cout << mapCode << ' ' << name << ' ' << attr.hp << ' ' << attr.attack << ' ' << attr.defend << ' ' << attr.sp
             << endl;
        if (attr.sp == 10 || attr.sp == 12 || (attr.sp / 10000 == 1 && attr.sp % 10000 > 100))
        {
            addDef = false;
        }
        if (attr.sp == 10 || attr.sp == 11 || attr.sp / 10000 == 2)
        {
            addAtk = false;
        }
        nameMp[mapCode] = move(name);
        monsterMp[mapCode] = move(attr);
    }

    if (addAtk)
    {
        addSet.insert("c4");
        addSet.insert("c7");
    }
    if (addDef)
    {
        addSet.insert("c5");
        addSet.insert("c6");
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            MapCode &code = codeMap[i][j];
            if (isRoad(code))
            {
                idMap[i][j] = 0;
            }
            else if (isWall(code))
            {
                idMap[i][j] = 1;
            }
            else
            {
                idMap[i][j] = top++;
                id2MapCode.emplace_back(codeMap[i][j]);
            }
        }
    }
    idMap[initX][initY] = startId;
    codeMap[initX][initY] = startCode;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << codeMap[i][j] << ' ';
        }
        cout << endl;
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << idMap[i][j] << '\t';
        }
        cout << endl;
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            MapCode &mapCode = codeMap[i][j];
            Id &id = idMap[i][j];
            if (!isWall(mapCode) && !isRoad(mapCode))
            {
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

void readInit()
{
    cin >> initHp >> initAttr.attack >> initAttr.defend >> initAttr.yellow >> initAttr.blue >> initAttr.red;
    cout << initHp << ' ' << initAttr.attack << ' ' << initAttr.defend << ' ' << initAttr.yellow << ' ' << initAttr.blue
         << ' ' << initAttr.red << endl;
    MonsterAttr attr;
    MapCode mapCode;
    string name;
    while (name != bossName)
    {
        cin >> mapCode >> name >> attr.hp >> attr.attack >> attr.defend >> attr.sp;
        if (attr.sp == 10 || attr.sp == 12 || (attr.sp / 10000 == 1 && attr.sp % 10000 > 100))
        {
            addDef = false;
        }
        if (attr.sp == 10 || attr.sp == 11 || attr.sp / 10000 == 2)
        {
            addAtk = false;
        }
        nameMp[mapCode] = name;
        monsterMp[mapCode] = attr;
        cout << mapCode << ' ' << name << ' ' << attr.hp << ' ' << attr.attack << ' ' << attr.defend << ' ' << attr.sp
             << endl;
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> codeMap[i][j];
            cout << codeMap[i][j] << ' ';
        }
        cout << endl;
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> idMap[i][j];
            cout << idMap[i][j] << '\t';
            if (top < idMap[i][j] + 1)
            {
                top = idMap[i][j] + 1;
            }
            if (idMap[i][j] == startId)
            {
                initX = i;
                initY = j;
            }
        }
        cout << endl;
    }

    id2MapCode.resize(top);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            id2MapCode[idMap[i][j]] = codeMap[i][j];
        }
    }

    if (addAtk)
    {
        addSet.insert("c4");
        addSet.insert("c7");
    }
    if (addDef)
    {
        addSet.insert("c5");
        addSet.insert("c6");
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            MapCode &mapCode = codeMap[i][j];
            Id &id = idMap[i][j];
            if (!isWall(mapCode) && !isRoad(mapCode))
            {
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
}

int getI(int id)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (idMap[i][j] == id)
            {
                return i;
            }
        }
    }
    return 0;
}

int getJ(int id)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (idMap[i][j] == id)
            {
                return j;
            }
        }
    }
    return 0;
}

void clear()
{
    // dp.clear();
    // path.clear();
    // inside.clear();
}

vector<Id> spfa()
{
    // string str;
    // stringstream ss(str);
    State state = {{startId}, 0};
    Attr attr = initAttr;
    int hp = initHp;
    vector<Id> addRet = add(state, attr, hp);
    dp[state] = hp;
    path[state].clear();
    for (auto &addId : addRet)
    {
        path[state].emplace_back(addId);
    }
    inside.insert(state);
    std::queue<pair<State, Attr>> queue;
    queue.emplace(move(state), move(attr));

    while (!queue.empty())
    {
#ifdef QUEUE_LOG
        if (queue_cnt % LOG_INTERVAl == 0)
        {
            cout << "queue_cnt: " << queue_cnt << " queue.size(): " << queue.size() << endl;
        }
        ++queue_cnt;
#endif
        auto t = queue.front();
        queue.pop();
        inside.erase(t.first);
        IdSet nxtSet;
        for (const auto &u : t.first.first)
        {
            for (const auto &v : adjacent[u])
            {
                if (!t.first.first.count(v))
                {
                    nxtSet.insert(v);
                }
            }
        }
        for (const auto &id : nxtSet)
        {
            State state = t.first;
            Attr attr = t.second;
            int hp = dp[state];
            vector<Id> addRet;
            if (touch(id, state, attr, hp))
            {
                if (nameMp[id2MapCode[id]] != bossName)
                {
                    addRet = add(state, attr, hp);
                }
                if (hp > dp[state])
                {
                    dp[state] = hp;
                    path[state] = path[t.first];
                    path[state].emplace_back(id);
                    for (auto &addId : addRet)
                    {
                        path[state].emplace_back(addId);
                    }
                    if (nameMp[id2MapCode[id]] != bossName)
                    {
                        if (!inside.count(state))
                        {
                            inside.insert(state);
                            queue.emplace(move(state), move(attr));
                        }
                    }
                    else
                    {
                        if (bestHp < hp)
                        {
                            ofstream out;
                            out.open("idlist.txt", ios::out);
                            bestHp = hp;
                            bestState = state;
                            cout << bestHp << endl;
                            out << path[bestState].size() << endl;
                            for (const auto &id : path[bestState])
                            {
                                cout << id << ' ';
                                out << id << ' ';
                            }
                            cout << endl;
                        }
                    }
                }
            }
        }
    }

    auto ret = path[bestState];
    clear();
    return ret;
}

void print(vector<Id> idList)
{
    State state = {{startId}, 0};
    Attr attr = initAttr;
    int hp = initHp;
    cout << hp << ' ' << attr.attack << ' ' << attr.defend << ' ' << attr.yellow << ' ' << attr.blue << ' ' << attr.red
         << endl;
    for (const auto &id : idList)
    {
        touch(id, state, attr, hp);
        cout << nameMp[id2MapCode[id]] << ' ' << id << ' ' << getI(id) << ' ' << getJ(id) << endl;
        cout << hp << ' ' << attr.attack << ' ' << attr.defend << ' ' << attr.yellow << ' ' << attr.blue << ' '
             << attr.red << endl;
    }
    cout << endl;
}

vector<int> getPath(PathNode endNode)
{
    vector<int> rev;
    std::pair<void *, int> pp = pathDp[endNode];
    while (pp.first)
    {
        rev.emplace_back(pp.second);
        pp = *(std::pair<void *, int> *)pp.first;
    }
    vector<int> ret;
    for (int i = rev.size() - 1; i >= 0; i--)
    {
        ret.emplace_back(rev[i]);
    }
    return ret;
}

vector<int> makePath(vector<Id> idList)
{
    unordered_map<Id, int> demage;
    State state = {{startId}, 0};
    Attr attr = initAttr;
    int hp = initHp;
    int lastHp;
    for (const auto &id : idList)
    {
        lastHp = hp;
        touch(id, state, attr, hp);
        if (monsterMp.count(id2MapCode[id]))
        {
            demage[id] = lastHp - hp;
            cout << id << ' ' << demage[id] << endl;
        }
    }
    PathNode s;
    s.state = {{startId}, 0};
    s.attr = initAttr;
    s.hp = initHp;
    s.x = initX;
    s.y = initY;
    pathDp[s] = {0, 0};
    std::queue<PathNode> pathQ;
    pathQ.push(s);
    while (!pathQ.empty())
    {
#ifdef QUEUE_LOG
        if (q_cnt % LOG_INTERVAl == 0)
        {
            cout << "q_cnt: " << q_cnt << " pathQ.size(): " << pathQ.size() << endl;
        }
        ++q_cnt;
#endif
        auto t = pathQ.front();
        pathQ.pop();
        // cout << t.hp << ' ' << t.x << ' ' << t.y << endl;
        for (int k = 0; k < 4; k++)
        {
            auto tt = t;
            tt.x += dx[k];
            tt.y += dy[k];
            // cout << tt.hp << ' ' << tt.x << ' ' << tt.y << ' ' << k << endl;
            if (tt.x < 0 || tt.x >= N || tt.y < 0 || tt.y >= N)
                continue;
            MapCode &mapCode = codeMap[tt.x][tt.y];
            Id &id = idMap[tt.x][tt.y];
            // cout << mapCode << ' ' << id << endl;
            if (isWall(mapCode))
            {
                // cout << mapCode << ' ' << id << " Wall" << endl;
                continue;
            }
            else if (!isRoad(mapCode) && !t.state.first.count(id))
            {
                // cout << mapCode << ' ' << id << " Touch" << endl;
                tt = t;
                int lastHp = tt.hp;
                // cout << tt.hp << ' ' << tt.attr.attack << ' ' << tt.attr.defend << ' ' << tt.attr.yellow << ' '
                //      << tt.attr.blue << ' ' << tt.attr.red << endl;
                bool b = touch(id, tt.state, tt.attr, tt.hp);
                // cout << tt.hp << ' ' << tt.attr.attack << ' ' << tt.attr.defend << ' ' << tt.attr.yellow << ' '
                //      << tt.attr.blue << ' ' << tt.attr.red << endl;
                if (!b)
                    continue;
                // cout << mapCode << ' ' << id << " TouchEnd" << endl;
                if (lastHp > tt.hp && !demage.count(id))
                    continue;
                if (demage.count(id) && lastHp - tt.hp != demage[id])
                    continue;
                // cout << mapCode << ' ' << id << " demageEnd" << endl;
            }
            if (!pathDp.count(tt))
            {
                pathDp[tt] = {&pathDp[t], k};
                pathQ.push(tt);
                // cout << mapCode << ' ' << id << " Add" << endl;
                if (nameMp[mapCode] == bossName)
                {
                    return getPath(tt);
                }
            }
            // cout << "pathQ.size() " << pathQ.size() << endl;
        }
    }
    // cout << "pathQ.size() " << pathQ.size() << endl;
    return {};
}

void printPath(vector<int> l)
{
    const char *fx = "adws";
    PathNode s;
    s.state = {{startId}, 0};
    s.attr = initAttr;
    s.hp = initHp;
    s.x = initX;
    s.y = initY;
    cout << s.hp << ' ' << s.attr.attack << ' ' << s.attr.defend << ' ' << s.attr.yellow << ' ' << s.attr.blue << ' '
         << s.attr.red << endl;
    for (int k : l)
    {
        cout << fx[k];
        auto tt = s;
        tt.x += dx[k];
        tt.y += dy[k];
        MapCode &mapCode = codeMap[tt.x][tt.y];
        Id &id = idMap[tt.x][tt.y];
        if (!isRoad(mapCode) && !tt.state.first.count(id))
        {
            cout << endl;
            tt = s;
            touch(id, tt.state, tt.attr, tt.hp);
            cout << nameMp[mapCode] << ' ' << tt.hp << ' ' << tt.attr.attack << ' ' << tt.attr.defend << ' '
                 << tt.attr.yellow << ' ' << tt.attr.blue << ' ' << tt.attr.red << ' ' << id << ' ' << tt.x << ' '
                 << tt.y << endl;
        }
        s = tt;
    }
    cout << endl;
}