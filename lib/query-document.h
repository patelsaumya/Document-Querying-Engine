#ifndef QUERY_DOCUMENT_H
#define QUERY_DOCUMENT_H

#include "fsm.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;


enum ContentType {
	ParagraphContent,
	SentenceContent,
	WordContent
};

class Tokenizer {
	protected:
		Fsm wordSeparator;
		Fsm sentenceSeparator;
		Fsm paragraphSeparator;
		Fsm queryParamSeparator;
		static Tokenizer* instance;
		
		static int isDot(char c);
		static int isSpace(char c);
		static int isNewLine(char c);

		static int isWordCharacter(char c);
		static int isSentenceCharacter(char c);
		static int isSentenceTerminator(char c);
		
		static int isQueryName(char c);		
		static int isQueryValue(char c);		
		static int isJunk(char c);
		
		void initializeWordSeparator();		
		void initializeSentenceSeparator();		
		void initializeParagraphSeparator();		
		void initializeQueryParamSeparator();
		
		Tokenizer();		
		~Tokenizer();

	public:
	
		static Tokenizer *getTokenizer();
		void getWord(const char* sentence, int *startPos, char* output);		
		void getSentence(const char* paragraph, int *startPos, char* output);
		void getParagraph(const char* document, int *startPos, char* output);		
		void getQueryParam(const char* query, int *startPos, char* output);
};
//Tokenizer* Tokenizer::instance = NULL;

class Sentence {
	protected:
		char *words[100];
		int wordCount;

		void tokenize(const char *sentence);
	public:
		Sentence(const char *buffer);
		int getWordCount(void);
		
		int print(void);		
		int printWord(int wordNumber);
};


class Paragraph {
	protected:
		Sentence *sentences[100];
		int sentenceCount;

		void tokenize(const char *paragraph);
	public:
		Paragraph(const char *buffer);		
		int getSentenceCount(void);
		int getWordCount(void);
		
		int print(void);
		int printWord(int wordNumber);		
		int printSentence(int sentenceNumber);
		int printSentenceWord(int sentenceNumber, int wordNumber);
};


class Document {
	protected:
		Paragraph *paragraphs[100];
		int paragraphCount;

		void tokenize(const char *document);
		
		void getQueryParams(const char *query, char **queryParams);		
		int findQueryValue(ContentType contentType, char **queryParams);
	public:
		Document(const char* fileName, const char *buffer);		
		int query(const char *query);
		
		int print(void);
		int printWord(int wordNumber);
		int printParagraphWord(int paragraphNumber, int wordNumber);
		int printSentenceWord(int sentenceNumber, int wordNumber);
		int printParagraphSentenceWord(int paragraphNumber, int sentenceNumber, int wordNumber);		
		int printSentence(int sentenceNumber);
		int printParagraphSentence(int paragraphNumber, int sentenceNumber);		
		int printParagraph(int paragraphNumber);
};


#endif