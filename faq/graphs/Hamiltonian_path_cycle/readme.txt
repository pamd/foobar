
* Hamiltonian path (Rudrata path): a path that visits every vertex exactly once.
NP hard problem. 
See Java code in: ~/Dropbox/coding/graph/9_hamiltonian_path.cpp

* Hamiltonian cycle (Rudrata cycle): a path that visits every vertex exactly once.
NP hard problem. See UCB "Algorithms" page 252-3.

* "Hamiltonian path" and "Hamiltonian cycle" are equivalent problems.
See the reduction on UCB "Algorithms" page 262-3. 

* As a special case, Hamiltonian cycle in DAG can be found in linear time.
ALGS4 page 599, Excersize 4.2.32:
----------------------------------
Answer: Compute a topological sort and check if there is an edge between 
each consecutive pair of vertices in the topological order.
