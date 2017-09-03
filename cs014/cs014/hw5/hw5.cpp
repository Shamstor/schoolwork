#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<vector>
#include<list>
#include<cmath>
#include<string>
#include <queue>
#include <list>

using namespace std;       

class Vertex{
public:
    string name;
    vector<int> adj_edges;
    Vertex(string m):name(m){}
};

struct Edge{
public:
    string name;
    float weight;
    int end1;
    int end2;
    Edge(string n, float w, int e1, int e2):name(n), weight(w), end1(e1), end2(e2){}
};

class UF    {
    int *id, cnt, *sz;
public:
	// Create an empty union find data structure with N isolated sets.
    UF(int N)   {
        cnt = N;
	id = new int[N];
	sz = new int[N];
        for(int i=0; i<N; i++)	{
            id[i] = i;
	    sz[i] = 1;
	}
    }
    ~UF()	{
	delete [] id;
	delete [] sz;
    }
	// Return the id of component corresponding to object p.
    int find(int p)	{
        int root = p;
        while (root != id[root])
            root = id[root];
        while (p != root) {
            int newp = id[p];
            id[p] = root;
            p = newp;
        }
        return root;
    }
	// Replace sets containing x and y with their union.
    void merge(int x, int y)	{
        int i = find(x);
        int j = find(y);
        if (i == j) return;
		
		// make smaller root point to larger one
        if   (sz[i] < sz[j])	{ 
		id[i] = j; 
		sz[j] += sz[i]; 
	} else	{ 
		id[j] = i; 
		sz[i] += sz[j]; 
	}
        cnt--;
    }
	// Are objects x and y in the same set?
    bool connected(int x, int y)    {
        return find(x) == find(y);
    }
	// Return the number of disjoint sets.
    int count() {
        return cnt;
    }
};

void print(vector<Vertex> v, vector<Edge> e){
    for(int i= 0; i < v.size(); i++){
        cout<<v.at(i).name<<endl;
        for(int j = 0; j < v.at(i).adj_edges.size(); j++){
            cout<<"    "<<e.at(v.at(i).adj_edges.at(j)).name<<endl;
        }
    }    
}

//this needs overloading to fix prioty_queue
bool operator <(const Edge& lhs, const Edge& rhs)
{
    return lhs.weight > rhs.weight;
}



