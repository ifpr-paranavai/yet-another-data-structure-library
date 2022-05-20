# Vector Pseudocode

    Vector(n)
        capacity = n
        arr = [capacity]

    Push-Back(value)
        if size < capacity
            arr[size] = value
        else
            capacity = capacity+1
            temp = arr
            delete arr
            arr = [capacity]
            for i = 0 to size
                arr[i] = temp[i]
            delete temp
            arr[size] = value
        size = size+1

    At(index)
        if index >= size
            error "index out of range"
        
        return arr[index]

    Pop()
        size = size - 1
    
    Erase(index)
        if index >= size
            return -1
        if index == size-1
            index = index-1
        else
            for i = index to size-1
                arr[i] = arr[i+1]

        size = size-1
        return index
