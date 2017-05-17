/**
 * BinaryTree.h
 *		Basic binary tree exercise for Data Structures.
 *      @author Matthew Brown
 */

/**
 * tree class to define our tree data structure
 */
template<typename T = int>
class tree {
public:
	/**
	 * node struct to handle the tree data points.
	 */
	struct node {
		T data;
		node *left, *right; 

		/**
		 * Default constructor for node. Sets data to 0 and the pointers to nullptr
		 */
		node():data(0),left(nullptr),right(nullptr){}
		/**
		 * Overloaded constructor for node. Initializes the data to the value passed
		 * and sets the pointers to null if pointers are not passed.
		 * @param data Data from the user to input into the tree
		 * @param left Pointer to a node with a value less than data
		 * @param right Pointer to a node with a value greater than data
		 */
		node(T data, node* left=nullptr, node* right=nullptr):data(data), left(left), right(right){}
	};
	
	/**
	 * tree default constructor. Initializes root to nullptr.
	 */
	tree():root(nullptr){}
	
	/**
	 * Public accessor function for main to use to call the tree in order
	 * @param f function to call at each node in the tree
	 * post: Calls the private inorder function with the root pointer to traverse the tree inorder
	 */
	template<typename fn>
	void inorder(fn f) {
		inorder(f,root);
	}
	
	/**
	 * Public accessor function for main to use to call the tree in preorder
	 * @param f function to call at each node in the tree
	 * Post: Calls the private preorder function with the root pointer to traverse the tree in preorder
	 */
	template<typename fn>
	void preorder(fn f) {
		preorder(f,root);
	}
	
	/**
	 * Public accessor function for main to use to call the tree in postorder
	 * @param f function to call at each node in the tree
	 * post: Calls the private postorder function with the root pointer to traverse the tree in postorder
	 */
	template<typename fn>
	void postorder(fn f) {
		postorder(f,root);
	}
	
	/**
	 * Public accessor function for users to call to insert a specific value into the tree
	 * @param n Value that the user wants to insert
	 * post: Calls the private insert function with the root pointer to insert the value given
	 */
	void ins(T n) {
		ins(n,root);
	}
	
	/**
	 * Public accessor function for users to call to delete a specific value from the tree
	 * @param n Value that the user wants to delete
	 * Post: Calls the private delete function with the root pointer to delete the value given
	 */
	void del(T n) {
		del(n,root);
	}
	
	/**
	 * Public accessor function for users to call to delete the entire tree
	 * Post: calls the private delete function with the root pointer to delete the entire tree
	 */
	void del() {
		del(root);
	}
private:
	node* root;

	/**
	 * Private Function to traverse the list in order from least to greatest and call f at every node.
	 * @param f Function passed from main to be called at every node
	 * @param p Pointer to the node we are currently at.
	 */
	template<typename fn>
	void inorder(fn f, node* p) {
		if(!p) return;				// if the node does not exist, return
		inorder(f,p->left);			// call f at the left child
		f(p);						// call f at the current node
		inorder(f,p->right);		// call f at the right child
	}
	
	/**
	 * Function to traverse the list in preorder and call f at every node.
	 * @param f Function passed from main
	 * @param p Pointer to the node that we are currently looking at
	 */
	template<typename fn>
	void preorder(fn f, node* p) {
		if(!p) return;				// if the node does not exist, return
		f(p);						// call f at the current node
		preorder(f,p->left);		// call f at the left child
		preorder(f,p->right);		// call f at the right child
	}
	
	/**
	 * Traverse the list in post order and call f at every node
	 * @param f Function that is passed from main
	 * @param p pointer to the current node we are interested in
	 */
	template<typename fn>
	void postorder(fn f, node* p) {
		if(!p) return;				// if the node does not exist, return
		postorder(f,p->left);		// call f at the left child
		postorder(f,p->right);		// call f at the right child
		f(p);						// call f at the current node
	}
	
	/**
	 * Private insert function to handle insertion into our tree
	 * @param n Value we are trying to insert.
	 * @param p Pointer to the node we are interested in.
	 */
	void ins(T n, node* &p) {
		// if there is no node, create a new node with the given value
		if (!p){
			p = new node(n);
			return;
		}
		// if the value given is less than the current value, insert to the left
		if(n<p->data){
			ins(n,p->left);
			return;
		}
		// if the value given is greater than the current value, insert to the right
		if(n>p->data){
			ins(n,p->right);
			return;
		}
		// if the value given is the current value, return because we don't want duplicates
		if(n==p->data) return;
	}
	
	/**
	 * Private delete function to handle single node deletion.
	 * @param n	Value we are trying to delete
	 * @param p Pointer of the node we are looking at.
	 */
	void del(T n, node* &p) {
		// if the node does not exist, return
		if(!p) return;
		// if the value given is less than the current value, check the left side of the node
		if(n<p->data){
			del(n,p->left);
			return;
		}
		// if the value given is greater than the current value, check the right side of the node.
		if(n>p->data){
			del(n,p->right);
			return;
		}
		// else the value must be equal or not exist
		node* prev, *temp=p;
		// if the value given is the current value
		if(n==p->data){
			// If the right child does not exist, replace the current node with the left child and delete the found node
			if(!p->right){
				temp = p;
				p = p->left;
				delete temp;
				return;
			}
			// If the left child does not exist, replace the current node with the right child and delete the found node
			else if (!p->left){
				temp = p;
				p = p->right;
				delete temp;
				return;
			}
			// else both children exist so we replace the current value with the maximum value from the left side of the tree
			else {
				temp = p->left;
				prev = p;
				while (temp->right!=0){
					prev = temp;
					temp = temp->right;
				}
			}
			// replace the current value with the max value from the left
			p->data = temp->data;
			// if there was only one child, we need to handle that carefully
			if(prev==p) prev->left = temp->left;
			// otherwise we can just reassign pointers
			else prev->right = temp ->left;
		}
		// and delete the temporary node holder
		delete temp;
	}

	/**
	 * Private delete all function to handle the behind the scenes tree deletion
	 * @param p pointer to the root node of the tree.
	 */
	void del(node* &p) {
		if(!p) return;			// if the node does not exist, return
		del(p->left);			// delete the nodes to the left
		del(p->right);			// delete the nodes to the right
		delete p;				// delete the current node
		p=0;					// set the pointer to null so that we don't have a dangling pointer
		return;
	}
};
