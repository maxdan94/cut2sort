
## Info:

Feel free to use these lines as you wish.
Computes graph cut recursively in order to obtain an ordering of the nodes

## To compile:

"make"

## To execute:

"./cut2sort edgelist.txt order.txt [bisection]"
- "edgelist.txt" should contain an undirected unweighted graph: one edge on each line (two unsigned long (nodes' ID)) separated by a space.
- "order.txt": will contain the resulting order of the nodes: "newID oldID\n" on each line.
- Nodes with degree 0 (i.e. belonging to 0 edges) in the original graph are ignored.

option "bisection": the  bisection algorithm to use, default is 1.
- 0: random bisection
- 1: greedy spasest cut
- 2: greedy densest cut
- 3: greedy max cut

## Initial contributor:
Maximilien Danisch  
April 2019  
http://bit.ly/danisch  
maximilien.danisch@gmail.com
