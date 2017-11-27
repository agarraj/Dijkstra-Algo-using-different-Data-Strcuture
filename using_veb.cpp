#include <bits/stdc++.h>
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;
int VEB_SIZE=100000;
int MAX_INT=99999;
class VanEmdeBoasTree {
	typedef unsigned long u_type;
	bool empty_status = true;
	u_type 	min;
	u_type 	max;
	u_type 	m;
	u_type 	unpadded_m;
	VanEmdeBoasTree* summary;
	std::vector<VanEmdeBoasTree*> children;
private:/* Internal calculations */
	inline u_type floor_root(u_type q){
		return static_cast<u_type>(exp2(std::floor(log2(static_cast<double>(q))/2)));
	}
	inline u_type ceil_root(u_type q){
		return static_cast<u_type>(exp2(std::ceil(log2(static_cast<double>(q))/2)));
	}
	inline u_type high(u_type x){
		return static_cast<u_type>(std::floor(static_cast<long double>(x)/floor_root(m)));
	}
	inline u_type low(u_type x){
		return x%floor_root(m);
	}
	inline u_type index(u_type x, u_type y){
		return ((x*floor_root(m)) + y);
	}
public:
	VanEmdeBoasTree(u_type universe):min(0),max(0),m(universe),unpadded_m(universe),summary(nullptr){
		if((universe & (universe - 1)) != 0){
			--universe;
			universe |= universe >> 1;
			universe |= universe >> 2;
			universe |= universe >> 4;
			universe |= universe >> 8;
			universe |= universe >> 16;
			universe |= universe >> 32;
			++universe;
			m = universe;
			min = universe;
		}
		if(universe > 2){
			summary = new VanEmdeBoasTree(ceil_root(universe));
			children.reserve(ceil_root(universe));
			for(u_type i = 0; i < ceil_root(universe); ++i){
				children.push_back(new VanEmdeBoasTree(floor_root(universe)));
			}
		}
	}
	~VanEmdeBoasTree(){
		if(m > 2){
		  for (auto iter = children.begin() ; iter != children.end(); ++iter){
		     delete (*iter);
		   }
		   delete summary;

		   children.clear();
		}
	}
	inline bool empty() const{
		return(empty_status);
	}
	long minimum() const {
		if(!empty()){
			return min;
		} else {
			return -1;
		}
	}
	long maximum() const {
		if(!empty()){
			return max;
		} else {
			return -1;
		}
	}

	

	 bool search(u_type key) {
		 if(empty()){
			 return false;
		 } else if(key == min || key == max){
/*			 std::cout<<"Found:"<<"("<<key<<","<<high(key)<<","<<low(key)<<")"<<children.size()<<","<<min<<","<<max<<","<<m<<"\n";*/
			 return true;
		 } else if(m == 2){
			 return false;
		 } else {
/*			 std::cout<<"recurse:"<<"("<<key<<","<<high(key)<<","<<low(key)<<")"<<children.size()<<","<<min<<","<<max<<","<<m<<"\n";*/
			 return (children[high(key)]->search(low(key)));
		 }
	 }

     void empty_insert(u_type key){
    	empty_status = false;
 		min = key;
 		max = key;
 		return;
     }

	 void insert(u_type key) {
		 if(empty()){
			 empty_insert(key);
			 return;
		 } else {
			 if( key < min ){
				 u_type swp = key;
				 key = min;
				 min = swp;
			 }
			 if( m > 2){
				 if(children[high(key)]->empty()){
					 summary->insert(high(key));
					 children[high(key)]->empty_insert(low(key));
				 } else {
					 children[high(key)]->insert(low(key));
				 }
			 }
			 if(key > max){
				 max = key;
			 }
			 if(empty()){//XXX
				 empty_status = false;
			 }
		 }
	 }

