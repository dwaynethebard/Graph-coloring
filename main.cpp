
#include <vector>       // std::vector
#include <tuple>		//std::tuples
#include <stdio.h>		//file open
#include <string.h>
#include <fstream>
#include <iostream>
#include <stdlib.h> 
#include <string>
#include <algorithm>    // std::sort
using namespace std;
bool changeformat(); //change the format of one of the graphs 
bool isfeasible(int c,int v,vector<vector<int>> adj,int color[]);//cand you color code color c
void LCV(vector<tuple<int,int>> &queue,vector<vector<int>> adj,int color[],int m, int v); //Least contraining value:rules out the fewest ramaing variables.
void MRV(vector<tuple<int,int>> &queue,vector<vector<int>> adj,int color[],int m);//MRV: minimum remaining values: chose to color the vertex with the least avaible colors to choose from
void DegreeQueue(vector<tuple<int,int>> &queue,vector<tuple<int,int>> MRV,vector<vector<int>> adj);//Degree heuristic: choose variable with highest degree
bool graphcoloring(vector<vector<int>> adj, int m,int n, int color[],int h); //graph coloring with heuristics
bool nohuresticgraphColoring(vector<vector<int>> adj, int m,int n, int color[], int v); //graph coloring without heuristics
int main(int argc, char* argv[])
{
	//although the nodes are number 1..n the index for the color and other list are 0..n-1. So the acess the color of node 4, look in spot 3

	typedef vector<int> adjacency_inside_list;//the list of nodes adjacent to the current node
	//adjacency list for the nodes 
  	std::vector<adjacency_inside_list> adjacency_list;

	// First read the file to create the graph
	string line; //stores the line read in
	ifstream myfile (argv[1]);	//open the file sent 

	int N;//number of vertixes
	int M;//number of colors
	if (myfile.is_open()) //if it opened properly
	{
		/*Read first line to get the number of nodes and the number of colors*/
		getline (myfile,line);
		char str[line.size()]; //create an array to parse later
		std::size_t length = line.copy(str,line.size(),0); //move line into char so i can convert properly
		char * pch;
		str[length]='\0';
				
		
		pch = strtok (str," ( ) ");
		N=atoi(pch); //grab the number of vertices
		pch = strtok (NULL, " ()");
		M=atoi(pch); //grab the number of colors to be used
		
		adjacency_inside_list v; //create adjacency list
		while ( getline (myfile,line) ) //while not at the end of the file
		{		
			
			char str[line.size()]; //create an array to parse later
			std::size_t length = line.copy(str,line.size(),0); //move line into char so i can convert properly
			char * pch;
			str[length]='\0';//put somthing in the last spot
			int i;//node
			pch = strtok (str," ( ) ");//grab first node
			adjacency_inside_list v; //create the list for the nodes
			while (pch != NULL)
			{
				
				i=atoi(pch);//turn item into intege
				v.push_back(i);//put on the list
			    pch = strtok (NULL, " ()"); //grab next item
  			}
  			if(line.size()>=3)//make sure the line has at least ( n )
  			{
  				adjacency_list.push_back(v);
  			}
  			
			
  			
  			
		}
		myfile.close(); //close the file
		
	}
	
	
	int COLOR[N]; //list of the nodes and what color they are
	//initizile to 0 to be safe. color 0 means node is not colored
	for (int i = 0; i < N; ++i)
	{
		COLOR[i]=0;
	}
	
    
	if(graphcoloring(adjacency_list,M,N,COLOR,0))
	{
		cout<<"The graph is colorable:"<<endl;
		for (int i = 0; i < N; ++i)
    	{
    		cout<<COLOR[i]<<" ";
    	}
    	cout<<endl;
	}
	else
	{
		cout<<"The graph is not colorable"<<endl;
	}
	
	return 0;
}

bool changeformat() //gets the file into the right format mostly, i can add the minor changes afterwards
{
	ofstream my;
  	my.open ("example.txt"); //the new file im writing to

  


  	string line; //stores the line read in
	ifstream myfile ("test32.txt");	//open the file sent 
	
	int last=1;
	my << "(";
	while(getline (myfile,line))
	{
		char str[line.size()]; //create an array to parse later
		std::size_t length = line.copy(str,line.size(),0); //move line into char so i can convert properly
		char * pch;
		str[length]='\0';
		int i;//
		int cur;//current node
		pch = strtok (str," ");
		cur=atoi(pch);
		pch = strtok (NULL, " ");
		pch = strtok (pch, " ");
		i=atoi(pch);
		if (last==cur)
		{
			my << " "<<i;
		}
	  	if(last!=cur)//make sure the line has at least ( n )
	  	{

	  		my <<" "<< last;
	  		my << ")\n";
	  		
	  		last=cur;
	  		my << "(";
	  		my << " "<<i;
	  	}
	  }
}
  	


