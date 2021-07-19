# ACM 知识点

## 1.动态规划

### 线性 dp

#### 题目

- [ ] hihocoder 1469
- [ ] POJ 1080
- [ ] POJ 3267
- [ ] POJ 1836
- [ ] hihocoder 1453
- [ ] HDU 5904

#### 类型

##### 整数划分

###### 将 n 划分为不大于 m 的划分法

###### 将 n 划分为 k 个数的划分法

### 状压 dp

### 区间 dp

### 树形 dp

### 概率 dp

### 数位 dp

```cpp
/*
 * 题意：求区间[x , y]中beautiful number的个数，
 * a positive integer number is beautiful if and only
 * if it is divisible by each of its nonzero digits.
分析：一个数能被它的所有非零数位整除，则能被它们的最小公倍数整除，而1到9的最小公倍数为2520，
数位DP时我们只需保存前面那些位的最小公倍数就可进行状态转移，到边界时就把所有位的lcm求出了，
为了判断这个数能否被它的所有数位整除，我们还需要这个数的值，显然要记录值是不可能的，其实我们只
需记录它对2520的模即可，这样我们就可以设计出如下数位DP：dfs(pos,mod,lcm,f),pos为当前
位，mod为前面那些位对2520的模，lcm为前面那些数位的最小公倍数，f标记前面那些位是否达到上限，
这样一来dp数组就要开到19*2520*2520，明显超内存了，考虑到最小公倍数是离散的，1-2520中可能
是最小公倍数的其实只有48个，经过离散化处理后，dp数组的最后一维可以降到48，这样就不会超了。
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;
const int MAXN=25;
const int MOD=2520;//1~9的lcm为2520
long long dp[MAXN][MOD][48];
int index[MOD+10];//记录1~9的最小公倍数
int bit[MAXN];
int gcd(int a,int b)
{
    if(b==0)return a;
    else return gcd(b,a%b);
}
int lcm(int a,int b)
{
    return a/gcd(a,b)*b;
}

void init()
{
    int num=0;
    for(int i=1;i<=MOD;i++)
        if(MOD%i==0)
            index[i]=num++;
}
long long dfs(int pos,int preSum,int preLcm,bool flag)
{//pos:dfs填数的位数，0代表个位，1代表十位，以此类推
//preSum:已经填写过的数字位的和，如千位填1，百位填2，现在pos=1（要填十位了）时，preSum=12
//preLcm:先前各位非零数字的LCM
//flag:前一位填的数字是否到达临界值
    if(pos==-1)
        return preSum%preLcm==0;//判断是否填完，填完后能整除返回1，否则返回0；
    if(!flag && dp[pos][preSum][index[preLcm]]!=-1)//上一位没到达边界值时如果dp已经存储了当前情况，直接返回
        return dp[pos][preSum][index[preLcm]];
    long long ans=0;
    int end=flag?bit[pos]:9;//当前位置可填数字的上界
    for(int i=0;i<=end;i++)//当前位置填i
    {
        int nowSum=(preSum*10+i)%MOD;//更新位置和
        int nowLcm=preLcm;
        if(i)nowLcm=lcm(nowLcm,i);//更新LCM，当前位为0时不更新
        ans+=dfs(pos-1,nowSum,nowLcm,flag && i==end);//填下一位
    }
    if(!flag)dp[pos][preSum][index[preLcm]]=ans;//当前位置所有可能已尝试，存储下该情况
    return ans;            //index数组用来压缩preLcm的情况
}
long long calc(long long x)
{
    int pos=0;
    while(x)
    {
        bit[pos++]=x%10;
        x/=10;
    }
    return dfs(pos-1,0,1,1);
}
int main()
{
    int T;
    long long l,r;
    init();
    memset(dp,-1,sizeof(dp));
    scanf("%d",&T);
    while(T--)
    {
        scanf("%I64d%I64d",&l,&r);
        printf("%I64d\n",calc(r)-calc(l-1));
    }
    return 0;
}
```

### 背包问题

- [ ] 背包九讲

#### 01 背包问题

$$
有N件物品和一个容量为V的背包。第i件物品的费用是w[i]，价值是v[i]\\
求将哪些物品装入背包可使价值总和最大。
$$

$$
f[i][j]表示前i件物品恰放入一个容量为j的背包可以获得的最大价值。
$$

```cpp
f[i][j]=max(f[i−1][j],f[i−1][j−w[i]]+v[i])
```

### 记忆化搜索

### 轮廓线 dp

### 插头 dp

## 2.字符串

### 基础术语

prefix[i]=s[1..i],即 s 的第 i 个前缀
lcp(s1,s2):s1 与 s2 的最长公共前缀
lcs(s1,s2):s1 与 s2 的最长公共后缀
suffix[i]=s[i..n],即 s 的第 i 个后缀

### 最小表示法

```cpp
#pragma GCC optimize(2)
#include<bits/stdc++.h>
using namespace std;
const int MAX=300005;
int a[MAX];
int n,i,j,k,t;
int id(int x)
{
    if(x%n)return x%n;
    return n;
}
int main()
{
    ios::sync_with_stdio(0);cin.tie(0),cout.tie(0);
    cin>>n;
    for(i=1;i<=n;i++)cin>>a[i];
    k=1;
    for(i=2;i<=n;)
    {
        for(j=1;j<=n;++j)
            if(a[id(i+j-1)]!=a[id(k+j-1)])break;
        if(j>n)break;
        if(a[id(i+j-1)]<a[id(k+j-1)])
        {
            t=i;
            i=max(k+j,t+1);
            k=t;
        }
        else i+=j;
    }
    for(i=0;i<n;i++)cout<<a[id(i+k)]<<' ';
    return 0;
}

```

### KMP

```cpp
#pragma GCC optimize(2)
#include<bits/stdc++.h>
using namespace std;
const int MAX=1000007;
int nex[MAX];
void kmp_next(string pattern)
{
    int len=pattern.size();
    nex[0]=-1;
    for(int i=0,j=-1;i<len;)
    {
        if(j==-1||pattern[i]==pattern[j])
        {
            nex[++i]=++j;
        }
        else
            j=nex[j];
    }
}
int a=0,b=0;
int kmp(string s,string pattern)
{
    int ls=s.size(),lp=pattern.size(),flag=0;
    for(int i=0,j=0;i<ls;)
    {
        if(j==-1||s[i]==pattern[j]){
            i++,j++;
        }
        else j=nex[j];
        if(j==lp){
            flag++;
            j=nex[j];
            if(flag==1)a=i-lp;
            else if(flag==2)b=i-lp;
        }
    }
    return flag;
}

int main()
{
    ios::sync_with_stdio(0);cin.tie(0),cout.tie(0);
    string s,pattern="suqingnianloveskirito";
    cin>>s;
    if(s.size()<pattern.size()){
        cout<<"Yes"<<endl;
        cout<<1<<" "<<2;
        return 0;
    }
    else{
        kmp_next(pattern);
        int n=kmp(s,pattern);
        if(n>2)
        {
            cout<<"No";
            return 0;
        }
        else if(n==2)
        {
            cout<<"Yes"<<endl;
            cout<<a+1<<" "<<b+2;
            return 0;
        }
        else
        {
            cout<<"Yes"<<endl;
            cout<<a+1<<" "<<a+2;
            return 0;
        }

    }
    return 0;
}

```

### EXKMP

```cpp
#pragma GCC optimize(2)
#include<bits/stdc++.h>
using namespace std;
const int N=2e7+7;
int q;
int nxt[N];     //t与t的每一个后缀的LCP长度
int extend[N];  //s与t的每一个后缀的LCP长度
int slen,tlen;
char s[N],t[N];

void getnxt()
{
    nxt[0]=tlen;
    int now=0;
    while(t[now]==t[1+now]&&now+1<tlen)now++;
    nxt[1]=now;
    int p0=1;
    for(register int i=2;i<tlen;i++)
    {
        if(i+nxt[i-p0]<nxt[p0]+p0)nxt[i]=nxt[i-p0];
        else
        {
            int now=nxt[p0]+p0-i;
            now=max(now,0);
            while(t[now]==t[i+now]&&i+now<tlen)now++;
            nxt[i]=now;
            p0=i;
        }
    }
}
void exkmp()
{
    getnxt();
    int now=0;
    while(s[now]==t[now]&&now<min(slen,tlen))now++;
    extend[0]=now;
    int p0=0;
    for(register int i=1;i<slen;i++)
    {
        if(i+nxt[i-p0]<extend[p0]+p0)extend[i]=nxt[i-p0];
        else
        {
            int now=extend[p0]+p0-i;
            now=max(now,0);
            while(t[now]==s[i+now]&&now<tlen&&now+i<slen)now++;
            extend[i]=now;
            p0=i;
        }
    }
}
int main()
{
    scanf("%s%s",s,t);
    slen=strlen(s),tlen=strlen(t);
    exkmp();
    long long z=0,p=0;
    for(register int i=0;i<tlen;i++)z^=1ll*(i+1)*(nxt[i]+1);
    for(register int i=0;i<slen;i++)p^=1ll*(i+1)*(extend[i]+1);
    printf("%lld\n%lld\n",z,p);
    return 0;
}
```

### Manacher

```cpp
#include<bits/stdc++.h>
#define maxn 3000
using namespace std;
int n,hw[maxn],ans;
char a[maxn],s[maxn];
void change(char *str)
{
    str[0]=str[1]='#';
    for(int i=0; i<n; i++)
    {
        str[i*2+2]=a[i];
        str[i*2+3]='#';
    }
    n=n*2+2;
    str[n]=0;
}
void manacher(char *str,int *hw)
{
    change(str);
    int maxright=0,mid;
    for(int i=1; i<n; i++)
    {
        if(i<maxright)
            hw[i]=min(hw[(mid<<1)-i],hw[mid]+mid-i);
        else
            hw[i]=1;
        for(; str[i+hw[i]]==str[i-hw[i]]; ++hw[i]);
        if(hw[i]+i>maxright)
        {
            maxright=hw[i]+i;
            mid=i;
        }
    }
}
int main()
{
    while(scanf("%s",a)!=EOF)
    {
        n=strlen(a);
        manacher(s,hw);
        ans=1;
        for(int i=0; i<n; i++)
            ans=max(ans,hw[i]);
        printf("%d\n",ans-1);
    }
    return 0;
}
```

### AC 自动机

```cpp
#include<bits/stdc++.h>
#define maxn 2000001
using namespace std;
char s[maxn],T[maxn];
int n,cnt,ans,in[maxn],Map[maxn];
int vis[200051]//s在T中出现的次数
struct kkk{
    int son[26],fail,flag,ans;
    void clear(){memset(son,0,sizeof(son)),fail=flag=ans=0;}
}trie[maxn];
queue<int>q;
void insert(char* s,int num){
    int u=1,len=strlen(s);
    for(int i=0;i<len;i++){
        int v=s[i]-'a';
        if(!trie[u].son[v])trie[u].son[v]=++cnt;
        u=trie[u].son[v];
    }
    if(!trie[u].flag)trie[u].flag=num;
    Map[num]=trie[u].flag;
}
void getFail(){
    for(int i=0;i<26;i++)trie[0].son[i]=1;
    q.push(1);
    while(!q.empty()){
        int u=q.front();q.pop();
        int Fail=trie[u].fail;
        for(int i=0;i<26;i++){
            int v=trie[u].son[i];
            if(!v){trie[u].son[i]=trie[Fail].son[i];continue;}
            trie[v].fail=trie[Fail].son[i]; in[trie[v].fail]++;
            q.push(v);
        }
    }
}
void topu(){
    for(int i=1;i<=cnt;i++)
    if(in[i]==0)q.push(i);
    while(!q.empty()){
        int u=q.front();q.pop();vis[trie[u].flag]=trie[u].ans;
        int v=trie[u].fail;in[v]--;
        trie[v].ans+=trie[u].ans;
        if(in[v]==0)q.push(v);
    }
}
void query(char* s){
    int u=1,len=strlen(s);
    for(int i=0;i<len;i++)
    u=trie[u].son[s[i]-'a'],trie[u].ans++;
}
int main(){
    scanf("%d",&n); cnt=1;
    for(int i=1;i<=n;i++){
        scanf("%s",s);
        insert(s,i);
    }getFail();scanf("%s",T);
    query(T);topu();
    for(int i=1;i<=n;i++)printf("%d\n",vis[Map[i]]);
}
```

### 后缀数组

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
const int MAXN = 1e6 + 10;
using namespace std;
char s[MAXN];
int N, M, rak[MAXN], sa[MAXN], tax[MAXN], tp[MAXN];
void Debug()
{
    printf("*****************\n");
    printf("下标");
    for (int i = 1; i <= N; i++)
        printf("%d ", i);
    printf("\n");
    printf("sa  ");
    for (int i = 1; i <= N; i++)
        printf("%d ", sa[i]);
    printf("\n");
    printf("rak ");
    for (int i = 1; i <= N; i++)
        printf("%d ", rak[i]);
    printf("\n");
    printf("tp  ");
    for (int i = 1; i <= N; i++)
        printf("%d ", tp[i]);
    printf("\n");
}
void Qsort()
{
    for(int i=0;i<=M;i++)tax[i]=0;
    for(int i=1;i<=N;i++)tax[rak[i]]++;
    for(int i=1;i<=M;i++)tax[i]+=tax[i-1];
    for(int i=N;i>=1;i--)sa[tax[rak[tp[i]]]--]=tp[i];
}
void SuffixSort()
{
    M=75;
    for(int i=1;i<=N;i++)
        rak[i]=s[i]-'0'+1,tp[i]=i;
    Qsort();
    Debug();
    for(int w=1, p = 0; p < N; M = p, w <<= 1)
    {
        //w:当前倍增的长度，w = x表示已经求出了长度为x的后缀的排名，现在要更新长度为2x的后缀的排名
        //p表示不同的后缀的个数，很显然原字符串的后缀都是不同的，因此p = N时可以退出循环
        p = 0;//这里的p仅仅是一个计数器000
        for (int i = 1; i <= w; i++)
            tp[++p] = N - w + i;
        for (int i = 1; i <= N; i++)
            if (sa[i] > w)
                tp[++p] = sa[i] - w; //这两句是后缀数组的核心部分，我已经画图说明
        Qsort();//此时我们已经更新出了第二关键字，利用上一轮的rak更新本轮的sa
        std::swap(tp, rak);//这里原本tp已经没有用了
        rak[sa[1]] = p = 1;
        for (int i = 2; i <= N; i++)
            rak[sa[i]] = (tp[sa[i - 1]] == tp[sa[i]] && tp[sa[i - 1] + w] == tp[sa[i] + w]) ? p : ++p;
        //这里当两个后缀上一轮排名相同时本轮也相同
        Debug();
    }
    for (int i = 1; i <= N; i++)
        printf("%d ", sa[i]);
}
int main()
{
    scanf("%s",s+1);
    N=strlen(s+1);
    SuffixSort();
    return 0;
}
```

### 后缀自动机

#### 功能

$$1.在另一个字符串中搜索一个字符串的所有出现位置。$$
$$2.计算给定的字符串中有多少个不同的子串。$$

### 字符串 hash

## 3.数据结构

### 分块

```cpp
#include<cstdio>
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdlib>
#include <sstream>
#include<unordered_map>
#include<math.h>
#include<vector>

#include<bits/stdc++.h>
using namespace std;
#define ll long long

ll n,i,opt,l,r,c,mod;
ll a[50050],block[50050],sum[50050],b[50050],ans,j,k;
void query(ll x,ll y)
{
    ll i;
    ans=0;
    if(block[x]==block[y])
    {
        for(i=x;i<=y;i++)ans+=a[i]+b[block[i]];
        return;
    }
    for(i=x;block[i]==block[x];i++)ans+=a[i]+b[block[i]];
    for(i=block[x]+1;i<block[y];i++)ans+=sum[i]+1ll*b[i]*k;
    for(i=y;block[i]==block[y];i--)ans+=a[i]+b[block[i]];
}
void update(ll x,ll y,ll c)
{
    ll i;
    if(block[x]==block[y])
    {
        for(i=x;i<=y;i++)a[i]+=c,sum[block[i]]=sum[block[i]]+c;
        return;
    }
    for(i=x;block[i]==block[x];i++)a[i]+=c,sum[block[i]]=sum[block[i]]+c;
    for(i=block[x]+1;i<block[y];i++)b[i]=b[i]+c;
    for(i=y;block[i]==block[y];i--)a[i]+=c,sum[block[i]]=sum[block[i]]+c;
}
int main()
{
    std::ios::sync_with_stdio(false);
    ll key[50050],num=0;
    cin>>n;
    k=sqrt(n);
    memset(b,0,sizeof b);
    memset(sum,0,sizeof sum);
    for(i=1;i<=n;i++)
    {
        cin>>a[i];
        block[i]=i/k;
    }
    for(i=1;i<=n;i++)
    {
        sum[block[i]]+=a[i];
    }
    for(ll i=1;i<=n;i++)
    {
        cin>>opt>>l>>r>>c;
        if(opt)
        {
            c++;
            ans=0;
            query(l,r);
            ans=(ans+c)%c;
            cout<<ans<<endl;
        }
        else
        {
            update(l,r,c);
        }
    }
    return 0;
}
```

### 划分树

### RMQ

### 树链剖分

#### 树链剖分模板

$$
CodeForces \ 343D
$$

```cpp
#include<bits/stdc++.h>
#include<vector>
using namespace std;
#define mod 1000000007
#define ll long long
#define N 500005
vector<int>G[N];
int n,m;
int pos,x,y,cnt;
int dep[N],siz[N],fa[N],top[N],id[N],son[N];

//-------------------------------------- 以下为线段树
#define lson l,mid,id<<1
#define rson mid+1,r,id<<1|1
int tree[N<<3],lazy[N<<3];
void pushup(int id)
{
    if(tree[id<<1]||tree[id<<1|1])tree[id]=1;
    else tree[id]=0;
}
void pushdown(int id)
{
    if(lazy[id]!=-1)
    {
        lazy[id<<1]=lazy[id];
        lazy[id<<1|1]=lazy[id];
        tree[id<<1]=lazy[id];
        tree[id<<1|1]=lazy[id];
        lazy[id]=-1;
    }
}
void build(int l,int r,int id)
{
    lazy[id]=-1;
    if(l==r)
    {
        tree[id]=0;
        return;
    }
    int mid=(l+r)/2;
    build(lson);
    build(rson);
    pushup(id);
}
void update(int L,int R,int k,int l,int r,int id)
{
    if(L<=l&&R>=r)
    {
        tree[id]=k;
        lazy[id]=k;
        return;
    }
    int mid=(l+r)/2;
    pushdown(id);
    if(L<=mid)update(L,R,k,lson);
    if(R>mid)update(L,R,k,rson);
    pushup(id);
}
int query(int L,int R,int l,int r,int id)
{
    if(L<=l&&R>=r)
    {
        return tree[id];
    }
    int mid=(l+r)/2;
    pushdown(id);
    int ans=0;
    if(L<=mid)if(ans||query(L,R,lson))ans=1;
    if(R>mid)if(ans||query(L,R,rson))ans=1;
    pushup(id);
    return ans;
}
//-------------------------------------- 以上为线段树
void dfs1(int x,int f,int deep)//x当前节点,f父亲,deep深度
{
    dep[x]=deep;//标记点的深度
    siz[x]=1;//标记子树大小
    fa[x]=f;//标记点的父亲
    int maxson=-1;//记录重儿子的儿子数
    for(int i=0;i<G[x].size();i++)
    {
        if(G[x][i]==f)continue;//连边为父亲则退出
        dfs1(G[x][i],x,deep+1);//dfs儿子
        siz[x]+=siz[G[x][i]];//加上儿子的子树大小
        if(siz[G[x][i]]>maxson)
        {
            maxson=siz[G[x][i]];
            son[x]=G[x][i];//标记非叶子节点的重儿子编号
        }
    }
}
void dfs2(int x,int topf)//x当前节点,topf当前链的最顶端的节点
{
    id[x]=++cnt;//标记点的新编号
    top[x]=topf;//标记点所处链的顶端
    if(!son[x])return;
    dfs2(son[x],topf);//处理重儿子(优先)
    for(int i=0;i<G[x].size();i++)
    {
        if(G[x][i]==son[x]||G[x][i]==fa[x])continue;
        dfs2(G[x][i],G[x][i]);//处理轻儿子
    }
}
void Modify_Range(int x,int y,int k)
{
    while(top[x]!=top[y])//当x,y处于不同链
    {
        if(dep[top[x]]<dep[top[y]])swap(x,y);//把x点改为所在链顶端的深度更深的那个点
        update(id[top[x]],id[x],k,1,n,1);//维护深度更深的链
        x=fa[top[x]];//维护完后找到链的父节点，继续更新直到x,y属于同一条链
    }
    if(dep[x]>dep[y])swap(x,y);
    update(id[x],id[y],k,1,n,1);//维护x到y的区间
}
void Modify_Tree(int x,int k)
{
    update(id[x],id[x]+siz[x]-1,k,1,n,1);//维护x所处的链(x到链尾)
}

