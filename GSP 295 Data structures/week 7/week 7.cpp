#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>

using namespace std;

typedef vector<vector<pair<int,float> > > Graph;

class Comparator
{
public:
 int operator() ( const pair<int,float>& p1, const pair<int,float> &p2)
 {
 return p1.second>p2.second;
 }
};

void dijkstra(const Graph  &G,const int &source,const int &destination,vector<int> &path)
{
vector<float> d(G.size());

vector<int> parent(G.size());

	for(unsigned int i = 0 ;i < G.size(); i++)
	{
	 d[i] = std::numeric_limits<float>::max();
	 parent[i] = -1;
	}

priority_queue<pair<int,float>, vector<pair<int,float> >, Comparator> Q;

d[source] = 0.0f;

Q.push(make_pair(source,d[source]));

while(!Q.empty())
{
 int u = Q.top().first;
 if(u==destination) break;
 Q.pop();

 for(unsigned int i=0; i < G[u].size(); i++)
 {
  int v = G[u][i].first;

  float w = G[u][i].second;

  if(d[v] > d[u]+w)
  	{
   		d[v] = d[u]+w;
   		parent[v] = u;
   		Q.push(make_pair(v,d[v]));
  		}
	 }
}

path.clear();

int p = destination;

path.push_back(destination);

	while(p!=source)
	{
 	p = parent[p];
 	path.push_back(p);
	}
}

int main()
{
    
    Graph g;
    g.resize(7);
    g[1].push_back(make_pair(1,7));
    g[2].push_back(make_pair(0,7));

    g[1].push_back(make_pair(2,9));
    g[3].push_back(make_pair(0,9));

    g[1].push_back(make_pair(5,14));
    g[6].push_back(make_pair(0,14));

    g[2].push_back(make_pair(2,10));
    g[3].push_back(make_pair(1,10));

    g[2].push_back(make_pair(3,15));
    g[4].push_back(make_pair(1,15));

    g[3].push_back(make_pair(5,2));
    g[6].push_back(make_pair(2,2));

    g[3].push_back(make_pair(3,11));
    g[4].push_back(make_pair(2,11));

    g[4].push_back(make_pair(4,6));
    g[5].push_back(make_pair(3,6));

    g[5].push_back(make_pair(5,9));
    g[6].push_back(make_pair(4,9));

    vector<int> path;

	int choice1;
	int choice2;
	int mm;
	int nn;

	cout << "1. Dallas" << endl;
	cout << "2. Austin" << endl;
	cout << "3. Washington" << endl;
	cout << "4. Denver " << endl;
	cout << "5. Atlanta" << endl;
	cout << "6. Chicago" << endl;
	cout << endl;
	cout << "enter city number: " << endl;
	cin >> choice1;
	mm = choice1;
	cout << "enter city going to: " << endl;
	cin >> choice2;
	nn = choice2;

    dijkstra(g,mm,nn,path);

    for(int i=path.size()-1;i>=0;i--)
        cout << path[i] << "->";
    return 0;
}