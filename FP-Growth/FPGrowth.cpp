#include<bits/stdc++.h>
using namespace std;
#define el '\n'
#define FOR(i , a , b) for(int i = a ; i < b ; i++)
#define REP(i , a , b) for(int i = a ; i <= b ; i++)
const int N = 1005;
const int inf = 1e9;
struct node {
    set<string> items;
    string id;
};
struct node2 {
    string s;
    int freq;
};
struct FP {
    string name;
    unordered_map<string, FP*> child;
    int cnt;
    FP(string _name, FP* _par) {
        name = _name;
        cnt = 1;
    }
};
map<string, int> order;
node db[N];
vector<node2> newData;
int n, minsupp;
double conf;
vector<string> newTran[N];
bool cmp(node2 a, node2 b) {
    return a.freq > b.freq;
}
bool cmp2(string a, string b) {
    return order[a] < order[b];
}
FP *root;
void file() {
    cin >> n >> minsupp >> conf;
    cin.ignore();
    REP(i, 1, n) {
        string line;
        getline(cin, line);
        bool ok = 0;
        string s;
        node tmp;
        stringstream ss(line);
        while(ss >> s) {
            if(!ok) {
                tmp.id = s;
                ok = 1;
            } else tmp.items.insert(s);
        }
        db[i] = tmp;
    }
    map<string, int> mp;
    REP(i, 1, n) {
        for(auto x : db[i].items) {
            mp[x]++;
        }
    }
    for(auto [p, q] : mp) {
        if(q >= minsupp) newData.push_back({p, q});
    }
    //FOR(i , 0 , newData.size()) cout << newData[i].s << ' ' << newData[i].freq << el;
    sort(newData.begin(), newData.end(), cmp);
}

void buildTran() {
    for(int i = 0 ; i < newData.size() ; i++) {
        order[newData[i].s] = i;
    }
    REP(i, 1, n) {
        vector<string> loc;
        for(auto x : db[i].items) {
            if(order.find(x) != order.end()) {
                loc.push_back(x);
            }
        }
        sort(loc.begin(), loc.end(), cmp2);
        newTran[i] = loc;
    }
//    REP(i , 1 , n){
//        for(auto x : newTran[i]){
//            cout << x << ' ';
//        }
//        cout << el;
//    }
}

void buildTree() {
    REP(i, 1, n) {
        /// xu li tung cai tran[i]
        /// them vao nhu trie
        FP *p = root;
        for(auto str : newTran[i]) {
            if(p->child.count(str)) {
                p = p->child[str];
                p->cnt++;
            } else {
                FP *newNode = new FP(str, p);
                p->child[str] = newNode;
                p = newNode;
            }
        }
    }
}
vector<pair<vector<string>, int>> path[N];
void dfs(string s, FP* p, vector<pair<vector<string>, int>>& v, vector<string> &cur) {
    if(p->name != "null") cur.push_back(p->name);
    if(p->name == s) {
        vector<string> save(cur.begin(), cur.end() - 1);
        v.push_back({save, p->cnt});
    }
    for(auto x : p->child) {
        dfs(s, x.second, v, cur);
    }
    if(p->name != "null") cur.pop_back();
}

void loadtaphop(FP *p) {
    /// duyet cay dfs -> luu vet duong di tu root den newData[i] (di het tat ca nhanh) -> luu vao
    FOR(i, 0, newData.size()) {
        vector<string> cur;
        dfs(newData[i].s, p, path[i], cur);
    }
//    FOR(i, 0, newData.size()) {
//        cout << newData[i].s << el;
//        for (auto x : path[i]) {
//            for (auto y : x.first) cout << y << ' ';
//            cout << ": " << x.second << el;
//        }
//        cout << el;
//    }
}

vector<vector<pair<string, int>>> v(N);
vector<vector<pair<vector<string>, int>>> ans(N);
void locdulieutheominsupp() {
    /// dem so luong phan tu xuat hien trong tap path[i]
    REP(i, 1, n) {
        map<string, int> mp;
        for(auto x : path[i]) {
            for(auto y : x.first) {
                //cout << y << ' ';
                mp[y] += x.second;
            }
            //cout << el;
        }
        for(auto [p, q] : mp) {
            if(q >= minsupp && p != "") {
                v[i].push_back({p, q});
            }
        }
    }

//    REP(i, 1, n) {
//        cout << newData[i].s << "             ";
//        for(auto [item, cnt] : v[i]) {
//            cout << item << " : " << cnt << " ";
//        }
//        cout << el;
//    }
}

void sinhvaduaraketqua() {
    FOR(i, 0, n) {
        vector<pair<string, int>> p = v[i];
        int sz = p.size();
        FOR(mask, 1, (1 << sz)) {
            vector<string> a;
            int f = inf;
            FOR(j, 0, sz) {
                if((1 << j) & mask) {
                    f = min(f , p[j].second);
                    a.push_back(p[j].first);
                }
            }
            a.push_back(newData[i].s);
            ans[i].push_back({a , f});
        }
    }
    for (int i = 0; i < newData.size(); ++i) {
        cout << newData[i].s << el;
        for (auto &x : ans[i]) {
            cout << "{";
            for (int k = 0; k < x.first.size(); ++k) {
                cout << x.first[k];
                if (k + 1 < x.first.size()) cout << ",";
            }
            cout << "} : " << x.second << el;
        }
        cout << el;
    }
}

int main() {
    freopen("input.in", "r", stdin);
    //freopen("output.in", "w", stdout);
    root = new FP("null", nullptr);
    file();
    buildTran();
    buildTree();
    loadtaphop(root);
    locdulieutheominsupp();
    sinhvaduaraketqua();
}
