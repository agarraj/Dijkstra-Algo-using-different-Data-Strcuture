
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;
//*****************************************//


//***************************************//

int MAX_INT=999999;
template <class V> class FibonacciHeap;

template <class V> struct node {
private:
node<V>* prev;
node<V>* next;
node<V>* child;
node<V>* parent;
V value;
int degree;
bool marked;
public:
friend class FibonacciHeap<V>;
node<V>* getPrev() {return prev;}
node<V>* getNext() {return next;}
node<V>* getChild() {return child;}
node<V>* getParent() {return parent;}
V getValue() {return value;}
bool isMarked() {return marked;}

bool hasChildren() {return child;}
bool hasParent() {return parent;}
};

template <class V> class FibonacciHeap {
protected:
node<V>* heap;
public:

FibonacciHeap() {
    heap=_empty();
}
virtual ~FibonacciHeap() {
    if(heap) {
        _deleteAll(heap);
    }
}
node<V>* insert(V value) {
    node<V>* ret=_singleton(value);
    heap=_merge(heap,ret);
    return ret;
}
void merge(FibonacciHeap& other) {
    heap=_merge(heap,other.heap);
    other.heap=_empty();
}

bool isEmpty() {
    return heap==NULL;
}

V getMinimum() {
    return heap->value;
}

V removeMinimum() {
    node<V>* old=heap;
    heap=_removeMinimum(heap);
    V ret=old->value;
    delete old;
    return ret;
}

void decreaseKey(node<V>* n,V value) {
    heap=_decreaseKey(heap,n,value);
}

node<V>* find(V value) {
    return _find(heap,value);
}
private:
node<V>* _empty() {
    return NULL;
}

node<V>* _singleton(V value) {
    node<V>* n=new node<V>;
    n->value=value;
    n->prev=n->next=n;
    n->degree=0;
    n->marked=false;
    n->child=NULL;
    n->parent=NULL;
    return n;
}

node<V>* _merge(node<V>* a,node<V>* b) {
    if(a==NULL)return b;
    if(b==NULL)return a;
    if(a->value>b->value) {
        node<V>* temp=a;
        a=b;
        b=temp;
    }
    node<V>* an=a->next;
    node<V>* bp=b->prev;
    a->next=b;
    b->prev=a;
    an->prev=bp;
    bp->next=an;
    return a;
}

void _deleteAll(node<V>* n) {
    if(n!=NULL) {
        node<V>* c=n;
        do {
            node<V>* d=c;
            c=c->next;
            _deleteAll(d->child);
            delete d;
        } while(c!=n);
    }
}

void _addChild(node<V>* parent,node<V>* child) {
    child->prev=child->next=child;
    child->parent=parent;
    parent->degree++;
    parent->child=_merge(parent->child,child);
}

void _unMarkAndUnParentAll(node<V>* n) {
    if(n==NULL)return;
    node<V>* c=n;
    do {
        c->marked=false;
        c->parent=NULL;
        c=c->next;
    }while(c!=n);
}

node<V>* _removeMinimum(node<V>* n) {
    _unMarkAndUnParentAll(n->child);
    if(n->next==n) {
        n=n->child;
    } else {
        n->next->prev=n->prev;
        n->prev->next=n->next;
        n=_merge(n->next,n->child);
    }
    if(n==NULL)return n;
    node<V>* trees[64]={NULL};
    
    while(true) {
        if(trees[n->degree]!=NULL) {
            node<V>* t=trees[n->degree];
            if(t==n)break;
            trees[n->degree]=NULL;
            if(n->value<t->value) {
                t->prev->next=t->next;
                t->next->prev=t->prev;
                _addChild(n,t);
            } else {
                t->prev->next=t->next;
                t->next->prev=t->prev;
                if(n->next==n) {
                    t->next=t->prev=t;
                    _addChild(t,n);
                    n=t;
                } else {
                    n->prev->next=t;
                    n->next->prev=t;
                    t->next=n->next;
                    t->prev=n->prev;
                    _addChild(t,n);
                    n=t;
                }
            }
            continue;
        } else {
            trees[n->degree]=n;
        }
        n=n->next;
    }
    node<V>* min=n;
    do {
        if(n->value<min->value)min=n;
        n=n->next;
    } while(n!=n);
    return min;
}

node<V>* _cut(node<V>* heap,node<V>* n) {
    if(n->next==n) {
        n->parent->child=NULL;
    } else {
        n->next->prev=n->prev;
        n->prev->next=n->next;
        n->parent->child=n->next;
    }
    n->next=n->prev=n;
    n->marked=false;
    return _merge(heap,n);
}

node<V>* _decreaseKey(node<V>* heap,node<V>* n,V value) {
    if(n->value<value)return heap;
    n->value=value;
    if(n->value<n->parent->value) {
        heap=_cut(heap,n);
        node<V>* parent=n->parent;
        n->parent=NULL;
        while(parent!=NULL && parent->marked) {
            heap=_cut(heap,parent);
            n=parent;
            parent=n->parent;
            n->parent=NULL;
        }
        if(parent!=NULL && parent->parent!=NULL)parent->marked=true;
    }
    return heap;
}

node<V>* _find(node<V>* heap,V value) {
    node<V>* n=heap;
    if(n==NULL)return NULL;
    do {
        if(n->value==value)return n;
        node<V>* ret=_find(n->child,value);
        if(ret)return ret;
        n=n->next;
    }while(n!=heap);
    return NULL;
}
};












