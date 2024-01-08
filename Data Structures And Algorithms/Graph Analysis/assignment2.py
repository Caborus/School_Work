from collections import defaultdict
import re

class Graph():

    def __init__(self):
        self.graph = defaultdict(list)
        self.V = 0

    def addEdge(self,u,v):
        self.graph[u].append(v)

    def buildGraph(self, input1: str):
        i = 0
        key = 0
        val = 0
        temp = re.findall(r'\d+', input1)
        res = list(map(int, temp))
        for x in res:
            if i % 2 == 0:
                key = x
            else:
                val = x
                if key not in self.graph.keys():
                    self.V += 1
                self.addEdge(key, val)
            i += 1


    def isCyclicUtil(self, v, visited, recStack):
 
        visited[v] = True
        recStack[v] = True
 
        for neighbour in self.graph[v]:
            if visited[neighbour] == False:
                if self.isCyclicUtil(neighbour, visited, recStack) == True:
                    return True
            elif recStack[neighbour] == True:
                return True
 
        recStack[v] = False
        return False
 
    def isCyclic(self):
        visited = [False] * (self.V + 1)
        recStack = [False] * (self.V + 1)
        for node in range(self.V):
            if visited[node] == False:
                if self.isCyclicUtil(node,visited,recStack) == True:
                    return True
        return False  


    def DFSUtil(self, v, visited):
        visited.append(v)
        for neighbour in self.graph[v]:
            if neighbour not in visited:
                self.DFSUtil(neighbour, visited)

    def DFS(self, v):
        visited = []
        self.DFSUtil(v, visited)
        for x in visited:
            if x == visited[0]:
                ans = str(x)
            else: 
                ans = ans + ',' + str(x)
        return ans

class q3Graph:
 
    def __init__(self, row, col, g):
        self.ROW = row
        self.COL = col
        self.graph = g
 
    def isSafe(self, i, j, visited):
        
        return (i >= 0 and i < self.ROW and
                j >= 0 and j < self.COL and
                not visited[i][j] and self.graph[i][j])
             
 
    # A utility function to do DFS for a 2D
    # boolean matrix. It only considers
    # the 8 neighbours as adjacent vertices
    def DFS(self, i, j, visited):
 
        rowNbr = [-1, -1, -1,  0, 0,  1, 1, 1];
        colNbr = [-1,  0,  1, -1, 1, -1, 0, 1];
        
        visited[i][j] = True
 
        for k in range(8):
            if self.isSafe(i + rowNbr[k], j + colNbr[k], visited):
                self.DFS(i + rowNbr[k], j + colNbr[k], visited)
 
 
    def countIslands(self):
        visited = [[False for j in range(self.COL)]for i in range(self.ROW)]
 
        count = 0
        for i in range(self.ROW):
            for j in range(self.COL):
                if visited[i][j] == False and self.graph[i][j] == 1:
                    self.DFS(i, j, visited)
                    count += 1
        return count


def question1(input1: str, input2: int) -> str:
    g = Graph()
    g.buildGraph(input1)
    return g.DFS(input2)


def question2(input1: str) -> str:
    h = Graph()
    h.buildGraph(input1)
    if h.isCyclic() == 1:
        return("Yes")
    else:
        return("No")


def question3(input1: list[list[int]]) -> int:
    row = len(input1)
    col = len(input1[0])
    j = q3Graph(row, col, input1)
    return j.countIslands()