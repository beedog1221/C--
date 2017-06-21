#include <iostream>
#include <deque> 
#include <list>  
#include <queue> 
#include <stack>
#include <cstring>
#include <fstream>


using namespace std;

const int NULL_EDGE = 0;

typedef char VertexType;

class GraphType
{
	public:
	GraphType();
	GraphType(int maxV);
	~GraphType();
	void MakeEmpty();
	bool IsEmpty() const;
	bool IsFull() const;
	void AddVertex(VertexType);
	void AddEdge(VertexType, VertexType, int);
	int GetWeight(VertexType, VertexType);
	void GetToVertices(VertexType);
	void ClearMarks();
	void MarkVertex(VertexType);
	bool IsMarked(VertexType);
	int IndexIs(VertexType);
	
	
	private:
	int numVertices;
	int maxVertices;
	VertexType* vertices;
	int edges[50][50];
	bool* marks;
};


int main(int argc, char *argv[]) {
	
		int option;
	    char city,target, search;
		string info;
	    int distance;
	   


	    while(1)
	    {
	        cout << " [1] Input data " << endl;
			cout << " [2] Search " << endl;
			cout << " [3] Exit" << endl;
	        cin >> option;


	            if(option==1)
	            {
	                cout<<"Add city: ";
	                cin>>city;
	                cout<<endl;
	                cout<<"Adjacent city: ";
	                cin>>target;
	                cout<<endl;
	                cout<<"Distance between the cities: ";
	                cin>>distance;
	                cout<<endl;
					info = city + target + distance;
					AddVertex(info);
					

	          
	            queue<string> city<<" "<<target<<" "<<dis<<" "<<endl;
	            

	            }
	          if(option==2)
	           {

	               cout << "enter location " << endl;
					cin >> search;
					cout << endl;
					cout << GetToVertices(search)
					

	           }

	           if(option==3)
	            {
	                return 0;
	            }



	        }
	    
}

GraphType::GraphType()
{
	numVertices = 0;
	maxVertices = 50;
	vertices = new VertexType[50];
	marks = new bool[50];
}

GraphType::GraphType(int maxV)
{
	numVertices = 0;
	maxVertices = maxV;
	vertices = new VertexType[maxV];
	marks = new bool[maxV];
}

GraphType::~GraphType()
{
	delete [] vertices;
	delete[] marks;
}

void GraphType::MakeEmpty()
{
	numVertices=0;
	maxVertices=0;
}

bool GraphType::IsEmpty() const
{
	return numVertices==0;
}

bool GraphType::IsFull() const
{
	return numVertices==maxVertices;
}

void GraphType::AddVertex(VertexType vertex)
{
	vertices[numVertices] = vertex;
	
	for (int index = 0; index < numVertices; index++)
	{
		edges[numVertices][index] = NULL_EDGE;
		edges[index][numVertices] = NULL_EDGE;
	}
	numVertices++;
}

int GraphType::IndexIs(VertexType vertex)
{
	int temp;
	    for(int i=0;i< numVertices;i++)
	       {
	           if(vertices[i]==vertex)
	           {

	               return i;
	               break;
	           }
	           temp=numVertices;

	       }
	       if(temp==numVertices)
	       return -1;
}


void GraphType::AddEdge(VertexType fromVertex, VertexType toVertex, int weight)
{
	int row;
	int col;
	
	row = IndexIs(fromVertex);
	col = IndexIs(toVertex);
	edges[row][col] = weight;
}

int GraphType::GetWeight(VertexType fromVertex, VertexType toVertex)
{
	int row;
	int col;
	
	row = IndexIs(fromVertex);
	col = IndexIs(toVertex);
	return edges[row][col];
}


void GraphType::GetToVertices(VertexType vertex)
{
	int fromIndex;
	int toIndex;
	
	fromIndex = IndexIs(vertex);
	for (toIndex = 0; toIndex < numVertices; toIndex++)
	{
		if (edges[fromIndex][toIndex] != NULL_EDGE)
		{
			//queue vertices[toIndex];
		}
	}
}

void GraphType::ClearMarks()
{
	for (int i =0; i < numVertices; i++) {
			marks[i] = false;
		}
}

void GraphType::MarkVertex(VertexType vertex)
{
	int ind=IndexIs(vertex);
	    marks[ind]=true;

}

bool GraphType::IsMarked(VertexType vertex)
{
	int ind=IndexIs(vertex);
	    return marks[ind];
}
