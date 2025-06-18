#include<bits/stdc++.h>
using namespace std;
#define el '\n'

struct database {
    string id;
    set<string> listdb;
};

struct Items {
    set<string> SET; // Tập hợp độ dài thứ k
    set<string> ListID; // supp = ListID.size
};
const int N = 105;
vector<database> db[N];
vector<Items> L[N];
vector<Items> C[N];
int n, minsupp;
double conf;
void file() {
    cin >> n >> minsupp >> conf;
    cin.ignore();
    for(int i = 1;  i <= n; i++) {
        string line;
        getline(cin, line);
        stringstream ss(line);
        string s;
        database newData;
        int ok = 0;
        while(ss >> s) {
            if(!ok) {
                newData.id = s;
                ok = 1;
            } else {
                newData.listdb.insert(s);
            }
        }
        db[i].push_back(newData);
    }
}

void sinh(int k) {

}

void init() {
    // k = 1
    map<string, vector<string>> mp;
    map<string, int> listt;
    for(int i = 1; i <= n; i++) {
        for(auto x : db[i]) {
            for(auto y : x.listdb) {
                listt[y] = 1;
            }
        }
    }

    for(auto [name, f] : listt) {
        for(int i = 1; i <= n; i++) {
            for(auto x : db[i]) {
                if(x.listdb.find(name) != x.listdb.end()) {
                    mp[name].push_back(x.id);
                }
            }
        }
    }
    for(auto x : mp) {
        Items newItems;
        newItems.SET.insert(x.first);
        for(auto y : x.second) newItems.ListID.insert(y);
        C[1].push_back(newItems);
    }
}

void sinhC(int k) {
    C[k].clear();
    int sz = C[k - 1].size();
    for(int i = 0 ; i < sz ; i++) {
        for(int j = i + 1 ; j < sz ; j++) {
            set<string> giao;
            for(auto x : C[k - 1][j].SET) {
                if(C[k - 1][i].SET.count(x)) {
                    giao.insert(x);
                }
            }
            if(giao.size() == k-2) {
                Items newItems;
                newItems.SET = C[k-1][i].SET;
                for(auto x : C[k-1][j].SET) {
                    newItems.SET.insert(x);
                }
                set<string>newListID;
                for(auto x : C[k - 1][j].ListID) {
                    if(C[k - 1][i].ListID.count(x)) {
                        newListID.insert(x);
                    }
                }
                newItems.ListID = newListID;
                if(newItems.SET.size() == k && !newItems.ListID.empty()) {
                    C[k].push_back(newItems);
                }
            }
        }
    }
}

void locL(int k) {
    L[k].clear();
    for(auto x : C[k]) {
        if(x.ListID.size() >= minsupp) {
            L[k].push_back(x);
        }
    }
}

void printSet(const set<string>& s) {
    cout << "{";
    bool ok = 1;
    for (auto& x : s) {
        if (!ok) cout << ", ";
        cout << x;
        ok = 0;
    }
    cout << "}";
}

vector<set<string>> sinhtapcon(set<string>& s) {
    vector<set<string>> ans;
    vector<string> v;
    for(auto x : s) {
        v.push_back(x);
    }
    int n = v.size();
    for(int mask = 1 ; mask < (1 << n) - 1 ; mask++) {
        set<string> se;
        for(int i = 0 ; i < n ; i++) {
            if(mask & (1 << i)) se.insert(v[i]);
        }
        ans.push_back(se);
    }
    return ans;
}

void Solve() {
    set<pair<set<string>, set<string>>> luat;
    for(int k = 2 ; L[k].size() ; k++){
        for(auto x : L[k]){
            vector<set<string>> subsets = sinhtapcon(x.SET);
            for(auto a : subsets){
                set<string> b;
                for(auto s : x.SET){
                    if(a.find(s) == a.end()){
                        b.insert(s);
                    }
                }
                if(!a.size() || !b.size()) continue;
                int supA = 0;
                for(int i = 1; i < k ; i++){
                    for(auto l : L[i]){
                        if(l.SET == a){
                            supA = l.ListID.size();
                            break;
                        }
                    }
                    if(supA > 0) break;
                }
                if(supA == 0) continue;
                double confident = 1.0 * x.ListID.size() / supA;
                if(confident >= conf){
                    luat.insert({a , b});
                }
            }
        }
    }
    for(auto x : luat){
        printSet(x.first);
        cout << " => ";
        printSet(x.second);
        cout << el;
    }
}

int main() {
    freopen("input.in", "r", stdin);
    freopen("output.out" , "w" , stdout);
    file();
    init();
    locL(1);
    int k = 2;
    while(L[k - 1].size()) {
        sinhC(k);
        locL(k);
        k++;
    }
    Solve();
}
