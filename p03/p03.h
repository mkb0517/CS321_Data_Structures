/**
 * p03.h
 *
 *      Author: Matthew Brown
 */

/**
 * Class to deal with solving the 8 queens problem.
 * Uses a recursive function to solve for all 92 solutions.
 */
class p03 {
public:
	unsigned long long int n;
	/**
	 * Default constructor for p03 class to set the unsigned long long int to 0.
	 */
	p03() : n(0ULL){}
	/**
	 * Checks to see if the current position we are interested in is a valid spot to place a queen
	 * @param row Row where we want to place a queen
	 * @param col Column where we want to place a queen
	 * @return True if it is a valid spot to place a queen, false if there is a conflict.
	 */
	bool checkQueen(int row, int col){
		for (int i=0; i<row; i++){
			unsigned long long int bitD = 0ULL;
			unsigned long long int bitL = 0ULL;
			unsigned long long int bitR = 0ULL;
			bitD = 1ULL<<(8*i+col);
			if(!((col+row-i)>7)) bitL = 1ULL<<(8*i+col+(row-i));
			if(!((col-row+i)<0)) bitR = 1ULL<<(8*i+col-(row-i));
			if (n&bitD) return false;
			if (n&bitL) return false;
			if (n&bitR) return false;
		}
		return true;
	}
	/**
	 * Toggles the bit we are interested in to place or remove a queen.
	 * @param row Row where the bit we are interested in is located
	 * @param col Column where the bit we are interested in is located
	 */
	void setQueen(int row, int col){
		unsigned long long int bit = 0ULL;
		bit = 1ULL<<((8*row)+col);
		n = n ^ bit;
	}
	/**
	 * Recursively solves the 8 queens problem
	 * @param f function passed from main to print out solutions
	 * @param row the current row that the algorithm is analyzing
	 */
	void putQueen(void (*f)(unsigned long long int), int row){
		for (int col=0; col<8; col++){
			if(checkQueen(row, col)){
				setQueen(row,col);
				if (row<7){
				putQueen(f, row+1);
				}
				else {
					f(n);
				}
				setQueen(row,col);
			}
		}
	}

	/**
	 * creates a p03 object to call the putQueen function and recursively solve the 8 queens problem.
	 * Pre: Calling object to pass it a function function.
	 * Post: Prints out the 92 solutions to the 8 Queens problem.
	 */
	static void queens(void (*f)(unsigned long long int)) {
		p03 board;
		int row=0;
		//const static unsigned long long int n = 0x0820800240010410ULL; // a sample solution: board 585608698710262800
		board.putQueen(f,row);
	}
};
