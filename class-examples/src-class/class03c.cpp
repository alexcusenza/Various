/*
 * class03c.cpp
 *
 *  Created on: Feb 28, 2014
 *      Author: acusenza
 */

#include <iostream>
using namespace std;

/*
 *  CLASS
 *  Using Child Protected Members
 */

/*
 * Base Class
 */
class Box
{
protected:
	double width;
};

/*
 * Derived Class
 */
class SmallBox:Box // SmallBox is the derived class.
{
public:
	void setSmallWidth( double wid );
	double getSmallWidth( void );
};

// Member functions of child class
double SmallBox::getSmallWidth(void)
{
	return width ;
}

void SmallBox::setSmallWidth( double wid )
{
	width = wid;
}

// Main function for the program
int main( )
{
	cout << "class03c.cpp : Class Protected/Child Members" << endl;

	SmallBox box;

	// set box width using member function
	box.setSmallWidth(5.0);
	cout << "Width of box : "<< box.getSmallWidth() << endl;

	return 0;
}