int main()
{
    std::ios::sync_with_stdio(false);
    cin>>n;
    for(int i=1;i<n;i++)
    {
        cin>>x>>y;
        G[x].push_back(y);
        G[y].push_back(x);
    }
    cnt=0;
    dfs1(1,0,1);
    dfs2(1,1);
    build(1,n,1);
    cin>>m;
    for(int i=1;i<=m;i++)
    {
        cin>>pos>>x;
        if(pos==1)
        {
            Modify_Tree(x,1);
        }
        else if(pos==2)
        {
            Modify_Range(1,x,0);
            update(id[x],id[x],0,1,n,1);
        }
        else if(pos==3)
        {
            if(query(id[x],id[x],1,n,1))
            {
                cout<<1<<endl;
            }
            else
            {
                cout<<0<<endl;
            }
        }
    }
    return 0;
}
```

### 伸展树

### 动态树

### 主席树

```cpp
#include<bits/stdc++.h>
using namespace std;
const int N=3e4+5;
int sum[N*40],ls[N*40],rs[N*40],rt[N*40];
int tot,n,m;
int vis[N],a[N],b[N];
void update(int l,int r,int root,int last,int q,int val)
{
    sum[root]=sum[last]+val;
    ls[root]=ls[last];
    rs[root]=rs[last];
    if(l==r)
        return;
    int mid=l+r>>1;
    if(mid>=q)
        update(l,mid,ls[root]=++tot,ls[last],q,val);
    else
        update(mid+1,r,rs[root]=++tot,rs[last],q,val);
}
int query(int l,int r,int root,int ql,int qr)
{
    if(l>=ql&&r<=qr)
        return sum[root];
    int mid=l+r>>1;
    int ans=0;
    if(mid>=ql)
        ans+=query(l,mid,ls[root],ql,qr);
    if(mid<qr)
        ans+=query(mid+1,r,rs[root],ql,qr);
    return ans;
}
int main()
{
    scanf("%d",&n);
    for(int i=1;i<=n;i++)
        scanf("%d",&a[i]),b[i]=a[i];
    sort(b+1,b+1+n);
    int all=unique(b+1,b+1+n)-(b+1);
    for(int i=1;i<=n;i++)
        a[i]=lower_bound(b+1,b+1+all,a[i])-b;
    for(int i=1;i<=n;i++)
    {
        if(!vis[a[i]])
            update(1,n,rt[i]=++tot,rt[i-1],i,1);
        else
        {
            update(1,n,rt[i]=++tot,rt[i-1],i,1);
            int del=++tot;
            update(1,n,del,rt[i],vis[a[i]],-1);
            rt[i]=del;
        }
        vis[a[i]]=i;
    }
    scanf("%d",&m);
    int l,r;
    while(m--)
    {
        scanf("%d%d",&l,&r);
        printf("%d\n",query(1,n,rt[r],l,r));
    }
    return 0;
}

```

### Treap

#### 功能

$$
1.插入x数\\
2.删除x数(若有多个相同的数，因只删除一个)\\
3.查询x数的排名(排名定义为比当前数小的数的个数+1)\\
4.查询排名为x的数\\
5.求x的前驱(前驱定义为小于x，且最大的数)\\
6.求x的后继(后继定义为大于x，且最小的数)\\
$$

#### Treap（模板）

```cpp
#include <bits/stdc++.h>
using namespace std;
const int N=100005;
int root,tot;
int val[N],num[N],cnt[N],pre[N],lson[N],rson[N];
void update(int k)
{
    num[k]=num[lson[k]]+num[rson[k]]+cnt[k];
}
void lturn(int &k)
{
    int t=rson[k];rson[k]=lson[t];lson[t]=k;
    num[t]=num[k];update(k);k=t;
}
void rturn(int &k)
{
    int t=lson[k];lson[k]=rson[t];rson[t]=k;
    num[t]=num[k];update(k);k=t;
}
void insert(int &k,int x)
{
    if(k==0)
    {
        k=++tot;
        num[k]=cnt[k]=1;
        val[k]=x;
        pre[k]=rand();
        return;
    }
    num[k]++;
    if(val[k]==x)cnt[k]++;
    else if(x>val[k])
    {
        insert(rson[k],x);
        if(pre[rson[k]]<pre[k])lturn(k);
    }
    else
    {
        insert(lson[k],x);
        if(pre[lson[k]]<pre[k])rturn(k);
    }
}
void remove(int &k,int x)
{
    if(k==0)return;
    if(val[k]==x)
    {
        if(cnt[k]>1)cnt[k]--,num[k]--;
        else
        {
            if(!lson[k]||!rson[k])k=lson[k]+rson[k];
            else if(pre[lson[k]]<pre[rson[k]])rturn(k),remove(k,x);
            else lturn(k),remove(k,x);
        }
    }
    else if(x>val[k])num[k]--,remove(rson[k],x);
    else num[k]--,remove(lson[k],x);
}
int get_rank(int &k,int x)//查询排名
{
    if(k==0) return 1;//改成2e9代表找不到当前数
    if(val[k]==x)return num[lson[k]]+1;
    if(x>val[k])return num[lson[k]]+cnt[k]+get_rank(rson[k],x);
    else return get_rank(lson[k],x);
}
int get_val(int &k,int x)//查询排名为x的数
{
    //cout<<k<<endl;
    if(k==0) return 2e9;
    if(x<=num[lson[k]])return get_val(lson[k],x);
    x-=num[lson[k]];
    if(x<=cnt[k])return val[k];
    x-=cnt[k];
    return get_val(rson[k],x);
}
int get_pre(int &k,int x)//查询前驱
{
    if(k==0)return -2e9;
    if(val[k]<x)return max(val[k],get_pre(rson[k],x));
    else return get_pre(lson[k],x);
}
int get_next(int &k,int x)//查询后继
{
    if(k==0) return 2e9;
    if(val[k]>x)return min(val[k],get_next(lson[k],x));
    else return get_next(rson[k],x);
}
void solve(int opt,int x)
{
    switch(opt)
    {
        case 1:insert(root,x);break;
        case 2:remove(root,x);break;
        case 3:printf("%d\n",get_rank(root,x));break;
        case 4:printf("%d\n",get_val(root,x));break;
        case 5:printf("%d\n",get_pre(root,x));break;
        case 6:printf("%d\n",get_next(root,x));break;
    }
    return;
}
void work()
{
    int opt,T,x;
    cin>>T;
    for(int i=1;i<=T;i++)
    {
        cin>>opt>>x;
        solve(opt,x);
    }
}
int main(){
    std::ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    work();
    return 0;
}
```

### FHQ Treap

### KD 树

### 替罪羊树

### 树套树

### 线段树

#### 线段树（模板）

```c++
#include<bits/stdc++.h>
#define ll long long
using namespace std;
int i,n,base[100010],q;
typedef long long treetype;
struct point
{
    int l,r;
    treetype sum,lazy;
    void update(treetype v)
    {
        sum+=(r-l+1)*v;
        lazy+=v;
    }
}tree[400040];
inline void push_up(int id)
{
    tree[id].sum=tree[2*id].sum+tree[2*id+1].sum;
}
inline void build(int id,int l,int r)
{
    tree[id].l=l;
    tree[id].r=r;
    tree[id].sum=tree[id].lazy=0;
    if(l==r){tree[id].sum=base[l];}
    else
    {
        int mid=(l+r)>>1;
        build(id<<1,l,mid);
        build(id<<1|1,mid+1,r);
        push_up(id);
    }
}
inline void push_down(int id)
{
    treetype lazyval=tree[id].lazy;
    if(lazyval)
    {
        tree[2*id].update(lazyval);
        tree[2*id+1].update(lazyval);
        tree[id].lazy=0;
    }
}
inline void update(int id,int l,int r,treetype val)
{
    int L=tree[id].l,R=tree[id].r;
    if(l<=L&&R<=r){tree[id].update(val);}
    else
    {
        push_down(id);
        int mid=(L+R)>>1;
        if(mid>=l)update(id<<1,l,r,val);
        if(r>mid)update(id<<1|1,l,r,val);
        push_up(id);
    }
}
inline treetype query(int id,int l,int r)
{
    int L=tree[id].l,R=tree[id].r;
    if(l<=L&&R<=r){return tree[id].sum;}
    else
    {
        push_down(id);
        treetype ans=0;
        int mid=(L+R)>>1;
        if(mid>=l)ans+=query(id<<1,l,r);
        if(r>mid)ans+=query(id<<1|1,l,r);
        push_up(id);
        return ans;
    }
}
int main()
{
    scanf("%d",&n);
    scanf("%d",&q);
    for(i=1;i<=n;++i)
        scanf("%d",&base[i]);
    build(1,1,n);
    for(i=1;i<=q;++i)
    {
        int l,r,val,k;
        scanf("%d%d%d",&k,&l,&r);
        if(k==1)
        {
            scanf("%d",&val);
            update(1,l,r,val);
        }
        else printf("%lld\n",query(1,l,r));
    }
    return 0;
}
```

##### 注释版代码

```cpp
#include<bits/stdc++.h>
#define ll long long
using namespace std;
int i,n,base[100010],q;
typedef long long treetype;
struct point
{
    int l,r;
    treetype sum,lazy;
    void update(treetype v)
    {
        sum+=(r-l+1)*v;//区间和增加值：区间长度*增加值
        lazy+=v;//当前节点下属区间先咕掉，将增加值堆叠，以后调用到时一次性更新
    }
}tree[400040];
inline void push_up(int id)
{
    tree[id].sum=tree[2*id].sum+tree[2*id+1].sum;
}
inline void build(int id,int l,int r)//主程序调用时id为1,tree[id]掌管的区间为[l,r]
{
    tree[id].l=l;//记录当前节点的子节点下标
    tree[id].r=r;
    tree[id].sum=tree[id].lazy=0;//建完树后的各点lazy和sum值都为0
    if(l==r){tree[id].sum=base[l];}//当前节点为叶子节点，初始化赋值
    else
    {
        int mid=(l+r)>>1;//查找子节点
        build(id<<1,l,mid);//子节点建树
        build(id<<1|1,mid+1,r);
        push_up(id);//子节点建完后讲子节点的值累加到父节点上
    }
}
inline void push_down(int id)
{
    treetype lazyval=tree[id].lazy;//开始处理当前节点堆积的lazy
    if(lazyval)
    {
        tree[2*id].update(lazyval);//当前节点下属两个节点更新堆积的lazy
        tree[2*id+1].update(lazyval);
        tree[id].lazy=0;//当前节点lazy值清零
    }
}
inline void update(int id,int l,int r,treetype val)
{
    int L=tree[id].l,R=tree[id].r;
    if(l<=L&&R<=r){tree[id].update(val);}//节点id掌管的区间在更新区间内，更新
    else
    {
        push_down(id);//下属区间要被调用到了，清空之前堆积的lazy值
        int mid=(L+R)>>1;//将当前掌管区间分为两半
        if(mid>=l)update(id<<1,l,r,val);//左半部分与更新区间有交集，更新左子节点
        if(r>mid)update(id<<1|1,l,r,val);//右半部分与更新区间有交集，更新右子节点
        push_up(id);//两个子节点都更新完后刷新当前节点的值
    }
}
inline treetype query(int id,int l,int r)
{
    int L=tree[id].l,R=tree[id].r;
    if(l<=L&&R<=r){return tree[id].sum;}//当前区间全部属于所求区间，直接返回当前区间和
    else
    {
        push_down(id);//先将当前点堆积的lazy值加到子节点，再划分子节点
        treetype ans=0;
        int mid=(L+R)>>1;
        if(mid>=l)ans+=query(id<<1,l,r);//查询子节点中的值
        if(r>mid)ans+=query(id<<1|1,l,r);
        push_up(id);//之前清除过lazy值，当前节点的sum值需要再次更新
        return ans;
    }
}
int main()
{
    scanf("%d",&n);//点的个数
    scanf("%d",&q);//询问次数
    for(i=1;i<=n;++i)
        scanf("%d",&base[i]);//要存储的初始数据
    build(1,1,n);//build(id,left,right)从点1开始构建长度为(left~right)的树;
    for(i=1;i<=q;++i)
    {
        int l,r,val,k;
        scanf("%d%d%d",&k,&l,&r);
        if(k==1)
        {
            scanf("%d",&val);
            update(1,l,r,val);
        }
        else printf("%lld\n",query(1,l,r));//询问区间(l,r)的和
    }
    return 0;
}

```

### 树状数组

#### 树状数组（模板）

```cpp
int n;
int a[1005],c[1005]; //对应原数组和树状数组

int lowbit(int x){
    return x&(-x);
}

void updata(int i,int k){    //在i位置加上k
    while(i <= n){
        c[i] += k;
        i += lowbit(i);
    }
}

int getsum(int i){        //求A[1 - i]的和
    int res = 0;
    while(i > 0){
        res += c[i];
        i -= lowbit(i);
    }
    return res;
}
```

### 可持久化数据结构

#### 单调栈

##### 题目

###### 区区区间间间

$$
链接：https://ac.nowcoder.com/acm/problem/20806\\
来源：牛客网\\

给出长度为n的序列a，其中第i个元素为a_ia
i
​
 ，定义区间(l,r)的价值为\\

v_{l,r} = max(a_i - a_j | l \leqslant i,j\leqslant r)v
l,\\
r
​
 =max(a
i
​
 −a
j
​
 ∣l⩽i,j⩽r)
\\
请你计算出\sum_{l = 1}^n \sum_{r = l + 1}^n v_{l,r}∑
l=1
n
​
 ∑
r=l+1
n
​
 v
l,r
​
$$

```cpp
#include<bits/stdc++.h>
long long a[100005],n,i,j,T,l[100005],r[100005];
long long ans;
long long solve()
{
    for(int i=1;i<=n;i++)
    {
        int j=i;
        while(j>1 && a[j-1]<=a[i])
        {
            j=l[j-1];
        }
        l[i]=j;
    }
    for(int i=n;i>=1;i--)
    {
        int j=i;
        while(j<n && a[j+1]<a[i])
        {
            j=r[j+1];
        }
        r[i]=j;
    }
    long long sum=0;
    for(int i=1;i<=n;i++)
    {
        sum+=a[i]*(r[i]-l[i]);
        sum+=a[i]*(r[i]-i)*(i-l[i]);
    }
    return sum;
}
int main()
{
    scanf("%lld",&T);
    while(T--)
    {
        ans=0;
        scanf("%lld",&n);
        for(i=1;i<=n;i++)
            scanf("%lld",&a[i]);
        ans=solve();
        for(i=1;i<=n;i++)
            a[i]=-a[i];
        ans+=solve();
        printf("%lld\n",ans);
    }
    return 0;
}
```

###### Bas Hair Day

![Bad Hair Day](G:\模板\Bad Hair Day.png)

```text
一群身高不等的奶牛排成一排，向右看，每个奶牛只能看到身高小于自己的奶牛发型，问这些奶牛能够看到的发型总和是多少
思路：利用单调栈（栈中元素从栈顶往下越来越大）的思想，可以计算出，每只奶牛能被他前面的多少只奶牛看到自己的发型，就反向得到了奶牛看到的发型总和
借用别人的解释：首先弹出元素是因为它右边相邻牛比它高（看不到它的头发并且挡住了该牛的视线），“挡住”这个关键字很重要，这就是说该牛已经看不到后面的其他牛的头发啦，而思路一是按顺序比较身高，统计每头牛前面能看到它头发的牛数，既然该牛望不到后面，那么把它出栈对整个结果没有影响。
例如：10 3 7 4 12 2
①Height_List：10 入栈首身高
②3<10，不弹出，num=0+1（当前栈中元素数），3入栈后 Height_List：10 3
③7 > 3（3 弹出） 7<10（10保留），栈中剩 10 ，num=1+1，7入栈后 Height_List：10 7
④4 < 7，没有弹出，栈中10 7都能看到4，num=2+2 ，4入栈后 Height_List：10 7 4
⑤12大于栈中全部元素，表示栈中所有元素看不到12，全部出栈，num不变，12入栈后 Height_List：12
⑥2 < 12，没有弹出，12能看到2，num=4+1
```

```cpp
#include<bits/stdc++.h>
using namespace std;
int n,i,a[80005];
long long ans;
stack<long long> dyy;
int main()
{
    memset(a,0,sizeof(a));
    scanf("%d",&n);
    for(i=1;i<=n;i++)
        scanf("%d",&a[i]);
    ans=0;
    for(i=1;i<=n;i++)
    {
        while(!dyy.empty()&&dyy.top()<=a[i])
            dyy.pop();
        ans+=dyy.size();
        dyy.push(a[i]);
    }
    printf("%lld\n",ans);
    return 0;
}
```

### 差分数组

### 并查集

#### 并查集（模板）

$$
完整版
$$

```cpp
int par[max_n];//父亲
int rank[max_n];//树高

//初始化n个元素，每个元素的初始父节点为自己本身
void init(int n)
{
    for(int i=1;i<=n;i++)
    {
        par[i]=i;
        rank[i]=0;
    }
}

//查询树的根
int find(int x)
{
    if(par[x]==x)
    {
        return x;
    }
    else
    {
        return par[x]=find(par[x]);
    }
}

//合并x和y所属的集合
void unite(int x,int y)
{
    x=find(x);
    y=find(y);
    if(x==y)return;
    if(rank[x]<rank[y])
    {
        par[x]=y;
    }
    else
    {
        par[y]=x;
        if(rank[x]==rank[y])rank[x]++;
    }
}

//判断x和y是否属于同一个集合
bool same(int x,int y)
{
    return find(x)==find(y);
}
```

$$
简化版（路径压缩）
$$

```c++
int par[max_n];//父亲

//初始化n个元素，每个元素的初始父节点为自己本身
void init(int n){
    for(int i=1;i<=n;i++){
        par[i]=i;
    }
}

//查询树的根
int find(int x){
    if(par[x]==x){
        return x;
    }
    else{
        return par[x]=find(par[x]);
    }
}

//合并x和y所属的集合
void unite(int x,int y){
    x=find(x);
    y=find(y);
    if(x==y)return;
    par[x]=par[y];
}

