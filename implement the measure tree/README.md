# CSC541-Project2
course project for CSC 541 advance data structure

Implement the measure tree, as described in the book chapter 4.3.
The measure tree is a dynamic structure that maintains a system of intervals under insertion and
deletion, and can answer the query: give the total length of the union of the current intervals.
So the structure should support the following operations:
- m_tree_t * create_m_tree() creates an empty measure tree.
- void insert_interval(m_tree_t * tree, int a, int b) inserts the interval [a,b[.
- void delete_interval(m_tree_t * tree, int a, int b) deletes the interval [a,b[, if it exists.
- int query_length(m_tree_t * tree) returns the length of the union of all intervals in the
current set.
