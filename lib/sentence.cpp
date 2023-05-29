#include "query-document.h"
#include <string.h>
#include <iostream>
using namespace std;

void Sentence::tokenize(const char *sentence) {
	Tokenizer* tokenizer = Tokenizer::getTokenizer();
	char output[1000];
	int len = strlen(sentence);
	int startPos = 0;
	while(startPos < len) {
		tokenizer->getWord(sentence, &startPos, output);
		char *word = new char[strlen(output) + 1];
		strcpy(word, output);
		if(strlen(output) == 0) {
		} else {
			words[wordCount++] = word;
		}
		++startPos;
	}
}

Sentence::Sentence(const char *buffer) {
	wordCount = 0;
	tokenize(buffer);
}

int Sentence::getWordCount(void) {
	return wordCount;
}

int Sentence::print(void){
	for(int i = 0; i < wordCount; ++i) {
		cout << words[i] << ((i < wordCount - 1) ? " " : "");
	}
	
	return 1;
}

int Sentence::printWord(int wordNumber) {
	if ((wordNumber == 0) || (wordNumber > wordCount)) {
		return 0;
	}

	cout << "Word " << wordNumber << ": " << words[wordNumber-1];
	return 1;
}
		