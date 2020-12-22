---
Author:
- 'Ayan Banerjee'
Date: May 2019
Title: Pipelining
---

Introduction
============

**Problem Statement: Find the Minimum Average Latency of a Non-Linear
Pipeline**\
A Pipeline is a modified queue designed to implement instruction-level
parallelism in a CPU. Using the concept of Pipelining we can overlap
multiple instructions in execution.\
The computer pipeline is divided in a number of stages. Each stage
completes a part of an instruction in parallel. The stages are connected
to each other to form a pipe - instructions enter at one end, progress
through the stages, and exit at the other end.\
Before I proceed, a few important things to note about pipelines are:

-   Pipelining does not decrease latency or the time for individual
    instruction execution.

-   Pipelining increases instruction throughput. The throughput of the
    instruction pipeline is determined by how often an instruction exits
    the pipeline.

-   Pipelining implements a form of parallelism called instruction-level
    parallelism within a single processor.

Non-Linear Pipelining
---------------------

Unlike traditional pipelines (or linear pipelines), **dynamic** or
non-linear pipelines allow for feedback and feed-forward connections
between the various stages of a pipeline.\
A couple of key differences between linear pipelines and non-linear
pipelines are:

-   The Output of a Dynamic Pipeline is not necessarily produced from
    the last stage.

-   Non-Linear Pipelines can be reconfigured to perform different
    actions at different times.

Design
======

The main crux of the problem is: Given a reservation table of a
pipeline, find the minimum average latency for the pipeline.\

Input
-----

A 2D array consisting of 0’s and 1’s. The rows represent the pipeline
stages, and the columns represent the clock cycles. 1’s in the 2D array
represent the clock cycle at which an instruction was initiated in a
particular pipeline stage.

Output
------

My program produces the following output on being provided with a
reservation table:

-   A single floating point value denoting the minimum average latency
    of the given pipeline.

-   A list of all possible average latencies in the given pipeline.

-   A description of all the states in the state diagram. This includes
    a detailed description of all the vertices and the edges present in
    the state diagram.

-   A Text File containing all the elementary cycles possible in the
    state diagram. An **Elementary Cycle** is one in which no vertex is
    repeated twice except for the start node.

Procedure
---------

: The whole process can be divided into 8 atomic stages which must be
conducted sequentially for me to derive the aforementioned outputs from
the reservation table. These stages are listed below:

1.  Find non-repeating Forbidden Latencies from reservation table.

2.  Sort this list in ascending order and form the initial collision
    vector.

3.  Start constructing the state diagram from the initial collision
    vector.

4.  Convert the state diagram into a directed graph.

5.  Find all elementary cycles in the aforementioned directed graph.

6.  Find the average latency for each such mentioned elementary cycle.

7.  Select all elementary cycles with average latency which is less than
    the number of forbidden latencies.

8.  Select the smallest such latency.

Main Design Issues
==================

There were four main design issues in the whole program, which are
listed as follows:

1.  Designing the structure which would be used for representing the
    different states of the state diagram.

2.  Actually populating the state diagram.

3.  Converting the state diagram into a directed graph.

4.  Finding all Elementary Circuits of the directed graph.

Designing the States
--------------------

Each state is based on it’s unique collision vector. Also each state has
an array of next states, and edge weights in a One-One correspondence
with every element of the next states.\
After some deliberation I came up with the following structures to
accommodate the aforementioned specifications:

    struct collisionVector{
            int arr[COL];
            int length;
    };

    struct state{ 
            int value;
            struct collisionVector* cv;
            struct state* next[SIZE];
            int latency[SIZE];
    };

Creating a Directed Graph representing the State Diagram
--------------------------------------------------------

The function **populateDiagram()** basically does a depth first population
of the state diagram. It takes each node as it is created, and computes
every possible child node (and the edges to these child nodes). Later
the function is recursively called for every child node if they have not
been populated earlier.\
\
While each node is being thus populated, I simulataneously
construct an Adjacency List representation of the Directed Graph
which the State Diagram ultimately represents.\
\
**function $populateDiagram()$**\
**Input**:

-   'struct state* firstState'\
    This is the very first state created from the initial collision
    vector. This state serves as the seed for the population process of
    the State Diagram.

