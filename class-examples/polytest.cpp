// dynamic allocation and polymorphism
#include <iostream>
#include <stdio.h>

using namespace std;

class Polygon
{
protected:
	int width, height;
public:
	Polygon (int a, int b) : width(a), height(b) {}
	~Polygon();

	virtual int area (void) =0;
	void printarea(){
		cout << this->area() << '\n';
	}
};

class Rectangle: public Polygon
{
public:
	Rectangle(int a,int b) : Polygon(a,b) {}
	int area(){
		return width*height;
	}
};

class Triangle: public Polygon
{
public:
	Triangle(int a,int b) : Polygon(a,b) {}
	int area(){
		return width*height/2;
	}
};

int main ()
{
	Polygon * ppoly1 = new Rectangle (4,5);
	Polygon * ppoly2 = new Triangle (4,5);
	ppoly1->printarea();

	printf("derived: ppoly1 %d\n",ppoly1->area());
	ppoly2->printarea();
	delete ppoly1;
	delete ppoly2;
	return 0;
}