//check if color c is allowable at node v
bool isfeasible(int c,int v,vector<vector<int>> adj,int color[])
{
	int list_size=adj[v].size(); //how many neighbours to look at
	int temp; //change of index
	for (int i = 0; i <list_size; ++i) //for every neighbour check
	{
		temp=adj[v][i]-1;//change node number to fix the index
		if (color[temp]==c)	//if you have a neighbour with the color c, can't color node c then.
		{
			return false; 
		}
	}
	return true; //no neighbour has color c

}
//create queue for Least contraining value, m colors, 
void LCV(vector<tuple<int,int>> &queue,vector<vector<int>> adj,int color[],int m,int v)
{
	int list_size=adj[v].size();//how many neighbours to look at
	int LCV_value=0;//how many options are lost
	int J;//current node 
	for (int i = 1; i <= m; ++i)//create loop though all colors
	{
		if (isfeasible(i,v,adj,color))//if it is feasible to color i
		{
			for (int j = 0; j < list_size; ++j)
			{
				J=adj[v][j]-1;             //j is the current adjacent node we are looking at
				if (isfeasible(i,J,adj,color)) //if you can color c, then now neighbours cant use this color
				{
					LCV_value=LCV_value+1; //a neighbour will lose the option to color with this color
				}

			}
			tuple<int,int> temppy (LCV_value,i);//create tuple to add to the queue
          	queue.push_back(temppy);
          	LCV_value=0; 

		}
	}
	std::sort (queue.begin(), queue.begin()+queue.size()); 
}

void MRV(vector<tuple<int,int>> &queue,vector<vector<int>> adj,int color[],int m)
{
	int list_size=adj.size();
	
	int temp=0;//how many aviable colors
	
	for (int i = 0; i < list_size; ++i)//i is what node we are at
	{
		if (color[i]==0)//if the node is not colored yet
		{
			for (int j = 1; j <=m; ++j) //j is what color we are at
			{
					if (isfeasible(j,i,adj,color))
					{
						temp=temp+1; //count how mamy colors are feasible for each node
					}
			}
			tuple<int,int> temppy (temp,i);//create tuple to add to the queue
	        queue.push_back(temppy);
			temp=0;
		}
	}
	std::sort (queue.begin(), queue.begin()+queue.size()); 
}


void DegreeQueue(vector<tuple<int,int>> &queue,vector<tuple<int,int>> MRV,vector<vector<int>> adj) //sort all the nodes with the smallest amount of color to chose from by there degree
{
	int colormin;//the number of colors a node can color with
	colormin=get<0>(MRV[0]);
	int J=0;//Node
	int DEG=0;//degree
	
	for (int i = 0; i < MRV.size(); ++i)
	{
		if (colormin==get<0>(MRV[i]))//if node has the same amount as the min for the MRV queue
		{
			
			J=get<1>(MRV[i]);
			DEG=adj[J].size();
			tuple<int,int> temppy (DEG,J);//create tuple to add to the queue
	        queue.push_back(temppy);
			
		}
		J=0;
		DEG=0;
	}
	std::sort (queue.begin(), queue.begin()+queue.size()); //sort by degree
	std::reverse(queue.begin(),queue.end());//since sorting goes from min to max, we want the opposite
}



bool graphcoloring(vector<vector<int>> adj, int m,int n, int color[],int h)
{
    //base case: If all vertices are assigned a color then
        
    if (n == h)
    {
        return true;
 	}
  	
	
	std::vector<tuple<int,int>> Nodecolorqueue;
  	MRV(Nodecolorqueue,adj,color,m); //create list of nodes baised on avaiable colors

  	std::vector<tuple<int,int>> DEgqueue;
	DegreeQueue(DEgqueue,Nodecolorqueue,adj);//sort smallest mrv nodes by their degree
	
	int node=0;
	node=get<1>(DEgqueue[0]);//get the node we are to color

	std::vector<tuple<int,int>> Colorqueue;
	LCV(Colorqueue,adj,color,m,node); //create a queue of colors for the node

	int C;//the color
    while(Colorqueue.size()!=0)
    {
    	C=get<1>(Colorqueue[0]);                    //get the color
    	Colorqueue.erase(Colorqueue.begin());       //remove the min node
		color[node]=C;								//color the node
		if(graphcoloring(adj, m,n, color,h+1))		//repeat
			return true;
		color[node]=0;								//set back to uncolored
		
    } 
 
     //If no color can be assigned to this vertex then return false 
    return false;
    
}


bool nohuresticgraphColoring(vector<vector<int>> adj, int m,int n, int color[], int v)
{
    /* base case: If all vertices are assigned a color then
       return true */
    if (v == n)
        return true;

    /* Consider this vertex v and try different colors */
    for (int c = 1; c <= m; c++)
    {
        /* Check if assignment of color c to v is fine*/
        if (isfeasible(c,v,adj,color))
        {
           color[v] = c;
 
           /* recur to assign colors to rest of the vertices */
           if (nohuresticgraphColoring (adj, m,n, color, v+1) == true)
             return true;
 
            /* If assigning color c doesn't lead to a solution
               then remove it */
           color[v] = 0;
        }
    }
 
    /* If no color can be assigned to this vertex then return false */
    return false;
}