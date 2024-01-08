# Alexander Cabos
# CS 372 - Programming assignment 1
# assignment1.py 
# question 1: find and return the index of a peak element in an array
# question 2: find the smallest missing non-negative element in a sorted array
# question 3: implement quicksort with random pivot
# question 4: take two sorted input1s and compute the kth element in the union of the input1s

import random
  
# ----------------------QUESTION 1--------------------- 

def question1(input1: list[int]) -> int:
    left = 0
    right = len(input1) - 1
    while left < right:
        mid = (left + right) / 2
        mid = int(mid)
        if input1[mid] > input1[mid + 1]:
            right = mid
        else:
            left = mid + 1
    return left

# ----------------------QUESTION 2--------------------- 

def question2(input1: list[int]) -> int:
    left = 0
    right = len(input1) - 1

    while left <= right:
        mid = (left + right) / 2
        mid = int(mid)

        if mid == input1[mid]:
            left = mid + 1
        else:
            right = mid - 1
    return(left)

# ----------------------QUESTION 3--------------------- 

#recursive functions for question 3
def quicksort(arr, lo, hi):
    if(lo < hi):
        pivot = randPartition(arr, lo, hi)
        quicksort(arr, lo, pivot - 1)
        quicksort(arr, pivot + 1, hi)


def randPartition(arr, lo, hi):
    randpivot = random.randrange(lo, hi)
    arr[lo], arr[randpivot] = arr[randpivot], arr[lo]
    return partition(arr, lo, hi)

def partition(arr, lo, hi):
    pivot = lo
    i = lo + 1

    for j in range(lo + 1, hi + 1):
        if arr[j] <= arr[pivot]:
            arr[i], arr[j] = arr[j], arr[i]
            i += 1
    arr[pivot], arr[i - 1] = arr[i - 1], arr[pivot]
    pivot = i - 1
    return(pivot)


def question3(input1: list[int]) -> str:
    quicksort(input1, 0, len(input1) - 1)
    return(input1)


# ----------------------QUESTION 4--------------------- 

def question4(input1: list[int], input2: list[int], input3: int) -> int:
    sorted = [0] * (len(input1) + len(input2))
    i = 0
    j = 0
    d = 0
    while(i < len(input1) and j < len(input2)):
 
        if(input1[i] < input2[j]):
            sorted[d] = input1[i]
            i += 1
        else:
            sorted[d] = input2[j]
            j += 1
        d += 1
 
    while(i < len(input1)):
        sorted[d] = input1[i]
        d += 1
        i += 1
    while(j < len(input2)):
        sorted[d] = input2[j]
        d += 1
        j += 1
    return sorted[input3 - 1]