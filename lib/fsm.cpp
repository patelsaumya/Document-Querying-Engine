#include "fsm.h"
#include <string.h>

State *Fsm::getState(const char *name) {
	for(int i =0; i < stateCount; ++i) {
		if(states[i]->isMatch(name)) {
			return states[i];
		}
	}
	return NULL;
}
Symbol *Fsm::getSymbol(const char *name) {
	for(int i =0; i < symbolCount; ++i) {
		if(symbols[i]->isMatch(name)) {
			return symbols[i];
		}
	}
	return NULL;
}

State *Fsm::getNextState(State *fromState, Symbol *symbol) {
	State *result = NULL;
	for(int  i = 0; i < transitionCount; ++i) {
		result = transitions[i]->getToState(fromState, symbol);
		if(result != NULL) {
			break;
		}
	}
	return result;
}

State *Fsm::getInitialState() {
	for(int i =0; i < stateCount; ++i) {
		if(states[i]->isInitialState() == 1) {
			return states[i];
		}
	}
	return NULL;
}

Symbol *Fsm::getSymbol(char c) {
	for(int i = 0; i < symbolCount; ++i) {
		if(symbols[i]->test(c) == 1) {
			return symbols[i];
		}
	}
	
	return NULL;
}

Fsm::Fsm() {
	this->stateCount = 0;
	this->symbolCount = 0;
	this->transitionCount = 0;
}

Fsm::~Fsm() {
	for(int i = 0; i < transitionCount; ++i) {
		delete transitions[i];
	}
	for(int i = 0; i < stateCount; ++i) {
		delete states[i];
	}
	for(int i = 0; i < symbolCount; ++i) {
		delete symbols[i];
	}
}

void Fsm::addSymbol(const char* name, int (*testAddress)(char)) {
	Symbol *s = new Symbol(name, testAddress);
	symbols[symbolCount] = s;
	++symbolCount;
	return;
}

void Fsm::addState(const char* name, int isInitialState, int isAcceptingState, int isDiscardingState) {
	State *s = new State(name, isInitialState, isAcceptingState, isDiscardingState);
	states[stateCount] = s;
	++stateCount;
	return;
}

void Fsm::addTransition(const char *fromStateName, const char *symbolName, const char *toStateName) {
	State *fromState = getState(fromStateName);
	State *toState = getState(toStateName);
	Symbol *sym = getSymbol(symbolName);
	Transition* newTransition = new Transition(fromState, sym, toState);
	transitions[transitionCount] = newTransition;
	++transitionCount;
	return;
}
	
void Fsm::run(const char* input, int *startIndex, char* output) {
	State *currentState = getInitialState();
	char tmpArray[100];
	tmpArray[0] = '\0';
	output[0] = '\0';
	
	int len = strlen(input);
	int i = *startIndex;
	for(int j = 0; i < len; ++i) {
		char c = input[i];
		Symbol *symbol = getSymbol(c);
		if(symbol == NULL) {
			break;
		}
		
		State *tmp = getNextState(currentState, symbol);
		if(tmp == NULL) {
			break;
		}
		
		if(tmp->isDiscardingState() == 0) {
			tmpArray[j] = c;
			++j;
		}
		
		if(tmp->isAcceptingState() == 1) {
			tmpArray[j] = '\0';
			strcat(output, tmpArray);
			j = 0;
			tmpArray[j] = '\0';
		}
		
		currentState = tmp;
	}
	
	*startIndex = i;
}		
		