arr = [1,2,2,2,3,4,4,4,5,6,7,8]


def max(arr):
    max = 0
    for i in arr:
        if i > max:
            max = i
    return max

def maxRec(arr):
    if len(arr) == 1:
        return arr[0]
    max = maxRec(arr[1:])
    if (arr[0] > max):
        return arr[0]
    return max

def removeDupes(arr):
    result = [arr[0]]
    for i in range(1, len(arr)):
        if arr[i] != arr[i-1]:
            result.append(arr[i])
    return result

def removeDupesRec(arr):
    if len(arr) == 1:
        return arr
    
    rest = removeDupesRec(arr[1:])
    if arr[0] == rest[0]:
        return rest
    else:
        return [arr[0]] + rest

print(removeDupesRec(arr))