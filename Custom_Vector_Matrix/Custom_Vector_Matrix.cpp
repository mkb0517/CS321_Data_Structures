/**===========================================================================
// Name        : Custom_Vector_Matrix.cpp
// Author      : Matthew Brown
// Version     : 321.01
// Description : Starter code for project 1 for CS 321: Data Structures
// Requirements: Requires GTK and gcc to run
//==========================================================================*/

#include "Custom_Vector_Matrix.h"
#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <iostream>
using namespace std;

class MyArea: public Gtk::DrawingArea {
	double dx = 0, dy = 0, dz = 0; // rate of rotation
	double sensitivity = .01; // sensitivity of mouse for rotation
	mat<double> t;
	int r = -0, g = 0, b = 10;
public:
	MyArea() {
		Glib::signal_timeout().connect(
				sigc::mem_fun(*this, &MyArea::on_timeout), 20);
	}
protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
		for (int i = 0; i < 6; i++) { // examine all pairs of points to make lines between them
			for (int j = 0; j < i; j++) {
				if (i == j + 3)
					continue; // skip opposite points. They are spaced 3 apart.
				vec<double> p1 = t * corners[i], p2 = t * corners[j]; // endpoints
				cr->move_to(p1[0], p1[1]);
				cr->line_to(p2[0], p2[1]);
				cr->set_line_cap(Cairo::LINE_CAP_ROUND );
				cr->set_line_width((p1[2] + p2[2] + 75) / 25);
				cr->stroke();
			}
		}
		return true;
	}
	bool on_scroll_event(GdkEventScroll*event) {
		dz = (double(event->direction) - .5) * .1;
		return false;
	}
	bool on_motion_notify_event(GdkEventMotion*event) {
		static int mx0 = 0, my0 = 0;  // last known mouse position
		int mdx = event->x - mx0, mdy = event->y - my0; // calculate mouse motion rate
		mx0 = event->x, my0 = event->y; // update last known mouse position
		if (mdx > -10 && mdx < 10)
			dx = sensitivity * mdx;
		if (mdy > -10 && mdy < 10)
			dy = sensitivity * mdy;
		queue_draw();
		return true;
	}
	bool on_timeout() {
		mat<double> rotdx = {     // motion due to horizontal mouse movement
				cos(dx), 0, sin(dx), 0,          //
						0, 1, 0, 0,              //
						-sin(dx), 0, cos(dx), 0, //
						0, 0, 0, 1 };
		mat<double> rotdy = {    // motion due to vertical mouse movement
				1, 0, 0, 0,                      //
						0, cos(dy), sin(dy), 0,  //
						0, -sin(dy), cos(dy), 0, //
						0, 0, 0, 1 };
		mat<double> rotdz = {    // motion due to mouse wheel movement
				cos(dz), -sin(dz), 0, 0,         //
						sin(dz), cos(dz), 0, 0,  //
						0, 0, 1, 0,              //
						0, 0, 0, 1 };

		dx *= .99;        // deceleration
		dy *= .99;
		dz *= .95;

		rot = rotdz * rotdy * rotdx * rot; // update orientation with change in rotation
		t = translate * scale * rot; // combine rotation scaling and translating into position

		queue_draw();
		return true;
	}
private:
	mat<double> translate = { 1, 0, 0, 100,   // move right 100
			0, 1, 0, 100,   // move down  100
			0, 0, 1, 0, // no backward movement
			0, 0, 0, 1 };
	mat<double> scale = { 50, 0, 0, 0,    // stretch 50x wide
			0, 50, 0, 0,    // stretch 50x tall
			0, 0, 50, 0,    // stretch 50x deep
			0, 0, 0, 1 };
	mat<double> rot;
	vec<double> corners[6] = { { 0, 0, 1, 1 },    // front corner point
			{ 0, 1, 0, 1 },    // bottom corner point
			{ 1, 0, 0, 1 },    // right corner point
			// opposite points are spaced 3 apart
			{ 0, 0, -1, 1 },    // back corner point
			{ 0, -1, 0, 1 },    // top corner point
			{ -1, 0, 0, 1 },    // left corner point
			};

};

int main(int argc, char *argv[]) {
	auto app = Gtk::Application::create(argc, argv);
	Gtk::Window win;
	MyArea area;
	area.add_events(
			Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK
					| Gdk::BUTTON_MOTION_MASK | Gdk::SCROLL_MASK);
	win.add(area);
	area.show();

	// Test cases for my program to demonstrate proper implementation
	mat<> one(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16); // initialize a matrix with overloaded constructor
	mat<> two(4); 	// initialize a matrix with overloaded constructor that only takes 1 argument
	mat<> three = one - two;	// testing subtraction operator.
	mat<> four = one;	// testing assignment operator (Rule of Five)
	four -= two;		// testing overloaded subtraction assignment operator for matrices
	mat<> five = one + two;		// testing overloaded addition operator for matricies
	mat<> six = two;
	six += one;			// testing overloaded addition assignment operator for matrices
	mat<> seven = transpose(one); 	// testing matrix transpose function.
	mat<> eight = six*one; 		// testing overloaded matrix multiplication operator.
	mat<> nine = six;
	nine *= one;	// testing overloaded matrix multiplication assignment operator. should equal eight
	mat<> ten(nine);	// testing overloaded copy constructor (rule of five) should equal nine and eight
	vec<> eleven(1,2,3,4); // testing vector constructor
	eleven*=4;				// testing vector multiplication assignment operator
	vec<> twelve(2,2,2,2);
	vec<> thirteen = twelve + eleven; //testing overloaded vector addition operator
	vec<> fourteen(twelve);		// testing vector copy constructor
	fourteen += eleven;			// testing overloaded vector addition assignment operator. Should equal thirteen
	vec<> fifteen = eleven - twelve;	// testing overloaded vector subtraction operator.
	vec<> sixteen = eleven;		// testing overloaded vector assignment operator. (Rule of Five)
	sixteen -= twelve;			// testing overloaded vector subtraction assignment operator. Should equal fifteen.

	cout << one << endl << two << endl << three << endl << four << endl << five << endl << six << endl
			<< seven << endl << eight << endl << nine << endl << ten << endl << eleven << endl << twelve
			<< endl << thirteen << endl << fourteen << endl << fifteen << endl << sixteen;

	return app->run(win);
}
