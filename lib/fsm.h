#ifndef FSM_H
#define FSM_H

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;


class Symbol {
	protected:
		char* name;
		int (*testAddress)(char);
	
	public:
	
		Symbol(const char* name, int (*testAddress)(char));	
		~Symbol();
		
		int isMatch(const char *name);
		int test(char c);	
};

class State {
	protected:
		char* name;
		int acceptingState;
		int initialState;
		int discardingState;
		
	public:
	
		State(const char* name, int initialState, int acceptingState, int discardingState);		
		~State();
		
		int isAcceptingState();
		int isInitialState();		
		int isDiscardingState();		
		int isMatch(const char *name);
};

class Transition {
	protected:
		State *fromState;
		Symbol *symbol;
		State *toState;
		
	public:
	
		Transition(State *fS, Symbol *sy, State *tS);
		~Transition();
		
		State *getToState(State *state, Symbol *symbol);
};

class Fsm {
	protected:
		State *states[10];
		int stateCount;
		Symbol *symbols[10];
		int symbolCount;
		Transition *transitions[10];
		int transitionCount;
		
		State *getInitialState();
		State *getState(const char *name);
		Symbol *getSymbol(const char *name);
		Symbol *getSymbol(char c);
		State *getNextState(State *fromState, Symbol *symbol);

	public:
		Fsm();		
		~Fsm();
		
		void addSymbol(const char* name, int (*testAddress)(char));
		void addState(const char* name, int isInitialState, int isAcceptingState, int isDiscardingState);
		void addTransition(const char *fromStateName, const char *symbolName, const char *toStateName);			
		void run(const char* input, int *startIndex, char* output);
};


#endif