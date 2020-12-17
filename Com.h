#define _SCL_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <string>

#define CONSTANT_TIMEOUT 1000
#define BUFFER_SIZE 20

using namespace std;

class Com {

public:
	HANDLE COMport;
	DCB dcb;
	Com();
	void init_port(const string& port, int baud);
	void disconnect();
	void write(LPCVOID data, int datasize);
	void read(LPVOID data, int bufsize);

};
