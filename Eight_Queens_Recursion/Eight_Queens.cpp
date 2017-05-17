//============================================================================
// Name        : Eight_Queens.cpp
// Author      : Dr. Ted Shaneyfelt
// Version     : 16.01
// Copyright   : 2016
// Description : Pretty-printer for Eight Queens
//============================================================================
#include "Eight_Queens.h"
#include <iostream>
using namespace std;

//#define USE_ASCII
#ifdef USE_ASCII
/**
 * use American characters to draw the board for the 8-queens board #n.
 * The first 8 bits of n correspond to the top row, with the msb on the left side.
 * The solution number is printed in decimal next to the board.
 */
static void print_board(unsigned long long int n) {
	cout << endl << "+-----------------+" << endl << "| ";
	for (unsigned long long int bit = 1ULL << 63; bit; bit >>= 1) {
		if (bit & n) cout << "Q";
		else if (bit & 0x55AA55AA55AA55AAULL) cout << "#";
		else cout << "=";
		if (bit & 0x101010101010100ULL) cout << " |" << endl << "| ";
		else cout << " ";
	}
	cout << "|  #" << n << endl << "+-----------------+";
}
#else
/**
 * Use international characters to draw the board for the 8-queens board #n.
 * The first 8 bits of n correspond to the top row, with the msb on the left side.
 * The solution number is printed in decimal next to the board.
 */
static void print_board(unsigned long long int n) {
	wcout << endl << L"╭─────────────────╮" << endl << L"│ ";
	for (unsigned long long int bit = 1ULL << 63; bit; bit >>= 1) {
		wcout << (bit & n ? L"♛" : L" ");
		if (bit & 0x55AA55AA55AA55AAULL) wcout << L"\u20DE"; // box around previous char
		if (bit & 0x101010101010100ULL) wcout << L" │" << endl << L"│ ";
		else wcout << L" ";
	}
	wcout << L"│  №" << n << endl << L"╰─────────────────╯";
	wcout.flush();
}
#endif

/**
 * Print board#n as a solution with the count of solutions so far
 */
static void print(unsigned long long int n) {
	static int count=0;
	print_board(n);
    wcout << "  Solution " << ++count;
}

/**
 * Print all 92 solutions of the Eight Queens problem
 */
int main() {
#ifndef USE_ASCII
	setlocale(LC_ALL, "en_US.UTF-8"); // enable wide characters
#endif
	p03::queens(print);
	return 0;
}
