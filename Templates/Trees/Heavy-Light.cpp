// https://www.luogu.com.cn/problem/P3384
// livin' for the hope of it all

#include <bits/stdc++.h>
#define int long long
using namespace std;

const int N=100013;
int n, m;
vector<int> s[N];
int val[N];
int seq[N], pre[N], post[N], i=0;

struct node_seg
{
	int l, r, mid;
    int v, lazy;
    node_seg *lptr, *rptr;
    
    node_seg(int L, int R)
    {
        l=L; r=R; mid=(l+r)>>1;
        lazy=0;
        if (l==r)
        {
            lptr=rptr=nullptr;
            v=val[seq[l]];
        }
        else
        {
            lptr=new node_seg(l, mid);
            rptr=new node_seg(mid+1, r);
            v=lptr->v+rptr->v;
        }
    }
    void update(int x, int y, int val)
    {
        if (l==x && r==y)
        {
            lazy+=val;
            v+=val*(r-l+1);
            return;
        }
        pushdown();
        if (y<=mid) lptr->update(x, y, val);
        else if (x>=mid+1) rptr->update(x, y, val);
        else
        {
            lptr->update(x, mid, val);
            rptr->update(mid+1, y, val);
        }
        v=lptr->v+rptr->v;
    }
    int query(int x, int y)
    {
        if (x==l && r==y) return v;
        pushdown();
        if (y<=mid) return lptr->query(x, y);
        else if (x>=mid+1) return rptr->query(x, y);
        else return lptr->query(x, mid) + rptr->query(mid+1, y);
    }
    void pushdown()
    {
        if (lazy==0) return;
        lptr->lazy+=lazy;
        rptr->lazy+=lazy;
        lptr->v+=lazy*(mid-l+1);
        rptr->v+=lazy*(r-mid);
        lazy=0;
    }
} *segtree;
struct node_hld
{
    int dep[N], p[N], sz[N];
    int heavy[N], head[N];
    
    node_hld()
    {
        memset(heavy, -1, sizeof(heavy));
        dep[0]=p[0]=0;
        dfs_sz(0);
        dfs_hl(0, 0);
    }
    void dfs_sz(int u)
    {
        sz[u]=1;
        for (auto v:s[u])
        {
            if (v==p[u]) continue;
            
            dep[v]=dep[u]+1; p[v]=u;
            dfs_sz(v);
            if (heavy[u]==-1 || sz[heavy[u]]<sz[v]) heavy[u]=v;
            sz[u]+=sz[v];
        }
    }
    void dfs_hl(int u, int H)
    {
        seq[i]=u; pre[u]=i++;
        head[u]=H;
        if (heavy[u]!=-1) dfs_hl(heavy[u], H);
        for (auto v:s[u]) if (v!=p[u] && v!=heavy[u]) dfs_hl(v, v);
        post[u]=i-1;
    }
    void update(int x, int y, int val)
    {
        int fx=head[x], fy=head[y];
        while (fx!=fy)
        {
            if (dep[fx]<dep[fy])
            {
                swap(x, y);
                swap(fx, fy);
            }
            segtree->update(pre[fx], pre[x], val);
            x=p[fx]; fx=head[x];
        }
        if (dep[x]>dep[y]) swap(x, y);
        segtree->update(pre[x], pre[y], val);
    }
    int query(int x, int y)
    {
        int fx=head[x], fy=head[y]; int ans=0;
        while (fx!=fy)
        {
            if (dep[fx]<dep[fy])
            {
                swap(x, y);
                swap(fx, fy);
            }
            ans += segtree->query(pre[fx], pre[x]);
            x=p[fx]; fx=head[x];
        }
        if (dep[x]>dep[y]) swap(x, y);
        return ans + segtree->query(pre[x], pre[y]);
    }
} *hld;

signed main()
{
    cin>>n>>m;
    for (int a=0; a<n; a++) cin>>val[a];
    for (int a=0; a<n-1; a++)
    {
        int x, y; cin>>x>>y;
        s[x].push_back(y);
        s[y].push_back(x);
    }
    
    hld=new node_hld();
    segtree=new node_seg(0, n-1);
    while (m--)
    {
        int t; cin>>t;
        if (t==1)
        {
            int x, y, z; cin>>x>>y>>z;
            hld->update(x, y, z);
        }
        else if (t==2)
        {
            int x, y; cin>>x>>y;
            cout<<hld->query(x, y)<<endl;
        }
        else if (t==3)
        {
            int x, y; cin>>x>>y;
            segtree->update(pre[x], post[x], y);
        }
        else
        {
            int x; cin>>x;
            cout<<segtree->query(pre[x], post[x])<<endl;
        }
    }
    return 0;
}
