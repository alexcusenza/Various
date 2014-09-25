/*
 * modclass02.cpp
 *
 *  Created on: Mar 3, 2014
 *      Author: acusenza
 */


/*
 * sample class
 */

#include <iostream>
using namespace std;

struct Items
{
	int qty;
	Items(int qty): qty(qty) {}
};

class Basket
{
	Items InBasket;

public:
	Basket(int inval): InBasket(inval) {}

	void GetStuff() {
		cout << endl << "Inbasket = " << InBasket.qty << endl;
	}
};


int main()
{
	Basket B1(5);
	B1.GetStuff();
	return 0;
}

