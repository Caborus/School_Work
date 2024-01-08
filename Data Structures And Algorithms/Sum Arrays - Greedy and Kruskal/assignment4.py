from itertools import count
from operator import truediv
from pickle import FALSE, TRUE
import re


        
#----------------------------------------------------------------------------------------
#question 1
class Item:
    def __init__(self, value, weight):
        self.value = value
        self.weight = weight
 

def buildArr(input: str) -> list:
    arr = []
    val = 0
    weight = 0
    i = 0
    j = 0
    temp = re.findall(r'\d+', input)
    res = list(map(int, temp))
    for x in res:
        if i % 2 == 0:
            val = x
        else:
            weight = x
            arr.append(Item(val, weight))
            j+=1
        i+=1
    return arr



# Main greedy function to solve problem
def fractionalKnapsack(W, arr):
 
    # Sorting Item on basis of ratio
    arr.sort(key=lambda x: (x.value/x.weight), reverse=True)   
 
    # Result(value in Knapsack)
    finalvalue = 0.0
 
    # Looping through all Items
    for item in arr:
 
        # If adding Item won't overflow,
        # add it completely
        if item.weight <= W:
            W -= item.weight
            finalvalue += item.value
 
        # If we can't add current Item,
        # add fractional part of it
        else:
            finalvalue += item.value * W / item.weight
            break
     
    # Returning final value
    return finalvalue
 
 



def question1(input1: str, input2: int) -> int:
    return(int(fractionalKnapsack(input2, buildArr(input1))))
    

#-------------------------------------------------------------------------------------------------------------------
#question 2

def countNodes(input) -> int:
    i1 = TRUE
    i2 = FALSE
    i3 = FALSE
    i = 0
    keys = []
    temp = re.findall(r'\d+', input)
    res = list(map(int, temp))
    for x in res:
        if i1 == TRUE:
            if x not in keys:
                keys.append(x)
                i += 1
            i1 = FALSE
            i2 = TRUE
        elif i2 == TRUE:
            if x not in keys:
                keys.append(x)
                i += 1
            i2 = FALSE
            i3 = TRUE
        elif i3 == TRUE:
            i3 = FALSE
            i1 = TRUE
    return(i)


class Graph:
 
    def __init__(self, vertices):
        self.V = vertices  # No. of vertices
        self.graph = []
        # to store graph
 
    # function to add an edge to graph
    def addEdge(self, u, v, w):
        self.graph.append([u, v, w])
 
    def buildGraph(self, input: str):
        i1 = TRUE
        i2 = FALSE
        i3 = FALSE
        n1 = 0
        n2 = 0
        weight = 0
        temp = re.findall(r'\d+', input)
        res = list(map(int, temp))
        for x in res:
            if i1 == TRUE:
                n1 = x
                i1 = FALSE
                i2 = TRUE
            elif i2 == TRUE:
                n2 = x
                i2 = FALSE
                i3 = TRUE
            elif i3 == TRUE:
                weight = x
                self.addEdge(n1, n2, weight)
                i3 = FALSE
                i1 = TRUE

    # A utility function to find set of an element i
    # (truly uses path compression technique)
    def find(self, parent, i):
        if parent[i] != i:
          # Reassignment of node's parent to root node as
          # path compression requires
            parent[i] = self.find(parent, parent[i])
        return parent[i]
 
    # A function that does union of two sets of x and y
    # (uses union by rank)
    def union(self, parent, rank, x, y):
        
        # Attach smaller rank tree under root of
        # high rank tree (Union by Rank)
        if rank[x] < rank[y]:
            parent[x] = y
        elif rank[x] > rank[y]:
            parent[y] = x
 
        # If ranks are same, then make one as root
        # and increment its rank by one
        else:
            parent[y] = x
            rank[x] += 1
 
    # The main function to construct MST using Kruskal's
        # algorithm
    def KruskalMST(self) -> str:
 
        result = []  # This will store the resultant MST
 
        # An index variable, used for sorted edges
        i = 0
 
        # An index variable, used for result[]
        e = 0
 
        # Step 1:  Sort all the edges in
        # non-decreasing order of their
        # weight.  If we are not allowed to change the
        # given graph, we can create a copy of graph
        self.graph = sorted(self.graph,
                            key=lambda item: item[2])
 
        parent = []
        rank = []
 
        # Create V subsets with single elements
        for node in range(self.V):
            parent.append(node)
            rank.append(0)
 
        # Number of edges to be taken is equal to V-1
        while e < self.V - 1:
 
            # Step 2: Pick the smallest edge and increment
            # the index for next iteration
            u, v, w = self.graph[i]
            i = i + 1
            x = self.find(parent, u)
            y = self.find(parent, v)
 
            # If including this edge doesn't
            # cause cycle, then include it in result
            # and increment the index of result
            # for next edge
            if x != y:
                e = e + 1
                result.append([u, v, w])
                self.union(parent, rank, x, y)
            # Else discard the edge

        minimumCost = 0
        k = 0
        retstr = ""
        result.sort(key=lambda x: x[2])
        result.sort(key=lambda x: x[1])
        result.sort(key=lambda x: x[0])

        for u, v, weight in result:
            minimumCost += weight
            retstr += str(u) + "-" + str(v) + "(" + str(weight) + ")"
            k+=1
            if k < len(result):
                retstr += ","
        return(retstr)
 


def question2(input1: str) -> str:
    g = Graph(countNodes(input1))
    g.buildGraph(input1)

    return(g.KruskalMST())



#-----------------------------------------------------------------------------------------------------------------
#question 3

def solve(a, b, n):
 
    s = 0
 
    # find sum S of both arrays a and b.
    for i in range(0, n):
        s += a[i] + b[i]    
 
    # Single element case.
    if n == 1:
        return a[0] + b[0]
 
    # This checks whether sum s can be divided
    # equally between all array elements. i.e.
    # whether all elements can take equal value
    # or not.
    if s % n != 0:
        return -1
 
    # Compute possible value of new
    # array elements.
    x = s // n
 
    for i in range(0, n):
 
        # Possibility 1
        if a[i] > x:
            return -1   
 
        # ensuring that all elements of
        # array b are used.
        if i > 0:
            a[i] += b[i - 1]
            b[i - 1] = 0
         
        # If a(i) already updated to x
        # move to next element in array a.
        if a[i] == x:
            continue
 
        # Possibility 2
        y = a[i] + b[i]
        if i + 1 < n:
            y += b[i + 1]
         
        if y == x:
            a[i] = y
            b[i] = 0
            if i + 1 < n: b[i + 1] = 0
            continue
         
        # Possibility 3
        if a[i] + b[i] == x:
            a[i] += b[i]
            b[i] = 0
            continue
         
        # Possibility 4
        if i + 1 < n and a[i] + b[i + 1] == x:
            a[i] += b[i + 1]
            b[i + 1] = 0
            continue
         
        # If a(i) can not be made equal
        # to x even after adding all
        # possible elements from b(i)
        # then print -1.
        return -1
     
    # check whether all elements of b
    # are used.
    for i in range(0, n):
        if b[i] != 0:
            return -1   
 
    # Return the new array element value.
    return x


def question3(input1: list[int], input2: list[int]) -> int:
    n = len(input1)
    return(solve(input1, input2, n))

