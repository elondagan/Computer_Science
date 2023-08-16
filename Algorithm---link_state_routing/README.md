# Algorithm---link_state_routing (2023)
Distributed implementation of "link state routing" algorithm

Description:

implementing a network of routers and the algorithm "link stake routing". 
Initially, each router holds a list of its neighbors, the ports in which they communicate and the weight of the edges connecting between them.
During the program run, each router builds and maintains during the phases adjacency matrix using "link state routing", based on the new weights in each phase.

General notes:
- using only the imports: java.net.* , java.util.* , java.io.* 
- the input has a fixed known form - txt file where each row contains the new edge weights of the corresponding router's neighbors. 
