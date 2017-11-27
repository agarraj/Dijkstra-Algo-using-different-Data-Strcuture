#include <bits/stdc++.h>
using namespace std;
//LLONG_MAX
#define LL long long int
#define MP make_pair
typedef pair< int, int> pi;
typedef vector< pi > vpi;
typedef vector< vpi > vvpi;


class comp
{
public:
    bool operator() (const pair< int, LL>& p1, const pair< int, LL>& p2)
    {
        return p1.second > p2.second;
    }
};

int main(){
	
		int n, m;
		clock_t Start, Time;

		//cin >> n >> m;
		cout<<"number of vertex"<<endl;
		cin>>n;
		cout<<"number of edges"<<endl;

		cin>>m;

		vvpi adj_list(n+1);
		cout<<"enter the edges and cost"<<endl;
		for(int i=0; i<m; i++){
			int x, y, r;
			//cin >> x >> y >> r;
			scanf("%d %d %d", &x, &y, &r);

			adj_list[x].push_back(MP(y,r));
			adj_list[y].push_back(MP(x,r));

		}
		
		cout<<"enter the source"<<endl;
		int source;
		//LL k;
		//cin >> source >> k;
		cin>> source;
		Start = clock();


		bool visited[n+1]={false};
		vector<LL > cost(n+1, LLONG_MAX);
        //cout<<cost[n]<<"check"<<endl;
        
		priority_queue< pair< int, LL>, vector< pair< int,LL> >, comp > pq;

		pq.push(MP(source, 0)); //source pushed
        
		while( !pq.empty()){
            //cout<<"A"<<endl;
			pair<int, LL> p;
			p = pq.top();
			pq.pop();
			if(visited[p.first])
				continue;
			visited[p.first] = true;
			cost[p.first] = p.second;
			
			for(int i=0; i<adj_list[p.first].size(); i++)
			{	int node=(adj_list[p.first][i]).first;
                //cout<<"node:"<<node<<endl;
				int dist=(adj_list[p.first][i]).second + cost[p.first];
                //cout<<"dist:"<<dist<<endl;
                //cout<<"cosy of node"<<cost[node]<<endl;
                //cout<<cost[2];
   				if(dist < cost[node])
				{   ////cout<<"asdansa";
					cost[node] = dist;
					pq.push(MP(node, dist));

				}
			}
            
            
		}
		Time = clock()-Start;
		for(int i=1;i<=n;i++){   
        cout<<"vertex:"<<i<<" "<<cost[i]<<endl;
    	}
        
        
        cout<<"time taken:"<<Time<<endl;

	return 0;
}