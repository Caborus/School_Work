import re


#----------------------------------------------------------------
# Question 1

def knapSack(W, wt, val, n):
    
    if n == 0 or W == 0:
        return 0

    if (wt[n-1] > W):
        return knapSack(W, wt, val, n-1)

    else:
        return max(
            val[n-1] + knapSack(
                W-wt[n-1], wt, val, n-1),
            knapSack(W, wt, val, n-1))


def q1build(in1: str, in2: int) -> int:
    i = 0;
    val = []
    wt = []
    W = in2;
    temp = re.findall(r'\d+', in1)
    res = list(map(int, temp))
    for x in res:
        if i % 2 == 0:
            val.append(x)
        else:
            wt.append(x)
        i+=1
    
    n = len(val)
    return(knapSack(W, wt, val, n))



def question1(input1: str, input2: int) -> int:
    return(q1build(input1, input2))
    

#-----------------------------------------------------------------------
# Question 2

def unboundedKnapsack(W, index, val, wt):
     
    #Base case of recursion when only one element is there.
    if index==0 :return (W//wt[0])*val[0]
    #If the element with referred by index is doesn't occur even once in the required solution
    notTake=0+unboundedKnapsack(W,index-1,val,wt)
    #If the element is occur atleast once in the required solution
    take=-100000
    if wt[index]<=W:
        take=val[index]+unboundedKnapsack(W-wt[index],index,val,wt)
    return max(take,notTake)   

def q2build(in1: str, in2: int) -> int:
    i = 0;
    val = []
    wt = []
    W = in2;
    temp = re.findall(r'\d+', in1)
    res = list(map(int, temp))
    for x in res:
        if i % 2 == 0:
            val.append(x)
        else:
            wt.append(x)
        i+=1
    
    n = len(val)
    return(unboundedKnapsack(W, n - 1, val, wt))

def question2(input1: str, input2: int) -> int:
    return(q2build(input1, input2))


#-----------------------------------------------------------------------
# Question 3

def editDistance(str1, str2, m, n):
     
    if m == 0:
        return n
 
    if n == 0:
        return m

    if str1[m-1] == str2[n-1]:
        return editDistance(str1, str2, m-1, n-1)
 
    return 1 + min(editDistance(str1, str2, m, n-1),    # Insert
                   editDistance(str1, str2, m-1, n),    # Remove
                   editDistance(str1, str2, m-1, n-1)    # Replace
                   )


def question3(input1: str, input2: str) -> int:
    return(editDistance(input1, input2, len(input1), len(input2)))
