/**
 * p07 Graph_Coloring
 * Project to demonstrate graph coloring in C++
 * @author Matthew Brown
 */

template<typename T>
/**
 * graph class to store graph data.
 */
class graph {
public:
	/**
	 * struct to set up our linked list
	 */
	struct node{
		T data;
		int degree;
		int nodecolor;
		node *next, *list;
		/**
		 * Default constructor for node
		 */
		node() : data(0), degree(0), nodecolor(-2), next(nullptr), list(nullptr){}
		/**
		 * overloaded constructor for node insertion at the end of a list.
		 */
		node(T data) : data(data), degree(0), nodecolor(-2), next(nullptr), list(nullptr) {}
		/**
		 * Overloaded constructor for node insertion in the middle of the list
		 */
		node(T data, node* list): data(data), degree(0), nodecolor(-2), next(nullptr), list(list) {}
	} *root = nullptr, *current = nullptr;
	int vert;
	int totaldegree;
	int* colored;
	/**
	 * default graph constructor
	 */
	graph() : vert(0), totaldegree(0), colored(nullptr){}
	/**
	 * Sort function to sort the vertices by degree
	 */
	void sort(node* &p){
		if(!p) return;
		else(sort(p->next));
		if(!p->next) return;
		else{
			if(p->degree<p->next->degree){
				node* temp = p;
				p = p->next;
				temp->next = p->next;
				p->next = temp;
				return;
			}
		}
		return;
	}
	/**
	 * Sorts list by alphabetical order
	 */
	void sort2(node* &p){
		bool swap = false;
		node* head = p;
		do{
			swap = false;
			while(p->next){
				if(p->data>p->next->data){
					T temp = p->data;
					p->data = p->next->data;
					p->next->data = temp;
					swap = true;
				}
				p = p->next;
			}
			p = head;
		}while(swap);
	}
	/**
	 * caller function for sort2
	 */
	void sort2(){
		sort2(root);
	}
	/**
	 * public insert function
	 */
    void ins(const T a, const T b) {
    	ins(root,a,b);
    	ins(root,b,a);
    	sort(root);
    	return;
    }
    template<typename fn>
    /**
     * function to traverse the edges of a graph
     */
    void traverseEdges(fn f) {
        if (!root) return;
        node* p = root;
        node* trav = p->list;
        while(p){
        	trav = p->list;
        	while(trav){
        		f(p->data, trav->data);
        		trav = trav->list;
        	}
        	p = p->next;
        }
	}
    template<typename fn>
    /**
     * function to traverse the vertices of a graph
     * handles deletion of the graphs
     */
    void traverseVertices(fn f) {
    	sort2();
    	node* p = root;
    	while (p) {
    		f(p->data);
    		p = p->next;
    	}
    	p = root;
    	while(p){
    		node* temp = p->next;
    		node* listing = p->list;
    		while(listing){
    			node* doomed = listing->list;
    			delete listing;
    			listing = doomed;
    		}
    		listing = nullptr;
    		delete p;
    		p = temp;
    	}
    	p=nullptr;
    	return;
    }
    /**
     * function to solve the coloring problem.
     */
    void solver(node *p){
    	if (!p) return;
    	T* adjacent = new T[p->degree];
    	int *used = new int[p->degree];
    	int color=0;
    	node* trav = p->list;
    	// initialize the adjacency and used colors list
    	for(int i=0; i<p->degree; i++){
    		adjacent[i] = trav->data;
    		used[i] = trav->nodecolor;
    		trav = trav->list;
    	}
    	// Go back to the top of the list
    	trav = root;
    	// Go through the graph and populate the colors
    	while(trav){
    		for (int i=0; i<p->degree; i++)
    			if (adjacent[i]==trav->data){
    				used[i]=trav->nodecolor;
    				break;
    			}
    		trav = trav->next;
    	}
    	/*
    	 * Sort the used colors from lowest to highest
    	 */
    	bool swap = false;
    	do{
    		swap = false;
    		for (int i=0; i<p->degree-1; i++){
    			if(used[i]>used[i+1]){
    				int temp = used[i];
    				used[i] = used[i+1];
    				used[i+1] = temp;
    				swap = true;
    			}
    		}
    	}while(swap);
    	/*
    	 * check the adjacent colors and increment if they are the same
    	 */
    	for(int i=0; i<p->degree; i++){
    		if(used[i]==color) color++;
    	}
    	p->nodecolor = color;
    	solver(p->next);
    	delete[] adjacent;
    	delete[] used;
    }
private:
    /**
     * private insert function to handle pointers.
     */
    void ins(node* &p, T a, T b){
    	if(!p){
    		p = new node(a);
    		p->list = new node(b);
    		p->degree++;
    		totaldegree++;
    		vert++;
    		return;
    	}
    	else if(a==p->data){
    		if(inslist(p->list,b)){
    			p->degree++;
    			totaldegree++;
    			return;
    		}
    		else return;
    	}
    	else ins(p->next, a, b);
    	return;
    }
    /**
     * list insertion for adjacency list
     */
    bool inslist(node* &p, T b){
    	if(!p){
    		p = new node(b);
    		return true;
    	}
    	else if (b<p->data){
    		p = new node(b, p);
    		return true;
    	}
    	else if (b==p->data) return false;
    	else return inslist(p->list, b);
    }
};

template<typename T>
/**
 * Solution class to handle graph coloring
 */
class solution : public graph<T> {
public:
	int* colors;
	T *data;
	int vert;
	/**
	 * solution constructor
	 */
    solution(graph<T> g) {
    	colors = new int[g.vert];
    	data = new T[g.vert];
    	vert = g.vert;
    	g.current = g.root;
    	g.solver(g.current);
    	for(int i=0; i<vert; i++){
    		data[i] = g.current->data;
    		colors[i] = g.current->nodecolor;
    		g.current = g.current->next;
    	}
    }
    /**
     * destructor for solution to handle the dynamic arrays
     */
    ~solution(){
    	delete[] colors;
    	delete[] data;
    }
    /**
     * returns the color index of a node.
     */
    unsigned color(const T a) const  {
    	int index = 0;
    	for(int i=0; i<vert; i++){
    		if(data[i] == a) index = i;
    	}
    	return colors[index]; }
};