int main(int argc, char *argv[]){
    vector<Vertex> nodes;
    vector<Edge> edges;
    string temp = ""; string filename = "";
    int numNodes = 0; int numEdges = 0;
    
    fstream fin;
    
    fin.open(argv[1]);
    filename = argv[1];
    
    if(filename.find(".") != string::npos)
            filename = filename.substr(0,filename.find("."));
    filename.append(".out");
    ofstream fout(filename.c_str());
    getline(fin, temp, ' ');
    numNodes = atoi(temp.c_str());
    getline(fin, temp, '\n');
    numEdges = atoi(temp.c_str());
    cout<<numNodes<<" "<<numEdges<<endl;
    for(int i = 0; i < numNodes; i++){
        string temp;
        getline(fin, temp, '\n');
        //cout<<temp<<endl;
        Vertex v = Vertex(temp);   
        nodes.push_back(v);
    }
    //cout<<nodes.size()<<endl;
    for(int i = 0; i < numEdges; i++){
        string name = ""; string end1 = ""; string end2 = ""; string weight = "";
        int index1, index2;
        getline(fin, name, ' ');
        getline(fin, weight, ' ');
        int w = atoi(weight.c_str());
        getline(fin, end1, ' ');
        getline(fin, end2, '\n');
        //this for loop finds the index in nodes for end1 and end2 for each edge
        for(int i = 0; i < nodes.size(); i++){
            if(end1 == nodes.at(i).name)
                index1 = i;
            if(end2 == nodes.at(i).name)
                index2 = i;
        }
            Edge e = Edge(name, w, index1, index2);
            edges.push_back(e);
            
            //cout<<nodes.at(e.end1).name<<"   "<<nodes.at(e.end2).name<<endl;
            
            for(int i = 0; i < nodes.size(); i++){
                if(nodes.at(i).name == nodes.at(e.end1).name)
                    nodes.at(i).adj_edges.push_back(edges.size()-1);
                if(nodes.at(i).name == nodes.at(e.end2).name)
                    nodes.at(i).adj_edges.push_back(edges.size()-1);
            }
            //cout<<nodes.at(e.end1).name<<" "<<nodes.at(e.end2).name<<endl;
    }

    


///////////////////////////START OF YAMA IMPLEMENTATION///////////////////////////
    UF obj(nodes.size());
    vector<Edge> Yama;
    queue<int> fifo;
    vector<priority_queue<Edge> > pqEdges;
    //filling fifo queue with integers to correspond to nodes
    
    for(int i = 0; i < nodes.size(); i++){
        fifo.push(i);
    }
    
    cout<<nodes.at(fifo.front()).name;
    
        
    
    //filling vector of priority queues 
    for(int i = 0; i < nodes.size(); i++){
        
        priority_queue<Edge> pq;
        pqEdges.push_back(pq);
        for(int j = 0; j< nodes.at(i).adj_edges.size(); j++){
            
            pqEdges.at(i).push(edges.at(nodes.at(i).adj_edges.at(j)));
        }
        
    }
    
    //creating the YAMA MST (minimmun spanning tree)
    while(Yama.size() < (nodes.size()-1)){
        int frag1 = fifo.front();
        int frag2;
        if(obj.find(frag1) != frag1){
            fifo.pop();
        }else{
            //this while makes sure the top in pq is MOE and is alive edge
            while(obj.find(pqEdges.at(frag1).top().end2) != pqEdges.at(frag1).top().end2){
                pqEdges.at(frag1).pop();
            }
           
            Yama.push_back(pqEdges.at(frag1).top());
            frag2 = pqEdges.at(frag1).top().end2;
            obj.merge(frag1,frag2);
            
            if(obj.find(frag1) == frag1){
               
                for(int i = 0; i<pqEdges.at(frag2).size(); i++){
                    pqEdges.at(frag1).push(pqEdges.at(frag2).top());
                    pqEdges.at(frag2).pop();
                }
                fifo.push(frag1);
                fifo.pop();
            }else{
                while(!(pqEdges.at(frag1).empty())){
                    pqEdges.at(frag2).push(pqEdges.at(frag1).top());
                    pqEdges.at(frag1).pop();
                }
               
                fifo.pop();
            }
            
        }
    }
    
    float weight = 0;    
    for(int i = 0; i < Yama.size(); i++){
        weight += Yama.at(i).weight;
    }

    fout<<"Weight of MST: ";
    fout<<weight<<endl;
    fout<<"Edges of MST: "<<endl;
    for(int i = 0; i < Yama.size(); i++){
        fout<<"    "<<Yama.at(i).name<<endl;
    }
    
    
   
////////////////////////////END OF YAMA///////////////////////////

/*
///////////this is NUTS////////////////////////////////////////////
    UF obj(nodes.size());
    vector<Edge> nuts;
    for(int i = 0; nuts.size()< (nodes.size() -1); i++){
        if(obj.find(edges.at(i).end1) != obj.find(edges.at(i).end2)){
            obj.merge(edges.at(i).end1, edges.at(i).end2);
            nuts.push_back(edges.at(i));
        }
    }
    
    //prints out nuts reqs
    cout<<"NUTS: "<<endl;
    float weight = 0;    
    for(int i = 0; i < nuts.size(); i++){
        weight += nuts.at(i).weight;
    }
    cout<<"Number of Edges: "<<nuts.size()<<" "<<"Weight: "<<weight<<endl;
    for(int i = 0; i < nuts.size(); i++){
        cout<<nuts.at(i).name<<endl;
    }
///////////////////END OF NUTS//////////////////////////////////////    
*/

////////////////KRUSKALS////////////////////
/*
    UF obj(nodes.size());
    vector<Edge> krusk;
    priority_queue<Edge> kruskPQ;
    for(int i = 0; i < edges.size();i++){
        kruskPQ.push(edges.at(i));
    }
    
    for(int i = 0; krusk.size()< (nodes.size() -1); i++){
        
        if(obj.find(kruskPQ.top().end1) != obj.find(kruskPQ.top().end2)){
            obj.merge(kruskPQ.top().end1, kruskPQ.top().end2);
            krusk.push_back(edges.at(i));
        }
        kruskPQ.pop();
    }
    
    //prints out krusk reqs
    cout<<"krusk: "<<endl;
    float weight = 0;    
    for(int i = 0; i < krusk.size(); i++){
        weight += krusk.at(i).weight;
    }
    cout<<"Number of Edges: "<<krusk.size()<<" "<<"Weight: "<<weight<<endl;
    for(int i = 0; i < krusk.size(); i++){
        cout<<krusk.at(i).name<<endl;
    }
    
    


*/



//////////////////////END OF KRUSKAL////////////////
    return 0;
}