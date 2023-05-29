#include <stdio.h>
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

class Symbol {
	protected:
		char* name;
		int (*testAddress)(char);
	
	public:
	
		Symbol(const char* name, int (*testAddress)(char)) {
			this->name = new char[strlen(name) + 1];
			strcpy(this->name, name);
			this->testAddress = testAddress;
		}
	
		~Symbol() {
			delete[] name;
		}

		int isMatch(const char *name) {
			if (strcmp(this->name, name) == 0) {
				return 1;
			}
			return 0;
		}

		int test(char c) {
			if(testAddress(c)) {
				return 1;
			}
			return 0;
		}
	
};

class State {
	protected:
		char* name;
		int acceptingState;
		int initialState;
		int discardingState;
		
	public:
	
		State(const char* name, int initialState, int acceptingState, int discardingState) {
			this->initialState = initialState;
			this->acceptingState = acceptingState;
			this->discardingState = discardingState;
			this->name = new char[strlen(name) + 1];
			strcpy(this->name, name);
		}
		
		~State() {
			delete[] name;
		}
		
		int isAcceptingState() {
			return this->acceptingState;
		}

		int isInitialState() {
			return this->initialState;
		}
		
		int isDiscardingState() {
			return this->discardingState;
		}
		
		int isMatch(const char *name) {
			if (strcmp(this->name, name) == 0) {
				return 1;
			}
			return 0;
		}
};

class Transition {
	protected:
		State *fromState;
		Symbol *symbol;
		State *toState;
		
	public:
	
		Transition(State *fS, Symbol *sy, State *tS) {
			this->fromState = fS;
			this->symbol = sy;
			this->toState = tS;
		}
		~Transition() {
		}		
		
		State *getToState(State *state, Symbol *symbol) {
			if ((this->fromState == state) &&  (this->symbol == symbol)) {
				return this->toState;
			}
			return NULL;
		}
};

class Fsm {
	protected:
		State *states[10];
		int stateCount;
		Symbol *symbols[10];
		int symbolCount;
		Transition *transitions[10];
		int transitionCount;
		
		State *getState(const char *name) {
			for(int i =0; i < stateCount; ++i) {
				if(states[i]->isMatch(name)) {
					return states[i];
				}
			}
			return NULL;
		}
		Symbol *getSymbol(const char *name) {
			for(int i =0; i < symbolCount; ++i) {
				if(symbols[i]->isMatch(name)) {
					return symbols[i];
				}
			}
			return NULL;
		}

		State *getNextState(State *fromState, Symbol *symbol) {
			State *result = NULL;
			for(int  i = 0; i < transitionCount; ++i) {
				result = transitions[i]->getToState(fromState, symbol);
				if(result != NULL) {
					break;
				}
			}
			return result;
		}
		
		State *getInitialState() {
			for(int i =0; i < stateCount; ++i) {
				if(states[i]->isInitialState() == 1) {
					return states[i];
				}
			}
			return NULL;
		}
		
		Symbol *getSymbol(char c) {
			for(int i = 0; i < symbolCount; ++i) {
				if(symbols[i]->test(c) == 1) {
					return symbols[i];
				}
			}
			
			return NULL;
		}

	public:
		Fsm() {
			this->stateCount = 0;
			this->symbolCount = 0;
			this->transitionCount = 0;
		}
		
