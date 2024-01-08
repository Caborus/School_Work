# Alexander Cabos
# CS372
# Assignment 6
#-------------------------------------------------------------------
# Question 1

def lcs(X, Y, m, n): 
  
    if m == 0 or n == 0: 
        return 0
    elif X[m-1] == Y[n-1]: 
        return 1 + lcs(X, Y, m-1, n-1); 
    else: 
        return max(lcs(X, Y, m, n-1), lcs(X, Y, m-1, n)); 
  

def question1(input1: str, input2: str) -> int:
    return lcs(input1, input2, len(input1), len(input2))


#-------------------------------------------------------------------
# Question 2


def lps(str):
    n = len(str)
  
    # Create a table to store
    # results of subproblems
    L = [[0 for x in range(n)]for y in range(n)]
  
    # Strings of length 1
    # are palindrome of length 1
    for i in range(n):
        L[i][i] = 1
  
    # Build the table. Note that
    # the lower diagonal values
    # of table are useless and
    # not filled in the process.
    # c1 is length of substring
    for cl in range( 2, n+1):
        for i in range(n - cl + 1):
            j = i + cl - 1
            if (str[i] == str[j] and cl == 2):
                L[i][j] = 2
            elif (str[i] == str[j]):
                L[i][j] = L[i + 1][j - 1] + 2
            else:
                L[i][j] = max(L[i][j - 1],L[i + 1][j])
  
    # length of longest
    # palindromic subseq
    return L[0][n - 1]
  
# function to calculate
# minimum number of deletions
def minimumNumberOfDeletions( str):
 
    n = len(str)
  
    # Find longest palindromic
    # subsequence
    l = lps(str)
  
    # After removing characters
    # other than the lps, we
    # get palindrome.
    return (n - l)

def question2(input1: str) -> int:
    return minimumNumberOfDeletions(input1)


#-------------------------------------------------------------------
# Question 3


def fib(n):
    if n <= 1:
        return n
    return fib(n-1) + fib(n-2)
 
# Returns no. of ways to
# reach sth stair
def countWays(s):
    return fib(s + 1)



def question3(input1: int) -> int:
    return countWays(input1)


