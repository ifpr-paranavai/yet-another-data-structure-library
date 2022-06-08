# Binary  Tree Pseudocode

	Node
		value
		right = NIL
		left = NIL

		Node(value)
			this.value = value

	BinaryTree
		root = NIL
		size = 0

		private Add(node, value)
			if value < node.value
				if node->left == NIL
					node->left = alloc Node(value)
				else
					return this.Add(node.left, value)
			else
				if node.right == NIL
					node.right = alloc Node(value)
				else
					return this.Add(node.right, value)
			this.size = this.size + 1
		
		private Erase(&parent, node, value)
			if node == NIL
				return NIL
			else if value > node.value
				return this.Erase(&node.right, node.right, value)
			else if value < node.value
				return this.Erase(&node.left, node.left, value)
			
			if node.right == NIL and node.left == NIL
				&parent = NIL
			else if node.right == NIL
				&parent = node.left
			else if node.left == NIL
				&parent = node.right
			else
				&parent = node.right
				temp = node.right
				while node.left
					temp = temp.left
				temp.left = node.left
			
			dealloc node
			this.size = this.size + 1
			return &parent

		Add(value)
			if this.root == NIL
				this.root = alloc Node(value)
				this.size = this.size + 1
			else
				this.Add(this.root, value)
		
		Get(node, value)
			if this.root == NIL
				return NIL
			node = this.root
			while node.value != value
				if value < node.value
					node = node.left
				else
					node = node.right
				if node == NIL
					break
			return node

		Erase (value)
			return this.Erase(&this.root, this.root, value)