//判断x和y是否属于同一个集合
bool same(int x,int y){
    return find(x)==find(y);
}
```

### Set

#### 自定义比较

##### 题目

###### 指纹锁

链接：<https://ac.nowcoder.com/acm/problem/17508>
来源：牛客网

> HA实验有一套非常严密的安全保障体系，在HA实验基地的大门，有一个指纹锁。
> 该指纹锁的加密算法会把一个指纹转化为一个不超过1e7的数字，两个指纹数值之差越小，就说明两个指纹越相似，当两个指纹的数值差≤k时，这两个指纹的持有者会被系统判定为同一个人。
>现在有3种操作，共m个，
操作1：add x，表示为指纹锁录入一个指纹，该指纹对应的数字为x，如果系统内有一个与x相差≤k的指纹，则系统会忽略这次添加操作
操作2：del x，表示删除指纹锁中的指纹x，若指纹锁中多个与x相差≤k的指纹，则全部删除，若指纹锁中没有指纹x，则可以忽略该操作，
操作3：query x，表示有一个持有指纹x的人试图打开指纹锁，你需要设计一个判断程序，返回该人是否可以打开指纹锁（只要x与存入的任何一个指纹相差≤k即可打开锁）。
    初始状态，指纹锁中没有任何指纹。

```cpp
#include<bits/stdc++.h>
using namespace std;
int m,k;
struct cmp
{
    bool operator()(const int &a,const int &b)
        const
    {
        if(abs(a-b)<=k)return false;//绝对值小于k：删除
        return a<b;
    }
};
set <int,cmp> dyy;
int main()
{
    std::ios::sync_with_stdio(false);//解绑c++和c
    std::cin.tie(0);//解绑scanf和cin
    std::cout.tie(0);
    cin>>m>>k;
    while(m--)
    {
        string s;
        int x;
        cin>>s;
        cin>>x;
        if(s=="add")dyy.insert(x);
        else if(s=="del")dyy.erase(x);
        else if(dyy.find(x)!=dyy.end())cout<<"Yes"<<endl;
        else cout<<"No"<<endl;
    }
    return 0;
}
```

## 4.数论

$$𝑎|𝑏 ∶ 𝑎整除b$$
$$(𝑎, 𝑏) ∶ 𝑎和𝑏的最大公约数$$
$$[𝑎, 𝑏] ∶ 𝑎和𝑏的最小公倍数$$
$$\left\lfloor\frac{x}{y}\right\rfloor: 𝑎除以𝑏向下取整$$
$$\left\lceil\frac{x}{y}\right\rceil: 𝑎除以𝑏向上取整$$
$$[𝑎 = 1] ∶ 逻辑判断，当括号内逻辑正确时，值为1，反之值为0$$

### GCD+扩展

#### lcm

```cpp
ll gcd(ll a,ll b)
{
    return b?gcd(b,a%b):a;
}
ll lcm(ll a,ll b)
{
    return a/gcd(a,b)*b;
}
```

#### 辗转相除法

$$
性质1：gcd(a,b)=gcd(a-b,b)，gcd(a,b)=gcd(a \% b,b)
$$

```cpp
ll gcd(ll a,ll b)
{
    return b?gcd(b,a%b):a;
}
```

#### 拓欧求二元一次方程

裴(pei)蜀定理：若a和b为整数，二元一次方程ax+by=m有解的充要条件是gcd(a,b)|m。
推论：a,b互质的充要条件是存在整数x,y使ax+by=1。
拓展欧几里得算法：在处理gcd的过程中，顺便求解二元一次方程。
二元一次方程：ax+by=m

$$
做法：\\
若gcd(a,b)|m不成立，则方程无解。\\
否则把方程看成ax+by=t×gcd(a,b)的形式。\\
那么我们只要考虑求解方程ax+by=gcd(a,b)，最后答案乘上t即可。\\
考虑辗转相除a和b求gcd过程的同时，在每层的a和b，都求出解x和y，然后将这一层的解上传用于求出上一层的解。\\
比如，在最底层的时候，gcd(a,b)x+0y=gcd(a,b)，此时x=1,y=0是一组特解。
$$

##### 代码

```cpp
ll ex_gcd(ll a,ll b,ll& x,ll& y)
{
    if(b==0)
    {
        x=1;y=0;
        return a;
    }
    ll g=ex_gcd(b,a%b,x,y);
    ll tmp=x;
    x=y;
    y=tmp-a/b*y;
    return g;
}
int main(){
    cin>>x>>y>>m>>n>>l;
    a=m-n;
    b=l;
    c=y-x;
    if(a<0)
    {
        a=-a;
        c=-c;
    }
    ngcd=ex_gcd(a,b,x,y);
    if (c%ngcd)
    {
        printf("Impossible");
        return 0;
    }
    x=x*(c/ngcd);//一组普通解
    y=y*(c/ngcd);
    t=b/ngcd;
    x=(x%t+t)%t<0?(x%t+t)%t+(t>0?t:-t):(x%t+t)%t;//最小非负整数解
    y=(c-a*x)/b;
    cout<<x;
    return 0;
}
```

### 素数

#### 素数判定

##### $$o(√n)素数判定$$

```cpp
bool isPrime(ll n)
{
    if(n==1)return false;
    for(ll i=2;i*i<=n;i++)
        if(n%i==0) return false;
    return true;
}
```

##### $$o(nlogn)埃氏筛$$

埃氏筛： o(n)预处理出 1 到 n 的素性情况。特判 1 不是素数。每发现一个素数后，就将它的倍数全部标记为非素数。每次遍历到的第一个未被标记的数，就是素数。从而 o(nlogn)预处理出 n 以内所有数的素性情况。
复杂度证明：调和级数 o(n(1/1+1/2+1/3+…+1/n))=o(nlogn)

```cpp
notPrime[1]=1;
    for(ll i=1;i<=n;i++)
        if(!notPrime[i])
        {
            for(ll j=2*i;j<=n;j+=i)
                notPrime[j]=1;
        }
```

##### $$o(n)线性欧拉筛$$

欧拉筛：o(n)预处理出 1 到 n 的素性情况。考虑埃氏筛，每个非素数都会被它的素因子标记一次，从而造成了不必要的多次标记。优化:让每个非素数只被它最小的素因子标记，这样优化到 o(n)。

```cpp
int prime[MAXN], vis[MAXN], tot;
void GetPrime(int N)
{
    vis[1] = 1;
    for(int i = 2; i <= N; i++)
        {
            if(!vis[i]) prime[++tot] = i;
            for(int j = 1; j <= tot && i * prime[j] <= N; j++)
                {
                    vis[i * prime[j]] = 1;
                    if(!(i % prime[j])) break;
                }
        }
}
```

##### $$o(tlogn)素数测试$$

$$
素数测试（Miller-Rabin算法）：随机算法，通过多次测试判断一个数是否为素数。\\
每次测试复杂度o(log \ n)，测试t次复杂度为o(t \ log \ n)。检测一次的正确率大概为1/4。\\
原理：\\
⚫ 费马小定理：若p为素数，且gcd(a,p)=1，则a
p−1≡1(mod p) 。\\
⚫ 二次探测：若p为素数，则方程x^2≡1(mod p) 的解为x=1或x=p−1。
$$

##### $$o(n^{1/4})大数分解$$

$$
大数分解（ Pollard-Rho算法）：将一个数分解质因子。传统算法可以通过o(√n) 将一个数分解质因子。\\
而Pollard-Rho算法算法可以做到o(n1/4)。\\
不过同样是随机算法，这个比较不稳定，不到万不得已要少用。
$$

#### 素数密度定理

$$
两个素数不会相距太远。可以理解为两个素数之间的距离为o(log²n)。\\
大概1e18以内的素数相距都不超过几百。
$$

#### 唯一分解定理

$$
所有正整数都可以分解为p1^{k1}p2^{k2}p3^{k3}…pn^{kn}的形式，其中pi为质数。
$$

$$
两个数的gcd就可以理解为，这两个数的每一位质因子的幂取一个最小值。\\
两个数的lcm就可以理解为，这两个数的每一位质因子的幂取一个最大值。
$$

### 积性函数问题

### 同余

[csdn]: https://blog.csdn.net/weixin_43785386/article/details/104086765

#### 定义

设m是正整数，若a和b是整数，且m|(a-b)，则称a和b模意义下同余，记作a ≡ b( mod m)。

$$
给定一个正整数m，如果两个整数a和b满足(a-b)能够被m整除，即(a-b)/m得到一个整数\\那么就称整数a与b对模m同余，记作a ≡ b (mod m)。
$$

#### 模意义下的运算

$$加法：(a+b) \% m $$
$$减法：(a-b \% m+m ) \% m$$
$$乘法：a*b \% m$$
$$除法：a*inv(b) \% m，其中 inv(b)是 b 模 m 意义下的逆元$$

$$如果m|p，那么a\%m==a\%p\%m成立$$

##### 快速乘

$$
原理和快速幂一样,只是乘法运算变成了加法运算,复杂度是O(\log N).
$$

```cpp
ll fmul(ll a,ll b,ll mod)
{
    ll sum=0,base=(a%mod+mod)%mod;
    while(b)
    {
        if(b%2)sum=(sum+base)%mod;
        base=(base+base)%mod;
        b/=2;
    }
    return sum;
}
```

$$
o(1)的快速乘模板
$$

```cpp
ll fmul(ll x,ll y,ll mod)
{
    ll tmp=(x*y-(ll)((long double)x/mod*y+1.0e-8)*mod);
    return tmp<0?tmp+mod:tmp;
}
```

```cpp
注意:o(1)快速乘因为原理是利用128位的long double，所以将__int128和其混用，并不能改善爆__int128的问题。
但是用o(log)的快速乘和__int128混用却可以解决模数大至__int128的乘法问题。
```

##### 二次剩余

$$
对于二次同余方程x2≡n(modp)\;x^{2}\equiv n \; (mod \; p)x
2
 ≡n(modp) \\若[gcd(n,p)=1]，且存在一个x满足该方程，则称n是模p意义下的二次剩余 \\若无解，则称n为p的二次非剩余。
$$

[csdn]: https://blog.csdn.net/weixin_43785386/article/details/104086765

### 逆元

#### 定义

$$a×a^{-1} ≡1(mod \ p)，则称a^{-1}是a在模p意义下的逆元。$$

一个数的倒数

$$
(a/b)\ mod \ m=(a/b)*1\ mod \ m=(a/b)*b*c \ mod \ m=a*c\ (mod\ m)\\
即a/b的模等于a*(b的逆元)的模；
$$

#### 费马小定理求解逆元

$$
费马小定理：a是不能被质数p整除的正整数，则有a^{p-1}\equiv 1 \ (mod \ p)\\
推导：a^{p-1} \equiv 1 \ (mod \ p) \\ \ \ \ \  a*a^{p-2}\equiv 1 \ (mod \ p) \\ a的逆元=a^{p-2}\ \ \ \\
限制:a和p要互质，p为质数\ \ \ \ \ \
$$

##### 代码实现

```cpp
long long quickpow(long long a,long long b)
{
    if(b<0)  return 0;
    long long ret=1;
    a%=mod;
    while(b)
    {
        if(b & 1 ) ret = ( ret *a ) % mod;
        b>>=1;
        a = (a * a)% mod;
    }
    return ret;
}
long long inv(long long a)
{
    return quickpow(a,mod-2);
}
```

```cpp
ans=a*quick(b,mod-2)%mod;
ll quick(ll x,ll k)
{
    ll ans = 1;
    while(k)
    {
        if(k%2!=0) ans = ans*x%mod;
        k=k>>1;
        x=x*x%mod;
    }
    return ans;
}
a/b的取模(限制:mod是素数且b不为mod的倍数)
```

#### 拓欧求逆元

$$
用费马小定理求逆元的时候，限制了模数p为素数。\\
为了处理模数不为素数的情况，我们需要另一个办法来求逆元。\\
$$

$$
考虑方程ax≡1(mod \ p)，\\
等价于方程ax+py=1。\\
所以就变成求解二元一次方程的问题了，求出的x就是逆元。\\
那么就用拓欧来求解。\\
a在模p意义下逆元存在的充要条件是：gcd(a,p)=1
$$

##### 代码实现

```cpp
ll ex_gcd(ll a,ll b,ll& x,ll& y)
{
if(b==0)
{
x=1;y=0;
return a;
}
ll ans=ex_gcd(b,a%b,x,y);
ll tmp=x;
x=y;
y=tmp-a/b*y;
return ans; }
ll inv(ll a,ll mod)//存在逆元条件：gcd(a,mod)=1
{
ll x,y;
ll g=ex_gcd(a,mod,x,y);
if(g!=1)return -1;
return (x%mod+mod)%mod;
}
```

### 数论四大定理

#### 威尔逊定理

$$
p可整除(p-1)!+1是p为质数的充要条件
$$

#### 欧拉定理

$$
若gcd(a,n)=1，则a^{φ(n)} ≡ 1( mod \ n) ，其中φ(n)为欧拉函数。
$$

##### 欧拉函数

$$
对于一个正整数n，小于n且和n互质的正整数（包括1）的个数，记作φ(n)。
$$

###### 通式

$$
φ(n)=n*(1-\frac{1}{p1})(1-\frac{1}{p2})(1-\frac{1}{p3})*(1-\frac{1}{p4})……(1-\frac{1}{pn})
$$

###### 欧拉函数模板（通式版）

```cpp
ll eular(ll n)
{
    ll ans = n;
    for(int i=2; i*i <= n; ++i)
    {
        if(n%i == 0)
        {
            ans = ans/i*(i-1);
            while(n%i == 0)
                n/=i;
        }
    }
    if(n > 1) ans = ans/n*(n-1);
    return ans;
}
```

###### 欧拉函数模板（打表版）

```cpp
ll maxn=100000;
ll E[maxn+5];
void euler()
{
    for(int i=2;i<maxn;i++){
        if(!E[i])
        for(int j=i;j<maxn;j+=i){
            if(!E[j])E[j]=j;
            E[j]=E[j]/i*(i-1);
        }
    }
}
```

###### 欧拉函数模板（欧拉筛素数）

```cpp
/*
特性 :
1.若a为质数,phi[a]=a-1
2.若a为质数,b mod a=0,phi[a*b]=phi[b]*a
3.若a,b互质,phi[a*b]=phi[a]*phi[b](当a为质数时,if b mod a!=0 ,phi[a*b]=phi[a]*phi[b])
*/
int m[n],phi[n],p[n],nump;
//m[i]标记i是否为素数,0为素数,1不为素数;p是存放素数的数组;nump是当前素数个数;phi[i]为欧拉函数
void euler()
{
        phi[1]=1;
    for (int i=2;i<=n;i++)
    {
        if (!m[i])//i为素数
        {
            p[++nump]=i;//将i加入素数数组p中
            phi[i]=i-1;//因为i是素数,由特性得知
        }
        for (int j=1;j<=nump&&p[j]*i<=n;j++)  //用当前已得到的素数数组p筛,筛去p[j]*i
        {
            m[p[j]*i]=1;//可以确定i*p[j]不是素数
            if (i%p[j]==0) //看p[j]是否是i的约数,因为素数p[j],等于判断i和p[j]是否互质
            {
                phi[p[j]*i]=phi[i]*p[j]; //特性2
                break;
            }
            else phi[p[j]*i]=phi[i]*(p[j]-1); //互质,特性3其,p[j]-1就是phi[p[j]]
        }
    }
}
```

#### 孙子定理

$$
中国剩余定理
$$

#### 费马小定理

$$
若p为素数，且gcd(a,p)=1，则a^{p-1}≡1(mod p) 。（其实当p为素数时，φ(n)=p−1）
$$

### 矩形和线性方程组

### 莫比乌斯

### 阶乘

### 三角形数

### 降幂公式

$$
a^b≡ a^{b*φ(n)} (mod \ n) \ \ \ \  a,n互质\\
a^b≡ a^b (mod \ n) \ \ \ \ b < φ(n)\\
a^b≡ a^{φ(n)*b+φ(n)} (mod \ n) \ \ \ \ b >= φ(n)
$$

$$
a^b≡a^{b \% \varphi(n) }(mod \ n)
$$

### 线性基

## 5.图论

### 图

#### 图的连通性

##### 连通图与连通分量

###### 连通图

$$
无向图 G 中，若对任意两点，从顶点 V_i 到顶点 V_j 有路径，则称 V_i 和 V_j 是连通的
$$

###### 连通分量

$$
无向图 G 的连通子图称为 G 的连通分量\\
任何连通图的连通分量只有一个，即其自身，而非连通的无向图有多个连通分量
$$

##### 强连通图与强连通分量

###### 强连通图

$$
有向图 G 中，若对任意两点，从顶点 V_i 到顶点 V_j，\\都存在从 V_i 到 V_j 以及从 V_j 到 V_i 的路径，则称 G 是强连通图
$$

###### 强连通分量

$$
有向图 G 的强连通子图称为 G 的强连通分量\\
强连通图只有一个强连通分量，即其自身，非强连通的有向图有多个强连通分量。
$$

##### 1.Tarjan 求强连通分量

###### 概述

```cpp
Tarjan 算法是基于对图深度优先搜索的算法，每个强连通分量为搜索树中的一棵子树。

搜索时，把当前搜索树中未处理的节点加入一个堆栈，回溯时可以判断栈顶到栈中的节点是否为一个强连通分量。
```

$$
定义 DFN(u) 为节点 u 搜索的次序编号（时间戳），即是第几个被搜索到的\\
Low(u) 为 u 或 u 的子树能够追溯到的最早的栈中节点的次序号。\\
$$

$$
每次找到一个新点 i，有：DFN(i)=low(i)\\

当点 u 与点 v 相连时，如果此时（时间为 DFN[u] 时）v不在栈中\\
u 的 low 值为两点的 low 值中较小的一个\\

即：low[u]=min(low[u],low[v])\\
$$

$$
当点 u 与点 v 相连时，如果此时（时间为 DFN[u] 时）v 在栈中\\
u 的 low 值为 u 的 low 值和 v 的 dfn 值中较小的一个\\

即：low[u]=min(low[u],dfn[v]) \\
$$

$$
当 DFN(u)=Low(u) 时，以 u 为根的搜索子树上所有节点是一个强连通分量。\\
$$

###### 时间复杂度

$$
运行 Tarjan 算法的过程中，每个顶点都被访问了一次，且只进出了一次堆栈，\\
每条边也只被访问了一次，所以该算法的时间复杂度为 O(N+M)。
$$

###### 代码

```cpp
#include<bits/stdc++.h>
#define N 20001
using namespace std;
int n,m;
vector<int> G[N];
stack<int> S;
int dfn[N],low[N];
bool vis[N];//标记数组
int scc[N];//记录结点i属于哪个强连通分量
int index;//时间戳
int sccnum;//记录强连通分量个数
void Tarjan(int x){
    vis[x]=true;
    dfn[x]=low[x]=++index;//每找到一个新点，纪录当前节点的时间戳
    S.push(x);//当前结点入栈

    for(int i=0;i<G[x].size();i++){//遍历整个栈
        int y=G[x][i];//当前结点的下一结点
        if(vis[y]==false){//若未被访问过
            Tarjan(y);
            low[x]=min(low[x],low[y]);
        }
        else if(!scc[y])//若已被访问过，且不属于任何一个连通分量
            low[x]=min(low[x],dfn[y]);
    }

    if(dfn[x]==low[x]){//满足强连通分量要求
        sccnum++;//记录强连通分量个数

        while(true){//记录元素属于第几个强连通分量
            int temp=S.top();
            S.pop();
            scc[temp]=sccnum;
            if(temp==x)
                break;
        }
    }
}
void init()
{
        for(int i=0;i<n;i++)
            G[i].clear();
        while(m--)
        {
            int x,y;
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
        }
        sccnum=0;
        index=0;
        for(int i=0;i<=n;i++)vis[i]=dfn[i]=low[i]=scc[i]=0;
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        init();

        for(int i=0;i<n;i++)
            if(vis[i]==false)
                Tarjan(i);

        for(int i=0;i<n;i++)
            printf("%d号点属于%d分量\n",i,scc[i]);
    }
    return 0;
}
```

### 树

### 最短路径问题

#### 单源最短路问题

##### Bellman-Ford 算法

##### Dijkstra 算法

```c++
int k;
const int N=100005;
int dis[N],vis[N],head[N];
priority_queue<pair<int,int> >q;
struct node
{
    int to,net,v;
}e[N*2];
void init()//切记要初始化
{
    k=0;
    memset(head,0,sizeof(head));
}
void add(int u,int v,int w)
{
    e[++k].to=v;
    e[k].net=head[u];//记录上一个连通u的路径
    e[k].v=w;
    head[u]=k;
}
void dijkstra(int id)
{
    memset(dis,inf,sizeof(dis));//dis[i]:点1到点i的最短路径
    memset(vis,0,sizeof(vis));
    dis[id]=0;
    q.push(make_pair(0,id));
    while(!q.empty())
    {
        int x=q.top().second;
        q.pop();
        if(vis[x]==1)continue;
        vis[x]=1;
        for(int i=head[x];i;i=e[i].net)
        {
            int v=e[i].to;
            if(dis[v]>dis[x]+e[i].v)
            {
                dis[v]=dis[x]+e[i].v;
                q.push(make_pair(-dis[v],v));
            }
        }
    }
}
```

#### 任意两点间的最短路问题

##### Floyd-Warshall 算法

### 二分图

#### 定义

​ 设 G=(V,E)是一个无向图，如果顶点 V 可分割为两个互不相交的子集(A,B)，并且图中的每条边（i，j）所关联的两个顶点 i 和 j 分别属于这两个不同的顶点集(i in A,j in B)，则称图 G 为一个二分图。简单来说，如果图中点可以被分为两组，并且使得所有边都跨越组的边界，则这就是一个二分图。准确地说：把一个图的顶点划分为两个不相交子集 ，使得每一条边都分别连接两个集合中的顶点。如果存在这样的划分，则此图为一个二分图。（如下图）
![img](https://img-blog.csdnimg.cn/20190609121130545.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI2ODIyMDI5,size_16,color_FFFFFF,t_70)

#### 相关概念

##### 匹配

$$给定一个二分图G，在G的一个子图M中，M的边集{E}中的任意两条边都不依附于同一个顶点，则称M是一个匹配。$$

##### 最大匹配

$$包含的边数最多的匹配。$$

##### 多重匹配

$$即一个左节点或右节点可以与多个右节点或左节点相连。这样的匹配叫做多重匹配。$$

##### 完美匹配（完备匹配）

$$所有的点都在匹配边上的匹配。$$

##### 最佳匹配

$$如果G为加权二分图,则权值和最大的完备匹配称为最佳匹配。$$

#### 二分图匹配算法

##### 匈牙利算法 O(V\*E)

$$
求二分图最大匹配
$$

###### 交替路

$$
从一个未匹配点出发，依次经过非匹配边、匹配边、非匹配边...形成的路径叫交替路。
$$

###### 增广路

$$
从一个未匹配点出发，走交替路，如果途径另一个未匹配点（出发的点不算），则这条交替路称为增广路。\\（agumenting path）
$$

```c++
const int N=1e5+5;
int n,m,e,ans=0,link[N];
bool vis[N];//link[v]表示v连向的点， vis表示某个点是否被访问过。
vector<int> g[4*N];//vector存图