		~Fsm() {
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
		
		void addSymbol(const char* name, int (*testAddress)(char)) {
			Symbol *s = new Symbol(name, testAddress);
			symbols[symbolCount] = s;
			++symbolCount;
			return;
		}

		void addState(const char* name, int isInitialState, int isAcceptingState, int isDiscardingState) {
			State *s = new State(name, isInitialState, isAcceptingState, isDiscardingState);
			states[stateCount] = s;
			++stateCount;
			return;
		}

		void addTransition(const char *fromStateName, const char *symbolName, const char *toStateName) {
			State *fromState = getState(fromStateName);
			State *toState = getState(toStateName);
			Symbol *sym = getSymbol(symbolName);
			Transition* newTransition = new Transition(fromState, sym, toState);
			transitions[transitionCount] = newTransition;
			++transitionCount;
			return;
		}
			
		void run(const char* input, int *startIndex, char* output) {
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
};

class Tokenizer {
	protected:
		Fsm wordSeparator;
		Fsm sentenceSeparator;
		Fsm paragraphSeparator;
		Fsm queryParamSeparator;
		static Tokenizer* instance;
		
		static int isWordCharacter(char c) {
			if(c != ' ') {
				return 1;
			}
			
			return 0;
		}
		

		static int isSentenceCharacter(char c) {
			if((c == ' ') || (c == '.') || (c == '!') || (c == '?')) {
				return 0;
			}
			
			return 1;
		}

		static int isSentenceTerminator(char c) {
			if((c == '!') || (c == '?')) {
				return 1;
			}
			
			return 0;
		}

		static int isDot(char c) {
			if(c == '.') {
				return 1;
			}
			
			return 0;
		}

		static int isSpace(char c) {
			return c == ' ';
		}
		
		static int isNewLine(char c) {
			if((c == '\n') || (c == '\r')) {
				return 0;
			}
			
			return 1;
		}
		
		static int isQueryName(char c) {
			if(
				((c == 'p') || (c == 'P')) ||
				((c == 's') || (c == 'S')) ||
				((c == 'w') || (c == 'W')))
			{
				return 1;
			}
			return 0;
		}
		
		static int isQueryValue(char c) {
			if((c >= '0') && (c <= '9')) {
				return 1;
			}
			return 0;
		}		
		
		static int isJunk(char c) {
			if((isQueryName(c) != 1) && (isQueryValue(c) != 1))
			{
				return 1;
			}
			return 0;
		}
		
		void initializeWordSeparator() {
			wordSeparator.addSymbol("char", Tokenizer::isWordCharacter);

			wordSeparator.addState("s1", 1, 0, 0);
			wordSeparator.addState("s2", 0, 1, 0);
			
			wordSeparator.addTransition("s1", "char", "s2");
			wordSeparator.addTransition("s2", "char", "s2");
		}
		
		void initializeSentenceSeparator() {
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
		
		void initializeParagraphSeparator() {
			paragraphSeparator.addSymbol("char", Tokenizer::isNewLine);

			paragraphSeparator.addState("s1", 1, 0, 0);
			paragraphSeparator.addState("s2", 0, 1, 0);
			
			paragraphSeparator.addTransition("s1", "char", "s2");
			paragraphSeparator.addTransition("s2", "char", "s2");	
		}
		
		void initializeQueryParamSeparator() {
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
		
		Tokenizer() {
			initializeWordSeparator();
			initializeSentenceSeparator();
			initializeParagraphSeparator();
			initializeQueryParamSeparator();
		}
		
		~Tokenizer() {
			instance = NULL;
		}

	public:
	
		static Tokenizer *getTokenizer() {
			if(instance == NULL) {
				instance = new Tokenizer();
			}
			return instance;
		}
		
		
		void getWord(const char* sentence, int *startPos, char* output) {
			wordSeparator.run(sentence, startPos, output);
		}
		
		void getSentence(const char* paragraph, int *startPos, char* output) {
			sentenceSeparator.run(paragraph, startPos, output);
		}
		
		void getParagraph(const char* document, int *startPos, char* output) {
			paragraphSeparator.run(document, startPos, output);
		}
		
		void getQueryParam(const char* query, int *startPos, char* output) {
			queryParamSeparator.run(query, startPos, output);
		}
};
Tokenizer* Tokenizer::instance = NULL;

class Sentence {
	protected:
		char *words[100];
		int wordCount;

		void tokenize(const char *sentence) {
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
	public:
		Sentence(const char *buffer) {
			wordCount = 0;
			tokenize(buffer);
		}
		
		int getWordCount(void) {
			return wordCount;
		}
		
		int print(void){
			for(int i = 0; i < wordCount; ++i) {
				cout << words[i] << ((i < wordCount - 1) ? " " : "");
			}
			
			return 1;
		}
		
		int printWord(int wordNumber) {
			if ((wordNumber == 0) || (wordNumber > wordCount)) {
				return 0;
			}

			cout << "Word " << wordNumber << ": " << words[wordNumber-1];
			return 1;
		}
};


class Paragraph {
	protected:
		Sentence *sentences[100];
		int sentenceCount;

		void tokenize(const char *paragraph) {
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
	public:
		Paragraph(const char *buffer) {
			sentenceCount = 0;
			tokenize(buffer);
		}
		
		int getWordCount(void) {
			int wordCount = 0;
			for(int i = 0; i < sentenceCount; ++i) {
				wordCount = wordCount + sentences[i]->getWordCount();
			}
			return wordCount;
		}
		
		int getSentenceCount(void) {
			return sentenceCount;
		}
		
		int print(void) {
			for(int i = 0; i < sentenceCount; ++i) {
				sentences[i]->print();
				cout << ((i < sentenceCount - 1) ? " " : "");
			}
			return 1;
		}

		int printWord(int wordNumber) {
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
		
		int printSentence(int sentenceNumber) {
			if ((sentenceNumber == 0) || (sentenceNumber > sentenceCount)) {
				return 0;
			}

			cout << "Sentence " << sentenceNumber << ": ";
			return sentences[sentenceNumber-1]->print();
		}
		
		int printSentenceWord(int sentenceNumber, int wordNumber) {
			if ((sentenceNumber == 0) || (sentenceNumber > sentenceCount)) {
				return 0;
			}

			cout << "Sentence " << sentenceNumber << ": ";
			return sentences[sentenceNumber-1]->printWord(wordNumber);
		}
};


class Document {
	protected:
		Paragraph *paragraphs[100];
		int paragraphCount;

		void tokenize(const char *document) {
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
		
		void getQueryParams(const char *query, char **queryParams) {
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
		
		int findQueryValue(ContentType contentType, char **queryParams) {
			char c;
			if(contentType == ParagraphContent) {
				c = 'p';
			}
			else if(contentType == SentenceContent) {
				c= 's';
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
	public:
		Document(const char* fileName, const char *buffer) {
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
		
		int query(const char *query) {
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
		
		int print(void){
			for(int i = 0; i < paragraphCount; ++i) {
				paragraphs[i]->print();
				cout << "\n\n";
			}
			return 1;
		}

		int printWord(int wordNumber) {
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

		int printParagraphWord(int paragraphNumber, int wordNumber) {
			if ((paragraphNumber == 0) || (paragraphNumber > paragraphCount)) {
				return 0;
			}

			cout << "Paragraph " << paragraphNumber << ": ";			
			return paragraphs[paragraphNumber-1]->printWord(wordNumber);
		}

		int printSentenceWord(int sentenceNumber, int wordNumber) {
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
		int printParagraphSentenceWord(int paragraphNumber, int sentenceNumber, int wordNumber) {
			if ((paragraphNumber == 0) || (paragraphNumber > paragraphCount)) {
				return 0;
			}

			cout << "Paragraph " << paragraphNumber << ": ";
			return paragraphs[paragraphNumber-1]->printSentenceWord(sentenceNumber, wordNumber);
		}
		
		int printSentence(int sentenceNumber) {
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
		int printParagraphSentence(int paragraphNumber, int sentenceNumber) {
			if ((paragraphNumber == 0) || (paragraphNumber > paragraphCount)) {
				return 0;
			}

			cout << "Paragraph " << paragraphNumber << ": ";
			return paragraphs[paragraphNumber-1]->printSentence(sentenceNumber);
		}		
		
		int printParagraph(int paragraphNumber) {
			if ((paragraphNumber == 0) || (paragraphNumber > paragraphCount)) {
				return 0;
			}

			cout << "Paragraph " << paragraphNumber << ": ";
			return paragraphs[paragraphNumber-1]->print();
		}
};

int main(int argc, char **argv) {
	if(argc == 2) {
		Document *doc = new Document(argv[1], NULL);
		char query[100];
		while(1) {
			query[0] = '\0';
			cout << "\n\nQuery: ";
			scanf("%[^\n]", query);
			if(!(doc->query(query))) {
				cout << "\nResult not found...";
			}
			cout << "\n\nPress any key to continue...\n";
			getch();
			fflush(stdin);
			system("cls");
		}
		delete doc;
	}
}