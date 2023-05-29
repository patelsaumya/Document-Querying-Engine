#include "Fsm.h"
#include <string.h>

State::State(const char* name, int initialState, int acceptingState, int discardingState) {
	this->initialState = initialState;
	this->acceptingState = acceptingState;
	this->discardingState = discardingState;
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
}

State::~State() {
	delete[] name;
}

int State::isAcceptingState() {
	return this->acceptingState;
}

int State::isInitialState() {
	return this->initialState;
}

int State::isDiscardingState() {
	return this->discardingState;
}

int State::isMatch(const char *name) {
	if (strcmp(this->name, name) == 0) {
		return 1;
	}
	return 0;
}