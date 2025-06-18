#include<bits/stdc++.h>
using namespace std;
#define el '\n'
const int N = 105;
int n, minsupp;
double min_conf;
struct Items {
    int id;
    set<string> item;
    int supp;
};
vector<Items> db[N];
vector<Items> L[N];
vector<Items> C[N];
vector<set<string>> trans;
void file() {
    cin >> n >> minsupp >> min_conf;
    cin.ignore();
    for(int i = 1 ; i <= n ; i++) {
        string line;
        getline(cin, line);
        stringstream ss(line);
        string s;

        Items luu;
        luu.id = i;
        luu.supp = 0;
        while(ss >> s) {
            luu.item.insert(s);
        }
        db[i].push_back(luu);
    }
}
void initC1() {
    map<string, int> cnt;
    for(int i = 1 ; i <= n ; i++) {
        for(auto x : db[i]) {
            for(auto y : x.item) {
                cnt[y]++;
            }
        }
    }
    for(auto [items, freq] : cnt) {
        Items i;
        i.item.insert(items);
        i.supp = freq;
        C[1].push_back(i);
    }
}

void locL(int k) {
    L[k].clear();
    for(auto x : C[k]) {
        if(x.supp >= minsupp) {
            L[k].push_back(x);
        }
    }
}

void demSupp(int k) {
    for (auto &c : C[k]) {
        c.supp = 0;
        for(int i = 1 ; i <= n ; i++) {
            bool ok = 1;
            for(const auto &x : c.item) {
                if(db[i][0].item.find(x) == db[i][0].item.end()) {
                    ok = 0;
                    break;
                }
            }
            if(ok) {
                c.supp++;
            }
        }
    }
}

void sinh(int k) {
    C[k].clear();
    int sz = C[k - 1].size();
    for(int i = 0 ; i < sz ; i++) {
        for(int j = i + 1 ; j < sz ; j++) {
            Items newItems;
            string s;
            newItems.item = C[k - 1][i].item;
            for(auto x : C[k - 1][j].item) {
                newItems.item.insert(x);
            }
            if(newItems.item.size() == k) {
                C[k].push_back(newItems);
            }
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
    for(int mask = 1 ; mask < (1 << s.size()) ; mask++) {
        set<string> se;
        for(int i = 0 ; i < n ; i++) {
            if(mask & (1 << i)) se.insert(v[i]);
        }
        ans.push_back(se);
    }
    return ans;
}

int getSupport(const set<string>& s) {
    for (int k = 1; k < N; ++k) {
        for (auto& x : L[k]) {
            if (x.item == s) return x.supp;
        }
    }
    return 0;
}

void Solve() {
    for (int k = 2; k < N; ++k) {
        for (auto& S : L[k]) {
            vector<set<string>> subsets = sinhtapcon(S.item);
            for (auto& X : subsets) {
                set<string> Y;
                for(auto s : S.item){
                    if(X.find(s) == X.end()){
                        Y.insert(s);
                    }
                }
                if (Y.empty()) continue;
                int supp_S = S.supp;
                int supp_X = getSupport(X);
                if (supp_X == 0) continue;
                double conf = 1.0 * supp_S / supp_X;
                if (conf >= min_conf) {
                    printSet(X);
                    cout << " => ";
                    printSet(Y);
                    cout << " (conf = " << fixed << setprecision(2) << conf << ")" << el;
                }
            }
        }
    }
}

int main() {
    freopen("input.in", "r", stdin);
    freopen("output.out", "w", stdout);
    file();
    initC1();
    locL(1);
    int k = 2;
    while(L[k - 1].size()) {
        sinh(k);
        demSupp(k);
        locL(k);
        k++;
    }
    Solve();
}