	 void remove(u_type key) {
/*			std::cout<<"Remove:"<<"("<<key<<","<<high(key)<<","<<low(key)<<")"<<children.size()<<","<<min<<","<<max<<","<<m<<"\n";*/
		 if(empty()){
			 return;
		 } else if(min == max){
			 min = 0;
			 max = 0;
			 empty_status = true;
			 return;
		 } else if(m == 2){
			 if(key == 0){
				 min = 1;
			 } else {
				 min = 0;
			 }
			 max = min;
		 } else {
			 if(key == min){
				 long first_child = summary->minimum();
/*				 std::cout<<"also:"<<index(first_child, children[first_child]->minimum())<<first_child;if(children[first_child]->empty()){std::cout<<"empt";} std::cout<<"\n";*/
				 if(children[first_child]->empty()){
					 min = max;
					 return;
				 }
				 key = index(first_child, children[first_child]->minimum());
				 min = key;
			 }
			 children[high(key)]->remove(low(key));
			 if(children[high(key)]->empty()){
				 summary->remove(high(key));
				 if(key == max) {
					 if(summary->empty()){
						 max = min;
					 } else {
						 max = index(summary->maximum(), children[summary->maximum()]->maximum());
					 }
				 }
			 } else if (key == max){
				 max = index(high(key),children[high(key)]->maximum());
			 }
		 }
	 }

	void pretty_print(int tabs) const {
		for(int i = 0; i<tabs; ++i){std::cout<<"   ";}
		std::cout<<"*Summary "<<m<<" elements"<<"("<<min<<","<<max<<")"<<"\n";
		if(m != 2){summary->pretty_print(tabs + 1); }
		if(m != 2){
			for(int i = 0; i<tabs; ++i){std::cout<<"   ";}
			std::cout<<"*Children:\n";
			for(auto iter = children.begin(); iter != children.end(); ++iter){
				(*iter)->pretty_print(tabs +1);
			}
		}
	}
};

int main(){

	VanEmdeBoasTree *VanEmdeBoasTree = new class VanEmdeBoasTree(VEB_SIZE);
	clock_t Start, Time;
	int v,c,i,a,b,x,e,s;
	cout <<"Enter the number of vertices of the graph\n";
	cin >> v;
	cout << "Enter the number of edges\n";
	cin >> e;
	// cout <<"Enter the maximum cost of an edge\n";
	// cin >> max_cost;
	cout << "Enter the edge u --> v and cost of edge\n";
	vector <pair <int,int > > g[v+1];
	vector <int > map_cost_edge[VEB_SIZE]; 
	for(i = 0; i < e; i++)
	{
		cin >> a >> b >> c;
		g[a].push_back(make_pair(b,c));
		g[b].push_back(make_pair(a,c));
	}
	cout <<"Enter the source vertices\n";
	cin >> s;

	vector <int> dist (v+1,MAX_INT);
    dist[s] = 0;
    Start = clock();
    VanEmdeBoasTree -> insert(0);
    map_cost_edge[0].push_back(s);

    while(!VanEmdeBoasTree->empty())
    {
    	x = VanEmdeBoasTree -> minimum(); //minimum cost from the veb tree
    	if(map_cost_edge[x].size()==1)
    	{
    		int extractMin = VanEmdeBoasTree -> minimum();
    		//VanEmdeBoasTree -> extractMin();
    		VanEmdeBoasTree -> remove(extractMin);

    	}
		int node = map_cost_edge[x].front(); // vertex with min cost
		map_cost_edge[x].erase(map_cost_edge[x].begin());
		vector < pair <int , int > > ::iterator it;
		for(it = g[node].begin(); it != g[node].end(); ++it)
		{
			a = it-> first; // dest vertex
			b = it-> second; // cost of the edge
			if(dist[a] > dist[node]+ b)
			{
				if(dist[a] != MAX_INT)
				{
					VanEmdeBoasTree -> remove(dist[a]);
					map_cost_edge[dist[a]].erase(find(map_cost_edge[dist[a]].begin(),map_cost_edge[dist[a]].end(),a));
					//adjacent is present in veb with greater cost
				}  	
				dist[a] = dist[node] + b;
				map_cost_edge[dist[a]].push_back(a);
				VanEmdeBoasTree	-> insert(dist[a]);			
			}
		}
    }
    Time = clock()-Start;
    for(int i=1;i<dist.size();i++){

    	cout<<"vertex:"<<i<<" "<<dist[i]<<endl;
    }
    
    cout<<"time taken:"<<Time<<endl;
	return 0;
}