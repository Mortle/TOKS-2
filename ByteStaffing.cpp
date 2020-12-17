#include "ByteStaffing.h"

void ByteStaffing::encode(char* message, int& size) {

	char result[BUFFER_SIZE*2];
	result[0] = BYTE_7E;
	int newSize = 1;
	int j = 1;
	for (int i = 0; i < size; i++) {
		if (message[i] == BYTE_7E) {
			result[j] = BYTE_7D;
			result[j+1] = BYTE_5E;
			j += 2;
			newSize+=2;
		}
		else if (message[i] == BYTE_7D) {
			result[j] = BYTE_7D;
			result[j + 1] = BYTE_5D;
			j += 2;
			newSize += 2;
		}
		else {
			result[j] = message[i];
			j++;
			newSize += 1;
		}
	}
	for (int i = 0; i < newSize; i++) {
		message[i] = result[i];
	}
	size = newSize;
}

void ByteStaffing::decode(char* message, int size) {
	char result[BUFFER_SIZE];
	int newSize = 0;
	int j = 0;
	if (message[0] == BYTE_7E) {
		for (int i = 1; i < size; i++) {
			if (message[i] == BYTE_7D) {
				if (message[i + 1] == BYTE_5E) {
					result[j] = BYTE_7E;
					j++;
					i++;
					newSize++;
				}
				else if (message[i + 1] == BYTE_5D) {
					result[j] = BYTE_7D;
					j++;
					i++;
					newSize++;
				}
			}
			else {
				result[j] = message[i];
				j++;
				newSize++;
			}
		}
		for (int i = 0; i < size; i++) {
			message[i] = result[i];
		}
		size = newSize;
	}
	
}
void ByteStaffing::showBytes(char* message, int size) {
	for (int i = 0; i < size; i++) {
		printf("%x ", message[i]);
	}
}