-   'struct$ collisionVector* initialVector'\
    This is the initial collision vector which was created from the
    input reservation table.

**Output**: None\
**Global Variables**:

-   'struct state* adjacencyList[]'\
    This is an Adjacency List used to store the directed graph notation
    of the state diagram.

-   'struct state* traversedArray[]'\
    This is a hashtable used to implement the Depth First population of
    the states in the state diagram.

**Algorithm**:

    Begin:

    1.  Set traversedArray value for firstState as True.
    2.  Traverse over the initial collision vector.
        a.  For every right shifted 0, we have two things - 
        a new state configuration, and a latency associated with the new state.
        
        b.  Update this new state and the associated latency 
        as a child node and edge of the given firstState.
        
        c.  Update this as a node in the adjacency list entry for firstState.
        
    3.  Mark firstState as traversed.

    4.  For every child of firstState recursively call populateDiagram()
    if it is not traversed.

    End

Finding all Elementary Cycles of a Directed Graph
-------------------------------------------------

This problem is a well known NP-Hard problem (possibly even PSPACE). In
all my ignorance, at first I tried doing a simple DFS search (modified to
fit my needs) on the adjacency list. However the search yielded results
which were duplicate, and ignored some crucial cycles in the graph. This
lead me to realize that there was more to this particular problem than
met the eye, and I started researching on the topic.\
I came across quite a few papers published from 1970 (by Tiernan) to
1975 (by Johnson) that gave approximate algorithms to solve the problem.
The best algorithm in this case was clearly Johnson’s algorithm by his
own admission. 

This shows that even after reducing the Running Time complexity by an
order of O(e.(c+1)), I was still left with an exponential time
complexity since the number of elementary circuits in the graph could
itself be exponential in nature.\
Also just because Johnson’s algorithm was the best in terms of running
time complexity, didn’t mean it was the most adaptable to my situation.
Note that Johnson’s algorithm itself is not explained in detail here, as
I ended up using my own modified version. I had to code the program
**entirely in ANSI C**, and Johnson’s algorithm provided the following
challenges:

-   I needed to implement an algorithm to find and return Strongly
    Connected Components to the main algorithm. This meant implementing
    either Tarjan’s algorithm or Kosaraju’s algorithm and somehow
    returning the set of Strongly Connected Components to the main
    algorithm to process.

-   Johnson’s algorithm itself needs complicated datastructures like a
    blocket set and a blocked map, which are quite tricky to implement
    in C.

I made the following discoveries which led to our own modified
algorithm:

-   The Blocking Mechanism in Johnson’s algorithm could be ignored with
    a penalty induced in the running time, but on the other hand
    simplifying the process manifold by removing dependency on any
    external Data Structure except for a stack.

-   The initial state diagram is always a strongly connected component.
    And even if resultant state diagrams (after removing vertices)
    aren’t strongly connected components, we just incur an extra penalty
    of O(n^2) at most for searching through the adjacency list looking
    for cycles that don’t exist.

A concise algorithm for our implementation is given as follows:

    Begin main():
        
    For every node in the list of vertices:
        a. Create a new stack.
        b. Call the findCycles() method using this node, and the stack.
        c. Remove the current node from the adjacency list.
        d. Recalculate the Hash Table which associates the collision
        vector values of the nodes with respective vertex numbers.
        e. Free the stack.
    End

    Begin findCycles(stack, current_node):

    1. Push current_node onto the stack.
    2. Mark the node as present on stack.
    3. For every child of current node u do:
        a. If the child of u is the current_node, its a cycle. Send
        the stack to printCycles().
        b. If child of u is already present in stack then move onto 
        the next child. This is our own implementation of the blocking
        mechanism of Johnson's algorithm.
        c. Otherwise push this child onto the stack and recursively call
        findCycles() on this node.
    4. Pop stack.
    5. Mark node as absent on stack.
    6. Return to the parent node to look for alternate routes.

    End.

    Begin printCycle(stack):

    1. Pop every element from the stack and push it into a temporary stack.
    2. Pop every element from the temporary stack:
        a. Print this element.
        b. Reinsert it into original stack.

    End
