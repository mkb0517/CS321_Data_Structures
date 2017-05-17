/**
 * B-Tree.h
 * @author Matthew Brown
 */

/**
 * tree class to contain the information of how to store data in our B-tree.
 */
template<typename T = int>
class tree {
public:
    /**
     * node struct to deal with how nodes interact with each other.
     */
    struct node {
        enum {
            LEAF1 = 0x10, INTERIOR12 = 0x12, LEAF2 = 0x20, INTERIOR23 = 0x23
        } type;
        T data[2];
        node* child[3];
        /**
         * overloaded constructor for leaf insertion with only a value.
         * @param value value we want to insert
         */
        node(T value): type(LEAF1), data{value,0}, child{nullptr, nullptr, nullptr}{}

        /**
         * overloaded constructor for interior node insertion.
         * @param value value we want to insert
         * @param left pointer to the left child
         * @param right pointer to the right child
         */
        node(T value, node* left, node* right): type(INTERIOR12), data{value,0}, child{left, right, nullptr}{}
    }*root = nullptr;

    /**
     * preorder function that handles tree traversal.
     * @param f function passed to be called at each node.
     * @param p pointer to a node to use f at.
	 * post: calls f at each node in the b-tree in preorder
     */
    template<typename fn>
    void preorder(fn f, const node* const &p) {
        if (!p) return;
        f(p);
        for (int childcount = p->type&0xF, i = 0; i<childcount; i++) {
            preorder(f, p->child[i]); // preorder and postorder visitors don't need the index parameter.
        }
    }

    /**
     * Public preorder traversal function for the B-tree.
     * Behaves according to the type associated with a node.
     * If only one value at node, only checks that value. Otherwise it checks both before moving on.
     * @param f function passed to be called at each node.
	 * post calls the private preorder function with the root pointer
     */
    template<typename fn>
    void preorder(fn f) {
        preorder(f, root);
    }

    /**
     * Inorder function to handle inorder traversal of our tree.
     * @param f function be be called at each node in order.
     * @param p pointer to the node where we want to call f.
     */
    template<typename fn>
    void inorder(fn f, const node* const &p) {
        if (!p) return;
        if(!(p->type&0xF)){
        	if(p->type==p->LEAF1){
        		f(p,0);
        	}
        	else{
        		for(int i=0;i<2;i++){
        			f(p, i);
        		}
        	}
        }
        else{
        	inorder(f, p->child[0]);
        	for (int childcount = p->type&0xF, i = 1; i<childcount; i++) {
        		f(p, i-1); // the extra parameter for the visitor function f for in-order traversal is index
        		inorder(f, p->child[i]);
        	}
        }
    }
    /**
     * inorder function to be called by the user.
     * @param f function to be called at each node in order.
     */
    template<typename fn>
    void inorder(fn f) {
        inorder(f, root);
    }    /**
     * Function to handle postorder traversal of the tree.
     * @param f function to be called at each node of the tree.
     * @param p pointer to the node we want to call f at.
     */
    template<typename fn>
    void postorder(fn f, node* &p) {
        if (!p) return;
        for (int childcount = p->type&0xF, i = 0; i<childcount; i++) {
            postorder(f, p->child[i]); // preorder and postorder visitors don't need the index parameter.
        }
        f(p);
    }
    /**
     * Postorder function for the user to call.
     * @param f Function that should be called at each node in the tree.
     */
    template<typename fn>
    void postorder(fn f) {
    	postorder(f, root);
    }
    /**
     * Public function to insert into 2-3 tree. Handles root insertions.
     * @param value Value we are trying to insert.
     */
    void ins(T &value) {
    	node* temp = ins(value, root);
    	if(!temp){
    		return;
    	}
    	else if (root->type==root->LEAF1){
    		root = new node(value, temp, root);
    		return;
    	}
    	else if (value<root->data[0]){
    		root = new node(value, temp, root);
    		return;
    	}
    	else if (value>root->data[0]){
    		root = new node(value, root, temp);
    		return;
    	}
    }
    /**
     * Public delete function to call private delete function and handle root cases.
     */
    void del(T &value) {
    	del(value, root);
    }