bool dfs(int x)
{
    for(int i=0;i<g[x].size();i++)
    {
        int v=g[x][i];
        //如果没被访问
        if (!vis[v])
        {
            vis[v]=1;
            if(link[v]==-1||dfs(link[v]))   //若是v还没有被配对，就把v配对给x，否则让link[v]腾出v给它。
            {
                link[v]=x; //把v连接到x
                return 1; //表示x能配对到点
            }
        }
    }
    return 0; //x不能配对到点
}

int main()
{
    memset(link,-1,sizeof(link));
    read(n);//A集合数目
    read(e);//边数
    m=n;//B集合数目
    for(int i=1;i<=e;i++)
    {
        int u,v;
        read(u);
        read(v);
        if(u>n||v>m||u<1||v<1)continue;
        g[u].push_back(v+n);//建边，注意一定要是单向边
    }
    for(int i=1;i<=n;i++)
    {
        memset(vis,0,sizeof(vis));
        if(dfs(i))ans++; //如果能匹配到答案加一
    }
    cout<<ans;
    return 0;
}
```

##### HK 算法 O(sqrt(n) \*E)

##### 网络流最大流

##### KM 算法(优化版本 O（n^3))

### 网络流

```cpp
#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f

template<class T>void read(T &x)
{
    x=0;
    int f=0;
    char ch=getchar();
    while(ch<'0'||ch>'9')
    {
        f|=(ch=='-');
        ch=getchar();
    }
    while(ch>='0'&&ch<='9')
    {
        x=(x<<1)+(x<<3)+(ch^48);
        ch=getchar();
    }
    x=f?-x:x;
    return;
}


const int Ni = 30;    //总点个数，要改哦！
const int MAX = 1<<26;
struct Edge
{
    int u,v,c;
    int next;
};
struct Dinic
{
    int n,m;
    int edn;//边数
    int p[Ni];//父亲
    int d[Ni];
    int sp,tp;//原点，汇点
    Edge edge[6*1000];

    void init(int sp,int tp)
    {
        this->sp=sp;
        this->tp=tp;
        edn=0;
        memset(p,-1,sizeof(p));
    }
    void addedge(int u,int v,int c)
    {
        edge[edn].u=u;
        edge[edn].v=v;
        edge[edn].c=c;
        edge[edn].next=p[u];
        p[u]=edn++;
        edge[edn].u=v;
        edge[edn].v=u;
        edge[edn].c=0;//改成0就是原图单向边，改成c就是双向边
        edge[edn].next=p[v];
        p[v]=edn++;

    }
    int bfs()
    {
        queue <int> q;
        while(!q.empty())
            q.pop();
        memset(d,-1,sizeof(d));
        d[sp]=0;
        q.push(sp);

        while(!q.empty())
        {
            int cur=q.front();
            q.pop();
            for(int i=p[cur]; i!=-1; i=edge[i].next)
            {
                int u=edge[i].v;
                if(d[u]==-1 && edge[i].c>0)
                {
                    d[u]=d[cur]+1;
                    q.push(u);
                }
            }
        }
        return d[tp] != -1;
    }
    int dfs(int a,int b)
    {
        int r=0;
        if(a==tp)
            return b;
        for(int i=p[a]; i!=-1 && r<b; i=edge[i].next)
        {
            int u=edge[i].v;
            if(edge[i].c>0 && d[u]==d[a]+1)
            {
                int x=min(edge[i].c,b-r);
                x=dfs(u,x);
                r+=x;
                edge[i].c-=x;
                edge[i^1].c+=x;
            }
        }
        if(!r)
            d[a]=-2;
        return r;
    }

    int Maxflow()
    {
        int total=0,t;
        while(bfs())
        {
            while(t=dfs(sp,MAX))
                total+=t;
        }
        return total;
    }
} dinic;
int T,a,b,c;
int main()
{
    int n,m,k;
    read(T);
    k=0;
    while(T--)
    {
        read(n);
        read(m);
        dinic.init(1,n);
        int i,j;
        for(i=1;i<=m; i++)
        {
            read(a);
            read(b);
            read(c);
            dinic.addedge(a,b,c);
        }
        printf("Case %d: %d\n",++k,dinic.Maxflow());
    }
    return 0;
}


```

### 仙人掌图

### 差分约束

### 2 SAT

## 6.组合数学

### 一.排列组合

#### 1.排列

$$
从n个不同的元素中任取m(m≤n)个元素的所有排列的个数,叫做排列数\\记作P(n,m)或A_n^m=\frac{n!}{(n-m)!}
$$

$$
而如果把选出的m个元素放到圆上，就是圆排列,个数为\frac{n!}{m\cdot (n-m)!}
$$

#### 2.组合

$$
从n个不同的元素中任取m(m≤n)个元素的方案数,叫做排列数，记作\binom nm或C_n^m=\frac{n!}{m!(n-m)!}
$$

##### 大数组合数（模板）

```cpp
#define mod 1e9 + 7
vector<int> nPrime(int n) {
    int k = 2;
    vector<int> v;
    while (k <= n) {
        bool isPrime = true;
        int t = sqrt(k);
        for (; t > 1; t--) {
            if (k%t == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime)
            v.push_back(k);
        k++;
    }
    return v;
}
int dPrime(int n, int m) {
    int pow = 0;
    while (n >= m) {
        int temp = n / m;
        pow += temp;
        n = temp;
    }
    return pow;
}
int C(int n, int m) {
    long long ans = 1;
    vector<int> v = nPrime(n);
    for (int i = 0; i < v.size(); i++) {
        int k = v.at(i),pow;
        pow = dPrime(n, k) - dPrime(m, k) - dPrime(n - m, k);
        for (int j = 0; j < pow; j++) {
            ans *= k;
            ans %= (int)mod;
        }
    }
    return (int)ans;
}

int main() {
    int n, m;
    while (cin >> n >> m) {
        cout << C(n, m) << endl;
    }
    return 0;
}
```

#### 3.多重集排列

​ 设$a_1,a_2\cdots a_n$是互不相同的元素
​ (1)从$\{K_1\cdot a_1,\cdots,K_n\cdots a_n \}$中选$r$个元素作为排列,当满足$\forall i K_i\ge r$时，方案数是$n^r$
​ (2)从$\{K_1\cdot a_1,\cdots,K_n\cdots a_n \}$中选所有元素元素作为排列,方案数是$\frac{(K_1+\cdots+K_n)!}{K_1!\cdots K_n!}$

#### 4.多重集组合

​ 设$a_1,a_2\cdots a_n$是互不相同的元素
​ (1)从$\{K_1\cdot a_1,\cdots,K_n\cdots a_n \}$中选$r$个元素,当满足$\forall i K_i\ge r$时，方案数是$C_{n+r-1}^r$
​ (2)从$\{K_1\cdot a_1,\cdots,K_n\cdots a_n \}$中选$r$个元素,不满足$\forall i K_i\ge r$时，一般用 DP 或者生成函数做

#### 5.二项式定理及其扩展

$$
(a+b)^n=a^n+C_n^1a^{n-1}b+\cdots + C_n^{n-1}ab^{n-1}+b^n=\sum_{i=0}^{n}C_n^ia^{n-i}b^i
$$

$$
(a+b)^\alpha=\sum_{i=0}^{\infty}\binom{\alpha}{i}a^{\alpha-i}b^{i},其中\binom {\alpha}{i}=\frac{(\alpha)\cdot(\alpha-1)\cdots(\alpha-i+1)}{i!}
$$

$$
(a+b)^{-\alpha}=\sum_{i=0}^{\infty}\binom{-\alpha}{i}a^{\alpha-i}b^i=\sum_{i=0}^{\infty}(-1)^i\binom{\alpha+i-1}{i}a^{\alpha-i}b^i
$$

#### 6.常用组合数公式

​ $C_n^k=C_{n-1}^{k-1}+C_{n-1}^k$

​ $C_n^k=C_n^{n-k}$

​ $C_n^k=\frac{n-k+1}{k}C_n^{k-1}$

​ $\sum_{i=0}^nC_n^i=2^i$

​ $\sum _{i=0}^{n}(C_n^i)^2=C_{2n}^n$

​ $\sum_{i=0}^{n}C_{x+i}^x=C_{n+x+1}^n$

​ $F_{2n}=C_{2n}^0+C_{2n-1}^1+\cdots+C_n^n,\quad F_{2n+1}=C_{2n+1}^0+\cdots+C_{n+1}^n$

### 二.线性递推

​ 满足$F_n=a_1F_{n-1}+a_2F_{n-2}+\cdots+a_kF_{n-k}$的$F$称作线性递推数列，他有通项公式：

$$
F_n=c_1q_1^n+\cdots +c_kq_k^{n}
$$

​ 其中$q_i$时方程$q^k-a_1q^{k-1}-\cdots -a_kq^0=0$的解，而$c_i$是常数，由初始值决定

​ 一般解不出方程的或者甚至不确定$a_i$的值但感觉是线性递推的可以直接上 BM 板子求第$n$项，复杂度可以$O(k^2\log n)$

### 三.特殊计数数列

#### 1.斐波那契数列

$$
F_n=F_{n-1}+F_{n-2},n\ge2;F_0=0,F_1=1\\
F_n=\frac{1}{\sqrt5}[(\frac{\sqrt 5+1}{2})^n-(\frac{\sqrt 5-1}{2})^n]\\
\gcd(F_i,F_j)=F_{\gcd(i,j)}\\
\sum_{i=0}^nF(i)=F(n+2)-1\\
\sum_{i=0}^nF^2(i)=F(n)F(n+1)\\
\sum_{i=0}^{n}F(2i-1)=F(2n)\\
F(n)=F(m)F(n-m+1)+F(m-1)F(n-m),n\ge m\\
F(n)^2+(-1)^n=F(n-1)F(n+1)\\
F_{2n}=C_{2n}^0+C_{2n-1}^1+\cdots+C_n^n,\quad F_{2n+1}=C_{2n+1}^0+\cdots+C_{n+1}^n
$$

#### 2.卡特兰数

$$
C_n=\sum_{i=0}^{n-1}C_iC_{n-i-1},n\ge2;C_0=C_1=1\\
C=1,1,2,5,14,42,132,\cdots\\
C_n=\frac{1}{n+1}C_{2n}^n=C_{2n}^n-C_{2n}^{n-1}=\frac{4n-2}{n+1}C_{n-1}\\
$$

#### 3.贝尔数

​ 将 n 个不同的元素划分到任意个集合的方案数

$$
Bell_{n+1}=\sum_{i=0}^{n}\binom{n}{i}Bell_i,n\ge1;Bell_0=1\\
Bell=1,1,2,5,15,52,\cdots
$$

##### 贝尔数（模板）

```cpp
typedef long long ll;
const ll mod=1000;
int Bell[2020];
int num[2020][2020];
void init(){
    int i,j;
    for(i=1;i<2010;i++){
        num[i][0]=0,num[i][1]=1;
    }
    for(i=2;i<2010;i++){
        for(j=1;j<2010;j++){
            if(i==j){
                num[i][i]=1;
                continue;
            }
            num[i][j]=(num[i-1][j-1]+num[i-1][j]*j)%mod;
        }
    }
    memset(Bell,0,sizeof(Bell));
    for(i=1;i<=2000;i++){
        for(j=1;j<=i;j++){
                Bell[i]+=num[i][j];
                Bell[i]%=mod;
            }
    }
}
```

#### 4.第一类斯特林数

​将 n 个不同元素构成到 k 个圆排列的方案数

$$
\begin{align*}
&(1)  \begin{bmatrix} n\\k \end{bmatrix}=s(n,k)->s_u(n,k)  \\
&(2)s(n,k)=s(n-1,k-1)+(n-1)\cdot s(n-1,k)\\
&(3)s(n,k)=\begin{cases}0 &  n < k \\ 1 & n=k \\ 0 & n > 0 \ ∧\ k=0 \end{cases}\\
&(4)s_s(n,k)=(-1)^{n-k}s_u(n,k)\\
&(5)x^{\overline n}=(x)(x+1)(x+2)\cdots(x+n-1)=\sum_{i=1}^{n}s_u(n,i)x^i\\
&(6)x^{\underline n}=(x)(x-1)(x-2)\cdots(x-n+1)=\sum_{i=1}^{n}(-1)^{n-k}s_u(n,i)x^i=\sum_{i=1}^{n}s_s(n,i)x^i\\
\end{align*}
$$

#### 5.第二类斯特林数

​ n 个不同元素划分到恰好 k 个非空集合的方案数（n 个不同小球放入 k 个相同盒子，不能有空盒）

$$
\begin{align*}

&(1)\begin{Bmatrix}n\\k\end{Bmatrix}=S(n,k)\\
 &(2)S(n,k)=S(n-1,k-1)+kS(n-1,k)\\
 &(3)s(n,k)=\begin{cases}0 &  n < k \\ 1 & n=k \\ 0 & n > 0\ ∧\ k=0 \end{cases}\\
 &(4)x^n=\sum_{i=0}^nS(n,i)x^{\underline i}\\
 &(5)Bell_n=\sum_{i=1}^kS(n,i)\\
\end{align*}
$$

​ 关于斯特林数，建议阅读<https://www.cnblogs.com/Iking123/p/13308661.html>

#### 6.伯努利数

$$
\sum_{i=0}^n\binom{n+1}{i}B_i=0,n\ge1;B_0=1\\
B=1,-\frac{1}{2},\frac16,0,\frac1{30},\cdots\\
S_k(n)=\sum_{i=0}^{n-1}i^k=\frac{1}{k+1}\sum_{i=0}^kC_{k+1}^iB_in^{k+1-i}
$$

### 四.容斥与反演

#### 1.容斥

​ 设$A_i$是几何$S$的子集，则有：

$$
|A_1\cup A_2 \cdots\cup A_n |= \sum_{i=1}^n|A_i|-\sum_{1\le i<j\le n}|A_i\cap A_j|+\cdots+(-1)^{n-1}|A_1\cap A_2\cdots \cap A_n|
$$

​

#### 2.二项式反演

​ 若函数$f$和$g$满足

$$
f(n)=\sum_{i=0}^n\binom{n}{i}g(i)
$$

​ 那么

$$
g(n)=\sum_{i=0}^n(-1)^{n-i}\binom{n}{i}f(i)
$$

#### 3.莫比乌斯反演

​ 一般不用函数$f$和$g$来推，而是用$\sum_{d|n}\mu(i)=[n=1]$直接套，具体怎么玩就在数论里学啦

#### 4.子集反演

​ 就是容斥，总之若

$$
f(S)=\sum_{T \subseteq S }g(T)\\
$$

​ 则：

$$
g(S)=\sum_{T\subseteq S}(-1)^{|S|-|T|}f(T)
$$

#### 5.斯特林反演

​ 并不会

### 五.生成函数和多项式

#### 1.多项式

​ 不用多说了吧，就是$F(x)=a_0+a_1x+\cdots+a_nx^n$这种的，多项式除了有加减法外，还有乘法，除法，求导，积分，求逆元，开 k 次根，还能成为指数$(e^{F(x)})$或对数$(\ln F(x))$,总之有很多黑科技。而$[x^n]F(x)$表示这个多项式的$x^n$项系数

​ 重点：多项式乘法

$$
F(x)*G(x)=(\sum_{i\ge0}f_ix^i)*(\sum_{i\ge0}g_ix^i)=\sum_{i\ge0}\sum_{j=0}^if_jx^{j}\cdot g_{i-j}\cdot x^{i-j}
$$

#### 2.FFT 和 NTT

​ 就是用来算卷积的,或者说是多项式乘法,也就是在$O(n\log n)$时间里对每个$i\in[0,n)$,求$C_i=\sum_{j=0}^iA_j*B_{i-j}$

#### 3.生成函数

​ 分为一般生成函数（OGF）（也叫母函数）和指数生成函数（EGF）

##### 一般生成函数

​ 对于一个数列$\{a_0,a_1,a_2\cdots\}$来说，他的生成函数就是$F(x)=a_0+a_1x+a_2x^2+\cdots$这样的一个幂级数

​ 比如斐波那契数列$\{1,1,2,3,5,\cdots \}$就是$Fib(x)=1+x+2x^2+3x^3+5x^4+\cdots$

​ 一般来说幂级数可以是一个正常函数的展开，比如(用泰勒展开或者等比数列求和都可以简单证明)：

$$
\{1,1,1,1\cdots \}=>1+x+x^2+\cdots=\frac{1}{1-x}
$$

​ 常见的还有:

$$
\ln(1+x)=x-\frac {x^2}2+\frac {x^3}3-\cdots+(-1)^{n-1}\frac{x^n}{n}+\cdots\\
e^x=1+x+\frac {x^2}{2}+\cdots +\frac{x^n}{n!}+\cdots\\
\sin x=x-\frac{x^3}{3!}+\cdots+(-1)^n\frac{x^{2n+1}}{(2n+1)!}+\cdots\\
\cos x=1-\frac{x^2}{2!}+\cdots+(-1)^n\frac{x^{2n}}{(2n)!}+\cdots\\
\frac{x(x+1)}{(1-x)^3}=x+4x^2+9x^3+\cdots+n^2x^2+\cdots
$$

​ 而斐波那契数列的生成函数也是有对应的函数的:

$$
Fib(x)=1+x+2x^2+3x^3+5x^4+\cdots\\
x^2Fib(x)=0+0x+x^2+x^3+2x^4cdots\\
xFib(x)+x^2Fib(x)=0+x+2x^2+3x^3+5x^4+\cdots\\
1+xFib(x)+x^2Fib(x)=Fib(x)\\
Fib(x)=\frac{1}{1-x-x^2}
$$

​ 怎么求生成函数并不是很重要,重要的是利用生成函数解决问题.

​ **一道经典背包题:**

​ 有许多小球，其中重量为 1g、2g、3g、5g 的分别有 3、2、1、2 个，球上**没有**标号，也就是相同重量的球之间没有差别，问有多少种方案可以拿出 k 克重的球。

​ 直接 dp 可能大家都会了，就是对于每种重量的小球，枚举一次用几个

​ 但现在考虑另一种 dp，令$dp[1g][i]$表示只用 1g 的球拿出重量为$i$的方案数，$dp[2g][i]$也类似，显然

$$
dp[1g]=\{1,1,1,1,0,0\cdots \}\\dp[2g]=\{1,0,1,0,1,0,0,0\cdots\}\\

dp[1g+2g][i]=\sum_{j=0}^i dp[1g][j]*dp[2g][i-j]
$$

​ 上面这个式子很像多项式乘法，事实上，给$dp[1g]和dp[2g]$分别做一个生成函数:

$$
F_{1g}(x)=1+x+x^2+x^3\\
F_{2g}(x)=1+x^2+x^4\\
$$

​ 注意其中$x^n$项的系数就表示取出重量为 n 的方案数

$$
F_{1g+2g}(x)=F_{1g}*F_{2g}=(1+x+x^2+x^3)(1+x^2+x^4)=1+x+2x^2+2x^3+2x^4+2x^5+x^6+x^7\\
$$

​ $F_{1g+2g}$就是只拿 1g 和 2g 重的球的方案数了，而问题的答案就是$F_{1g}*F_{2g}*F_{3g}*F_{4g}$的 k 次方项系数

​ **用母函数求解通项公式：**

​ 以卡特兰数为例:

$$
C(x)=1+x+2x^2+5x^3+\cdots=\sum_{i\ge0}C_ix^i\\
\sum_{i\ge0}C_ix^i=1+\sum_{i\ge1}C_ix^i=1+\sum_{i\ge1}\sum_{j=0}^{i-1}C_jC_{i-j-1}x^i\\
=1+x\sum_{i\ge0}\sum_{j=0}^iC_jC_{i-j}x^i=1+xC(x)*C(x)\\xC^2(x)-C(x)+1=0\\
C(x)=\frac{1\pm\sqrt{1-4x}}{2x}\\
C(0)=C_0=1=\lim _{x\to 0}\frac{1\pm\sqrt{1-4x}}{2x}\\
C(x)=\frac{1-\sqrt{1-4x}}{2x}\\
$$

$$
(1-4x)^{\frac12}=\sum_{i=0}^\infty\binom{\frac12}{i}(-4x)^i\\
=1+\sum_{i=1}^\infty\frac{\frac12\cdot(-\frac12)\cdot(-\frac32)\cdots(-\frac{2i-3}{2})}{i!}(-4)^ix^i\\
=1+\sum_{i=1}^\infty \frac{1\cdot3\cdots(2i-3)}{2^ii!}(-1)^{2i-1}4^ix^i\\
=1-\sum_{i=1}^\infty \frac{(2i-2)!}{2^ii!\cdot(2\cdot4\cdots(2i-2))}4^ix^i\\
=1-\sum_{i=1}^\infty \frac{(2i-2)!}{2^ii!\cdot2^{i-1}\cdot(i-1)!}4^ix^i\\
=1-\sum_{i=1}^\infty\frac{2\cdot(2i-2)!}{i!(i-1)!}x^i\\
$$

$$
C(x)=\frac{1-\sqrt {1-4x}}{2x}=\frac{1-1+\sum_{i=1}^\infty\frac{2\cdot(2i-2)!}{i!(i-1)!}x^i}{2x}\\
=\sum_{i=1}^\infty\frac{(2i-2)!}{i!(i-1)!}x^{i-1}=\sum_{i=1}^\infty\frac{(2i-2)!}{i\cdot(i-1)!(i-1)!}x^{i-1}\\
=\sum_{i=1}^\infty\frac{1}{i}C_{2i-2}^{i-1}x^{i-1}=\sum_{i=0}^\infty \frac{C_{2i}^{i}}{i+1}x^i\\
C_n=\frac{C_{2n}^n}{n+1}
$$

**指数生成函数：**

​ 对于一个数列$\{a_0,a_1,a_2\cdots\}$来说，他的生成函数就是$\hat F(x)=a_0+a_1\frac x{1!}+a_2\frac{x^2}{2!}+\cdots$这样的一个幂级数,实际上$F(x)$也是$\{\frac{a_0}{0!},\frac{a_1}{1!},\frac{a_2}{2!},\cdots \}$的一般生成函数

​ 比如$\{1,1,1,1,1\cdots\}=>1+x+\frac{x^2}{2!}+\frac{x^3}{3!}+\cdots=e^x$

**经典例题：**

​ 用红黄蓝绿给 n 个格子染色，要求红色和绿色必须是偶数个，求方案数。

​ 由于问题是排列数，为了避免重复的问题，所以选用指数生成函数

​ 于是构造指数型生成函数

$$
r(x)=g(x)=1+\frac{x^2}{2!}+\frac{x^4}{4!}+\cdots=\frac{e^x-e^{-x}}{2}\\
y(x)=b(x)=1+x+\frac{x^2}{2!}+\frac{x^3}{3!}+\cdots=e^x
$$

​ 然后把他们乘起来：

$$
r(x)*g(x)*y(x)*b(x)=\frac{(e^x-e^{-x})^2e^{2x}}{4}=\frac{e^{4x}-2e^{2x}+1}{4}\\
=\frac{1+\sum_{i=0}^\infty\frac{(4x)^i-2(2x)^i}{i!}}{4}=\frac14+\sum_{i=0}^\infty\frac{4^i-2^{i+1}}{4}\cdot\frac{x^i}{i!}\\
$$

​ 于是答案就是$\frac{4^n-2^{n+1}+[n==0]}{4}$

**用来快速求伯努利数：**

$$
\sum_{i=0}^n\binom{n+1}{i}B_i=0,n\ge1;B_0=1\\
B_n=-\frac{1}{n+1}\sum_{i=0}^{n-1}\binom{n+1}{i}B_i\\
\hat B(x)=\sum_{i\ge0}\frac{B_ix^i}{i!}=1+\sum_{i\ge1}\frac{B_ix^i}{i!}\\
=1+\sum_{i\ge1}\frac{x^i}{i!}\cdot(-\frac{1}{i+1}\sum_{j=0}^{i-1}\binom{i+1}{j}B_j)\\
=1-\sum_{i\ge1}\frac{x^i}{(i+1)!}\sum_{j=0}^{i-1}\frac{(i+1)!}{j!(i+1-j)!}B_j\\
=1-\sum_{i\ge1}\sum_{j=0}^{i-1}\frac{B_jx^j}{j!}*\frac{x^{i-j}}{(i+1-j)!}\\
=1-\sum_{i\ge0}\sum_{j=0}^{i}\frac{B_jx^j}{j!}*\frac{x^{i+1-j}}{(i+2-j)!}\\
=1-x\sum_{i\ge0}\sum_{j=0}^{i}\frac{B_jx^j}{j!}*\frac{x^{i-j}}{(i+2-j)!}\\
=1-xB(x)T(x)\\
$$

$$
T(x)=\sum_{i\ge0}\frac{x^i}{(i+2)!}\\
x^2T(x)=\sum_{i\ge0}\frac{x^{i+2}}{(i+2)!}=\sum_{i\ge2}\frac{x^i}{i!}=\sum_{i\ge0}\frac{x^i}{i!}-1-x=e^x-1-x\\
T(x)=\frac{e^x-1-x}{x^2}\\
$$

$$
B(x)=1-xB(x)\frac{e^x-1-x}{x^2}\\
xB(x)=x-B(x)(e^x-1-x)\\
B(x)=\frac{x}{e^x-1}\\
=\frac{x}{\sum_{i\ge0}\frac{x^i}{i!}-1}\\
=\frac{x}{\sum_{i\ge1}\frac{x^i}{i!}}\\
=\frac{1}{\sum_{i\ge1}\frac{x^{i-1}}{i!}}\\
=\frac{1}{\sum_{i\ge0}\frac{x^i}{(i+1)!}}\\
$$

​ 接下来只要多项式求逆就可以$O(N\log N)$预处理出伯努利数,注意得到的 n 次项系数并不是伯努利数，因为这是指数生成函数，所以还要乘$n!$

### 六.Polya 计数

​ 具体的证明不是很会，主要是用来求环上本质不同的染色方案

​ 首先基本的定义

​ **置换：**

​ 置换是一个满射函数$f$，用前 n 个正整数组成的集合作为定义域和值域，简单理解就是 n 个人站成一排，经过一次置换后，第$i$个人变到了$p_i$位置上。一般用一个$2\times n$的矩阵表示

$$
\begin{bmatrix}1&2&3&\cdots&n\\p_1&p_2&p_3&\cdots&p_n \end{bmatrix}
$$

​ 由于置换是一个满射，所以显然$p$是一个排列

​ 比如一个大小为 4，可以翻转的环（或者可以称为正方形），就有一下几种置换:

$$
旋转：\{1,2,3,4\},\{2,3,4,1\},\{3,4,1,2\},\{4,1,2,3\}\\
翻转：\{1,3,2,4\},\{2,1,4,3\},\{3,2,1,4\},\{4,3,2,1\}\\
$$

​ 这 8 个置换可以称作置换群

**burnside 引理:**

$$
方案数=\frac{\sum_{置换群f}有多少种染色方案使得，经过置换后颜色也不会变}{置换群大小}
$$

**Polya 定理:**

$$
方案数=\frac{\sum_{置换群f}颜色数c^{置换上有多上个环}}{置换群大小}
$$

#### 代码（环）

```cpp
int gcd(int a,int b){return b==0?a:gcd(b,a%b);}
long long rotate(int c,int n){
    int i;
    long long sum=0;
    for(i=1;i<=n;i++){
        sum+=pow(c,gcd(i,n));
    }
    return sum;
}
 long long turn(long long c,int n){
    long long sum;
    if(n%2)sum=n*pow(c,(n+1)/2);
      else sum=n/2*((pow(c,n/2))+pow(c,(n+2)/2));
    return sum;
}
long long polya(int c,int n){//颜色数c，涂色块数n
    if(n==0) return 0;
    long long sum=0;
    sum+=rotate(c,n);
    sum+=turn(c,n);
    n*=2;
    return sum/n;
}
```

### stirling 数

### 鸽巢原理

### 康托展开与逆康托展开

举例而言，对于 1 ~ 4 的一个全排列 [1, 2, 3, 4] 和 [4, 3, 2, 1]，我们知道，从字典序而言，前者是该全排列集的第一个，后者是该集的最后一个。那么，所谓康托展开，即给定一个 n 位数的全排列，我们可以根据康托展开公式确定其应当是字典序中的第“几”个全排列。
由于康托展开计算的是某个全排列方式在该全排列集合中的字典序(或者说是排名)，其映射关系唯一且单调，故该映射关系是可逆的。即，我们给定一个全排列的所有字符，以及某个字典序号，我们可以利用逆康托展开得到相应的那个全排列。

#### 康托展开

$$
给定一个全排列，计算其字典序。\\
直观起见，我们举例[ 2, 3, 4, 1]来说明康托展开的运作步骤：\\
命所求字典序为 rank=0\\
1.第 1 位是 2， 那么以 1 打头的所有全排列一定排在这个全排列之前\\
那么以 1 打头的全排列有 (3!) = 6种，rank=rank+1∗3!=6。\\  .\\
2.第 2 位是 3，那么以 1 与 2 作为第二位的所有全排列一定在这个圈排列之前。\\
不过我们已经让 2 打头了，因此不需要再考虑 2 占第二位的情况，只需要计算 1 占第二位的情况。\\
rank=rank+1∗2!=8rank=rank+1∗2!=8。\\  .\\
3.第三位是 4，同时，我们计算以 1 占第三位的所有情况。rank=rank+1∗1!=9。\\  .\\
4.最后一位，是不需要判定的，因为前 n−1 位给定后，第 n 位自定。\\  .\\
当然，为了也适应前面推导，可以记 rank=rank+0∗0!=9。\\
由是，排在 [ 2, 3, 4, 1] 之前的全排列共有 9 个，那么 [ 2, 3, 4, 1] 应当是第 10 个全排列。总结康托展开公式为：\\
rank=an(n−1)!+an−1(n−2)!+⋯+a10!\\
其中，ai表示原排列中，排在下标 i 后面的，比下标 i 的字符还小的字符个数。\\
当然，如果排名是从 1 开始的话，最终结果应当再 + 1。
$$

#### 康托展开代码

```cpp
//对前 10 个自然数(0 ~ 9)的阶乘存入表
//以免去对其额外的计算
const int fact[10] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};
/**
 * @brief 康拓展开
 *
 * @param[in] permutation 输入的一个全排列
 * @param[out] num 输入的康拓映射，即是第几个全排列
 */
