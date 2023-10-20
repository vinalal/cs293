<h1>Part 1</h1>

Implement a Class Graph.
Takes constructor argument as no of nodes, followed by a vector of pairs representing a edge list.

Implement a function called `bfs()` that takes two arguments:
* A node x in the graph G
* A graph G

The function `bfs()` should perform a breadth-first search on the graph G starting from the node x, and should return a map containing the following information:
* The key of each entry in the map should be a node in the graph G.
* The value of each entry in the map should be the parent of the corresponding node in the breadth-first 
 search tree.

The parent of the root node of the breadth-first search tree is -1. 

Example:  
Input: 
* 1 // part-1
* 5 // no of nodes
* 5 // no of edges
* 0 1
* 0 2
* 2 3
* 2 4
* 3 4 
* 0   // source vertex 

Output: 

* -1
* 0
* 0
* 2
* 2  

This is the output of `bfs()` on the graph G starting from the node 0. The parent of the node 0 is -1, the parent of the node 1 is 0, and so on. 


***
<h1>Part 2</h1>

Suppose you want to go from A->B->C->D->E in a graph, where A,...,E are arbitrary nodes in our graph. How will you do it?  
Let's answer this question: We will take shortest route from (A-B) (B-C) (C-D) (D-E). 
Combining the above 4 routes we will get the shortest walk going from A-B-C-D-E. 

For this part Using part-1 as a subroutine, Implement a function `shortest_walk()` that takes input as a list of nodes to visit in order, and you have to return the shortest walk as a list of nodes visited during that walk. 

Define a relation R on pairs of nodes such that two nodes x and y are related iff one of them is an ancestor of the other. Now, if `xRy`, the shortest path between x and y in the original graph is just the path between x and y in the BFS tree. (Do you see why?)

Using this, you might be able to avoid some unnecessary calls to `bfs()`, thereby optimising the search.


Example:
Input:
* 2 // part-2
* 5 // no of nodes
* 5 // no of edges
* 0 1
* 0 2
* 2 3
* 2 4
* 3 4 
* 6 // no of stations
* 0 1 4 1 4 1 

Output: 
* 0 1 0 2 4 2 0 1 0 2 4 2 0 1

***
<h1>Part 3</h1>

Now, consider a time-based setting. You are given a graph G and a list of users, each user is   
represented by 3 tuples `(arrival_time,source,destination)`, where `source` and `destination` are nodes of the graph G. There is a vector `users` that is sorted by `arrival_time`.

Assume that to move along a edge user will take one unit of time. **A single edge can be traversed by a single user at a time instance**. Any no of users can wait at a vertex.

When a user arrives (You have to give priority to users according to their order in `users`) you have to give it a shortest possible run from `source` to `destination`, based on the information you have at that point of time (Which is all the paths that the users with lesser `arrival_time` have chosen). 
  

Output format: vector of vector of positions: Showing positions of every user in graph from their arrival 
time to end time.

Example:
Input:  
- Graph
  *  3 // part-3
  *  5 // no of nodes 
  *  5 // no of edges 
  *  0 1 
  *  0 2 
  *  2 3
  *  2 4
  *  3 4  
- User
  * 2 // no of users
  * 1 1 4 //U1
  * 2 0 2 //U2
 
Output: 
<<1,0,2,4>,<0,0,2>> 
User 1 arrives on time 1 and it is assigned a path <1,0,2,4>. Now, when user 2 arrives on time 2, it has to wait there for 1 second since edge between 0 and 2 is is in use by user 1, so user 2 is assigned the path <0,0,2>. 
***