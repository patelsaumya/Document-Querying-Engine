#include "fsm.h"
#include <string.h>

Symbol::Symbol(const char* name, int (*testAddress)(char)) {
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	this->testAddress = testAddress;
}

Symbol::~Symbol() {
	delete[] name;
}


int Symbol::isMatch(const char *name) {
	if (strcmp(this->name, name) == 0) {
		return 1;
	}
	return 0;
}

int Symbol::test(char c) {
	if(testAddress(c)) {
		return 1;
	}
	return 0;
}