int contor(const vector<int>& permutation) {
    int num = 0;
    int len = permutation.size();
    for (int i = 0; i < len; ++i) {
        int cnt = 0; // 在 i 之后，比 i 还小的有几个
        for (int j = i + 1; j < len; ++j)
            if (permutation[i] > permutation[j]) ++cnt;
        num += cnt * fact[len - i - 1];
    }
    return num + 1;
}
```

#### 逆康拓展开

$$
同样以[2, 3, 4, 1]为例，以说明逆康拓展开的执行方法。\\
这里输入和输出互反，同时，我们还需要输入全排列的字符个数(否则有无穷多个解)。\\
给定，字符个数 4，字典序序号 10，首先字典序 - 1 得到排在该字典序前的全排列个数，然后：\\  .\\
①9 / 3! 结果，商 1 余 3。\\
说明首位要余出一个给 当前没用过的，最小的一个字符，因为它们占据了前 6 个排序。\\
这里 “1” 没有用过，又是最小的字符，因此，我们应当使用 “2” 作为首位，并标记其已经使用。\\
取余数进行下一步操作。\\  .\\
②3 / 2! 结果，商 1 余 1。\\说明第二位要余出一个给 当前没用过的，最小的字符。\\
这里 “1” 没有用过，“2” 已经用了。因此，我们应当使用 “3” 作第二位。\\  .\\
③1 / 1! 结果，商 1 余 0。\\说明第三位要余出一个给 当前没用过的，最小的字符。\\
这里 “1” 没有用过，“2” 已经用了，“3”也用了。因此，我们应当使用 “4” 作第三位。\\
同康托展开，最后一位无需判断，所有字符中至今未使用的填入即可。
$$

#### 逆康托展开代码

```cpp
//对前 10 个自然数(0 ~ 9)的阶乘存入表
//以免去对其额外的计算
const int fact[10] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};
/**
 * @brief 逆康拓展开
 *
 * @param[in] bits 给定全排列的使用数字个数
 * @param[in] num 给定全排列的次位
 * @param[out] permutation 输出对应的全排列
 */
vector<int> revContor(int bits, int num) {
    num = num - 1; //有 num - 1 个排列比目标序列要小
    vector<bool> vis(bits + 1, false);
    vector<int> permutation(bits, -1);

    int n, residue = num;
    for (int i = 0; i < bits; ++i) {
        n = residue / (fact[bits - i - 1]);
        residue = residue % (fact[bits - i - 1]);

        for (int j = 1; j <= bits; ++j) {
            if (!vis[j] && !(n--)) {
                vis[j] = true;
                permutation[i] = j;
                break;
            }
        }
    }
    return permutation;
}
```

#### 应用

$$
康托展开与逆康托展开与全排列的联系十分密切，在解决全排列的字典序问题时能够发挥极大的作用。\\
此外，康托展开也是一个数组到一个数的映射，可以应用于hash中进行空间压缩。\\
例如，在八数码问题中，我们可以把一种排列状态压缩成一个整数存放在数组中。
$$

## 7.计算几何

`@Artiprocher` 段忠杰

`@tieway59` 伍泰炜

```text
白嫖最快乐了QwQ
```

### 一.向量的基本运算

#### 1.1 点和向量的表示

在平面直角坐标系中，任意一点的坐标可以用一个有序数对 $(x,y)$ 表示，向量也是如此

```cpp
struct Point//点或向量
{
    double x, y;
    Point() {}
    Point(double x, double y) :x(x), y(y) {}
};
typedef Point Vector;
```

#### 1.2 基本向量运算

设向量 $v_1=(x_1,y_1),v_2=(x_2,y_2)$ ，定义如下运算

##### 1.2.1 向量加法

$$
v_1+v_2=(x_1+x_2,y_1+y_2)
$$

##### 1.2.2 向量减法

$$
v_1-v_2=(x_1-x_2,y_1-y_2)
$$

若 $P=(x_1,y_1),Q=(x_2,y_2)$ ，则 $\overrightarrow{PQ}=Q-P=(x_2-x_1,y_2-y_1)$

##### 1.2.3 向量模长

$$
|v_1|=\sqrt{x_1^2+y_1^2}
$$

向量模长可以用来求两点间的距离

##### 1.2.4 向量数乘

$$
av_1=(ax_1,ay_1),a\in \mathbb{R}
$$

向量数乘可以实现向量的长度伸缩

##### 1.2.5 向量内积（点积）

$$
v_1\cdot v_2=|v_1||v_2|\cos<v_1,v_2>=x_1x_2+y_1y_2
$$

$v_1\cdot v_2=0$ 当且仅当 $v_1\perp v_2$

向量内积可以用来求向量间的夹角內积（点乘）

概括地说，向量的内积（点乘/数量积）。对两个向量执行点乘运算，就是对这两个向量对应位一一相乘之后求和的操作。
注意：点乘的结果是一个标量(数量而不是向量)

$$
定义：两个向量a与b的内积为 a·b = |a||b|cos∠(a, b)\\
特别地，0·a =a·0 = 0；若a，b是非零向量，则a与b正交的充要条件是a·b = 0
$$

##### 1.2.6 向量外积（叉积）

这个定义可能来自张量（Tensor）代数

$$
v_1\times v_2=\begin{vmatrix}
x_1 & y_1\\
x_2 & y_2
\end{vmatrix}=x_1y_2-x_2y_1
$$

$$
|v_1\times v_2|=|v_1||v_2|\sin <v_1,v_2>
$$

外积是很重要的一个概念，有很多应用

外积可以用来求面积，以 $v_1,v_2$ 为邻边的平行四边形面积为 $|v_1\times v_2|$

$v_1\times v_2=0$ 当且仅当 $v_1\parallel v_2$

外积可以用来判断向量间的位置关系，若 $v_1$ 旋转到 $v_2$ 的方向为顺时针，则 $v_1\times v_2<0$ ，反之 $v_1\times v_2>0$

概括地说，两个向量的外积，又叫叉乘、叉积向量积，其运算结果是一个向量而不是一个标量。并且两个向量的外积与这两个向量组成的坐标平面垂直。

$$
定义：向量a与b的外积a×b是一个向量，其长度等于|a×b| = |a||b|sin∠(a,b)\\
其方向正交于a与b。并且，(a,b,a×b)构成右手系。\\
特别地，0×a = a×0 = 0.此外，对任意向量a，a×a=0。
$$

##### 1.2.7 向量旋转

向量 $v_1$ 逆时针旋转 $\theta$ 后的坐标满足

$$
\begin{cases}
x'=x_1\cos \theta-y_1\sin \theta\\
y'=x_1\sin \theta+y_1\cos \theta
\end{cases}
$$

##### 代码

```cpp
#include <bits/stdc++.h>
using namespace std;
const double eps = 1e-6;//eps用于控制精度
const double pi = acos(-1.0);//pi
struct Point//点或向量
{
    double x, y;
    Point() {}
    Point(double x, double y) :x(x), y(y) {}
};
typedef Point Vector;
Vector operator + (Vector a, Vector b)//向量加法
{
    return Vector(a.x + b.x, a.y + b.y);
}
Vector operator - (Vector a, Vector b)//向量减法
{
    return Vector(a.x - b.x, a.y - b.y);
}
Vector operator * (Vector a, double p)//向量数乘
{
    return Vector(a.x*p, a.y*p);
}
Vector operator / (Vector a, double p)//向量数除
{
    return Vector(a.x / p, a.y / p);
}
int dcmp(double x)//精度三态函数(>0,<0,=0)
{
    if (fabs(x) < eps)return 0;
    else if (x > 0)return 1;
    return -1;
}
bool operator == (const Point &a, const Point &b)//向量相等
{
    return dcmp(a.x - b.x) == 0 && dcmp(a.y - b.y) == 0;
}
double Dot(Vector a, Vector b)//内积
{
    return a.x*b.x + a.y*b.y;
}
double Length(Vector a)//模
{
    return sqrt(Dot(a, a));
}
double Angle(Vector a, Vector b)//夹角,弧度制
{
    return acos(Dot(a, b) / Length(a) / Length(b));
}
double Cross(Vector a, Vector b)//外积
{
    return a.x*b.y - a.y*b.x;
}
Vector Rotate(Vector a, double rad)//逆时针旋转
{
    return Vector(a.x*cos(rad) - a.y*sin(rad), a.x*sin(rad) + a.y*cos(rad));
}
double Distance(Point a, Point b)//两点间距离
{
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}
double Area(Point a, Point b, Point c)//三角形面积
{
    return fabs(Cross(b - a, c - a) / 2);
}
```

### 二. 直线与线段

#### 2.1 线段相交问题

![相交](D:/Desktop/%25E8%25AE%25A1%25E7%25AE%2597%25E5%2587%25A0%25E4%25BD%2595/template.assets/image-20200719173019974.png)

线段 $AB$ 与 $CD$ 相交（不考虑端点）的充分必要条件是

$$
(\overrightarrow{CA}\cdot \overrightarrow{CB})
(\overrightarrow{DA}\cdot \overrightarrow{DB})<0,
(\overrightarrow{AC}\cdot \overrightarrow{AD})
(\overrightarrow{BC}\cdot \overrightarrow{BD})<0
$$

```cpp
bool Intersect(Point A, Point B, Point C, Point D)//线段相交（不包括端点）
{
    double t1 = Cross(C - A, D - A)*Cross(C - B, D - B);
    double t2 = Cross(A - C, B - C)*Cross(A - D, B - D);
    return dcmp(t1) < 0 && dcmp(t2) < 0;
}
bool StrictIntersect(Point A, Point B, Point C, Point D) //线段相交（包括端点）
{
    return
        dcmp(max(A.x, B.x) - min(C.x, D.x)) >= 0
        && dcmp(max(C.x, D.x) - min(A.x, B.x)) >= 0
        && dcmp(max(A.y, B.y) - min(C.y, D.y)) >= 0
        && dcmp(max(C.y, D.y) - min(A.y, B.y)) >= 0
        && dcmp(Cross(C - A, D - A)*Cross(C - B, D - B)) <= 0
        && dcmp(Cross(A - C, B - C)*Cross(A - D, B - D)) <= 0;
}
```

#### 2.2 点到直线的距离

![点到直线的距离](D:/Desktop/%25E8%25AE%25A1%25E7%25AE%2597%25E5%2587%25A0%25E4%25BD%2595/template.assets/image-20200719173055477.png)

如图所示，要计算点 A 到直线 MN 的距离，可以构建以 AM，MN 为邻边的平行四边形，其面积

$$
S=|\overrightarrow{MA}\times \overrightarrow{MN}|
$$

平行四边形的面积为底乘高，选取 MN 为底，高为

$$
d=\frac{S}{\left|\overrightarrow{MN}\right|}
$$

即为所求的 A 到直线 MN 的距离

```cpp
double DistanceToLine(Point A, Point M, Point N)//点A到直线MN的距离,Error:MN=0
{
    return fabs(Cross(A - M, A - N) / Distance(M, N));
}
```

#### 2.3 两直线交点

在实际应用中，通常的已知量是直线上某一点的坐标和直线的方向向量，对于两直线 $l_{1}$,$\ l_{2}$ ,设 $P\left( x_{1},y_{1} \right)$ , $\text{Q}\left( x_{2},y_{2} \right)$ 分别在 $l_{1}$ , $\ l_{2}$ 上， $l_{1}$ , $\ l_{2}$ 的方向向量分别为 $v = \left( a_{1},b_{1} \right)$ , $w = \left( a_{2},b_{2} \right)$ ,由此可以得到两直线的方程

$$
l_{1}:\left( x - x_{1},y - y_{1} \right) \times \left( a_{1},b_{1} \right) = 0
$$

$$
l_{2}:\left( x - x_{2},y - y_{2} \right) \times \left( a_{2},b_{2} \right) = 0
$$

即

$$
l_{1}:a_{1}x - b_{1}y = a_{1}x_{1} - b_{1}y_{1}
$$

$$
l_{2}:a_{2}x - b_{2}y = a_{2}x_{2} - b_{2}y_{2}
$$

联立两直线的方程，由克拉默法则得，方程组的解为

$$
\left\{ \begin{matrix}
x = \frac{\left| \begin{matrix}
a_{1}x_{1} - b_{1}y_{1} & - b_{1} \\
a_{2}x_{2} - b_{2}y_{2} & - b_{2} \\
\end{matrix} \right|}{\left| \begin{matrix}
a_{1} & - b_{1} \\
a_{2} & - b_{2} \\
\end{matrix} \right|} \\
y = \frac{\left| \begin{matrix}
a_{1} & a_{1}x_{1} - b_{1}y_{1} \\
a_{2} & a_{2}x_{2} - b_{2}y_{2} \\
\end{matrix} \right|}{\left| \begin{matrix}
a_{1} & - b_{1} \\
a_{2} & - b_{2} \\
\end{matrix} \right|} \\
\end{matrix} \right.\
$$

进一步进行化简，得到

$$
(x,y)=P+v\cdot \frac{w\times u}{v\times w}
$$

其中 $u=-\overrightarrow{PQ}$

```cpp
Point GetLineIntersection(Point P, Vector v, Point Q, Vector w)//两直线的交点
{
    Vector u = P - Q;
    double t = Cross(w, u) / Cross(v, w);
    return P + v * t;
}
```

### 三. 多边形

#### 3.1 点和多边形的位置关系

设有（凸）$n(n≥3)$ 边形 $P_0 P_2\dots P_{n-1}$，点的顺序为顺时针或逆时针，以及点 A，记

$$
\theta_{i} = \left\{ \begin{matrix}
 < \overrightarrow{AP_{i}},\overrightarrow{AP_{i + 1}} > ,i < n - 1 \\
 < \overrightarrow{AP_{n - 1}},\overrightarrow{AP_{0}} > ,i = n - 1 \\
\end{matrix} \right.\
$$

点在多边形内等价于

$$
\sum_{i = 0}^{n - 1}\theta_{i} = 2\pi
$$

```cpp
/*模板说明：P[]为多边形的所有顶点，下标为0~n-1，n为多边形边数*/
Point P[1005];
int n;
bool InsidePolygon (Point A) //判断点是否在凸多边形内（角度和判别法）
{
    double alpha = 0;
    for (int i = 0; i < n; i++)
        alpha += fabs(Angle(P[i] - A, P[(i + 1) % n] - A));
    return dcmp(alpha - 2 * pi) == 0;
}

