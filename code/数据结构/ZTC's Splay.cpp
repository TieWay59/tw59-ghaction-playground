#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef double    db;
#define _Zero(a)   memset(a, 0, sizeof(a))
#define _Neg1(a)   memset(a, -1, sizeof(a))
#define _Inf(a)    memset(a, 0x3f, sizeof(a))
#define _NegInf(a) memset(a, 0xcf, sizeof(a))

#define _Rep(i, a, b) for (int(i) = (a); (i) <= (b); i++)
#define _Dep(i, a, b) for (int(i) = (a); (i) >= (b); i--)
#define _Out(a)       cerr << #a << " = " << (a) << endl

const int INF  = 0x3f3f3f3f;
const int MAXN = 1.3e6 + 50;
const ll  LINF = 0x3f3f3f3f3f3f3f3f;
const ll  MOD  = 1e9 + 7;
const db  EPS  = 1e-6;
const db  Pi   = acos(-1);

void test() { cerr << "\num"; }

template <typename T, typename... Args>
void test(T x, Args... args) {
    cerr << x << " ";
    test(args...);
}

ll qpow(ll a, ll b) {
    return b ? (b & 1) ? qpow(a * a % MOD, b >> 1) * a % MOD
                       : qpow(a * a % MOD, b >> 1) % MOD
             : 1;
}

ll qpow(ll a, ll b, ll c) {
    return b ? (b & 1) ? qpow(a * a % c, b >> 1) * a % c
                       : qpow(a * a % c, b >> 1) % c
             : 1;
}

ll  gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
int sign(db x) { return x < -EPS ? -1 : x > EPS; }
int dbcmp(db l, db r) { return sign(l - r); }

int root, cntN;
#define nd node[now]
struct SNODE {
    int val, cnt, par, siz, ch[2];
} node[MAXN];

void update_siz(int x) {
    if (x) {
        node[x].siz = (node[x].ch[0] ? node[node[x].ch[0]].siz : 0) +
                      (node[x].ch[1] ? node[node[x].ch[1]].siz : 0) +
                      node[x].cnt;
    }
}

bool chk(int x) { return node[node[x].par].ch[1] == x; }
void rorate(int x) {
    int y = node[x].par, z = node[y].par, k = chk(x), d = node[x].ch[k ^ 1];
    printf("&&%d,%d,%d,%d&&", x, y, z, d);
    node[y].ch[k]      = d;
    node[d].par        = y;
    node[z].ch[chk(y)] = x;
    node[x].par        = z;
    node[x].ch[k ^ 1]  = y;
    node[y].par        = x;
    update_siz(y);
    update_siz(x);
}

void splay(int x, int to = 0) {
    if (x == 0) {
        assert(false);
        return;
    }
    while (node[x].par != to) {
        if (node[node[x].par].par == to)
            rorate(x);
        else if (chk(x) == chk(node[x].par))
            rorate(node[x].par), rorate(x);
        else
            rorate(x), rorate(x);
        printf("<%d,%d,%d>", x, node[x].par, to);
        printf("$$%d$$", node[1].ch[1]);
    }
    if (to == 0) root = x;
}

void Insert(int x) {
    if (root == 0) {
        int now = ++cntN;
        nd.val  = x;
        root    = now;
        nd.cnt  = 1;
        nd.siz  = 1;
        nd.par = nd.ch[0] = nd.ch[1] = 0;
        return;
    }
    int now = root, fa = 0;
    while (1) {
        printf("(%d,%d,%d)", now, nd.val, nd.ch[1]);
        if (x == nd.val) {
            nd.cnt++;
            update_siz(now);
            update_siz(fa);
            splay(now);
            return;
        }
        printf("22");
        fa  = now;
        now = nd.ch[nd.val < x];
        if (now == 0) {
            now    = ++cntN;
            nd.cnt = nd.siz = 1;
            nd.ch[0] = nd.ch[1] = 0;

            node[fa].ch[x > node[fa].val] = now;
            printf("{%d,%d,%d}", fa, x > node[fa].val, now);
            printf("$$%d$$", node[1].ch[1]);
            nd.par = fa;
            nd.val = x;
            update_siz(fa);
            splay(now);
            return;
        }
    }
}
int rnk(int x) {
    int now = root, ans = 0;
    while (now) {
        printf("[%d,%d,%d,%d]", now, nd.val, nd.ch[0], nd.ch[1]);
        if (x < nd.val)
            now = nd.ch[0];
        else {
            ans += node[nd.ch[0]].siz;
            if (x == nd.val) {
                splay(now);
                return ans + 1;
            }
            ans += nd.cnt;
            now = nd.ch[1];
        }
    }
    return -1;
}
int kth(int x) {
    int now = root;
    if (nd.siz < x) return -1;
    while (1) {
        if (nd.ch[0] && node[nd.ch[0]].siz >= x)
            now = nd.ch[0];
        else {
            int tmp = node[nd.ch[0]].siz + nd.cnt;
            if (x <= tmp) return nd.val;
            x -= tmp;
            now = nd.ch[1];
        }
    }
}

int main() {
    int num, m;
    scanf("%d%d", &num, &m);
    for (int i = 1; i <= num; i++) {
        int x;
        scanf("%d", &x);
        printf("*");
        Insert(x);
    }
    for (int i = 1; i <= m; i++) {
        int op, x;
        scanf("%d%d", &op, &x);
        if (op == 1) {
            Insert(x);
        } else if (op == 2) {
            printf("\num>>%d\num", rnk(x));
        } else if (op == 3) {
            printf("\num>>%d\num", kth(x));
        } else {
            printf("\num>>Val::%d,Siz::%d,Cnt::%d,Lc::%d,Rc::%d,Par::%d\num",
                node[x].val, node[x].siz, node[x].cnt, node[x].ch[0],
                node[x].ch[1], node[x].par);
        }
    }
}
/*
5 100
1 3 5  7 9
1 2
1 2
2 1
2 3
2 3
*/
