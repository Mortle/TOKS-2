#include "Com.cpp"
#include "ByteStaffing.cpp"

void Server(char* path);
void Client();

int main(int argc, char* argv[]){
	switch (argc){
	case 1:
		Server(argv[0]);
		break;
	default:
		Client();
		break;
	}
}

int enterBaud() {

	int baud;
	cout << "Enter baud: " << endl;
	cin >> baud;
	if (cin.peek() == '\n') {
		cin.ignore();
	}
	return baud;
}

void Server(char* path) {

	HANDLE semaphoreBaud = CreateSemaphore(NULL, 0, 1, "BAUD");
	string portName = "COM3";
	string writeString;
	char buffer[BUFFER_SIZE*2];

	cout << "Server start working (COM1)\n";
	
	Com com1 = Com();
	com1.init_port(portName, enterBaud());
	
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess(						
		(LPSTR)path,
		(LPSTR)" Client",
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi
	)) {
		CloseHandle(com1.COMport);
		cout << "Error during creating client process" << endl;
		return;
	}

	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = CONSTANT_TIMEOUT;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = CONSTANT_TIMEOUT;

	if (!SetCommTimeouts(com1.COMport, &timeouts)) {
		CloseHandle(com1.COMport);
		cout << "Error during setting timeouts" << endl;
		return;
	}
	
	WaitForSingleObject(semaphoreBaud, INFINITE);
	

	while (true){
		DWORD writtenBytes = 0;

		cout << "Enter string to pass: ";
		cin.clear();
		getline(cin, writeString);
		
		int portionNumber = writeString.size() / BUFFER_SIZE + 1;
																   

		com1.write(LPCVOID(&portionNumber), sizeof(portionNumber));

		int size = writeString.size();
		com1.write(LPCVOID(&size), sizeof(size));

		for (int i = 0; i < portionNumber; i++) {
			writeString.copy(buffer, BUFFER_SIZE, i * BUFFER_SIZE);
			ByteStaffing byteStaff = ByteStaffing();
			int sizeOfBytes;
			if (size > 20) {
				if (i == portionNumber - 1) {
					sizeOfBytes = size % BUFFER_SIZE;
				}
				else {
					sizeOfBytes = BUFFER_SIZE;
				}
			}
			else{
				sizeOfBytes = size;
			}
			cout << "Buffer before: ";
			byteStaff.showBytes(buffer, sizeOfBytes);
			cout << endl;
			byteStaff.encode(buffer, sizeOfBytes);
			cout << "Buffer after: ";
			byteStaff.showBytes(buffer, sizeOfBytes);
			cout << endl;
			com1.write(LPCVOID(&sizeOfBytes), sizeof(sizeOfBytes));
			com1.write(LPCVOID(buffer), sizeOfBytes);
			for (int i = 0; i < BUFFER_SIZE*2; i++) {
				buffer[i] = BUFFER_SIZE+i;
			}
		}	
	}

	com1.disconnect();
	CloseHandle(semaphoreBaud);

	return;
}



void Client()
{
	string portName = "COM4";
	string readString;
	char buffer[BUFFER_SIZE*2];
	HANDLE semaphoreBaud = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, "BAUD");

	cout << "Client start working(COM2)\n";
	
	Com com2 = Com();
	com2.init_port(portName, enterBaud());

	ReleaseSemaphore(semaphoreBaud, 1, NULL);
	
	while (true){
		DWORD readBytes;
		readString.clear();

		int portionNumber;
		com2.read(&portionNumber, sizeof(portionNumber));

		int size;
		com2.read(&size, sizeof(size));

		int sizeOfBytes;

		for (int i = 0; i < portionNumber; i++)
		{
			com2.read(&sizeOfBytes, sizeof(sizeOfBytes));
			com2.read(buffer, sizeOfBytes);
			ByteStaffing byteStaff = ByteStaffing();
			byteStaff.decode(buffer, sizeOfBytes);
			readString.append(buffer, BUFFER_SIZE);
		}

		readString.resize(size);

		cout << "COM2 get: " << endl;
		for (int i = 0; i < size; i++)
		{
			cout << readString[i];
		}
		cout << endl;

	}

	com2.disconnect();
	return;
}