// STL：求多边形面积（叉积和计算法）
double PolygonArea(const vector <Point> &P) {
    int n = P.size();
    // assert(n > 2);
    double sum = 0;
    Point O = Point(0, 0);
    for (int i = 0; i < n; i++)
        sum += Cross(P[i] - O, P[(i + 1) % n] - O);
    if (sum < 0) sum = -sum;
    return sum / 2;
}
```

#### 3.2 多边形的面积

设有（凸）$n(n≥3)$ 边形 $P_0 P_2\dots P_{n-1}$ ，点的顺序为顺时针或逆时针，以及多边形内一点 A，把多边形切割成如下所示 n 个三角形

![多边形](D:/Desktop/%25E8%25AE%25A1%25E7%25AE%2597%25E5%2587%25A0%25E4%25BD%2595/template.assets/image-20200719173137935.png)

多边形的面积等于所有三角形（有向）面积之和，代入坐标 $P_i (x_i,y_i ),i=0,1,\dots,n-1$ 计算得

$$
S = \left| \frac{1}{2}\sum_{i = 0}^{n - 2}\left( x_{i}y_{i + 1} - x_{i + 1}y_{i} \right) + \frac{1}{2}\left( x_{n - 1}y_{0} - x_{0}y_{n - 1} \right) \right|
$$

与 A 的坐标无关，因此 A 可任取，甚至可取在多边形外，通常为计算方便，取 A 为坐标原点

```cpp
/*模板说明：P[]为多边形的所有顶点，下标为0~n-1，n为多边形边数*/
Point P[1005];
int n;
double PolygonArea()//求多边形面积（叉积和计算法）
{
    double sum = 0;
    Point O = Point(0, 0);
    for (int i = 0; i < n; i++)
        sum += Cross(P[i] - O, P[(i + 1) % n] - O);
    if (sum < 0)sum = -sum;
    return sum / 2;
}
```

### 四.圆

#### 4.1 圆的参数方程

![圆](D:/Desktop/%25E8%25AE%25A1%25E7%25AE%2597%25E5%2587%25A0%25E4%25BD%2595/template.assets/image-20200719173213778.png)

以$(x_{0},y_{0})$为圆心，$r$为半径的圆的参数方程为

$$
\left\{ \begin{matrix}
x = x_{0} + r\cos\theta \\
y = y_{0} + r\sin\theta \\
\end{matrix} \right.\
$$

根据圆上一点和圆心连线与$x$轴正向的夹角可求得该点的坐标

#### 4.2 两圆交点

![图两圆交点](D:/Desktop/%25E8%25AE%25A1%25E7%25AE%2597%25E5%2587%25A0%25E4%25BD%2595/template.assets/image-20200719173233704.png)

设两圆$C_{1},C_{2}$，其半径为$r_{1},r_{2}(r_{1} \geq r_{2})$，圆心距为$d$，则有

① 两圆重合$\Longleftrightarrow d = 0\ \ r_{1} = r_{2}$

② 两圆外离$\Longleftrightarrow d > r_{1} + r_{2}$

③ 两圆外切$\Longleftrightarrow d = r_{1} + r_{2}$

④ 两圆相交$\Longleftrightarrow r_{1} - r_{2} < d < r_{1} + r_{2}$

⑤ 两圆内切$\Longleftrightarrow d = r_{1} - r_{2}$

⑥ 两圆内含$\Longleftrightarrow d < r_{1} - r_{2}$

对于情形 ④，如下图所示，要求 A 与 B 的坐标，只需求$\angle AC_{1}D$与$\angle BC_{1}D$，进而通过圆的参数方程即可求得

$$
\angle AC_{1}D = \angle C_{2}C_{1}D + \angle AC_{1}C_{2}
$$

$$
\angle BC_{1}D = \angle C_{2}C_{1}D - \angle AC_{1}C_{2}
$$

$\angle C_{2}C_{1}D$可以通过$C_{1},C_{2}$的坐标求得，而$\angle AC_{1}C_{2}$可以通过$\Delta AC_{1}C_{2}$上的余弦定理求得

对于情形 ③ 和情形 ⑤，上述方法求得的两点坐标是相同的，即为切点的坐标

```cpp
struct Circle
{
    Point c;
    double r;
    Point point(double a)//基于圆心角求圆上一点坐标
    {
        return Point(c.x + cos(a)*r, c.y + sin(a)*r);
    }
};
double Angle(Vector v1)
{
    if (v1.y >= 0)return Angle(v1, Vector(1.0, 0.0));
    else return 2 * pi - Angle(v1, Vector(1.0, 0.0));
}
int GetCC(Circle C1, Circle C2)//求两圆交点
{
    double d = Length(C1.c - C2.c);
    if (dcmp(d) == 0)
    {
        if (dcmp(C1.r - C2.r) == 0)return -1;//重合
        else return 0;
    }
    if (dcmp(C1.r + C2.r - d) < 0)return 0;
    if (dcmp(fabs(C1.r - C2.r) - d) > 0)return 0;
 
    double a = Angle(C2.c - C1.c);
    double da = acos((C1.r*C1.r + d * d - C2.r*C2.r) / (2 * C1.r*d));
    Point p1 = C1.point(a - da), p2 = C1.point(a + da);
    if (p1 == p2)return 1;
    else return 2;
}
```

`// 从这里开始更新`

#### 4.3 不共线三点求圆心（外心）

设圆的方程：

$$
(x - x_0)^2 + (y - y_0)^2 = r^2
$$

然后带入三个点：

$$
\begin{cases}
(x_1 - x_0)^2 + (y_1-y_0)^2 = r^2 & (1)\\
(x_2 - x_0)^2 + (y_2-y_0)^2 = r^2 & (2)\\
(x_3 - x_0)^2 + (y_3-y_0)^2 = r^2 & (3)
\end{cases}
$$

通过带入和化简，最后可以这样求：

$$
\begin{align*}
a &= x_1-x_2\\
b &= y_1-y_2\\
c &= x_1-x_3\\
d &= y_1-y_3\\
e &= (x_1^2 - x_2^2 + y_1^2 - y_2^2)\div 2\\
f &= (x_1^2 - x_3^2 + y_1^2 - y_3^2)\div 2 \\
\\
x_0 &= \frac{d e - b f}{a d - b c}\\
y_0 &= \frac{a f - c e}{a d - b c}
\end{align*}
$$

```cpp
template<typename tp>
inline tp pow2(const tp &x) {
    return x * x;
}

inline Point circumcenter(Point p1, Point p2, Point p3) {
    double a = p1.x - p2.x;
    double b = p1.y - p2.y;
    double c = p1.x - p3.x;
    double d = p1.y - p3.y;
    double e = (pow2(p1.x) - pow2(p2.x) +
                pow2(p1.y) - pow2(p2.y)) / 2;
    double f = (pow2(p1.x) - pow2(p3.x) +
                pow2(p1.y) - pow2(p3.y)) / 2;
    return Point((d * e - b * f) /
                 (a * d - b * c),
                 (a * f - c * e) /
                 (a * d - b * c));
}
```

如果你眼力强大：

```cpp
Point circumcenter(Point a, Point b, Point c) {
    Point res;
    res.x = ((a.x * a.x - b.x * b.x + a.y * a.y - b.y * b.y) * (a.y - c.y) -
             (a.x * a.x - c.x * c.x + a.y * a.y - c.y * c.y) * (a.y - b.y)) /
            (2 * (a.y - c.y) * (a.x - b.x) - 2 * (a.y - b.y) * (a.x - c.x));
    res.y = ((a.x * a.x - b.x * b.x + a.y * a.y - b.y * b.y) * (a.x - c.x) -
             (a.x * a.x - c.x * c.x + a.y * a.y - c.y * c.y) * (a.x - b.x)) /
            (2 * (a.y - b.y) * (a.x - c.x) - 2 * (a.y - c.y) * (a.x - b.x));
    return res;
}
```

#### 4.4 最小圆覆盖

```cpp
/**
 *  @Source: https://www.luogu.com.cn/problem/solution/P1742
 *  @Author: snowbody -> tieway59
 *  @Description:
 *      时间复杂度 O(N)
 *      为了减少中途过度开根，距离都是先按照平方计算的。
 *
 *  @Example:
 *      vector<Point> p(n);
 *      for (auto &pi : p) cin >> pi;
 *      Circle circle;
 *      MinCircleCover(p, circle);
 *
 *      6
 *      8.0 9.0
 *      4.0 7.5
 *      1.0 2.0
 *      5.1 8.7
 *      9.0 2.0
 *      4.5 1.0
 *      // r = 5.0000000000 (5.0000000000,5.0000000000)
 *
 *  @Verification:
 *      https://www.luogu.com.cn/problem/P1742
 */



//点或向量 (iostream选择性抄写)
struct Point {
    double x, y;

    Point() {}

    Point(double x, double y) : x(x), y(y) {}

    friend ostream &operator<<(ostream &ut, Point &r) { return ut << r.x << " " << r.y; }

    friend istream &operator>>(istream &in, Point &r) { return in >> r.x >> r.y; }
};

typedef Point Vector;

inline Vector operator+(Vector a, Vector b) {
    return Vector(a.x + b.x, a.y + b.y);
}

inline Vector operator-(Vector a, Vector b) {
    return Vector(a.x - b.x, a.y - b.y);
}

//向量数乘
inline Vector operator*(Vector a, double p) {
    return Vector(a.x * p, a.y * p);
}

//向量数除
inline Vector operator/(Vector a, double p) {
    return Vector(a.x / p, a.y / p);
}

//两点间距离
inline double Distance(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

inline double Distance2(Point a, Point b) {
    return ((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

struct Circle {
    Point c;
    double r;

    Point point(double a)//基于圆心角求圆上一点坐标
    {
        return Point(c.x + cos(a) * r, c.y + sin(a) * r);
    }
};

template<typename tp>
inline tp pow2(const tp &x) {
    return x * x;
}

inline Point circumcenter(Point p1, Point p2, Point p3) {
    double a = p1.x - p2.x;
    double b = p1.y - p2.y;
    double c = p1.x - p3.x;
    double d = p1.y - p3.y;
    double e = (pow2(p1.x) - pow2(p2.x) +
                pow2(p1.y) - pow2(p2.y)) / 2;
    double f = (pow2(p1.x) - pow2(p3.x) +
                pow2(p1.y) - pow2(p3.y)) / 2;
    return Point((d * e - b * f) /
                 (a * d - b * c),
                 (a * f - c * e) /
                 (a * d - b * c));
}


void MinCircleCover(vector <Point> &p, Circle &res) {
    int n = p.size();
    random_shuffle(p.begin(), p.end());
    // avoid *sqrt* too much killing your precision.
    for (int i = 0; i < n; i++) {
        if (Distance2(p[i], res.c) <= res.r) continue;
        res.c = p[i];
        res.r = 0;
        for (int j = 0; j < i; j++) {
            if (Distance2(p[j], res.c) <= res.r)continue;
            res.c = (p[i] + p[j]) / 2;
            res.r = Distance2(p[j], res.c);
            for (int k = 0; k < j; k++) {
                if (Distance2(p[k], res.c) <= res.r)continue;
                res.c = circumcenter(p[i], p[j], p[k]);
                res.r = Distance2(p[k], res.c);
            }
        }
    }
    res.r = sqrt(res.r);
}

void solve(int kaseId = -1) {
    int n;
    cin >> n;
    vector <Point> p(n);
    for (auto &pi : p) cin >> pi;
    Circle circle;
    MinCircleCover(p, circle);
    cout << fixed << setprecision(10) << circle.r << endl;
    cout << circle.c.x << " " << circle.c.y << endl;
}
```

### 五. 几何公式

#### 5.1 三角形

1. 半周长`P=(a+b+c)/2`

2. 面积 `S=aHa/2=absin(C)/2=sqrt(P(P-a)(P-b)(P-c))`

3. 中线 `Ma=sqrt(2(b^2+c^2)-a^2)/2=sqrt(b^2+c^2+2bccos(A))/2`

4. 角平分线 `Ta=sqrt(bc((b+c)^2-a^2))/(b+c)=2bccos(A/2)/(b+c)`

5. 高线 `Ha=bsin(C)=csin(B)=sqrt(b^2-((a^2+b^2-c^2)/(2a))^2)`
6. 内切圆半径 `r=S/P=asin(B/2)sin(C/2)/sin((B+C)/2)`
​    - =`4Rsin(A/2)sin(B/2)sin(C/2)=sqrt((P-a)(P-b)(P-c)/P)`
​    - =`Ptan(A/2)tan(B/2)tan(C/2)`
7. 外接圆半径 `R=abc/(4S)=a/(2sin(A))=b/(2sin(B))=c/(2sin(C))`

#### 5.2 四边形

D1,D2 为对角线,M 对角线中点连线,A 为对角线夹角

1. `a^2+b^2+c^2+d^2=D1^2+D2^2+4M^2`

2. `S=D1D2sin(A)/2`

(以下对圆的内接四边形)

1. `ac+bd=D1D2`

2. `S=sqrt((P-a)(P-b)(P-c)(P-d))` , P 为半周长

#### 5.3 正 n 边形

R 为外接圆半径,r 为内切圆半径

1. 中心角 `A=2PI/n`

2. 内角 `C=(n-2)PI/n`

3. 边长 `a=2sqrt(R^2-r^2)=2Rsin(A/2)=2rtan(A/2)`

4. 面积 `S=nar/2=nr^2tan(A/2)=nR^2sin(A)/2=na^2/(4tan(A/2))`

#### 5.4 圆

1. 弧长 `l=rA`

2. 弦长 `a=2sqrt(2hr-h^2)=2rsin(A/2)`

3. 弓形高 `h=r-sqrt(r^2-a^2/4)=r(1-cos(A/2))=atan(A/4)/2`

4. 扇形面积 `S1=rl/2=r^2A/2`

5. 弓形面积 `S2=(rl-a(r-h))/2=r^2(A-sin(A))/2`

#### 5.5 棱柱

1. 体积 `V=Ah`,A 为底面积,h 为高

2. 侧面积 `S=lp` ,l 为棱长,p 为直截面周长

3. 全面积 `T=S+2A`

#### 5.6 棱锥

- 体积 `V=Ah/3`,A 为底面积,h 为高

(以下对正棱锥)

- 侧面积 `S=lp/2`,l 为斜高,p 为底面周长

- 全面积 `T=S+A`

#### 5.7 棱台

1. 体积 `V=(A1+A2+sqrt(A1A2))h/3`,A1.A2 为上下底面积,h 为高

(以下为正棱台)

- 侧面积 `S=(p1+p2)l/2`,p1.p2 为上下底面周长,l 为斜高

- 全面积 `T=S+A1+A2`

#### 5.8 圆柱

1. 侧面积 `S=2PIrh`

2. 全面积 `T=2PIr(h+r)`

3. 体积 `V=PIr^2h`

#### 5.9 圆锥

1. 母线 `l=sqrt(h^2+r^2)`

2. 侧面积 `S=PIrl`

3. 全面积 `T=PIr(l+r)`

4. 体积 `V=PIr^2h/3`

#### 5.10 圆台

