//============================================================================
// Name        : B-Tree.cpp
// Author      : Ted Shaneyfelt
// Version     : 2016a
// Copyright   : Permission granted to use for course project
// Description : Command interface for project p06 B trees
//============================================================================

#include "B-Tree.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <future>
#include <vector>
#include <iomanip>

using namespace std;

/**
 * Usage:
 * traversal(data).digraph();    generates the digraph text in dot language of the data
 * traversal(data).inorder();    generates the inorder  traversal listing of data to stdout
 * traversal(data).preorder();   generates the preorder  traversal listing of data to stdout
 * traversal(data).postorder();  generates the postorder  traversal listing of data to stdout
 */
class traversal {
    const char *prefix =
            "digraph { /*splines=false;*/ bgcolor=transparent; sametail=true; node[shape=none;label=\"?\";fontcolor=red];edge[color=gray];"
            "\n<ROOT>[shape=underline,label=<ROOT>,color=darkkhaki,style=filled,fillcolor=khaki,fontcolor=saddlebrown,height=.4,penwidth=12]";
    const char *nodestyle = "fillcolor=beige,fontcolor=black,style=filled,color=tan,shape=record";

    const char *suffix = "}";
    tree<> &data;
    ostringstream ss;
    int count;
    /**
     * Replace long ugly addresses with short concise instance numbering text.
     * Find the first @0xXXXXXXXX in the text that matches the pattern,
     * replace all instances of that substring with "#1"
     * repeat for #2, ...
     */
    string compact(string text) {
        for (int n = 1;; n++) {
            int at = text.find('@');
            if (at<0) break; // -1 if not found
            string x = text.substr(at, 11); // text to replace
            //cout << "{"<<x<<"}";

            string y = "#";
            y += to_string(n);
            int i;
            for (;;) {
                i = text.find(x);
                if (i<0) break;
                text.replace(i, 11, y);
            }
        }
        return text;
    }
    /**
     * text representation of a node's id as its data, at-sign @, and memory address. <all in angle-brackets>
     */
    string id(const tree<>::node* const &p, string suffix = "", int n = 0) {
        static int nullcount = 0;
        ostringstream ss;
        if (!p) {
            ss<<"<nuLLll"<< ++nullcount<<">";
            return ss.str();
        }
        ss<<"<";
        ss<<p->data[0];
        for (int datasize = p->type>>4, i = 1; i<datasize; i++) {
            ss<<","<<p->data[i];
        }
        ss<<'@'<<setw(10)<<p;
        if (!suffix.empty()) ss<<suffix<<n;
        ss<<">";
        return ss.str();
    }
    /**
     * return graphviz code for the attributes of a node in the form
     * nodename[attribute=value][attribute=value]
     * such as
     * 10@0x1d2e3a4d5b6e7e8f[label=10]
     */
    string node_attributes(const tree<>::node* const &p) {
        int datacount = (p->type>>4);
        ostringstream ss;
        ss<<id(p)<<"[label=\"{";
        if (p->type!=tree<>::node::LEAF1 and p->type!=tree<>::node::LEAF2
            and p->type!=tree<>::node::INTERIOR12 and p->type!=tree<>::node::INTERIOR23) {
            ss<<showbase<<hex<<p->type;
            if (datacount<1 || datacount>2) datacount = 2;
        }
        ss<<"{";
        ss<<p->data[0];
        for (int i = 1; i<datacount; i++) {
            ss<<'|'<<p->data[i];
        }
        ss<<"}";
        ss<<"}\"]";
        ss<<"["<<nodestyle<<"]";
        return ss.str();
    }
    /**
     * return graphviz code for the attributes of a null pointer in the tree such as
     * 10@0x1d2e3a4d5b6e7e8fRnull[label=< >][shape=none]
     */
    string null_attributes(const tree<>::node* const &p, string suffix = "", int i = 0) {
        ostringstream ss;
        ss<<id(p, suffix, i)<<"[label=< >]" "[shape=none]";
        return ss.str();
    }
    /**
     * visitor callback function to generate dot language digraph text of a node of a binary tree
     */
    void digraph_step(const tree<>::node* const &p) {
        if (!p) {
            if (count==0) ss<<"<ROOT>-><ROOTnull>[arrowhead=odot];<ROOTnull>[label=< >;shape=none]";
            return;
        }
        if (count==0) {
            ss<<"<ROOT>->"<<id(p)<<'\n';
        }
        count++;
        int childcount = p->type&0xF;
        //if (p->child[0]) ss <<"  "<<id(p)<<":sw->"<<id(p->child[0])<<":n";
        for (int i = 0; i<childcount; i++) {
            string dir = i==0 ? ":sw" : i<childcount-1 ? ":s" : ":se";
            if (p->child[i]) ss<<"  "<<id(p)<<dir<<"->"<<id(p->child[i])<<":n"<<endl;
            else {
                ss<<"  "<<id(p)<<dir<<"->"<<id(p, "null.", i)<<":n";
                ss<<"[arrowhead=odot]";
                ss<<';'<<null_attributes(p, "null.", i)<<endl;
            }

        }
        ss<<"  "<<node_attributes(p)<<endl;
    }
    /**
     * visitor callback function to print a numerical value as text, 16 values per line
     */
    void text_step(const tree<>::node* const &p, int index) {
        cout<<(count++ &0xF ? ' ' : '\n')<<p->data[index]<<flush;
    }
    void text_step(const tree<>::node* const &p) {
        for (int datacount = p->type>>4, i = 0; i<datacount; i++) {
            cout<<(count++ &0xF ? ' ' : '\n')<<p->data[i];
        }
        cout<<flush;
    }
public:
    static vector<future<void>> futures;
    traversal(tree<> &data) :
            data(data), count(0) {
    }
    /**
     * Traverse the data structure to return the digraph in dot language as a string
     */
    string digraph() {
        ss.str("");
        ss<<prefix<<endl;
        data.preorder([&](const tree<>::node* const&p) {this->digraph_step(p);});
        ss<<suffix<<endl;
        return compact(ss.str());
    }
    /**
     * Traverse the data structure to print the contents 16 items per row
     */
    void inorder() {
        count = 0;
        data.inorder([&](const tree<>::node* const&p,int index) {if (p) this->text_step(p,index);});
        cout<<endl;
    }
    /**
     * Traverse the data structure to print the contents 16 items per row
     */
    void preorder() {
        count = 0;
        data.preorder([&](const tree<>::node* const&p) {if (p) this->text_step(p);});
        cout<<endl;
    }
    /**
     * Traverse the data structure to print the contents 16 items per row
     */
    void postorder() {
        count = 0;
        data.postorder([&](const tree<>::node* const&p) {if (p) this->text_step(p);});
        cout<<endl;
    }
    /**
     * Return the string that can be used to launch xdot from the system prompt.
     */
    string systemcmd() {
        ostringstream cmd;
        cmd<<"xdot <<END\n"<<digraph()<<"\nEND\n";
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
        string s = systemcmd();
        futures.push_back(async(launch::async, [&,s]() {system(s.c_str());}));
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
 *  =			list the data in-order
 *  <			list the data pre-order
 *  >			list the data post-order
 *  .			quit (same as q or end of input)
 */
int main() {
    {
        tree<> data;
        char cmd;
        int n;
        cout<<"/*B*/\t";
        while (cin>>cmd) {
            cmd = tolower(cmd); // ignore case
            switch (cmd) {
            case '!':
                //data.unbalanced = !data.unbalanced; // uncomment to allow disabling of balancing for debugging
                break;
            case 'z': // delete all records
                data.del();
                break;
            case 'i': // insert a number
                cin>>n;
                data.ins(n);
                break;
            case 'd': // delete a number
                cin>>n;
                data.del(n);
                break;
            case 'w': // Run xdot to show the graph in a window using inorder() and wait until the window is closed
                traversal(data).window();
                break;
            case 'x': // Run xdot to show the graph in a window using inorder(), but do not wait for the window to close
                traversal(data).windows();
                break;
            case 'g':
                cout<<traversal(data).digraph();
                break;
            case 's':
                cout<<traversal(data).systemcmd();
                break;
            case '=': // list the data using inorder traversal
                traversal(data).inorder();
                break;
            case '<': // list the data using preorder traversal
                traversal(data).preorder();
                break;
            case '>': // list the data using postorder traversal
                traversal(data).postorder();
                break;
            case '?':
                cout<<"  z      delete all\n"
                    "  i 10    insert #10\n"
                    "  d 10    delete #10\n"
                    "  w       show the tree in a window and wait for it to close\n"
                    "  x       show the tree in a window and don't wait for it to close\n"
                    "  g       show the graphviz program that is used for drawing the data structure\n"
                    "  s       show the system command that is used for drawing the data structure\n"
                    "  =       list the data in order\n"
                    "  .       quit (same as q or end of input)"
                    <<endl;
                break;
            case '.':
            case 'q': // quit
                return 0;
            }
            cout<<"/*B*/\t";
        }
    }
    return 0;
}
