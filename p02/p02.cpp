//============================================================================
// Name        : p02.cpp
// Author      : Ted Shaneyfelt
// Version     : 2016a
// Copyright   : Permission granted to use for course project
// Description : Command interface for project p02 lists
//============================================================================

#include "p02.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <future>
#include <vector>

using namespace std;

class traversal {
	const char *prefix = "digraph { rankdir=LR; node[shape=none;label=\"?\";fontcolor=red];edge[color=gray];";
	const char *suffix = "}";
	list<> &data;
	ostringstream ss;
	int count;
	void digraph_step(const list<>::node* const&p) {
		if (!p) return ;
		if (!p->next) {
			ss << "  "
					"\"" << p->data<<"@"<<p << "\"->"
					"\"" << p->data<<"@"<<p << "null\""
					"[arrowhead=odot];"
					"\"" << p->data<<"@"<<p << "null\""
					"[label=\"\";shape=none];"
					"\"" << p->data<<"@"<<p << "\""
					"[label="<<p->data<<";""shape=oval;color=lightblue;fontcolor=black];"
					<<endl;
			return;
		}
		ss << "  "
				"\"" << p->data<<"@"<<p << "\"->"
				"\"" << p->next->data<<"@"<< p->next << "\";"
				"\"" << p->data<<"@"<<p << "\""
				"[label="<<p->data<<";shape=oval;color=lightblue;fontcolor=black];"
				<<endl;
	}
	void text_step(const list<>::node* const&p) {
		cout << (count++&0xF ? ' ': '\n')<< p->data << flush;
	}
public:
	static vector<future<void>> futures;
	traversal(list<> &data) : data(data),count(0) {
	}
	/**
	 * Traverse the data structure to return the digraph in dot language as a string
	 */
	string digraph() {
		ss.str("");
		ss << prefix << endl;
		data.inorder([&](const list<>::node* const&p){this->digraph_step(p);});
		ss << suffix << endl;
		return ss.str();
	}
	/**
	 * Traverse the data structure to print the contents 16 items per row
	 */
	void inorder() {
		count = 0;
		data.inorder([&](const list<>::node* const&p){if (p) this->text_step(p);});
		cout << endl;
	}
	/**
	 * Return the string that can be used to launch xdot from the system prompt.
	 */
	string systemcmd() {
		ostringstream cmd;
		cmd << "xdot <<END\n" << digraph() << "\nEND\n";
		return cmd.str();
	}
	/**
	 * Display the data structure graph in a modal window
	 */
	void window() {
		system(systemcmd().c_str());
	}
	/**
	 * Display the data structure graph in a non-modal window
	 */
	void windows() {
		string s=systemcmd();
		futures.push_back(async(launch::async, [&,s](){system(s.c_str());}));
	}
};
vector<future<void>> traversal::futures;

/**
 * Repeat prompt for input, get and execute a command.
 * Commands with integer parameter:
 *  z			delete all
 *  i 10		insert #10
 *  d 10		delete #10
 *  w			show the tree in a window and wait for it to close
 *  x			show the tree in a window and don't wait for it to close
 *  g			show the graphviz program that is used for drawing the data structure
 *  s			show the system command that is used for drawing the data structure
 *  =			list the data in order
 *  .			quit (same as q or end of input)
 */
int main() {
	{
		list<> data;
		char cmd;
		int n;
		cout << "/**/\t";
		while (cin >> cmd) {
			cmd = tolower(cmd); // ignore case
			switch (cmd) {
			case 'z': // delete all records
				data.del();
				break;
			case 'i': // insert a number
				cin >> n;
				data.ins(n);
				break;
			case 'd': // delete a number
				cin >> n;
				data.del(n);
				break;
			case 'w': // Run xdot to show the graph in a window using inorder() and wait until the window is closed
				traversal(data).window();
				break;
			case 'x': // Run xdot to show the graph in a window using inorder(), but do not wait for the window to close
				traversal(data).windows();
				break;
			case 'g':
				cout << traversal(data).digraph();
				break;
			case 's':
				cout << traversal(data).systemcmd();
				break;
			case '=': // list the data using inorder traversal
				traversal(data).inorder();
				break;
			case '?':
				cout <<
				 "  z      delete all\n"
				 "  i 10    insert #10\n"
				 "  d 10    delete #10\n"
				 "  w       show the tree in a window and wait for it to close\n"
				 "  x       show the tree in a window and don't wait for it to close\n"
				 "  g       show the graphviz program that is used for drawing the data structure\n"
				 "  s       show the system command that is used for drawing the data structure\n"
				 "  =       list the data in order\n"
				 "  .       quit (same as q or end of input)" << endl;
				break;
			case '.':
			case 'q': // quit
				return 0;
			}
			cout << "/**/\t";
		}
	}
	return 0;
}
