# Vector Pseudocode

    Vector
        capacity
        arr = NIL
        size = 0
        
        Vector(capacity)
            this.capacity = capacity
            this.arr = alloc [capacity]

        Push-Back(value)
            if this.size < this.capacity
                this.arr[size] = value
            else
                this.capacity = this.capacity + 1
                temp = arr
                dealloc this.arr
                this.arr = alloc [capacity]
                for i = 0 to this.size - 1
                    this.arr[i] = temp[i]
                dealloc temp
                this.arr[this.size] = value
            this.size = this.size + 1

        At(index)
            if index >= this.size
                error "index out of range"

            return this.arr[index]

        Pop()
            this.size = this.size - 1

        Erase(index)
            if index >= this.size
                return -1
            if index == this.size - 1
                index = index - 1
            else
                for i = index to this.size - 1
                    this.arr[i] = this.arr[i+1]

            this.size = this.size - 1
            return index
