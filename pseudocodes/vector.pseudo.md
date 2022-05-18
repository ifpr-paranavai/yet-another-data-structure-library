Vector 
    arr
    capacity
    size = 0

    Vector(n)
        capacity = n
        arr = [capacity]

    Push-Back(value)
        arr[size] = value
        size = size+1

    At(index)
        if index >= size
            error "index out of range"
        
        return arr[index]
    
    erase(index)
        if index == size
            return -1
        if index == size-1
            index = index-1
        else
            for i = index to size-1
                arr[i] = arr[i+1]

        size = size-1
        return index
