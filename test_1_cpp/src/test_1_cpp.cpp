#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
	int i = 0;
	while(1)
	{
		cout << "momentics ide test_1_cpp 11111: " << i << endl;
		i++;
		usleep(500000);
	}
	return 0;
}
