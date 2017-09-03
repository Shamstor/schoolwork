//
//  main.cpp
//  Lab_5
//
//  Created by Ammar Rizvi on 2/24/16.
//  Copyright (c) 2016 Ammar Rizvi. All rights reserved.
#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <vector>
#include <stdlib.h>
#include <iterator>
#include <list>
#include <cmath>
#include <math.h>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <queue>
#include <stdio.h>

using namespace std;

double weightTotal=0;

class Vertex;
class Edge;
vector<Edge> edgesVec;

vector<Vertex> vertices;
class Edge
{
public:
    bool alive = true;
    double weight = 0;
    int origin;
    int target;
    string Name;
    
    Edge(string name, int A, int B, double w)
    {
        this->Name = name;
        this->origin = A;
        this->target = B;
        this->weight = w;
    }

};

class Vertex
{
public:
    priority_queue<Edge> edgesPriority; // priority queue of its edges
    string name = " ";
    vector<Edge> edges; // vector of its edges
    vector<Vertex> belongs; //
    
    vector<int> edgesIndex; //tells me what the adjacent nodes are indexed at
    
    Vertex(string n)
    {
        this->name = n;
    }
    Vertex()
    {
        this->name = "";
    }
    
  
    void printVec()
    {
        for(int i =0; i< edges.size();i++)
        {
            cout << edges[i].Name<< " ";
            cout << endl;

        }
        cout << endl;
    }
};

bool operator <(const Edge& lhs, const Edge& rhs)
{
    return lhs.weight > rhs.weight;
}
bool operator == (const Edge& lhs, const Edge& rhs)
{
    return lhs.Name == rhs.Name;
}


class UF    {
    int *id, cnt, *sz, totalSize;
public:
    // Create an empty union find data structure with N isolated sets.
    UF(int N)   {
        cnt = N;
        totalSize = N;
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
        }
        
