/**
 * p02.h
 *
 *  Created on: Jan 25, 2016
 *      Author: Matthew Brown cs321x1
 */


template<typename T=int>
/**
 * Data structure for a linked list.
 * Uses a node struct to handle the data storage.
 * Has public accessor functions ins, del, inorder to handle user input
 * and private accessor functions to deal with the behind-the-scenes stuff
 */
class list {
public:
	/**
	 * Stores the data of the linked list in nodes.
	 */
	struct node {
		T data;
		node *next;
		/**
		 * Default constructor for node.
		 * Post: sets data to 0 and next to a nullptr.
		 */
		node() : data(0), next(nullptr){}
		/**
		 * Overloaded constructor for node.
		 * @param data data the user wants to insert
		 * @param next pointer to the next node, defaults to nullptr if not passed.
		 */
		node(T data, node* next = nullptr) : data(data), next(next){}
	};
	/**
	 * Default constructor for list class.
	 * Post: Initializes root to a nullptr.
	 */
	list() : root(nullptr){}

	template<typename fn>
	/**
	 * Goes through the linked list and calls f for each node.
	 * @param f Function that is passed from main.
	 */
	void inorder(fn f) {
		inorder(f, root);
	}

	/**
	 * Public insert function for the user.
	 * @param n Data (default int) that the user wants to input into the list.
	 * Post: calls the private ins function.
	 */
	void ins( T n ) {
		ins(n, root);
    }

	/**
	 * Public delete function for the user.
	 * @param n Data point that the user wants to delete.
	 * Post: calls the private delete function.
	 */
    void del( T n ) {
	    del(n, root);
    }

    /**
     * Public delete-all function for the user.
     * Post: Calls the private delete-all function.
     */
    void del() {
    	del(root);
    	root = nullptr;
    }

private:
	node *root;		// node* root for our linked list to start at

	/**
	 * Private delete function for linked list management.
	 * @param n Data that the user wants to insert into the list.
	 * @param p Pointer that accesses the current node.
	 * Post: Recursively calls itself until the data is inserted in the proper place or found.
	 */
	void ins(T n, node* &p) {
		if(!p){
			p = new node(n);
			return;
		}
		if(n<p->data){
			p = new node(n,p);
			return;
		}
		if(n==p->data){
		    return;
	    }
	  	if(n>p->data) ins(n, p->next);
	}

	/**
	 * Private delete function to remove a data point from the list.
	 * @param n Data point that the user wants to delete.
	 * @param p Pointer to the node being accessed.
	 * Post: Recursively calls itself until the node that contains the data is found and deleted
	 * or exhausts the list.
	 */
	void del( T n, node* &p) {
		if (!p) return;
		if (n<p->data) return;
		if (n==p->data){
			if (!p->next){
				delete p;
				p=0;
				return;
			}
			else {
				node* temp = p;
				p = p->next;
				delete temp;
				return;
			}
		}
		if (n>p->data) {
			del(n, p->next);
		}
	}

	/**
	 * private delete all function that deletes the entire list.
	 * @param p pointer to access the current node.
	 * Post: recursively calls itself until the tree is deleted.
	 */
	void del(node* &p){
		if(!p) return;
		del(p->next);
		delete p;
		p=0;
		return;
	}

	template<typename fn>
	/**
	 * Private inorder function to call f for each node in the list.
	 * @param f Function passed from main.
	 * @param p Accessor for the node we are currently at.
	 * Post: Calls f for every node in the list.
	 */
	void inorder(fn f, node* p){
		if (!p) return;
		f(p);
		inorder(f, p->next);
		return;
	}
};
