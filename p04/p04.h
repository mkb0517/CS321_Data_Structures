/**
 * p05.h
 *
 *      @author Matthew Brown
 */

template<typename T = int>
/**
 * tree class to define our tree data structure
 */
class tree {
public:
	/**
	 * node struct to handle the tree data points.
	 */
	struct node {
		T data;
		node *left, *right;   // you may add a constructor and destructor, etc.
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
	template<typename fn>
	/**
	 * Public accessor function for main to use to call the tree in order
	 */
	void inorder(fn f) {
		inorder(f,root);
	}
	template<typename fn>
	/**
	 * Public accessor function for main to use to call the tree in preorder
	 * @param f
	 */
	void preorder(fn f) {
		preorder(f,root);
	}
	template<typename fn>
	/**
	 * Public accessor function for main to use to call the tree in postorder
	 * @param f
	 */
	void postorder(fn f) {
		postorder(f,root);
	}
	/**
	 * Public accessor function for users to call to insert a specific value into the tree
	 * @param n Value that the user wants to insert
	 */
	void ins(T n) {
		ins(n,root);
	}
	/**
	 * Public accessor function for users to call to delete a specific value from the tree
	 * @param n Value that the user wants to delete
	 */
	void del(T n) {
		del(n,root);
	}
	/**
	 * Public accessor function for users to call to delete the entire tree
	 */
	void del() {
		del(root);
	}
	// you may add a constructor and destructor, a root, etc.
private:
	node* root;
	template<typename fn>
	/**
	 * Private Function to traverse the list in order from least to greatest.
	 * @param f Function passed from main
	 * @param p Pointer to the node we are currently at.
	 */
	void inorder(fn f, node* p) {
		if(!p) return;
		inorder(f,p->left);
		f(p);
		inorder(f,p->right);
	}
	template<typename fn>
	/**
	 * Function to traverse the list before we order it.
	 * @param f Function passed from main
	 * @param p Pointer to the node that we are currently looking at
	 */
	void preorder(fn f, node* p) {
		if(!p) return;
		f(p);
		preorder(f,p->left);
		preorder(f,p->right);
	}
	template<typename fn>
	/**
	 * Traverse the list from greatest to least
	 * @param f Function that is passed from main
	 * @param p pointer to the current node we are interested in
	 */
	void postorder(fn f, node* p) {
		if(!p) return;
		postorder(f,p->right);
		postorder(f,p->left);
		f(p);
	}
	/**
	 * Private insert function to handle insertion into our tree
	 * @param n Value we are trying to insert.
	 * @param p Pointer to the node we are interested in.
	 */
	void ins(T n, node* &p) {
		if (!p){
			p = new node(n);
			return;
		}
		if(n<p->data){
			ins(n,p->left);
			return;
		}
		if(n>p->data){
			ins(n,p->right);
			return;
		}
		if(n==p->data) return;
	}
	/**
	 * Private delete function to handle single node deletion.
	 * @param n	Value we are trying to delete
	 * @param p Pointer of the node we are looking at.
	 */
	void del(T n, node* &p) {
		if(!p) return;
		if(n<p->data){
			del(n,p->left);
			return;
		}
		if(n>p->data){
			del(n,p->right);
			return;
		}
		node* prev, *temp=p;
		if(n==p->data){
			if(!p->right){
				temp = p;
				p = p->left;
				delete temp;
				return;
			}
			else if (!p->left){
				temp = p;
				p = p->right;
				delete temp;
				return;
			}
			else {
				temp = p->left;
				prev = p;
				while (temp->right!=0){
					prev = temp;
					temp = temp->right;
				}
			}
			p->data = temp->data;
			if(prev==p) prev->left = temp->left;
			else prev->right = temp ->left;
		}
		delete temp;
	}
	/**
	 * Private delete all function to handle the behind the scenes tree deletion
	 * @param p pointer to the root node of the tree.
	 */
	void del(node* &p) {
		if(!p) return;
		del(p->left);
		del(p->right);
		delete p;
		p=0;
		return;
	}
};
