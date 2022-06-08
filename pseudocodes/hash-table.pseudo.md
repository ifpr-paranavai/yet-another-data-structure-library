# Hash Table Pseudocode

	HashTable
		arr = NIL
		capacity = 0
		size = 0

		HashTable(capacity)
			this.capacity = this.Next-Prime-Number(capacity * 2)
			this.arr = alloc [this.capacity]
			for i = 0 to this.capacity
				this.arr[i] = List()

		private Is-Prime-Number(n)
			if n <= 1
				return false
			if n <= 3
				return true
			if (n mod 2) == 0 or (n mod 3) == 0
				return false
			i = 5
			while (i * i) <= n
				if (n mod i) == 0 or (n mod (i + 2)) == 0
					return false
				i = i + 6 
			return true

		private Next-Prime-Number(n)
			if n <= 1
				return 2
			while true
				n = n + 1
				if this.Is-Prime-Number(n) == true
					break
			return n
		
		private Hash()
			return value mod this.capacity
		
		Add(value)
			list = this.arr[this.Hash(value)]
			list.Push-Back(value)
			this.size = this.size + 1

		Get(value)
			list = this.arr[this.Hash(value)]
			list_node = list.Get(value)
			if (list_node == NIL)
				return NIL
			return list_node.value

		Erase(value)
			list = this.arr[this.Hash(value)]
			list.Erase(list.Get(value))
			this.size = this.size - 1

