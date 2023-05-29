#include "query-document.h"
#include <string.h>
#include <iostream>
using namespace std;

void Paragraph::tokenize(const char *paragraph) {
	Tokenizer* tokenizer = Tokenizer::getTokenizer();			
	char output[1000];
	int len = strlen(paragraph);
	int startPos = 0;
	while(startPos < len) {
		tokenizer->getSentence(paragraph, &startPos, output);
		if(strlen(output) == 0) {
		} else {
			Sentence *s = new Sentence(output);
			sentences[sentenceCount++] = s;
		}
		++startPos;
	}
}

Paragraph::Paragraph(const char *buffer) {
	sentenceCount = 0;
	tokenize(buffer);
}

int Paragraph::getWordCount(void) {
	int wordCount = 0;
	for(int i = 0; i < sentenceCount; ++i) {
		wordCount = wordCount + sentences[i]->getWordCount();
	}
	return wordCount;
}

int Paragraph::getSentenceCount(void) {
	return sentenceCount;
}

int Paragraph::print(void) {
	for(int i = 0; i < sentenceCount; ++i) {
		sentences[i]->print();
		cout << ((i < sentenceCount - 1) ? " " : "");
	}
	return 1;
}

int Paragraph::printWord(int wordNumber) {
	int sentenceNumber = 0;
	for(int i = 0; i < sentenceCount; ++i) {
		wordNumber = wordNumber - sentences[i]->getWordCount();
		if(wordNumber <= 0) {
			sentenceNumber = i + 1;
			wordNumber = wordNumber + sentences[i]->getWordCount();
			break;
		}
	}
	
	if ((sentenceNumber == 0) || (sentenceNumber > sentenceCount)) {
		return 0;
	}
	
	cout << "Sentence " << sentenceNumber << ": ";
	return sentences[sentenceNumber-1]->printWord(wordNumber);
}

int Paragraph::printSentence(int sentenceNumber) {
	if ((sentenceNumber == 0) || (sentenceNumber > sentenceCount)) {
		return 0;
	}

	cout << "Sentence " << sentenceNumber << ": ";
	return sentences[sentenceNumber-1]->print();
}

int Paragraph::printSentenceWord(int sentenceNumber, int wordNumber) {
	if ((sentenceNumber == 0) || (sentenceNumber > sentenceCount)) {
		return 0;
	}

	cout << "Sentence " << sentenceNumber << ": ";
	return sentences[sentenceNumber-1]->printWord(wordNumber);
}
		