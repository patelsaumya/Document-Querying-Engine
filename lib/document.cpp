#include "query-document.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;


void Document::tokenize(const char *document) {
	Tokenizer* tokenizer = Tokenizer::getTokenizer();
	char output[1000];
	int len = strlen(document);
	int startPos = 0;
	while(startPos < len) {
		tokenizer->getParagraph(document, &startPos, output);
		if(strlen(output) == 0) {
		} else {
			Paragraph *p = new Paragraph(output);
			paragraphs[paragraphCount++] = p;
		}
		++startPos;
	}
}

void Document::getQueryParams(const char *query, char **queryParams) {
	Tokenizer* tokenizer = Tokenizer::getTokenizer();
	char output[100];
	int startPos = 0;
	int len = strlen(query);
	int i = 0;
	while(startPos < len) {
		tokenizer->getQueryParam(query, &startPos, output);
		if(strlen(output) == 0) {
			++startPos;
		} else {
			strcpy(queryParams[i], output);
			cout << output << "\n";
			++i;
		}

	}
	return;
}

int Document::findQueryValue(ContentType contentType, char **queryParams) {
	char c;
	if(contentType == ParagraphContent) {
		c = 'p';
	}
	else if(contentType == SentenceContent) {
		c = 's';
	}
	else {
		c = 'w';
	}

	for(int i = 0; i < 3; ++i) {
		if(queryParams[i] != NULL) {
			char firstLetter =  queryParams[i][0];
			if((firstLetter >= 'A') && (firstLetter <= 'Z')) {
				firstLetter = firstLetter + 32;
			}
			if(firstLetter == c) {
				int k = atoi(&queryParams[i][1]);
				return k;
			}
		}
	}
	return 0;
}

Document::Document(const char* fileName, const char *buffer) {
	paragraphCount = 0;

	if(fileName != NULL) {
		char *content;
		FILE *fp = fopen(fileName, "r");
		if(fp == NULL) {
			content = new char[1];
			*(content) = '\0';
			return;
		}
		fseek(fp, 0, SEEK_END);
		long len = ftell(fp);
		content = new char[len + 1];
		fseek(fp, 0, SEEK_SET);
		long i = 0;
		while(1) {
			char c = fgetc(fp);
			if(feof(fp) != 0) {
				break;
			}
			*(content + i++) = c;
		}
		*(content + i) = '\0';
		tokenize(content);
	}
	else if (buffer != NULL) {
		tokenize(buffer);
	}
}

int Document::query(const char *query) {
	char **queryParams = new char*[3];
	for(int i = 0; i < 3; ++i) {
		queryParams[i] = new char[6];
		queryParams[i][0] = '\0';
	}
	getQueryParams(query, queryParams);
	int paragraphNumber = findQueryValue(ParagraphContent, queryParams);
	int sentenceNumber = findQueryValue(SentenceContent, queryParams);
	int wordNumber = findQueryValue(WordContent, queryParams);
	cout << "ParagraphNumber: " << paragraphNumber << "\n";
	cout << "SentenceNumber: " << sentenceNumber << "\n";
	cout << "WordNumber: " << wordNumber << "\n";
	if(paragraphNumber == 0) {
		if(sentenceNumber == 0) {
			if(wordNumber == 0) {
				return print();
			}
			else {
				return printWord(wordNumber);
			}
		} else {
			if(wordNumber == 0) {
				return printSentence(sentenceNumber);
			}
			else {
				return printSentenceWord(sentenceNumber, wordNumber);
			}
		}
	} else {
		if(sentenceNumber == 0) {
			if(wordNumber == 0) {
				return printParagraph(paragraphNumber);
			}
			else {
				return printParagraphWord(paragraphNumber, wordNumber);
			}
		} else {
			if(wordNumber == 0) {
				return printParagraphSentence(paragraphNumber, sentenceNumber);
			}
			else {
				return printParagraphSentenceWord(paragraphNumber, sentenceNumber, wordNumber);
			}
		}
	}
}

int Document::print(void){
	for(int i = 0; i < paragraphCount; ++i) {
		paragraphs[i]->print();
		cout << "\n\n";
	}
	return 1;
}

int Document::printWord(int wordNumber) {
	int paragraphNumber = 0;

	for(int i = 0; i < paragraphCount; ++i) {
		wordNumber = wordNumber - paragraphs[i]->getWordCount();
		if(wordNumber <= 0) {
			paragraphNumber = i + 1;
			wordNumber = wordNumber + paragraphs[i]->getWordCount();
			break;
		}
	}

	if ((paragraphNumber == 0) || (paragraphNumber > paragraphCount)) {
		return 0;
	}
	cout << "Paragraph " << paragraphNumber << ": ";
	return paragraphs[paragraphNumber-1]->printWord(wordNumber);
}

int Document::printParagraphWord(int paragraphNumber, int wordNumber) {
	if ((paragraphNumber == 0) || (paragraphNumber > paragraphCount)) {
		return 0;
	}

	cout << "Paragraph " << paragraphNumber << ": ";
	return paragraphs[paragraphNumber-1]->printWord(wordNumber);
}

int Document::printSentenceWord(int sentenceNumber, int wordNumber) {
	int paragraphNumber = 0;

	for(int i = 0; i < paragraphCount; ++i) {
		sentenceNumber = sentenceNumber - paragraphs[i]->getSentenceCount();
		if(sentenceNumber <= 0) {
			paragraphNumber = i + 1;
			sentenceNumber = sentenceNumber + paragraphs[i]->getSentenceCount();
			break;
		}
	}

	if ((paragraphNumber == 0) || (paragraphNumber > paragraphCount)) {
		return 0;
	}

	cout << "Paragraph " << paragraphNumber << ": ";
	return paragraphs[paragraphNumber-1]->printSentenceWord(sentenceNumber, wordNumber);
}

int Document::printParagraphSentenceWord(int paragraphNumber, int sentenceNumber, int wordNumber) {
	if ((paragraphNumber == 0) || (paragraphNumber > paragraphCount)) {
		return 0;
	}

	cout << "Paragraph " << paragraphNumber << ": ";
	return paragraphs[paragraphNumber-1]->printSentenceWord(sentenceNumber, wordNumber);
}

int Document::printSentence(int sentenceNumber) {
	int paragraphNumber = 0;

	for(int i = 0; i < paragraphCount; ++i) {
		sentenceNumber = sentenceNumber - paragraphs[i]->getSentenceCount();
		if(sentenceNumber <= 0) {
			paragraphNumber = i + 1;
			sentenceNumber = sentenceNumber + paragraphs[i]->getSentenceCount();
			break;
		}
	}

	if ((paragraphNumber == 0) || (paragraphNumber > paragraphCount)) {
		return 0;
	}

	cout << "Paragraph " << paragraphNumber << ": ";
	return paragraphs[paragraphNumber-1]->printSentence(sentenceNumber);
}

int Document::printParagraphSentence(int paragraphNumber, int sentenceNumber) {
	if ((paragraphNumber == 0) || (paragraphNumber > paragraphCount)) {
		return 0;
	}

	cout << "Paragraph " << paragraphNumber << ": ";
	return paragraphs[paragraphNumber-1]->printSentence(sentenceNumber);
}

int Document::printParagraph(int paragraphNumber) {
	if ((paragraphNumber == 0) || (paragraphNumber > paragraphCount)) {
		return 0;
	}

	cout << "Paragraph " << paragraphNumber << ": ";
	return paragraphs[paragraphNumber-1]->print();
}