    /**
     * Function to delete the entire B-tree.
     * Uses the postorder function to traverse the tree and delete each node
     * and set them all to nullptr.
     */
    void del() {
    	/**
    	 * Lambda function to delete all nodes in postorder.
    	 */
    	postorder([](node* &p){delete p;});
		root = nullptr;
		return;
    }
private:
    /**
     * Function to delete the minimum value
     */
    node* delmin(T &value, node* &p, node* &current){
    	node *returndel=0, *returner=0;
    	if(current->type!=current->LEAF1&&current->type!=current->LEAF2)
    		returndel = delmin(value, p, current->child[0]);
    	else if(p->data[0]==value){
    		if(current->type==current->LEAF1){
    			value = current->data[0];
    			returner = del(value, p->child[1]);
    			p->data[0] = value;
    			return returner;
    		}
    		else if (current->type==current->LEAF2){
    			p->data[0] = current->data[0];
    			current->data[0] = current->data[1];
    			current->type = current->LEAF1;
    			return current;
    		}
    	}
    	else if (p->data[1]==value){
    		if(current->type==current->LEAF1){
    			value = current->data[0];
    			p->data[1] = current->data[0];
    			returner = del(current->data[0], p->child[2]);
    			return returner;
    		}
    		else if (current->type==current->LEAF2){
    			p->data[1] = current->data[0];
    			current->data[0] = current->data[1];
    			current->data[1] = 0;
    			current->type = current->LEAF1;
    			return current;
    		}
    	}
  		return returndel;
    }
    /**
     * Private insert function to handle behind the scenes stuff.
     * @param value Value we are trying to insert.
     * @param p Pointer to node we are checking.
     * @return pointer to a node we created. Returns 0 if value is absorbed or duplicate.
     */
    node* ins(T &value, node* &p){
    	if(!p){
    		p = new node(value);
    		return 0;
    	}
    	else if(p->type==p->LEAF1){
    		if(value==p->data[0]) return 0;
    		else if(value<p->data[0]){
    			p->data[1]=p->data[0];
    			p->data[0]=value;
    		}
    		else { // value > p->data[0]
    			p->data[1]=value;
    		}
    		p->type=p->LEAF2;
    		return 0;
    	}
    	else if(p->type==p->INTERIOR12){
    		node* temp = 0;
    		if (value == p->data[0]) return 0;
    		else if (value<p->data[0]) {
    			temp = ins(value, p->child[0]);
    			if (temp){
					p->data[1] = p->data[0];
					p->data[0] = value;
					p->child[2] = p->child[1];
					p->child[1] = p->child[0];
					p->child[0] = temp;
    			}
    			else return 0;
    		}
    		else { //value>p->data[0]
    			temp = ins(value, p->child[1]);
    			if(temp){
					p->data[1] = value;
					p->child[2] = p->child[1];
					p->child[1] = temp;
    			}
    			else return 0;
    		}
			p->type = p->INTERIOR23;
    		return 0;
    	}
    	else if(p->type==p->INTERIOR23){
    		node* temp=0;
    		if (value==p->data[0]||value==p->data[1]) return 0;
    		else if (value<p->data[0]){
    			temp = ins(value, p->child[0]);
    			if(temp){
    				 temp = new node(value,temp,p->child[0]);
    				 value = p->data[0];
    				 p->data[0] = p->data[1];
    				 p->data[1] = 0;
    				 p->child[0] = p->child[1];
    				 p->child[1] = p->child[2];
    				 p->child[2] = 0;
    			}
    			else return 0;
    		}
    		else if (value>p->data[1]){
    			temp = ins(value, p->child[2]);
    			if (temp){
    				node* temp2 = new node(value, temp, p->child[2]);
    				value = p->data[1];
    				p->data[1] = 0;
    				p->child[2] = 0;
    				p->type = p->INTERIOR12;
    				temp = p;
    				p = temp2;
    				return temp;
    			}
    			else return 0;
    		}
    		else {
    			temp = ins(value,p->child[1]);
    			if (temp) {
    				temp = new node(p->data[0], p->child[0], temp);
    				p->data[0] = p->data[1];
    				p->data[1] = 0;
    				p->child[0] = p->child[1];
    				p->child[1] = p->child[2];
    				p->child[2] = 0;
    			}
    			else return 0;
    		}
    		p->type = p->INTERIOR12;
    		return temp;
    	}
    	else if(p->type==p->LEAF2){
    		node* temp=0;
    		if(value<p->data[0]) {
    			temp = new node(value);
    			value = p->data[0];
    			p->data[0] = p->data[1];
    			p->data[1]=0;
    		}
    		else if(value>p->data[1]) {
    			temp = new node(p->data[0]);
    			p->data[0] = value;
    			value = p->data[1];
    			p->data[1] = 0;
    		}
    		else {
    			temp = new node(p->data[0]);
    			p->data[0] = p->data[1];
    			p->data[1] = 0;
    		}
    		p->type = p->LEAF1;
    		return temp;
    	}
    	else return 0;
    }