        else	{
            
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
    
    void print() {
        
        for(int i= 0; i< totalSize;i++)
        {
            cout <<this->find(i)<< " " ;
            cout <<this->sz[i]<< " " << endl;

        }
    

        
        std::cout << "\n";
        
  
    }
    
    int getsize(int i)
    {
        return sz[i];
    }
    
  
};

void printPQ(priority_queue<Edge> A){
    while (!A.empty()) {
        
      cout << A.top().Name<<" " <<A.top().weight<< endl;
        A.pop();}
}

bool contains(vector<Edge> A, Edge B)
{
    bool contained=false;
    for (int i=0 ; i< A.size(); i++) {
        if(A[i].Name ==B.Name)
        {
            contained= true;
        }
    }
    return contained;
}

void survived(UF &obj, Edge &min, int temp, int &survivor, int &captured)
{
    if(obj.getsize(min.origin) >  obj.getsize(min.target))
    {
        survivor = min.origin;
        captured = min.target;
    }
    else if (obj.getsize(min.origin) <  obj.getsize(min.target)){
        survivor = min.target;
        captured = min.origin;
    }
    else if(obj.getsize(min.origin) ==  obj.getsize(min.target))
    {
        survivor = min.origin;
        captured = min.target;
    }
}

void mergePriority(int surviver, int captured, vector<Edge> MST)
{
    vector<Edge> vec;

    vertices[surviver].edgesPriority.pop();

    while(!vertices[captured].edgesPriority.empty())
    {
        if(contains(MST, vertices[captured].edgesPriority.top())){
            
            cout << "Popping Capped: "<<vertices[captured].edgesPriority.top().Name <<" "<< vertices[captured].edgesPriority.top().weight<< endl<< endl;
            vertices[captured].edgesPriority.pop();
        }
        else{
            vertices[surviver].edgesPriority.push(vertices[captured].edgesPriority.top());
            

            vertices[captured].edgesPriority.pop();
        }
        
    }


   
}

int main(int argc, const char * argv[]) {
   
    queue<int> FIFO;
    
    string nameOfVertex="";
    string firstLine = "";
    string tempString;
    int vertexSize = 0;
    int edgeSize = 0;
    string edgeName;
    double weight;
    Vertex A;
    Vertex B;
    int OrginIndex;
    int TargetIndex;
    ifstream fin;
    ofstream myfile;
    if (argc ==2){
        fin.open(argv[1]);
        if (fin) {
            TargetIndex =0;
            OrginIndex =0;
        getline(fin, firstLine, '\n');
        stringstream ss (firstLine);
        getline(ss, tempString, ' ');
        vertexSize = atoi(tempString.c_str());
        getline(ss, tempString, '\n');
        edgeSize = atoi(tempString.c_str());
        
        for (int i = 0 ; i < vertexSize; i++) {
            Vertex tempVertex;
            getline(fin, tempString, '\n');
            tempVertex.name = tempString;
            
            vertices.push_back(tempVertex);
           
        }
            for (int i = 0 ; i < edgeSize; i++) {
                getline(fin, tempString, ' ');
                edgeName = tempString;
//                cout << edgeName << " ";
                getline(fin, tempString, ' ');
//                cout << tempString;
//                cout <<atoi(tempString.c_str()) << " ";
                weight = stod(tempString.c_str());
//                cout << weight << " "<< endl;
                getline(fin, tempString, ' ');
//                cout << tempString << " ";
                for (int i = 0; i < edgeSize; i++)
            {
                if(vertices[i].name == tempString)
                {
                    OrginIndex = i;
//                    cout << "Origin" << endl;
                }
            }
            getline(fin, tempString, '\n');
//                cout << tempString<< endl;


            for (int i = 0; i < edgeSize; i++)
            {
                if(vertices[i].name == tempString)
                {
                    
                    TargetIndex = i;
//                    cout << "Target"<< endl;
                }
            }
                
                
                
                Edge temp(edgeName,  OrginIndex, TargetIndex, weight);
//                cout << edgeName << "     " << vertices[OrginIndex].name << " <-> "<< vertices[TargetIndex].name << " "<< weight<<  endl;
                edgesVec.push_back(temp);
                
                vertices[OrginIndex].edges.push_back(temp);
                vertices[TargetIndex].edges.push_back(temp);
                
                vertices[OrginIndex].edgesPriority.push(temp);
                vertices[TargetIndex].edgesPriority.push(temp);
                
                vertices[OrginIndex].edgesIndex.push_back(TargetIndex);
//               cout << OrginIndex<< " ";
//                cout << TargetIndex<< endl;
                vertices[TargetIndex].edgesIndex.push_back(OrginIndex);
//                cout << endl;
            }}}
   
    UF obj(vertices.size());

    
//        cout << vertices[0].edgesPriority.top();


    
    for (int i = 0; i < vertexSize; i++)
    {
//        cout << vertices[i].name << endl;
 
        FIFO.push(i);
//        cout << vertices[i].name << "   " << endl;
//        printPQ(vertices[i].edgesPriority);

        for(int j = 0 ; j< vertices[i].edges.size(); j++ )
        {
//            cout <<"["<<vertices[i].edges[j].Name << "] ";
//            cout <<" WEIGHT :"<<vertices[i].edges[j].weight;
//            cout<< endl;
            
        }
//        if(vertices[i].edgesPriority.size() > 0){
//            while (!vertices[i].edgesPriority.empty())
//            {
//            cout << vertices[i].name;
//            
////            cout <<"  LEAST EDGE: "<<vertices[i].edgesPriority.top().Name << " WEIGHT "  << vertices[i].edgesPriority.top().weight <<endl;
////                vertices[i].edgesPriority.pop();
//            }
//        }
//        cout << endl;
//        cout<< endl;

    
        
    vector<Edge> NUTs;
    
//    
//    cout << vertices[edgesVec[0].origin].name;
//    
//        for(int i =0; i < vertexSize; i++)
//        {
//    
//            if(obj.find(edgesVec[i].origin) != obj.find(edgesVec[i].target))
//               {
//                   obj.merge(edgesVec[i].origin, edgesVec[i].target);
//                   NUTs.push_back(edgesVec[i]);
//               }
//    
//               
//        }
//   
//    
//    cout << NUTs.size();
//    for(int i =0; i < NUTs.size(); i++)
//    {
//        cout << NUTs[i].Name << endl;
//    }
//    
    
    }

    
    
    vector<Edge> MST;
    
    while (MST.size() < vertexSize-1)
    {
        
        cout << "               NEXT STEP               "<< endl<< endl;
        
        int temp = FIFO.front();
        if(FIFO.size() >0){
            FIFO.pop();}
        int surviver=0;
        int captured=0;
//        cout << vertices[temp].name<< endl;
        if(!contains(MST, vertices[temp].edgesPriority.top())){

        if(temp == obj.find(temp))
        {
//            obj.print();

            Edge min = vertices[temp].edgesPriority.top();
            MST.push_back(min);
            
          

//            cout << "TOP: "<<min.Name<< endl;

//            vertices[temp].edgesPriority.pop();

            if (obj.find(min.origin) == obj.find(min.target)) {
                FIFO.push(temp);
            }
            else{
                string thing;
                if (vertices[temp].name == vertices[min.origin].name) {
                         thing = "Origin ";
                }
                else if(vertices[temp].name == vertices[min.target].name){
                     thing = "Target ";

                }
                
//                cout << endl;
//                obj.print();
                

                obj.merge(min.origin, min.target);
                
//                cout << obj.find(min.origin) <<  " "<< obj.find(min.target) << endl;
//
//                cout << "SIZE " <<obj.getsize(min.origin) << " " << obj.getsize(min.target) <<endl;
                
//                obj.merge(temp, min.origin);
                
                
                survived(obj, min, temp, surviver, captured);
                cout << "FIFO front: "<<vertices[temp].name<<endl;
                cout <<   "PQ Top: " <<vertices[temp].edgesPriority.top().Name <<" "<<thing<< endl <<"Connecting " << vertices[vertices[temp].edgesPriority.top().origin].name  << " " << vertices[temp].edgesPriority.top().origin << " and " << vertices[vertices[temp].edgesPriority.top().target].name<< " "<< min.target <<endl<<"By: "<<min.Name <<endl<< endl;
                cout << "SURVIVED: " << surviver << " CAPPED: " << captured << endl<< endl;
                cout << endl<< endl;

//                cout << " obj.find(surviver) " <<obj.find(surviver)<< endl;
                cout << vertices[obj.find(surviver)].name <<" SIZE 1: "<< vertices[obj.find(surviver)].edgesPriority.size()<< endl<< endl;
                
                printPQ(vertices[obj.find(surviver)].edgesPriority);


                cout << endl<< endl;
               
                cout << vertices[captured].name <<" SIZE 2: "<< vertices[captured].edgesPriority.size()<< endl<< endl;

                printPQ(vertices[captured].edgesPriority);                cout << endl;


//                vertices[temp].edgesPriority.pop();

                mergePriority(obj.find(surviver), captured, MST);
                weightTotal += min.weight;
                
                cout << vertices[obj.find(surviver)].name<<" SIZE 3: "<< vertices[obj.find(surviver)].edgesPriority.size()<< endl<< endl;

                printPQ(vertices[obj.find(surviver)].edgesPriority);
                cout << endl;

//                cout << endl<< "MST SIZE: "<<MST.size()<<  endl;

                
             
                cout << endl;
//                obj.print();
                
            }
//            cout << endl<< "MST SIZE: "<<MST.size()<<  endl;

            

        }
   
        if (obj.find(temp)== temp) {
            FIFO.push(temp);
        }
        }
        

    
}
    obj.print();

    cout << endl<< weightTotal<< endl;

    for(int i= 0; i< MST.size(); i++)
    {
                            cout << MST[i].Name<< " " << endl;
    }
}




