#include <iostream>
#include <string>
using namespace std;

class Foo{public: int val;Foo *pnext;};

int main()
{
	Foo bar;
    int* a;
	cout << bar.val << endl;
    cout << bar.pnext << endl;
    cout << a << endl;
   return 0;
}
