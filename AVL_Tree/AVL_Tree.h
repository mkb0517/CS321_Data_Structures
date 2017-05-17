/**
 * AVL_Tree.h
 * @author		: Matthew Brown
 * @description	: This project was created to teach us how to implement a version of red-black trees known as AVL (Adelson-Velsky-Landis) trees.
 *				: It uses a balancing factor to keep track of the weight of each subtree at each node.
 *				: It uses the invariant Balance(node) = {-1, 0, 1} to determine if the tree is balanced.
 *				: It checks the balance of the tree after every insertion and deletion.
 */

/**
 * AVL Tree class for the storing and balancing of data in a binary tree.
 */
template<typename T = int>
class tree {
public:
	/**
	 * Node struct to handle our data management. Has a tag member to keep track of the balance factor
	 */
	struct node {
		T data;
		signed tag; // the balance factor. negative if the left is taller, positive if right.
		node *left, *right;
		/**
		 * node overloaded constructor. Creates a node and sets it's data to n.
		 * @param n value we want to populate the node with.
		 */
		node(T n): data(n),tag(0),left(nullptr),right(nullptr){}
	};
	/**
	 * default constructor for tree. Initiallizes root to a nullptr.
	 */
	tree():root(nullptr){}

	/**
	 * public accessor function for inorder function.
	 * @param f function passed from main
	 * post: calls the private inorder function with the root pointer to travers the tree inorder
	 */
	template<typename fn>
	void inorder(fn f) const {
		inorder(f,root);
	}

	/**
	 * public accessor function for preorder function.
	 * @param f function passed from main
	 * post: calls the private preorder function with the root pointer to traverse the tree in preorder
	 */
	template<typename fn>
	void preorder(fn f) const {
		preorder(f,root);
	}

	/**
	 * public accessor function for post order function.
	 * @param f function passed from main.
	 * post: calls the private postorder function with the root pointer to traverse the tree in postorder
	 */
	template<typename fn>
	void postorder(fn f) const {
		postorder(f,root);
	}
	
	/**
	 * public accessor function for insertion.
	 * @param n value we want to insert
	 * Post: Calls the private insert function with the root pointer to insert a specific value into the tree
	 */
	void ins(T n) {
		ins(n,root);
	}
	
	/**
	 * public accessor function for deletion.
	 * @param n value we want to delete
	 * post: Calls the private delete function with the root pointer to delete a specific value from the tree
	 */
	void del(T n) {
		del(n,root);
	}
	
	/**
	 * public accessor function to delete the entire tree.
	 * pre: user must call it from the program
	 * Post: Calls the private delete function with the root pointer to delete the entire tree
	 */
	void del() {
		del(root);
	}

private:
	node* root;

	/**
	 * Private inorder function to handle node pointers and traverse the tree in order.
	 * @param f function passed from main.
	 * @param p node* to traverse the tree.
	 */
	template<typename fn>
	void inorder(fn f, node* p) const{
		if(!p) return;
		inorder(f,p->left);
		f(p);
		inorder(f,p->right);
	}

	/**
	 * Private preorder function to handle node pointers and traverse the tree in pre order
	 * @param f function passed from main
	 * @param p node pointer to traverse the tree
	 */
	template<typename fn>
	void preorder(fn f, node* p) const{
		if(!p) return;
		f(p);
		preorder(f,p->left);
		preorder(f,p->right);
	}

	/**
	 * private post order function to traverse the tree in post order
	 * @param f function passed from main
	 * @param p node* to traverse the tree in post order
	 */
	template<typename fn>
	void postorder(fn f, node* p) const{
		if(!p) return;
		postorder(f,p->left);
		postorder(f,p->right);
		f(p);
	}

	/**
	 * rotates the tree to the right at p
	 * @param p pointer to the node we want to rotate at p
	 */
	void rotateRight(node* &p){
		node* q = p;
		p = q->left;
		q->left = p->right;
		p->right = q;
	}

	/**
	 * rotates the tree left at p
	 * @param p pointer to the node we want rotate
	 * post: shuffles the node pointers so that they are rotated left
	 */
	void rotateLeft(node* &p){
		node* q = p;
		p = q->right;
		q->right = p->left;
		p->left = q;
	}

	/**
	 * Private function to delete by copying.
	 * @param p the pointer to the node we are currently at
	 * @paramm data the value of the node we are currently at
	 * @return -1 because we are deleting a node
	 */
	signed delMin(node* &p, T &data){
		// If we have reached as far left as we can go, we are at the minimum relative to the original calling node
		if(!p->left){
			node* temp = p;
			data=p->data;
			p = p->right;
			delete temp;
			return -1;
		}
		// else we traverse down to the left until we get to the leftmost node
		else {
			return growth(delMin(p->left, data),p,0);
		}
	}