1. 母线`l=sqrt(h^2+(r1-r2)^2)`

2. 侧面积 `S=PI(r1+r2)l`

3. 全面积 `T=PIr1(l+r1)+PIr2(l+r2)`

4. 体积 `V=PI(r1^2+r2^2+r1r2)h/3`

#### 5.11 球

1. 全面积 `T=4PIr^2`

2. 体积 `V=4PIr^3/3`

#### 5.12 球台

1. 侧面积 `S=2PIrh`

2. 全面积 `T=PI(2rh+r1^2+r2^2)`

3. 体积 `V=PIh(3(r1^2+r2^2)+h^2)/6`

#### 5.13 球扇形

1. 全面积 `T=PIr(2h+r0)`,h 为球冠高,r0 为球冠底面半径

2. 体积 `V=2PIr^2h/3`

### 六. 凸包

#### 6.1 点凸包

在一个实向量空间 $V$ 中，对于给定集合 $X$ ，所有包含 $X$ 的凸集的交集 $S$ 称为 $X$ 的凸包

$$
S=\cap_{X\subset K\subset V,K\text{ is convex}}K
$$

##### 6.1.1 Graham’s scan 算法

第一步：找到最下边的点，如果有多个点纵坐标相同的点都在最下方，则选取最左边的，记为点 A。这一步只需要扫描一遍所有的点即可，时间复杂度为 $O(n)$

第二步：将所有的点按照 $AP_i$ 的极角大小进行排序，极角相同的按照到点 A 的距离排序。时间复杂度为 $O(nlogn)$

第三步：维护一个栈，以保存当前的凸包。按第二步中排序得到的结果，依次将点加入到栈中，如果当前点与栈顶的两个点不是“向左转”的，就表明当前栈顶的点并不在凸包上，而我们需要将其弹出栈，重复这一个过程直到当前点与栈顶的两个点是“向左转”的。这一步的时间复杂度为 $O(n)$

```cpp
/**
 *  @Source: Graham_s_scan
 *  @Author: Artiprocher(Zhongjie Duan) -> tieway59
 *  @Description:
 *      n       点数
 *      P[]     点数组 index0
 *      top     栈顶, 凸包顶点数
 *      H[]     凸包的顶点 index0
 *      小心重复的凸包顶点, 也会加入凸包。
 *      H[]逆时针顺序
 *      数组形式，理论上常数会小？
 *
 *  @Example:
 *      4
 *      4 8
 *      4 12
 *      5 9.3 (exclude)
 *      7 8
 *
 *  @Verification:
 *      https://www.luogu.com.cn/record/35363811
 *
 */
//求凸包
int n, top;
Point P[10005], result[10005];
bool cmp(Point A, Point B)
{
    double ans = Cross(A - P[0], B - P[0]);
    if (dcmp(ans) == 0)
        return dcmp(Distance(P[0], A) - Distance(P[0], B)) < 0;
    else
        return ans > 0;
}
void Graham()//Graham凸包扫描算法
{
    for (int i = 1; i < n; i++)//寻找起点
        if (P[i].y < P[0].y || (dcmp(P[i].y - P[0].y) == 0 && P[i].x < P[0].x))
            swap(P[i], P[0]);
    sort(P + 1, P + n, cmp);//极角排序，中心为起点
    result[0] = P[0];
    result[1] = P[1];
    top = 1;
    for (int i = 2; i < n; i++)
    {
        while (top >= 1 && Cross(result[top] - result[top - 1], P[i] - result[top - 1]) < 0)
            top--;
        result[++top] = P[i];
    }
}
```

```cpp
/**
 *  @Source: Graham_s_scan
 *  @Author: Artiprocher(Zhongjie Duan) -> tieway59
 *  @Description:
 *      小心重复的凸包顶点, 也会加入凸包。
 *      H[]逆时针顺序
 *      数组形式，理论上常数会小？
 *
 *  @Example:
 *      4
 *      4 8
 *      4 12
 *      5 9.3 (exclude)
 *      7 8
 *
 *  @Verification:
 *      https://www.luogu.com.cn/record/35363811
 *
 */

// HEAD begin
const double EPS = 1e-6;

struct Point//点或向量
{
    double x, y;

    Point() {}

    Point(double x, double y) : x(x), y(y) {}

    friend ostream &operator<<(ostream &ut, Point &r) { return ut << r.x << " " << r.y; }

    friend istream &operator>>(istream &in, Point &r) { return in >> r.x >> r.y; }
};

typedef Point Vector;

inline double Distance(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

inline Vector operator+(Vector a, Vector b) {
    return Vector(a.x + b.x, a.y + b.y);
}

inline Vector operator-(Vector a, Vector b) {
    return Vector(a.x - b.x, a.y - b.y);
}

//外积
inline double Cross(Vector a, Vector b) {
    return a.x * b.y - a.y * b.x;
}

//精度三态函数(>0,<0,=0)
inline int dcmp(double x) {
    if (fabs(x) < EPS)return 0;
    else if (x > 0)return 1;
    return -1;
}
// HEAD end
void ConvexHull(vector<Point> &P, vector<Point> &H) {
    int n = int(P.size());
    for (int i = 1; i < n; i++)//寻找起点
        if (P[i].y < P[0].y || (dcmp(P[i].y - P[0].y) == 0 && P[i].x < P[0].x))
            swap(P[i], P[0]);

    //极角排序，中心为起点
    sort(P.begin() + 1, P.end(), [&P](Point A, Point B) {
        double ans = Cross(A - P[0], B - P[0]);
        if (dcmp(ans) == 0)
            return dcmp(Distance(P[0], A) - Distance(P[0], B)) < 0;
        else
            return ans > 0;
    });

    H.assign(n + n, {});
    H[0] = P[0];
    H[1] = P[1];
    int top = 2;
    for (int i = 2; i < n; i++) {
        while (top >= 2 && Cross(H[top - 1] - H[top - 2], P[i] - H[top - 2]) < 0)
            top--;
        H[top++] = P[i];
    }
    H.resize(top);
}
```

##### 6.1.2 Andrew's monotone chain 算法

原理与 Graham’s scan 算法相似，但上下凸包是分开维护的

```cpp
namespace ConvexHull{
    bool cmp1(Point a,Point b){
        if(fabs(a.x-b.x)<eps)return a.y<b.y;
        return a.x<b.x;
    }
    // 从左下角开始逆时针排列，去除凸包边上的点
    vector<Point> Andrew_s_monotone_chain(vector<Point> P){
        int n=P.size(),k=0;
        vector<Point> H(2*n);
        sort(P.begin(),P.end(),cmp1);
        for(int i=0;i<n;i++) {
            while(k>=2 && Cross(H[k-1]-H[k-2],P[i]-H[k-2])<eps)k--;
            H[k++]=P[i];
        }
        int t=k+1;
        for(int i=n-1;i>0;i--) {
            while(k>=t && Cross(H[k-1]-H[k-2],P[i-1]-H[k-2])<eps)k--;
            H[k++]=P[i-1];
        }
        H.resize(k-1);
        return H;
    }
}
```

```cpp
/**
 *  @Source: Andrew_s_monotone_chain
 *  @Author: Artiprocher(Zhongjie Duan) -> tieway59
 *  @Description:
 *      Andrew_s_monotone_chain
 *      从左下角开始逆时针排列，去除凸包边上的点。
 *      求出来的凸包是逆时针的。
 *      points in h[] are counter-clockwise
 *
 *  @Example:
 *      vector<Point> p(n);
 *      for (auto &pi : p) cin >> pi;
 *      vector<Point> r;
 *      ConvexHull(p, r);
 *
 *      4
 *      4 8
 *      4 12
 *      5 9.3 (exclude)
 *      7 8
 *
 *  @Verification:
 *      https://www.luogu.com.cn/problem/P2742
 */

// HEAD begin
const double EPS = 1e-6;

struct Point//点或向量
{
    double x, y;

    Point() {}

    Point(double x, double y) : x(x), y(y) {}

    friend ostream &operator<<(ostream &ut, Point &r) { return ut << r.x << " " << r.y; }

    friend istream &operator>>(istream &in, Point &r) { return in >> r.x >> r.y; }
};

typedef Point Vector;

inline double Distance(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

inline Vector operator+(Vector a, Vector b) {
    return Vector(a.x + b.x, a.y + b.y);
}

inline Vector operator-(Vector a, Vector b) {
    return Vector(a.x - b.x, a.y - b.y);
}

//外积
inline double Cross(Vector a, Vector b) {
    return a.x * b.y - a.y * b.x;
}

//精度三态函数(>0,<0,=0)
inline int dcmp(double x) {
    if (fabs(x) < EPS)return 0;
    else if (x > 0)return 1;
    return -1;
}
// HEAD end

inline bool pcmp(Point a, Point b) {
    if (dcmp(a.x - b.x) == 0)
        return a.y < b.y;
    return a.x < b.x;
}

void ConvexHull(vector <Point> &p, vector <Point> &h) {
    int n = p.size(), k = 0;
    h.assign(2 * n, {});
    sort(p.begin(), p.end(), pcmp);
    for (int i = 0; i < n; i++) {
        while (k >= 2 && dcmp(Cross(
                h[k - 1] - h[k - 2],
                p[i] - h[k - 2])) < 0) {
            k--;
        }
        h[k++] = p[i];
    }

    int t = k + 1;
    for (int i = n - 1; i > 0; i--) {
        while (k >= t && dcmp(Cross(
                h[k - 1] - h[k - 2],
                p[i - 1] - h[k - 2])) < 0) {
            k--;
        }
        h[k++] = p[i - 1];
    }

    h.resize(k - 1);
}
```

#### 6.2 直线凸包

```cpp
/* Author: bnfcc -> tc2000731 -> tieway59
 * Description:
 *      维护下凸包，对于每个x维护f(x)=k*x+b的最大值。
 *      query max value within all f(x) functions.
 *      c++11 features included.
 * Problems:
 *      https://nanti.jisuanke.com/t/41306
 *      https://nanti.jisuanke.com/t/41097
 */
template<typename var=long long, const int SIZE = 1000005, typename ldb=long double>
struct Hull {
    struct fx {
        var k, b;

        fx() {}

        fx(var k, var b) : k(k), b(b) {}

        var f(var x) { return k * x + b; }
    };

    int cnt;
    fx arr[SIZE];

    bool empty() {
        return cnt == 0;
    }

    void init() {
        cnt = 0;
    }

    void add(const fx &p) {
        arr[cnt++] = p;
    }

    void pop() {
        cnt--;
    }

    bool chek(const fx &a, const fx &b, const fx &c) {
        ldb ab, ak, bb, bk, cb, ck;
        tie(ab, ak, bb, bk, cb, ck) =
                tie(a.b, a.k, b.b, b.k, c.b, c.k);
        return (ab - bb) / (bk - ak) > (ab - cb) / (ck - ak);
    }

    void insert(const fx &p) {///k从小到大插入
        if (cnt && arr[cnt - 1].k == p.k) {
            if (p.b <= arr[cnt - 1].b)return;
            else pop();
        }
        while (cnt >= 2 && chek(arr[cnt - 2], arr[cnt - 1], p))pop();
        add(p);
    }

    /*var query(var x) {///x从大到小查询       从小到大用队列
        while (cnt > 1 && arr[cnt - 2].f(x) > arr[cnt - 1].f(x))pop();;
        return arr[cnt - 1].f(x);
    }*/

    var query(var x) {///二分查询，x顺序任意
        int l = 0, r = cnt - 1;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (arr[mid].f(x) >= arr[mid + 1].f(x))r = mid;
            else l = mid + 1;
        }
        return arr[l].f(x);
    }
};

// vector stack
template<typename var=long long, const int SIZE = 1000005, typename ldb=long double>
struct Hull {
    struct Line {
        var k, b;

        Line() {}

        Line(var k, var b) : k(k), b(b) {}

        var f(var x) { return k * x + b; }
    };

    int cnt;
    vector <Line> con;//

    bool empty() {
        return cnt == 0;
    }

    void init(const int &n) {
        con.clear();
        if (n > con.capacity())con.reserve(n);
        cnt = 0;
    }

    void add(const Line &p) {
        con.emplace_back(p);
        cnt++;
    }

    void pop() {
        cnt--;
        con.pop_back();
    }

    bool chek(const Line &a, const Line &b, const Line &c) {
        ldb ab, ak, bb, bk, cb, ck;
        tie(ab, ak, bb, bk, cb, ck) =
                tie(a.b, a.k, b.b, b.k, c.b, c.k);
        return (ab - bb) / (bk - ak) > (ab - cb) / (ck - ak);
    }

    void insert(const Line &p) {///k从小到大插入
        if (cnt && con[cnt - 1].k == p.k) {
            if (p.b <= con[cnt - 1].b)return;
            else pop();
        }
        while (cnt >= 2 && chek(con[cnt - 2], con[cnt - 1], p))pop();
        add(p);
    }

    var query(var x) {///二分查询，x顺序任意
        int l = 0, r = cnt - 1;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (con[mid].f(x) >= con[mid + 1].f(x))r = mid;
            else l = mid + 1;
        }
        return con[l].f(x);
    }
};

Hull<> hull;
```

### #. 附录

#### 真题

> 出过的题基本不会再出现，但是可以看看自己板子怎么样。

