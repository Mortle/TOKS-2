#define _SCL_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

#define BYTE_7D 0x7D
#define BYTE_7E 0x7E
#define BYTE_5D 0x5D
#define BYTE_5E 0x5E

#define BUFFER_SIZE 20


using namespace std;

class ByteStaffing {

public:
	void encode(char*, int&);
	void decode(char*, int);
	void showBytes(char*, int);
};
