//============================================================================
// Name        : p07.cpp
// Author      : Dr. Ted Shaneyfelt
// Description : Test interface for graphing assignment
//============================================================================

#include "p07.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;

int main() {

    graph<string> bigisle;
    bigisle.ins("Puna", "South Hilo");
    bigisle.ins("Puna", "Kau");
    bigisle.ins("South Hilo", "North Hilo");
    bigisle.ins("South Hilo", "Kau");
    bigisle.ins("North Hilo", "Hamakua");
    bigisle.ins("North Hilo", "Kau");
    bigisle.ins("Hamakua", "North Kohala");
    bigisle.ins("Hamakua", "South Kohala");
    bigisle.ins("Hamakua", "North Kona");
    bigisle.ins("Hamakua", "Kau");
    bigisle.ins("North Kohala", "South Kohala");
    bigisle.ins("South Kohala", "North Kona");
    bigisle.ins("North Kona", "Kau");
    bigisle.ins("South Kona", "Kau");

    solution<string> soln(bigisle);
    bigisle.traverse_vertices([&](string name){cout<<name<<" "<<soln.color(name)<<endl;});

    const char *colors[]={"black","red","blue","darkgreen","brown","orange","pink","purple","lightgrey","darkgray","tan","coral","yellow","cyan","green","magenta"};

    graph<int> g;
    ostringstream out;
    g.ins(0,20);
    for (int i=3;i<=20;i++) {
        g.ins(i-3,i-1);
        g.ins(i/3,i);
    }
    out << "xdot <<END" << endl;
    out << "graph {" << endl;
    solution<int> result(g);
    g.traverse_edges([&](int a,int b){if (a<b) out<<'<'<<a<<">--<"<<b<<'>'<<endl;});
    g.traverse_vertices([&](int name){out<<'<'<<name<<">[color="<<colors[result.color(name)&0xF]<<']'<<endl;});
    out << "}" << endl;
    out << "END" << endl;
    system(out.str().c_str());
    return 0;
}
