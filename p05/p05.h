/**
 * p05.h
 *
 *      @author Matthew Brown
 */

template<typename T = int>
/**
 * AVL Tree class for the storing and balancing of data in a binary tree.
 */
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
	template<typename fn>
	/**
	 * public accessor function for inorder function.
	 * @param f function passed from main
	 */
	void inorder(fn f) const {
		inorder(f,root);
	}
	template<typename fn>
	/**
	 * public accessor function for preorder function.
	 * @param f function passed from main
	 */
	void preorder(fn f) const {
		preorder(f,root);
	}
	template<typename fn>
	/**
	 * public accessor function for post order function.
	 * @param f function passed from main.
	 */
	void postorder(fn f) const {
		postorder(f,root);
	}
	/**
	 * public accessor function for insertion.
	 * @param n value we want to insert
	 */
	void ins(T n) {
		ins(n,root);
	}
	/**
	 * public accessor function for deletion.
	 * @param n value we want to delete
	 */
	void del(T n) {
		del(n,root);
	}
	/**
	 * public accessor function to delete the entire tree.
	 */
	void del() {
		del(root);
	}
private:
	node* root;
	template<typename fn>
	/**
	 * Private inorder function to handle node*.
	 * @param f function passed from main.
	 * @param p node* to traverse the tree.
	 */
	void inorder(fn f, node* p) const{
		if(!p) return;
		inorder(f,p->left);
		f(p);
		inorder(f,p->right);
	}
	template<typename fn>
	/**
	 * Private preorder function to handle node pointers behind the scenes
	 * @param f function passed from main
	 * @param p node pointer to traverse the tree
	 */
	void preorder(fn f, node* p) const{
		if(!p) return;
		f(p);
		preorder(f,p->left);
		preorder(f,p->right);
	}
	template<typename fn>
	/**
	 * private post order function to traverse the tree
	 * @param f function passed from main
	 * @param p node* to traverse the tree in post order
	 */
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
	 * @param p pointer to the node we want rotate at p
	 */
	void rotateLeft(node* &p){
		node* q = p;
		p = q->right;
		q->right = p->left;
		p->left = q;
	}

	/**
	 * Function to delete by copying.
	 */
	signed delMin(node* &p, T &data){
		if(!p->left){
			node* temp = p;
			data=p->data;
			p = p->right;
			delete temp;
			return -1;
		}
		else {
			return growth(delMin(p->left, data),p,0);
		}
	}
	/**
	 * function to keep track of the growth of the tree and call rebalance if necessary
	 */
	signed growth(signed leftGrowth, node *&p, signed rightGrowth) {
			int L=0, R=0;
			if(p->tag>0) L=-p->tag;
			else R=p->tag;
			L+=leftGrowth;
			R+=rightGrowth;
			p->tag=R-L;
			signed increase = R>L?R:L;
			return increase+rebalance(p);
		}

	/**
	 * Rebalances the tree based on the tags of the nodes that are passed to it.
	 */
	signed rebalance(node* &p) {
		switch(p->tag) {
		case -2:
			switch(p->left->tag) {
			case -1:
				rotateRight(p);
				p->right->tag=p->tag=0;
					return -1;
				case 0:
					rotateRight(p);
					p->tag=1;
					p->right->tag=-1;
					return 0;
				case 1:
					rotateLeft(p->left);
					rotateRight(p);
				switch(p->tag) {
				case -1: //0,1
					p->left->tag=0;
					p->right->tag=1;
					break;
				case 0: //0,0
					p->left->tag=0;
					p->right->tag=0;
					break;
				case 1://-1,0
					p->left->tag=0;
					p->right->tag=-1;
					break;
				}
				break;
			}
			break;
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
	 * returns the amount of growth (how much taller)
	 */
	signed ins(T n, node* &p){
		if(!p){
			p = new node(n);
			return 1;
		}
		if(n>p->data) return growth(0,p,ins(n, p->right));
		if(n<p->data) return growth(ins(n, p->left),p,0);
		return 0;
	}
	/**
	 * Private delete function to handle single node deletion.
	 * @param n	Value we are trying to delete
	 * @param p Pointer of the node we are looking at.
	 */
	signed del(T n, node* &p) {
		if(!p) return 0;
		if(n<p->data){
			return growth(del(n,p->left),p,0);
		}
		if(n>p->data){
			return growth (0,p,del(n,p->right));
		}
		node *temp=p;
		if(n==p->data){
			if(!p->right){
				temp = p;
				p = p->left;
				delete temp;
				return -1;
			}
			else {
				return growth(0,p,delMin(p->right,p->data));
			}
		}
	}
	/**
	 * private function to delete the entire tree
	 * @param p pointer to a node in the tree
	 */
	void del(node* &p){
		if(!p) return;
		del(p->left);
		del(p->right);
		delete p;
		p=0;
		return;
	}
};
