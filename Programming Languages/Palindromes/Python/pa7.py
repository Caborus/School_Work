def palindromep(lst):
    if(lst == None):
        return 1
    if(len(lst) == 1):
        return 1
    elif(lst[0] == lst[len(lst) - 1]):
        palindromep(lst[1:len(lst)-1])
        return 1
    else:
        return 0
    
print(palindromep([1]))