	/**
	 * function to keep track of the growth of the tree and call rebalance if necessary
	 * @param leftGrowth the growth of the left side of the node
	 * @param p the pointer to the node we are currently at
	 * @param rightGrowth the growth of the right side of the node
	 * @returns the growth of a node
	 */
	signed growth(signed leftGrowth, node *&p, signed rightGrowth) {
		// initialize a left and right growth counter to 0
		int L=0, R=0;
		// Check to see which side is is heavier
		if(p->tag>0) L=-p->tag;
		else R=p->tag;
		// add the left and right growth to each growth counter
		L+=leftGrowth;
		R+=rightGrowth;
		// the new value of the tag is the right minus the left
		p->tag=R-L;
		// the increase will be in which ever side is greater
		signed increase = R>L?R:L;
		// return the increase after rebalancing the node
		return increase+rebalance(p);
	}

	/**
	 * Rebalances the tree based on the tags of the nodes that are passed to it.
	 * @param p pointer to the node we are currently at
	 * @return the growth or shrinkage of the current node
	 */
	signed rebalance(node* &p) {
		switch(p->tag) {
		// If the tag is -2. we need to check the left tag
		case -2:
			switch(p->left->tag) {
			// If the left side is heavier on the left, rotate right to balance this node, return -1 to note that the tree shrank
			case -1:
				rotateRight(p);
				p->right->tag=p->tag=0;
				return -1;
			// If the left side is balanced, rotate right to maintain the proper order, return 0 to note that the tree is the same size
			case 0:
				rotateRight(p);
				p->tag=1;				// note that the node is heavier on the right
				p->right->tag=-1;		// note the right child is heavier on the left
				return 0;
			// If the left side is heavier on the right, rotate left on the left child, then rotate right on the current node
			case 1:
				rotateLeft(p->left);
				rotateRight(p);
				// Check to see the state of the rebalanced tree
				switch(p->tag) {
				// If the node was heavier on the left, it is now heavier on the right
				case -1: //0,1
					p->left->tag=0;
					p->right->tag=1;
					break;
				// IF the node was balanced before, it is still balanced
				case 0: //0,0
					p->left->tag=0;
					p->right->tag=0;
					break;
				// If the node was heavier on the right, it is now heavier on the left
				case 1://-1,0
					p->left->tag=0;
					p->right->tag=-1;
					break;
				}
				break;
			}
			break;
		// Same as above but swapped
		case 2:
			switch(p->right->tag) {
			case -1:
				rotateRight(p->right);
				rotateLeft(p);
				switch(p->tag) {
				case -1:
					p->left->tag=p->tag = 0;
					p->right->tag=1;
					break;
				case 0:
					p->right->tag=p->left->tag=0;
					break;
				case 1:
					p->right->tag=p->tag= 0;
					p->left->tag=-1;
					break;
				}
				return -1;
			case 0:
				rotateLeft(p);
				p->tag=-1;
				p->left->tag=1;
				p->right->tag=0;
				break;
			case 1:
				rotateLeft(p);
				p->tag=p->left->tag=0;
				return -1;
			}
			break;
		}
		return 0;
	}

	/**
	 * Private insert function for AVL tree
	 * @param n value we want to insert
	 * @param p pointer to node we are at
	 * @return the amount of growth (how much taller or shorter)
	 */
	signed ins(T n, node* &p){
		// if the tree does not exist, create a new node and return 1 for growth
		if(!p){
			p = new node(n);
			return 1;
		}
		// if the value we want to insert is greater than the current value, insert to the right and return growth
		if(n>p->data) return growth(0,p,ins(n, p->right));
		// if the value we want to insert is less than the current value, insert to the left and return growth
		if(n<p->data) return growth(ins(n, p->left),p,0);
		// Otherwise the value already exists and we can return no growth
		return 0;
	}

	/**
	 * Private delete function to handle single node deletion.
	 * @param n	Value we are trying to delete
	 * @param p Pointer of the node we are looking at.
	 */
	signed del(T n, node* &p) {
		if(!p) return 0;			// If the node does not exist, return
		if(n<p->data){
			// If the value we want to delete is less than the current value, go left and return the balance factor
			return growth(del(n,p->left),p,0);
		}
		if(n>p->data){
			// If the value we want to delete is greater than the current value, go right and return the balance factor
			return growth (0,p,del(n,p->right));
		}
		// When we get here it means the value has been found or does not exist in the tree
		node *temp=p;
		if(n==p->data){				// if the current value is the one we want to delete
			if(!p->right){			// and there is no child to the right
				temp = p;			
				p = p->left;		// Make p point to the left child (if p is a leaf node it is fine because the swap will make p a nullptr)
				delete temp;		// delete the current node
				return -1;			// return a reduced balance factor
			}
			else {
				// Else we must call the delete min function to replace the value
				return growth(0,p,delMin(p->right,p->data));
			}
		}
	}

	/**
	 * private function to delete the entire tree
	 * @param p pointer to a node in the tree
	 */
	void del(node* &p){
		if(!p) return;		// If the node doesn't exist, return
		del(p->left);		// If it does, delete the left node
		del(p->right);		// Delete the right node after
		delete p;			// Delete the current node
		p=0;				// Set the pointer to 0 to prevent a hanging pointer
		return;
	}
};