    /**
     * Private delete function to handle behind the scenes stuff.
     * @param value value we want to delete
     * @param p pointer to node we're looking at
     * @return 0 if imbalance, else p.
     */
    node* del(T &value, node* &p){
    	if(!p) return 0;
    	else if (p->type==p->LEAF1){
    		if (value == p->data[0]){
    			delete p;
    			p = nullptr;
				return 0;
    		}
    		else return p;
    	}
    	else if (p->type==p->LEAF2){
    		if (value==p->data[0]){
    			p->data[0] = p->data[1];
    			p->data[1] = 0;
    		}
    		else if (value==p->data[1]){
    			p->data[1] = 0;
    		}
    		else return p;
    		p->type=p->LEAF1;
    		return p;
    	}
    	else if (p->type==p->INTERIOR12){
    		node* temp=0;
    		if(value<p->data[0]){
    			temp = del(value, p->child[0]);
    			if(!temp){
    				if(p->child[1]->type==p->INTERIOR23){
    					p->child[0]->child[0] = new node(p->child[0]->data[0]);
    					p->child[0]->data[0] = p->child[0]->data[1];
    					p->child[0]->data[1] = 0;
    					p->child[0]->type = p->LEAF1;
    					p->child[0] = new node(p->data[0], p->child[0]->child[0], p->child[0]);
    					p->data[0] = p->child[1]->data[0];
    					p->child[1]->data[0] = p->child[1]->data[1];
    					p->child[1]->data[1] = 0;
    					p->child[1]->child[0]->data[1] = p->child[1]->child[1]->data[0];
    					p->child[1]->child[1]->type = p->LEAF2;
    					delete p->child[1]->child[1];
    					p->child[1]->child[1] = p->child[1]->child[2];
    					p->child[1]->child[2] = 0;
    					p->child[1]->type = p->INTERIOR12;
    					return p;
    				}
    				else if(p->child[1]->type==p->INTERIOR12){
    					temp = p->child[1];
    					p->data[1]=p->child[1]->data[0];
    					p->child[2]=p->child[1]->child[1];
    					p->child[1]=p->child[1]->child[0];
    					p->type=p->INTERIOR23;
    					delete temp;
    					temp=0;
    					return 0;
    				}
    				else if(p->child[1]->type==p->LEAF1){
    					p->data[1]=p->child[1]->data[0];
    					p->type = p->LEAF2;
    					delete p->child[1];
    					p->child[1]=0;
    					return 0;
    				}
    				else if(p->child[1]->type==p->LEAF2){
    					p->child[0]= new node(p->data[0]);
    					p->data[0] = p->child[1]->data[0];
    					p->child[1]->data[0] = p->child[1]->data[1];
    					p->child[1]->type=p->LEAF1;
    					return p;
    				}
				}
    			return p;
    		}
    		else if(value>p->data[0]){
    			temp = del(value, p->child[1]);
    			if(!temp){
    				if(p->child[0]->type==p->INTERIOR23){
    					p->child[1]->child[0] = new node(p->child[0]->child[2]->data[0]);
    					p->child[1]->child[0]->data[1] = p->child[1]->data[0];
    					p->child[1]->child[0]->type = p->LEAF2;
    					p->child[1]->data[0] = p->child[1]->data[1];
    					p->child[1]->data[1]=0;
    					p->child[1]->type = p->LEAF1;
    					p->child[1] = new node(p->data[0], p->child[1]->child[0], p->child[1]);
    					p->data[0] = p->child[0]->data[1];
    					p->child[0]->data[1] = 0;
    					delete p->child[0]->child[2];
    					p->child[0]->child[2] = 0;
    					p->child[0]->type = p->INTERIOR12;
    					return p;
    				}
    				else if(p->child[0]->type==p->INTERIOR12){
    					temp = p->child[0];
    					p->data[1]=p->data[0];
    					p->data[0]=p->child[0]->data[0];
    					p->child[2]=p->child[1];
    					p->child[1]=p->child[0]->child[1];
    					p->child[0]=p->child[0]->child[0];
    					p->type=p->INTERIOR23;
    					delete temp;
    					temp=0;
    					return 0;
    				}
    				else if(p->child[0]->type==p->LEAF1){
    					p->data[1]=p->data[0];
    					p->data[0]=p->child[0]->data[0];
    					p->type = p->LEAF2;
    					delete p->child[0];
    					p->child[0] = 0;
    					return 0;
    				}
    				else if(p->child[0]->type==p->LEAF2){
    					p->child[1] = new node(p->data[0]);
    					p->data[0] = p->child[0]->data[1];
    					p->child[0]->data[1] = 0;
    					p->child[0]->type = p->LEAF1;
    					return p;
    				}
    			}
    			return p;
    		}
    		else if(value == p->data[0]){
    			temp = delmin(value, p, p->child[1]);
    			if(!temp){
    				if(p->child[0]->type==p->INTERIOR23){
    					p->child[1] = new node(p->data[0], p->child[0]->child[2], p->child[1]);
    					p->data[0] = p->child[0]->data[1];
    					p->child[0]->data[1] = 0;
    					p->child[0]->child[2] = 0;
    					p->child[0]->type = p->INTERIOR12;
    					return p;
    				}
    				else if(p->child[0]->type==p->INTERIOR12){
    					temp = p->child[0];
    					p->data[1]=p->data[0];
    					p->data[0]=p->child[0]->data[0];
    					p->child[2]=p->child[1];
    					p->child[1]=p->child[0]->child[1];
    					p->child[0]=p->child[0]->child[0];
    					p->type=p->INTERIOR23;
    					delete temp;
    					temp=0;
    					return 0;
    				}
    				else if(p->child[0]->type==p->LEAF2){
    					p->child[1] = new node(p->data[0]);
    					p->data[0] = p->child[0]->data[1];
    					p->child[0]->data[1] = 0;
    					p->child[0]->type = p->LEAF1;
    					return p;
    				}
    				else if (p->child[0]->type==p->LEAF1){
    					p->data[1]=p->data[0];
    					p->data[0]=p->child[0]->data[0];
    					p->type = p->LEAF2;
    					delete p->child[0];
    					p->child[0] = 0;
    					return 0;
    				}
    			}
    			return p;
    		}
    		else return p;
    		return p;
    	}
    	else if (p->type==p->INTERIOR23){
    		node* temp=0;
    		if (value==p->data[0]){
    			temp = delmin(value, p,p->child[1]);
    			if(!temp){
    				if(p->child[0]->type == p->INTERIOR12){
    					p->child[0]->data[1] = p->data[0];
    					p->child[0]->child[2] = p->child[1];
    					p->child[0]->type=p->INTERIOR23;
    					p->data[0] = p->data[1];
    					p->data[1]=0;
    					p->child[1] = p->child[2];
    					p->child[2] = 0;
    					p->type=p->INTERIOR12;
    					return p;
    				}
    				else if(p->child[0]->type == p->INTERIOR23){
    					p->child[1] = new node(p->data[0], p->child[0]->child[2], p->child[1]);
    					p->data[0] = p->child[0]->data[1];
    					p->child[0]->data[1]=0;
    					p->child[0]->child[2] = 0;
    					p->child[0]->type = p->INTERIOR12;
    					return p;
    				}
    				else if(p->child[0]->type == p->LEAF1){
    					p->child[0]->data[1] = p->data[0];
    					p->child[0]->type = p->LEAF2;
    					p->data[0] = p->data[1];
    					p->child[1] = p->child[2];
    					p->child[2] = 0;
    					p->type = p->INTERIOR12;
    					return p;
    				}
    				else if(p->child[0]->type == p->LEAF2){
    					p->child[1] = new node(p->data[0]);
    					p->data[0] = p->child[0]->data[1];
    					p->child[0]->type = p->LEAF1;
    					return p;
    				}
    			}
    			return p;
    		}
    		else if (value==p->data[1]){
    			temp = delmin(value, p,p->child[2]);
    			if(p->child[1]->type == p->INTERIOR12){
    				p->child[1]->data[1] = p->data[1];
    				p->child[1]->child[2]=p->child[2];
    				p->child[2] = 0;
    				p->type=p->INTERIOR12;
    				p->child[1]->type=p->INTERIOR23;
    				return p;
    			}
    			else if(p->child[1]->type == p->INTERIOR23){
    				p->child[2] = new node(p->data[1], p->child[1]->child[2], p->child[2]);
    				p->data[1] = p->child[1]->data[1];
    				p->child[1]->data[1] = 0;
    				p->child[1]->child[2] = 0;
    				p->child[1]->type = p->INTERIOR12;
    				return p;
    			}
    			else if(p->child[1]->type == p->LEAF1){
    				p->child[1]->data[1] = p->data[1];
    				p->data[1] = 0;
    				p->child[1]->type = p->LEAF2;
    				p->type = p->INTERIOR12;
    				return p;
    			}
    			else if(p->child[1]->type == p->LEAF2){
    				p->child[2] = new node(p->data[1]);
    				p->data[1] = p->child[1]->data[1];
    				p->child[1]->data[1] = 0;
    				p->child[1]->type = p->LEAF1;
    				return p;
    			}
    			return p;
    		}
    		else if(value<p->data[0]){
    			temp = del(value,p->child[0]);
    			if(!temp){
    				if(p->child[1]->type == p->INTERIOR12){
    					p->child[1]->data[1] = p->child[1]->data[0];
    					p->child[1]->data[0] = p->data[0];
    					p->data[0] = p->data[1];
    					p->child[1]->child[2]=p->child[1]->child[1];
    					p->child[1]->child[1]=p->child[1]->child[0];
    					p->child[1]->child[0]=p->child[0];
    					p->child[0] = p->child[1];
    					p->child[1] = p->child[2];
    					p->child[2] = 0;
    					p->type=p->INTERIOR12;
    					p->child[0]->type=p->INTERIOR23;
    					return p;
    				}
    				else if(p->child[1]->type == p->INTERIOR23){
    					p->child[0] = new node(p->data[0], p->child[0], p->child[1]->child[0]);
    					p->data[0] = p->child[1]->data[0];
    					p->child[1]->data[0] = p->child[1]->data[1];
    					p->child[1]->child[0] = p->child[1]->child[1];
    					p->child[1]->child[1] = p->child[1]->child[2];
    					p->child[1]->child[2] = 0;
    					p->child[1]->type = p->INTERIOR12;
    					return p;
    				}
    				else if(p->child[1]->type == p->LEAF1){
    					p->child[1]->data[1] = p->child[1]->data[0];
    					p->child[1]->data[0] = p->data[0];
    					p->data[0] = p->data[1];
    					p->child[0] = p->child[1];
    					p->child[1] = p->child[2];
    					p->child[2] = 0;
    					p->child[0]->type = p->LEAF2;
    					p->type = p->INTERIOR12;
    					return p;
    				}
    				else if(p->child[1]->type == p->LEAF2){
    					p->child[0] = new node(p->data[0]);
    					p->data[0] = p->child[1]->data[0];
    					p->child[1]->type = p->LEAF1;
    					return p;
    				}
    			}
    			return p;
    		}
    		else if(value>p->data[1]){
    			temp = del(value, p->child[2]);
    			if(!temp){
    				if(p->child[1]->type == p->INTERIOR12){
    					p->child[1]->data[1] = p->data[1];
    					p->child[1]->child[2]=p->child[2];
    					p->child[2] = 0;
    					p->type=p->INTERIOR12;
    					p->child[1]->type=p->INTERIOR23;
    					return 0;
    				}
    				else if(p->child[1]->type == p->INTERIOR23){
    					p->child[2] = new node(p->data[1], p->child[1]->child[2], p->child[2]);
    					p->data[1] = p->child[1]->data[1];
    					p->child[1]->data[1] = 0;
    					p->child[1]->child[2] = 0;
    					p->child[1]->type = p->INTERIOR12;
    					return p;
    				}
    				else if(p->child[1]->type == p->LEAF1){
    					p->child[1]->data[1] = p->data[1];
    					p->data[1] = 0;
    					p->child[1]->type = p->LEAF2;
    					p->type = p->INTERIOR12;
    					return 0;
    				}
    				else if(p->child[1]->type == p->LEAF2){
    					p->child[2] = new node(p->data[1]);
    					p->data[1] = p->child[1]->data[1];
    					p->child[1]->data[1] = 0;
    					p->child[1]->type = p->LEAF1;
    					return p;
    				}
    			}
    			return 0;
    		}
    		else{ // data[0] < value < data[1]
    			temp = del(value, p->child[1]);
    			if(!temp){
    				if(p->child[0]->type==p->INTERIOR12){
    					p->child[0]->data[1] = p->data[0];
    					p->child[0]->child[2] = p->child[1];
    					p->child[0]->type = p->INTERIOR23;
    					p->data[0] = p->data[1];
    					p->child[1] = p->child[2];
    					p->type = p->INTERIOR12;
    					return p;
    				}
    				else if(p->child[0]->type==p->INTERIOR23){
    					p->child[1] = new node(p->data[0],p->child[0]->child[2], p->child[1]);
    					p->data[0] = p->child[0]->data[1];
    					p->child[0]->data[1] = 0;
    					p->child[0]->child[2] = 0;
    					p->child[0]->type = p->INTERIOR12;
    					return p;
    				}
    				else if(p->child[0]->type==p->LEAF1){
    					p->child[0]->data[1] = p->data[0];
    					p->data[0] = p->data[1];
    					p->data[1] = 0;
    					p->child[0]->type = p->LEAF2;
    					p->child[1] = p->child[2];
    					p->child[2] = 0;
    					p->type = p->INTERIOR12;
    					return p;
    				}
    				else if(p->child[0]->type==p->LEAF2){
    					p->child[1] = new node(p->data[0]);
    					p->data[0] = p->child[0]->data[1];
    					p->child[0]->data[1] = 0;
    					p->child[0]->type = p->LEAF1;
    					return p;
    				}
    			}
    			return p;
    		}
    		return p;
    	}
    	return p;
    }
};
