#include "fsm.h"
#include <string.h>

Transition::Transition(State *fS, Symbol *sy, State *tS) {
	this->fromState = fS;
	this->symbol = sy;
	this->toState = tS;
}
Transition::~Transition() {
}		

State* Transition::getToState(State *state, Symbol *symbol) {
	if ((this->fromState == state) &&  (this->symbol == symbol)) {
		return this->toState;
	}
	return NULL;
}