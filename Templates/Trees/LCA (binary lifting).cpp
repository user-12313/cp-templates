// https://www.luogu.com.cn/problem/P3379
// livin' for the hope of it all

#include <bits/stdc++.h>
#define int long long
using namespace std;

const int N=100013, L=26;
int n, m;
vector<int> s[N];

struct node
{
    int anc[N][L], dep[N];
    
    node()
    {
        memset(anc, -1, sizeof(anc));
        dep[0]=0; dfs(0, 0);
    }
    void dfs(int u, int p)
    {
        anc[u][0]=p;
        for (int a=1; a<L; a++) anc[u][a]=anc[anc[u][a-1]][a-1];
        
        for (auto v:s[u])
        {
            if (v!=p)
            {
                dep[v]=dep[u]+1;
                dfs(v, u);
            }
        }
    }
    int query(int x, int y)
    {
        if (dep[x]<dep[y]) swap(x, y);
        
        for (int a=L-1; a>=0; a--) if (dep[x]-(1<<a)>=dep[y]) x=anc[x][a];
        if (x==y) return x;
        
        for (int a=L-1; a>=0; a--) if (anc[x][a]!=anc[y][a])
        {
            x=anc[x][a];
            y=anc[y][a];
        }
        return anc[x][0];
    }
} *lca;

signed main()
{
    cin>>n>>m;
    for (int a=0; a<n-1; a++)
    {
        int x, y; cin>>x>>y;
        s[x].push_back(y);
        s[y].push_back(x);
    }
    
    lca=new node();
    while (m--)
    {
        int x, y; cin>>x>>y;
        cout<<lca->query(x, y)<<endl;
    }
    return 0;
}
