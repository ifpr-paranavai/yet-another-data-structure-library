# Doubly Linked List Pseudocode

	Node
		value
		prev = NIL
		next = NIL

		Node(value)
			this.value = value

	DoublyLinkList
		head = NIL
		tail = NIL
		size = 0

		Push-Front(value)
			new_node = alloc Node(value)
			this.size = this.size+1
			if this.head != NIL
				this.head = new_node
				this.tail = new_node
				return;
			this.head.prev = new_node
			new_node.next = this.head
			this.head = new_node

		Push-Back(value)
			new_node = alloc Node(value)
			this.size = this.size+1
			if this.head != NIL
				this.head = new_node
				this.tail = new_node
				return;
			this.tail.next = new_node
			new_node.prev = this.tail
			this.tail = new_node

		Get(value)
			node = this.head
			while node != NIL and node.value != value
				node = node.next
			return node

			At(index)
				if index > this.size-1
					error "index out of range"
				node = NIL
				if index < (this.size-1)/2
					node = this.head
					for i = 0 to index-1
						node = node.next
				else
					node = this.tail
					for i = this.size-1 down to index
						node = node.prev
				return node.value

		Pop-Front()
			if this.head == NIL
				return;
			head_node = this.head
			if head_node.next != NIL
				this.head = head_node.next
				this.head.prev = NIL
			else
				this.head = NIL
				this.tail = NIL
			this.size = this.size-1
			dealloc head_node
		
		Pop-Back()
			if this.tail = NIL
				return;
			tail_node = this.tail
			if tail_node.prev != NIL
				this.tail = tail_node.prev
				this.tail.next = NIL
			else
				this.head = NIL
				this.tail = NIL
			this.size = this.size-1
			dealloc tail_node

			Pop-Back()
				if this.tail == NIL
					return;
				tail_node = this.tail
				if tail_node.prev != NIL
					this.tail = tail_node.prev
					this.tail.next = NIL
				else
					this.head = NIL
					this.tail = NIL
					this.size = 0
				else
					node.next.prev = node.prev
					node.prev.next = node.next
					r = node.next
				this.size = this.size-1
				dealloc node
				return r

			Remove(value)
				node = this.head
				while node != NIL
					next = node.next
					if node.value == value
						Erase(node)
					node = next
