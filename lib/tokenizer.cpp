#include "fsm.h"
#include "query-document.h"
#include <string.h>

Tokenizer* Tokenizer::instance = NULL;

int Tokenizer::isWordCharacter(char c) {
	if(c != ' ') {
		return 1;
	}
	
	return 0;
}


int Tokenizer::isSentenceCharacter(char c) {
	if((c == ' ') || (c == '.') || (c == '!') || (c == '?')) {
		return 0;
	}
	
	return 1;
}

int Tokenizer::isSentenceTerminator(char c) {
	if((c == '!') || (c == '?')) {
		return 1;
	}
	
	return 0;
}

int Tokenizer::isDot(char c) {
	if(c == '.') {
		return 1;
	}
	
	return 0;
}

int Tokenizer::isSpace(char c) {
	return c == ' ';
}

int Tokenizer::isNewLine(char c) {
	if((c == '\n') || (c == '\r')) {
		return 0;
	}
	
	return 1;
}

int Tokenizer::isQueryName(char c) {
	if(
		((c == 'p') || (c == 'P')) ||
		((c == 's') || (c == 'S')) ||
		((c == 'w') || (c == 'W')))
	{
		return 1;
	}
	return 0;
}

int Tokenizer::isQueryValue(char c) {
	if((c >= '0') && (c <= '9')) {
		return 1;
	}
	return 0;
}		

int Tokenizer::isJunk(char c) {
	if((isQueryName(c) != 1) && (isQueryValue(c) != 1))
	{
		return 1;
	}
	return 0;
}

void Tokenizer::initializeWordSeparator() {
	wordSeparator.addSymbol("char", Tokenizer::isWordCharacter);

	wordSeparator.addState("s1", 1, 0, 0);
	wordSeparator.addState("s2", 0, 1, 0);
	
	wordSeparator.addTransition("s1", "char", "s2");
	wordSeparator.addTransition("s2", "char", "s2");
}

void Tokenizer::initializeSentenceSeparator() {
	sentenceSeparator.addSymbol("char", Tokenizer::isSentenceCharacter);
	sentenceSeparator.addSymbol("sentenceTerminator", Tokenizer::isSentenceTerminator);
	sentenceSeparator.addSymbol("space", Tokenizer::isSpace);
	sentenceSeparator.addSymbol("dot", Tokenizer::isDot);

	sentenceSeparator.addState("s1", 1, 0, 0);
	sentenceSeparator.addState("s2", 0, 1, 0);
	sentenceSeparator.addState("s3", 0, 1, 0);
	sentenceSeparator.addState("s4", 0, 1, 0);
	
	sentenceSeparator.addTransition("s1", "char", "s2");
	sentenceSeparator.addTransition("s2", "char", "s2");
	sentenceSeparator.addTransition("s2", "space", "s2");
	sentenceSeparator.addTransition("s2", "sentenceTerminator", "s4");
	sentenceSeparator.addTransition("s2", "dot", "s3");
	sentenceSeparator.addTransition("s3", "char", "s2");	
}

void Tokenizer::initializeParagraphSeparator() {
	paragraphSeparator.addSymbol("char", Tokenizer::isNewLine);

	paragraphSeparator.addState("s1", 1, 0, 0);
	paragraphSeparator.addState("s2", 0, 1, 0);
	
	paragraphSeparator.addTransition("s1", "char", "s2");
	paragraphSeparator.addTransition("s2", "char", "s2");	
}

void Tokenizer::initializeQueryParamSeparator() {
	queryParamSeparator.addSymbol("QueryName", Tokenizer::isQueryName);
	queryParamSeparator.addSymbol("junk", Tokenizer::isJunk);
	queryParamSeparator.addSymbol("QueryValue", Tokenizer::isQueryValue);
	
	queryParamSeparator.addState("s1", 1, 0, 0);
	queryParamSeparator.addState("s2", 0, 0, 0);
	queryParamSeparator.addState("s3", 0, 1, 0);
	queryParamSeparator.addState("s4", 0, 0, 1);
	
	queryParamSeparator.addTransition("s1", "QueryName", "s2");
	queryParamSeparator.addTransition("s2", "QueryName", "s4");
	queryParamSeparator.addTransition("s2", "junk", "s4");
	queryParamSeparator.addTransition("s2", "QueryValue", "s3");
	queryParamSeparator.addTransition("s4", "QueryName", "s4");
	queryParamSeparator.addTransition("s4", "junk", "s4");
	queryParamSeparator.addTransition("s4", "QueryValue", "s3");
	queryParamSeparator.addTransition("s3", "QueryValue", "s3");
	
}

Tokenizer::Tokenizer() {
	initializeWordSeparator();
	initializeSentenceSeparator();
	initializeParagraphSeparator();
	initializeQueryParamSeparator();
}

Tokenizer::~Tokenizer() {
	instance = NULL;
}

Tokenizer *Tokenizer::getTokenizer() {
	if(instance == NULL) {
		instance = new Tokenizer();
	}
	return instance;
}


void Tokenizer::getWord(const char* sentence, int *startPos, char* output) {
	wordSeparator.run(sentence, startPos, output);
}

void Tokenizer::getSentence(const char* paragraph, int *startPos, char* output) {
	sentenceSeparator.run(paragraph, startPos, output);
}

void Tokenizer::getParagraph(const char* document, int *startPos, char* output) {
	paragraphSeparator.run(document, startPos, output);
}

void Tokenizer::getQueryParam(const char* query, int *startPos, char* output) {
	queryParamSeparator.run(query, startPos, output);
}