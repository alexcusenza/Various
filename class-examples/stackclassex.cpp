/*
 * stackclassex.cpp
 *
 *  Created on: Mar 5, 2014
 *      Author: acusenza
 */
using namespace std;
#include <iostream>

/*
 * CLASS HEADER
 * ------------
 */
template <class T>

class Stack
{
public:
	Stack(int = 10);
	~Stack() {
		delete [] stackPtr;
	}

	int push(const T&);

	int pop(T&);

	int isEmpty() const {
		return top == -1;
	}

	int isFull() const {
		return top == size - 1;
	}
private:
	int size;
	int top;
	T *stackPtr;

};


/*
 * CLASS DEFINTION
 * ---------------
 */

// Constructor
template <class T>
Stack<T>::Stack(int s)
{
	size = s;
	top = -1;
	stackPtr = new T[size];
}


// Push Element onto Stack
template <class T>
int Stack<T>::push(const T &item)
{
	if (!isFull())
	{
		stackPtr[++top] = item;
		return 1;
	}
	return 0;
}


// Pop element off of stack
template  <class T>
int Stack<T>::pop(T &popValue)
{
	if (!isEmpty())
	{
		popValue = stackPtr[top--];
		return 1;
	}
	return 0;

}




/*
 * MAIN PROGRAM
 * ------------
 */

int main(void)
{
	Stack<float> floatStack(5);
	float f = 1.1;

	cout << "pushing elements onto floatStack" << endl;

	while (floatStack.push(f))
	{
		cout << f << ' ';
		f += 1.1;
	}

	cout << "Stack is full, cannot push" << endl
		 << "Popping elements from floatStack" << endl;

	while (floatStack.pop(f))
	{
		cout << f << ' ';
	}

	cout << "Stack is empty, cannot pop" << endl;

}
















