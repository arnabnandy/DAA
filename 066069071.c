/*****************************************************************************************************
*
*	Author	: 	Abhishek Vajareker
*					Arnab Nandy
*					Anshuli Patil
*	Date 		:	22/2/2014
*	Purpose	:	Determines the shortest path length from source to target via at least one node flag
*
******************************************************************************************************/

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define MAX_N 1000

int source, N, F, M;

int rem_min( long long int Q[])
{
	long long int min = LLONG_MAX;
	int min_index =-1;
	int i;
	for(i=0;i<N;i++)
	{
		if(Q[i]<min && Q[i]!=-1) 
		{
		min = Q[i];
		min_index = i;
		}
	}
	if(min_index==-1)
		return -1;
	else
		Q[min_index] = -1; 													//-1 represents that it's removed from the queue
	return min_index;
}

//returns the shortest path lengths between single source and all other points.
void shortest_path_dijkstra(int x, int y,  long long int graph[x][y], int source, long long int distance[x])
{
    int i,j;
    for(i=0;i<N;i++)
		distance[i] = LLONG_MAX;
    distance[source] = 0;
	
    long long int Q[N] ;
    for(i=0;i<N;i++) 												//initialize the priority queue
    {	
    	Q[i] = distance[i];
    }
    for(i=0;i<N;i++)													//this loop continues till the queue is empty; all the nodes are removed.
    {
    	int u = rem_min(Q);
		for(j=0;j<N;j++)												//for each vertex adjacent to u in the graph...
		{
			if(u==-1)
				break;	
			long long int edge_weight = graph[u][j];
			if(edge_weight != LLONG_MAX)							//edge exists if condition is true
			{
				if(distance[u]+edge_weight<distance[j])		//relax edge if true
				{
					distance[j] = (distance[u] + edge_weight);//update the better alternative for vertex 'j'
           		Q[j] = distance[j] ;
				}
			}
		}
    }
}

int main(void)
{
	scanf("%d %d %d %d", &N, &M, &F, &source);				//accepting the inputs

	int i,j;
	long long int adj_matrix [N][N];
	
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			adj_matrix[i][j] = LLONG_MAX;									//initializing unreachable nodes to infinity
	}

	int src_node, dest_node;
	long long int edge_weight;
	for(i=0;i<M ; i++)
	{
		scanf("%d %d ", &src_node, &dest_node); 

		scanf("%lld",&edge_weight);										//edge cost 

		if(edge_weight<adj_matrix[src_node][dest_node])
			adj_matrix[src_node][dest_node] = edge_weight;
	}

	int flag[MAX_N]={};														//used as a boolean to identify if the node is a flag
																					// 1 for yes
	int flags[F];
	
	for(i=0;i<F;i++)
	{
		scanf("%d",&flags[i]);
		flag[flags[i]]=1;
	}
	
	int P;
	scanf("%d",&P);
	
	long long int distance [N]; 											//Distance from source to all nodes

	shortest_path_dijkstra(N,N,adj_matrix,source,distance);
																					//To calculate the shortest distance from target to each flag
																					//we need to take transpose of the adj matrix as the graph is a directed graph 
	long long int temp;
	for(i=0;i<N;i++)
	{
		for(j=0;j<i;j++)
		{

			temp=adj_matrix[i][j];
			adj_matrix[i][j]=adj_matrix[j][i];
			adj_matrix[j][i]=temp;
		}
		
	}
			
	for(i=0;i<P;i++)
	{
		int target;
		scanf("%d",&target);
											//If the Target is a flag then the shortest path via at least one flag condition is satisfied
		if(flag[target]==1)
			printf("%lld\n",distance[target]);
		else
		{
			//if target not a flag
			//As subpaths of shortest paths are shortest paths
			//By applying the dijkstra's algorithm on target node as source and finding the minimum path as 
			// min{source-flag1+flag1-target,source-flag2+flag2-target,...}
			//no of conputation is reduced as constraints on no of  target is less
			long long int distance_target [N];
			shortest_path_dijkstra(N,N,adj_matrix,target,distance_target);
			
			int cur_flag = flags[0];
			long long int min_length=LLONG_MAX;
			
			if( (distance[cur_flag]==LLONG_MAX) || (distance_target[cur_flag]==LLONG_MAX) )			//case of infinity + x
				min_length=LLONG_MAX;
			else
				min_length = distance[cur_flag] + distance_target[cur_flag] ;
			
			for(j=1;j<F;j++)
			{
				cur_flag = flags[j];
				long long int dist_sum=0;
				if( (distance[cur_flag]==LLONG_MAX) || (distance_target[cur_flag]==LLONG_MAX) )		////case of infinity + x
					dist_sum=LLONG_MAX;
				else
					dist_sum=distance[cur_flag] + distance_target[cur_flag];
				if(min_length > dist_sum)
					min_length=dist_sum;
			}
			printf("%lld\n",min_length);
		}
		
	}
	return 0;
}