//Add dotty output to our heap, which produces nice diagrams
class DotFibonacciHeap : public FibonacciHeap<int> {
public:
void dump() {
    printf("digraph G {\n");
    if(heap==NULL) {
        printf("empty;\n}\n");
        return;
    }
    printf("minimum -> \"%p\" [constraint=false];\n",heap);
    node<int>* c=heap;
    do {
        _dumpChildren(c);
        c=c->getNext();
    } while(c!=heap);
    printf("}\n");
}

private:

void _dumpChildren(node<int>* n) {
    printf("\"%p\" -> \"%p\" [constraint=false,arrowhead=lnormal];\n",n,n->getNext());
    printf("\"%p\" -> \"%p\" [constraint=false,arrowhead=ornormal];\n",n,n->getPrev());
    if(n->isMarked())printf("\"%p\" [style=filled,fillcolor=grey];\n",n);
    if(n->hasParent()) {
        printf("\"%p\" -> \"%p\" [constraint=false,arrowhead=onormal];\n",n,n->getParent());
    }
    printf("\"%p\" [label=%d];\n",n,n->getValue());
    if(n->hasChildren()) {
        node<int>* c=n->getChild();
        do {
            printf("\"%p\" -> \"%p\";\n",n,c);
            _dumpChildren(c);
            c=c->getNext();
        } while(c!=n->getChild());
    }
}
};


int main() {

DotFibonacciHeap h;

node<int>* dm;

int v,c,i,a,b,x,e,s;
clock_t Start, Time;
    cout <<"Enter the number of vertices of the graph\n";
    cin >> v;
    cout << "Enter the number of edges\n";
    cin >> e;
    // cout <<"Enter the maximum cost of an edge\n";
    // cin >> max_cost;
    cout << "Enter the edge u --> v and cost of edge\n";
    vector <pair <int,int > > g[v+1];
    vector <int > map_cost_edge[1024]; 
    for(i = 0; i < e; i++)
    {
        cin >> a >> b >> c;
        g[a].push_back(make_pair(b,c));
        g[b].push_back(make_pair(a,c));
    }
    cout <<"Enter the source vertices\n";
    cin >> s;

    //vEB *vEB = new class vEB(VEB_SIZE);
    vector <int> dist (v+1,MAX_INT);
    dist[s] = 0;

    //vEB -> insert(0);
    h.insert(0);
    Start = clock();

    map_cost_edge[0].push_back(s);

    while(!h.isEmpty())
    {
        x = h.getMinimum(); //minimum cost from the veb tree
        if(map_cost_edge[x].size()==1)
        {
            h.removeMinimum();
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
                // if(dist[a] != MAX_INT)
                // {
                //    // vEB -> Delete(dist[a]);
                    
                //     dm=h.find(dist[a]);
                //     //h.decreaseKey(dm,dist[node]+b);
                //     h.delete(dist[a]);
                //     map_cost_edge[dist[a]].erase(find(map_cost_edge[dist[a]].begin(),map_cost_edge[dist[a]].end(),a));
                //     //adjacent is present in veb with greater cost
                // }   
                dist[a] = dist[node] + b;
                map_cost_edge[dist[a]].push_back(a);
               // vEB -> insert(dist[a]); 
               if(!h.find(dist[a]))  
               h.insert(dist[a]);      
            }
        }
    }
    Time = clock()-Start;
    for(int i=1;i<dist.size();i++){   
        if(dist[i]==999999)
        cout<<"vertex:"<<i<<" -1 ";
        else
        cout<<"vertex:"<<i<<" "<<dist[i]<<endl;
    }

    cout<<"time taken:"<<Time<<endl;

}