- [2019 ICPC 沈阳站 E.Capture Stars](https://www.cnblogs.com/AEMShana/p/12452762.html) （没有开放提交平台）
- [2019 ICPC 南京站 K.Triangle](https://nanti.jisuanke.com/t/42405) [题解](https://www.cnblogs.com/wulitaotao/p/11755964.html) 铜牌题
- [2019 ICPC 西安站邀请赛 C. Angel's Journey](https://blog.csdn.net/qq_41835683/article/details/90577692)
- [2019 ICPC 上海站 I](https://ac.nowcoder.com/acm/contest/4370/I) [一个题解](https://www.cnblogs.com/xiaobuxie/p/12485717.html)
- [2019 CCPC 秦皇岛 A 题（计算几何）](https://www.cnblogs.com/rentu/p/11642537.html)
- [2018 ICPC 南京站 D.Country Meow](http://www.baidu.com/link?url=pCccIM_daajkd8wfqGEZESGajRSTRpq-M0MsWfwoHTyNIdoZjhkZBT7GWnBxZXqFnZ6XCUAoqWTIkHpoR2yWRq)
- [2018 ICPC 沈阳站 L Machining Disc Rotors](https://blog.csdn.net/qq_40791842/article/details/100907900)
- [2017 ICPC 北京站 G.Liaoning Ship's Voyage](https://blog.csdn.net/qq_40791842/article/details/101486595)
- [2015 ICPC 上海站 I 计算几何+组合计数](https://blog.csdn.net/foreyes_1001/article/details/52228058)

#### 清单

> 一些可以准备的有意思的主题。

- [ ] 最远曼哈顿距离
- [ ] 包卡壳旋转求出所有对踵点、最远点对
- [ ] 最近点对
- [ ] 最近圆对
- [ ] 费马点（所有点到某坐标距离和最短）
- [ ] 求两个圆的交点
- [ ] 凸包+旋转卡壳求平面面积最大三角
- [ ] Pick 定理
- [ ] 求多边形面积和重心
- [ ] 判断一个简单多边形是否有核
- [ ] 模拟退火
- [ ] 六边形坐标系
- [ ] 用一个给定半径的圆覆盖最多的点
- [ ] 不等大的圆的圆弧表示
- [ ] 矩形面积并
- [ ] 矩形的周长并
- [ ] 求两个圆的面积交
- [ ] 圆的反演变换

#### 內积（点乘）

概括地说，向量的内积（点乘/数量积）。对两个向量执行点乘运算，就是对这两个向量对应位一一相乘之后求和的操作。
注意：点乘的结果是一个标量(数量而不是向量)

$$
定义：两个向量a与b的内积为 a·b = |a||b|cos∠(a, b)\\
特别地，0·a =a·0 = 0；若a，b是非零向量，则a与b正交的充要条件是a·b = 0
$$

#### 外积（叉乘）

概括地说，两个向量的外积，又叫叉乘、叉积向量积，其运算结果是一个向量而不是一个标量。并且两个向量的外积与这两个向量组成的坐标平面垂直。

$$
定义：向量a与b的外积a×b是一个向量，其长度等于|a×b| = |a||b|sin∠(a,b)\\
其方向正交于a与b。并且，(a,b,a×b)构成右手系。\\
特别地，0×a = a×0 = 0.此外，对任意向量a，a×a=0。
$$

### 凸包

#### Andrew 算法

##### 代码

```cpp
#include<bits/stdc++.h>
using namespace std;
int n;
double ans = 0;
#define maxn 11000
struct spot
{
    double x,y;
    bool operator < (const spot &a) const
    {
        if(x == a.x) return y < a.y;
        return x < a.x;
    }
    void read()
    {
        scanf("%lf%lf", &x, &y);
    }
}a[maxn];
#define inf 99999999
struct stacks
{
    int q[maxn],t;
    void clear(){t = 0;memset(q, 0, sizeof(q));}
    void push(int x){q[++ t] = x;}
    int top(){return q[t];}
    void pop(){-- t;}
    bool empty(){return t == 0;}
    int sec_top(){return q[t - 1];}
}q;//拒绝STL依赖
bool slope(int ax,int b,int c)
{
    return (a[ax].y-a[b].y)*(a[b].x-a[c].x)>(a[ax].x-a[b].x)*(a[b].y-a[c].y);
}
double dis(spot a,spot b){return sqrt((a.y - b.y) * (a.y - b.y) + (a.x - b.x) * (a.x - b.x));}
int main()
{
    scanf("%d", &n);
    for(int i = 1;i <= n;i ++) a[i].read();
    sort(a + 1,a + n + 1);
    for(int i = 1;i <= n;i ++)
    {
        while(q.t > 1 && slope(i, q.top(), q.sec_top())) q.pop();
        q.push(i);
    }
    int k = q.t;
    for(int i = n - 1;i >= 1;i --)
    {
        while(q.t > k && slope(i, q.top(), q.sec_top())) q.pop();
        q.push(i);
    }
    int last = 1;
    for(int i = 2;i <= q.t;i ++)
      ans += dis(a[last], a[q.q[i]]),last = q.q[i];
    printf("%.2lf", ans);
}
```

### 多边形面积&&周长

### 多边形的核

### 半平面交

### 模拟退火

### 平面最近点对

### 对踵点

## 基础模板

### 查找

#### 二分查找

##### 二分查找（模板一）

```cpp
int bsearch_1(int l, int r)
{
    while (l < r)
    {
        int mid = l + r >> 1;
        if (check(mid)) r = mid;
        else l = mid + 1;
    }
    return l;
}
```

##### 二分查找（模板二）

```cpp
int bsearch_2(int l, int r)
{
    while (l < r)
    {
        int mid = l + r + 1 >> 1;
        if (check(mid)) l = mid;
        else r = mid - 1;
    }
    return l;
}
```

#### 三分查找

##### 三分查找（整数模板）

```cpp
int l = 1,r = 100;
while(l < r) {
    int lmid = l + (r - l) / 3;
    int rmid = r - (r - l) / 3;
    lans = f(lmid),rans = f(rmid);
    // 求凹函数的极小值
    if(lans <= rans) r = rmid - 1;
    else l = lmid + 1;
    // 求凸函数的极大值
    if(lasn >= rans) l = lmid + 1;
    else r = rmid - 1;
}
// 求凹函数的极小值
cout << min(lans,rans) << endl;
// 求凸函数的极大值
cout << max(lans,rans) << endl;
```

##### 三分查找（浮点数模板）

```cpp
const double EPS = 1e-9;
while(r - l < EPS) {
    double lmid = l + (r - l) / 3;
    double rmid = r - (r - l) / 3;
    lans = f(lmid),rans = f(rmid);
    // 求凹函数的极小值
    if(lans <= rans) r = rmid;
    else l = lmid;
    // 求凸函数的极大值
    if(lans >= rans) l = lmid;
    else r = rmid;
}
// 输出 l 或 r 都可
cout << l << endl;
```

### 思想

#### 分治

#### 倍增

#### 对拍

什么是对拍？ 当我们的程序过了样例，是否意味着它一定能 AC 呢？显然大多数情况下都是不行的。所以我们需要自己设计一些数据来测试我们的程序，但有的题目数据很大，我们肉眼无法看出程序计算的结果是否正确，手工计算又非常耗时，在紧张的比赛中，我们该怎么应对呢？于是有了对拍。 对拍简单的说就是当你写完一个题目的程序以后，再写一个暴力求解该题目的程序，然后自己生成一些测试数据，看同样的数据，两个程序输出的结果是否相同，不同意味着被对拍的程序有问题。以此来帮助你修改程序，提高通过率的方法，我们称为对拍。
$$第一步：建立待测程序ZJ.cpp$$

```cpp
int main()
{
    freopen("data.in","r",stdin);          //从文件data.in中读入数据
    freopen("ZJ.out","w",stdout);    //输出的结果存在ZJ.out文件中
    //主程序
}
/*
我们把这个程序保存为ZJ.cpp,但这个程序是否正确呢？
我们再写一个暴力程序来验证它
*/
```

$$第二步：建立暴力程序BL.cpp$$

```cp
int main()
 {
     freopen("data.in","r",stdin);       //注意，暴力程序读入的数据仍然是data.in
     freopen("BL.out","w",stdout);    //暴力程序输出的结果是BL.out
     //暴力主程序
}
我们把这个程序保存为BL.cpp
注意：我们不在乎暴力程序效率，只需要保证它的结果是正确的就行了。
```

$$第三步：建立输入数据生成程序data.in$$

```cpp
//该程序按照题目给定的格式生成随机数据。
#include<cstdlib>                            //加入这个包才能使用随机函数rand()
#include<cstdio>
#include<ctime>                              //加入这个包就能以时间为种子初始化随机函数
#include<iostream>
using namespace std;
int main()
{
    freopen("data.in","w",stdout);           //注意：该程序生成的数据到data.in中
    srand(time(NULL));                       //重要：初始化随机函数，以时间为种子
    int n=rand()%10000+1;                    //生成一个1到10000之间的随机整数n
    int m=rand()%10000+1;
    printf("%d %d\n",n,m);
    for(int i=1;i<=n;i++)
    printf("%d ",rand()%20000-rand()%10000); //生成-10000到10000间的数字
    printf("\n");
    for(int i=1;i<=m;i++)
        {
            int x=rand()%n+1;               //保证生成的数据是x<=y
            int y=x+rand()%n+1;
            if(y>n)y=n;
            printf("%d %d\n",x,y);
        }
}
注意：
rand()只能生成0到32767之间的随机整数，如果要生成1到50000之间的整数，可以写成：
rand()%30000+rand()%20000+1
```

$$建立对拍文件（对拍.bat）$$

```shell
@echo off                                 //关闭回显
:loop                                     //执行循环
date.exe                                  //调用date、ZJ、BL
ZJ.exe
BL.exe
fc ZJ.out BL.out                          //对比程序答案
if not errorlevel 1 goto loop             //结果相同，继续对拍
pause                                     //结果不同，对拍暂停，显示出错的地方
:end
```

### 日期

#### 蔡勒 bai（Zeller）公式

w：星期%7 的值
c：年份前两位
y：世纪后两位
m：月(1 月 2 月+12)
d：日

```cpp
void check(int c,int y,int m,int d)
{
    int w;
    if(m<3)
        {
            m+=12;
            if(y==0)
            {
                y=99;
                c--;
            }
            else y--;
        }
    w=(c/4-2*c+y+y/4+(13*(m+1))/5+d-1)%7;

    {
        if(m>=13)
        {
            m-=12;
            if(y==99){y=0;c++;}
            else y++;
        }
    }
    cout<<w<<endl;
}
```

# 优化模板

## 卡常优化

### 读入挂

```cpp
template<class T>void read(T &x)
{
    x=0;int f=0;char ch=getchar();
    while(ch<'0'||ch>'9')  {f|=(ch=='-');ch=getchar();}
    while(ch>='0'&&ch<='9'){x=(x<<1)+(x<<3)+(ch^48);ch=getchar();}
    x=f?-x:x;
    return;
}
```

### 文件读入

```cpp
const int bsz=1<<18;
char bf[bsz],*he,*ta;
inline char gc(){
    if(he==ta){
        int l=fread(bf,1,bsz,stdin);
        ta=(he=bf)+l;
    }
    return *he++;
}
inline int read(){
    int x=0,f=1;
    char c=gc();
    for(;!isdigit(c);c=gc()) { if(c=='-') { f=-1;}}
    for(;isdigit(c);c=gc()) x=x*10+c-'0';
    return x*f;
}
inline void write(int x){
    if(x<0) putchar('-'), x=-x;
    if(x>=10) write(x/10);
    putchar(x%10+'0');
}
```

```cpp
#define getchar() (S==T&&(T=(S=BB)+fread(BB,1,1<<15,stdin),S==T)?EOF:*S++)
char BB[1<<20],*S=BB,*T=BB;
inline int read(){
    register int x=0;
    register char ch=getchar();
    while(ch<48) ch=getchar();
    while(ch>47) x=x*10+(ch^48),ch=getchar();
    return x;
}
inline void write(register int x){
    if(x>9) write(x/10);
    putchar(x%10+'0');
}
```

### 字符串读入优化

```cpp
inline string read()//inline继续加快速度
{
    char ch=getchar();
    string st1="";
    while (!((ch>='a')&&(ch<='z')))//把前面没用的东西去掉,当然,ch在什么范围内可以依据需要修改
      ch=getchar();
    while ((ch>='a')&&(ch<='z'))
      st1+=ch,ch=getchar();
    return st1;//返回
}//在主程序内可以写st=read(),这样子要读的字符串就到了st内
```

### 1LL

使用1LL加速（不是很懂）

### 读入优化

```c++
std::ios::sync_with_stdio(false);//解绑c++和c
std::cin.tie(0);//解绑scanf和cin
```

### 取模优化

```cpp
int MOD(int x, int y){
    return x - y * (x / y);
}//==x%y
```

### 绝对值优化

```cpp
inline int Abs(int a){//绝对值优化
{ int b=a>>31; return (a+b)^b; }
```

### 比较语句

```cpp
if(){
    ；
}
else{
    ；
} //慢

()==()?():();// ==可以换成任何二元比较运算符(== >= > < <= )能判断真值即可。
```

### 内联函数

```cpp
int IMhanshu()
{
}
inline int IMhanshu()
{
}
```

$$
重复使用多的可以使用内联函数，用的少的和递归就别用了。
$$

### 循环展开

```cpp
void Init_Array(int *dest, int n)
{
    int i;
    for(i = 0; i < n; i++)
        dest[i] = 0;
}
```

```cpp
void Init_Array(int *dest, int n)
{
    int i;
    int limit = n - 3;
    for(i = 0; i < limit; i+= 4)//每次迭代处理4个元素
    {
        dest[i] = 0;
        dest[i + 1] = 0;
        dest[i + 2] = 0;
        dest[i + 3] = 0;
    }
    for(; i < n; i++)//将剩余未处理的元素再依次初始化
        dest[i] = 0;
}
```

$$
在缓存和寄存器允许的情况下一条语句内大量的展开运算会刺激 CPU 并发
$$

### 卡 cache

$$
开数多维组的时候小的开在前面，访问多的一维开在前面寻址快。
$$

### 前置++/--运算符

$$
用++i代替i++
$$

### CPU 寄存器变量 register

$$
对于一些频繁使用的变量，可以声明时加上该关键字，运行时可能会把该变量放到CPU寄存器中。
$$

### 究极奥义 pragma

```cpp
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-fwhole-program")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-fstrict-overflow")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-skip-blocks")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("-funsafe-loop-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")

#pragma GCC optimize("O2")//这个好像别人常用
#pragma GCC optimize("O3")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("Ofast,no-stack-protector")
#pragma GCC target("sse3","sse2","sse")
#pragma GCC target("avx","sse4","sse4.1","sse4.2","ssse3")
#pragma GCC target("f16c")
#pragma GCC optimize("inline","fast-math","unroll-loops","no-stack-protector")
#pragma GCC diagnostic error "-fwhole-program"
#pragma GCC diagnostic error "-fcse-skip-blocks"
#pragma GCC diagnostic error "-funsafe-loop-optimizations"
#pragma GCC diagnostic error "-std=c++14"
```

### BM（模板）

```c++
#include <bits/stdc++.h>

using namespace std;
#define rep(i,a,n) for (long long i=a;i<n;i++)
#define per(i,a,n) for (long long i=n-1;i>=a;i--)
#define pb push_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define fi first
#define se second
#define SZ(x) ((long long)(x).size())
typedef vector<long long> VI;
typedef long long ll;
typedef pair<long long,long long> PII;
const ll mod=1e9+7;
ll powmod(ll a,ll b) {ll res=1;a%=mod; assert(b>=0); for(;b;b>>=1){if(b&1)res=res*a%mod;a=a*a%mod;}return res;}
// head

long long _,n;
namespace linear_seq
{
    const long long N=10010;
    ll res[N],base[N],_c[N],_md[N];

    vector<long long> Md;
    void mul(ll *a,ll *b,long long k)
    {
        rep(i,0,k+k) _c[i]=0;
        rep(i,0,k) if (a[i]) rep(j,0,k)
            _c[i+j]=(_c[i+j]+a[i]*b[j])%mod;
        for (long long i=k+k-1;i>=k;i--) if (_c[i])
            rep(j,0,SZ(Md)) _c[i-k+Md[j]]=(_c[i-k+Md[j]]-_c[i]*_md[Md[j]])%mod;
        rep(i,0,k) a[i]=_c[i];
    }
    long long solve(ll n,VI a,VI b)
    { // a 系数 b 初值 b[n+1]=a[0]*b[n]+...
//        printf("%d\n",SZ(b));
        ll ans=0,pnt=0;
        long long k=SZ(a);
        assert(SZ(a)==SZ(b));
        rep(i,0,k) _md[k-1-i]=-a[i];_md[k]=1;
        Md.clear();
        rep(i,0,k) if (_md[i]!=0) Md.push_back(i);
        rep(i,0,k) res[i]=base[i]=0;
        res[0]=1;
        while ((1ll<<pnt)<=n) pnt++;
        for (long long p=pnt;p>=0;p--)
        {
            mul(res,res,k);
            if ((n>>p)&1)
            {
                for (long long i=k-1;i>=0;i--) res[i+1]=res[i];res[0]=0;
                rep(j,0,SZ(Md)) res[Md[j]]=(res[Md[j]]-res[k]*_md[Md[j]])%mod;
            }
        }
        rep(i,0,k) ans=(ans+res[i]*b[i])%mod;
        if (ans<0) ans+=mod;
        return ans;
    }
    VI BM(VI s)
    {
        VI C(1,1),B(1,1);
        long long L=0,m=1,b=1;
        rep(n,0,SZ(s))
        {
            ll d=0;
            rep(i,0,L+1) d=(d+(ll)C[i]*s[n-i])%mod;
            if (d==0) ++m;
            else if (2*L<=n)
            {
                VI T=C;
                ll c=mod-d*powmod(b,mod-2)%mod;
                while (SZ(C)<SZ(B)+m) C.pb(0);
                rep(i,0,SZ(B)) C[i+m]=(C[i+m]+c*B[i])%mod;
                L=n+1-L; B=T; b=d; m=1;
            }
            else
            {
                ll c=mod-d*powmod(b,mod-2)%mod;
                while (SZ(C)<SZ(B)+m) C.pb(0);
                rep(i,0,SZ(B)) C[i+m]=(C[i+m]+c*B[i])%mod;
                ++m;
            }
        }
        return C;
    }
    long long gao(VI a,ll n)
    {
        VI c=BM(a);
        c.erase(c.begin());
        rep(i,0,SZ(c)) c[i]=(mod-c[i])%mod;
        return solve(n,c,VI(a.begin(),a.begin()+SZ(c)));
    }
};

int main()
{
    while(~scanf("%I64d", &n))
    {   printf("%I64d\n",linear_seq::gao(VI{1,5,11,36,95,281,781,2245,6336,18061, 51205},n-1));
    }
}
```

## 功能优化

### \_\_int 128 读入读出（模板）

```c++
ll read()
{
   int X=0,w=0; char ch=0;
   while(!isdigit(ch)) {w|=ch=='-';ch=getchar();}
   while(isdigit(ch)) X=(X<<3)+(X<<1)+(ch^48),ch=getchar();
   return w?-X:X;
}
void print(__int128 x)
{
   if(x<0){putchar('-');x=-x;}
   if(x>9) print(x/10);
   putchar(x%10+'0');
}
```

### 高精度

```c++
 //清除前缀0，如果结果是空字符串则设为0
inline void clear(string& a){
    while(a.length()>0 && a[0]=='0')
        a.erase(0, 1);
    if(a == "")
        a = "0";
}

//如果a>=b则返回真（如果包含前缀零会被消除）
bool isBigger(string a, string b){
    clear(a);
    clear(b);
    if(a.length() > b.length())
        return true;
    if(a.length()==b.length() && a>=b)
        return true;
    return false;
}

//两个高精度正整数加法 a+b
string stringAddString(string a, string b){
    //1、对位，将两个数补零直到其具有相同长度
    while(a.length() < b.length())
        a = '0' + a;
    while(a.length() > b.length())
        b = '0' + b;
    //2、补零，在开头再加一个0以便进位
    a = '0' + a;
    b = '0' + b;
    //3、从低位开始相加，注意进位
    for(int i=a.length()-1; i>=0; i--){
        a[i] = a[i] + b[i] - '0';
        if(a[i] > '9'){
            a[i] = a[i] - 10;
            a[i-1] += 1;
        }
    }
    clear(a);
    return a;
}

//两个高精度正整数减法 a-b
string stringSubString(string a, string b){
    bool aBigger = true;
    //1、对位，将两个数补零直到其具有相同长度
    while(a.length() < b.length())
        a = '0' + a;
    while(a.length() > b.length())
        b = '0' + b;
    //2、推测结果正负值，调整为前大后小
    if(a < b)
    {
        aBigger = false;
        string buf = b;
        b = a;
        a = buf;
    }
    //3、从低位开始相减，注意借位
    for(int i=a.length()-1; i>=0; i--){
        if(a[i] >= b[i]){
            a[i] = a[i] - (b[i] - '0');
        }else{
            a[i] = a[i] + 10;
            a[i-1] -= 1;
            a[i] = a[i] - (b[i] - '0');
        }
    }
    clear(a);
    if(!aBigger)
        a = '-' + a;
    return a;
}

//两个高精度正整数乘法 a*b
//依赖加法
string stringMultString(string a, string b){
    string result = "0";
    if(a.length() < b.length()){
        string buf = a;
        a = b;
        b = buf;
    }
    //多位数乘一位数可以直接使用加法
    //多位数乘以形如d*10^n的数可以转化为多位数乘以一位数
    //多位数乘以多位数可以转化为若干个多位数乘以一位数相加
    for(int i=b.length()-1; i>=0; i--){
        for(int j=0; j<b[i]-'0'; j++){
            result = stringAddString(result, a);
        }
        a = a + '0';
    }
    clear(result);
    return result;
}

//两个高精度正整数除法 a/b
//依赖减法
string stringDivString(string a, string b){
    clear(a);
    clear(b);
    if(b == "0")
        return "Error!";

    string result = "";
    string remainder = "";
    //从高位开始除，和手算除法一样
    // 一旦取位刚好大于被除数则开始用减法求商
    for(int i=0; i<a.length(); i++){
        remainder = remainder + a[i];
        result = result + '0';
        while(isBigger(remainder, b)){
            result[result.length()-1]++;
            remainder = stringSubString(remainder, b);
        }
    }
    clear(result);
    return result;
}

//两个高精度正整数求余 a%b
//依赖减法
string stringModString(string a, string b){
    clear(a);
    clear(b);
    if(b == "0")
        return "Error!";

    string result = "";
    string remainder = "";
    //和除法唯一的区别就是返回值不一样
    for(int i=0; i<a.length(); i++){
        remainder = remainder + a[i];
        result = result + '0';
        while(isBigger(remainder, b)){
            result[result.length()-1]++;
            remainder = stringSubString(remainder, b);
        }
    }
    clear(remainder);
    return remainder;
}

//两个高精度数求最大公约数 gcd(a,b)
//依赖求余
string stringGcd(string a, string b){
    clear(a);
    clear(b);
    if(!isBigger(a, b)){
        string buf = a;
        a = b;
        b = buf;
    }
    //使用辗转相除法求最大公约数
    if(b == "0"){
        return a;
    }else{
        return stringGcd(b, stringModString(a, b));
    }
}

//两个高精度数求最小公倍数 lcm(a,b)
//依赖乘法
//依赖除法
//依赖最大公约数
string stringLcm(string a, string b){
    clear(a);
    clear(b);
    string buf = stringMultString(a, b);
    //使用公式 lcm(a,b)=a*b/gcd(a,b)
    if(buf == "0"){
        return "0";
    }else{
        return stringDivString(buf, stringGcd(a, b));
    }
}
```

#### 数值转字符串

```cpp
#include <sstream>
string s;
stringstream ss;
ss<<n;
ss>>s;
```

### 自测功能

```cpp
#define FIN freopen("input.txt","r",stdin);
#define FON freopen("output.txt","w+",stdout);

#define bug printf("*********\n")
#define debug1(x) cout<<"["<<#x<<" "<<(x)<<"]\n"
#define debug2(x,y) cout<<"["<<#x<<" "<<(x)<<" "<<#y<<" "<<(y)<<"]\n"
#define debug3(x,y,z) cout<<"["<<#x<<" "<<(x)<<" "<<#y<<" "<<(y)<<" "<<#z<<" "<<z<<"]\n"

#ifndef ONLINE_JUDGE
    FIN
#endif
```

## 初始模板

```cpp
#include<cstdio>
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdlib>
#include <sstream>
#include<math.h>
#include<vector>
//#include<unordered_map>

#include<bits/stdc++.h>
using namespace std;
#define mod 1000000007
#define ll long long

template<class T>void read(T &x)
{
    x=0;int f=0;char ch=getchar();
    while(ch<'0'||ch>'9')  {f|=(ch=='-');ch=getchar();}
    while(ch>='0'&&ch<='9'){x=(x<<1)+(x<<3)+(ch^48);ch=getchar();}
    x=f?-x:x;
    return;
}

int main()
{
    //std::ios::sync_with_stdio(false);

    return 0;
}
```

## 庇佑

```cpp
// warm heart, wagging tail,and a smile just for you!
//
//                            _ooOoo_
//                           o8888888o
//                           88" . "88
//                           (| -_- |)
//                           O\  =  /O
//                        ____/`---'\____
//                      .'  \|     |//  `.
//                     /  \|||  :  |||//  \
//                    /  _||||| -:- |||||-  \
//                    |   | \  -  /// |  |
//                    | \_|  ''\---/''  |   |
//                    \  .-\__  `-`  ___/-. /
//                  ___`. .'  /--.--\  `. . __
//               ."" '<  `.___\_<|>_/___.'  >'"".
//              | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//              \  \ `-.   \_ __\ /__ _/   .-` /  /
//         ======`-.____`-.___\_____/___.-`____.-'======
//                            `=---='
//        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//                 佛祖保佑      永无BUG
```

```text
 *　　　　　　　　┏┓　　 　┏┓
 * 　　　　　　　┏┛┗━━━━━━━┛┗━━━┓
 * 　　　　　　　┃　　　　　　　 ┃ 　
 * 　　　　　　　┃　　　━　　 　 ┃
 * 　　　　　　　┃　＞　  　＜　 ┃
 * 　　　　　　　┃　　　　　  　 ┃
 * 　　　　　　　┃...　⌒　... 　┃
 * 　　　　　　　┃　　　　　　　 ┃
 * 　　　　　　　┗━┓　　　  ┏━┛
 * 　　　　　　　　　┃　　　┃　Code is far away from bug with the animal protecting　　　　　　　　　　
 * 　　　　　　　　　┃　　　┃   神兽保佑,代码无bug
 * 　　　　　　　　　┃　　　┃　　　　　　　　　　　
 * 　　　　　　　　　┃　　　┃  　　　　　　
 * 　　　　　　　　　┃　　　┃
 * 　　　　　　　　　┃　　　┃　　　　　　　　　　　
 * 　　　　　　　　　┃　　　┗━━━┓
 * 　　　　　　　　　┃　　　　　　　┣┓
 * 　　　　　　　　　┃　　　　　　　┏┛
 * 　　　　　　　　　┗┓┓┏━┳┓┏┛
 * 　　　　　　　　　　┃┫┫　┃┫┫
 * 　　　　　　　　　　┗┻┛　┗┻┛
 */
```
