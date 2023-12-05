#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include "hashtable.h"


using namespace std;
using namespace cop4530;

void menu()
{
	cout << "\n\n";
	cout << "l - Load Dictionary From File" << endl;
	cout << "a - Add Word" << endl;
	cout << "r - Remove Word" << endl;
	cout << "c - Clear HashTable" << endl;
	cout << "f - Find Word" << endl;
	cout << "d - Dump HashTable" << endl;
	cout << "s - HashTable Size" << endl;
	cout << "w - Write to File" << endl;
	cout << "x - Exit program" << endl;
	cout << "\nEnter choice : ";
}

string convertWord(string word){
    for (int i = 0; i < word.length(); i++){
        if (word[i] == ',' || word[i] == '.' || word[i] == ';') {   //remove punctuation
            word.pop_back();
        }
        word[i] = tolower(word[i]);
    }
    return word;
}

void outputSentence(string& line, string& word){

    string copy = word;    
    string element;
    string newLine = "";
    istringstream iss(line);
    vector<string> words;

    while (iss >> element){
        words.push_back(element);
    }

    for (int i = 0; i < copy.length(); ++i){
        copy[i] = toupper(copy[i]);
    }

    auto itr = std::find(words.begin(), words.end(), word);

    if (itr != words.end()){
        *itr = copy;
    }

    for (const auto & element : words){
        newLine += element + " ";
    }
    cout << newLine;
    cout << endl; 
}

void findCandidateWords(HashTable<string> &ht, vector<string>& cw, string & word){
    //clear previous candidate words
    cw.clear();

    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < word.length(); ++i){
        if (cw.size() >= 10) {
            break;
        }
        char previousChar = word[i]; //save the original

        for(int j = 0; j < alphabet.length(); ++j){
            if (cw.size() >= 10) {
                break;
            }
            word[i] = alphabet[j];
            if(ht.contains(word)){      //test to see if change is in hashtable
                cw.push_back(word);
            }

            word[i] = previousChar; //reset to original character before moving to the next
        }
    }

}

void displayCW(vector<string>& cw){
    cout << "====================================" << endl;

    for (int i = 0; i < cw.size(); ++i){
        cout << i << "): " << cw[i] << endl;
    }
    cout << "n (no change):" << endl; 
    cout << "====================================" << endl;
}

void changeWord(HashTable<string> &ht, const int& c, string& line, vector<string>& cw, const char* outputFilename) {
    string replacementWord;
    string originalWord;
    string checkWord;
    string element;
    string newLine;
    vector<string> words;
    istringstream iss(line);
    ofstream of(outputFilename, fstream::app);

    replacementWord = cw[c];

    while (iss >> element) {
        checkWord = convertWord(element);       //need to convert to lowercase to check if in ht
        if (!ht.contains(checkWord)) {
            originalWord = checkWord;
            words.push_back(originalWord);      //pushback lowercase version and skip the current iteration
            continue;
        }
        words.push_back(element);       //pushback other elements with normal punctuation
    }

    auto itr = std::find(words.begin(), words.end(), originalWord);
    if (itr != words.end()) {
        *itr = replacementWord;
    }

    for (const auto & element : words){
        newLine += element + " ";
    }
    of << newLine;
    of.close();

}

void changeWord(string& line, const char* outputFilename){
    ofstream of(outputFilename, fstream::app);

    if (!of.is_open()) {
        cerr << "Error opening the output file." << endl;
        return;  // Return an error code
    }
    of << line;
    of.close();

}

void chooseCW(HashTable<string> &ht, string& line, vector<string>& cw, const char* outputFileName){
    char choice = ' ';
    int integerChoice = 0;

    do {
        cout << "Your choice: ";
        cin >> choice;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear input buffer
        integerChoice = choice - '0';

        if (choice != 'n' && integerChoice > cw.size() - 1) {
            cout << "Invalid choice" << endl;
            displayCW(cw);
        }
    } while (choice != 'n' && integerChoice > cw.size() - 1);
    
    if (choice == 'n'){
        changeWord(line,outputFileName);        //output file with no changes
    }
    else {
        changeWord(ht,integerChoice,line,cw,outputFileName);  //output file with line changed with selected word
    }

}

int main(int argc, char* argv[]){
    const char* dictionary = argv[1];
    const char* checkFile = argv[2];
    const char* outputFile = argv[3];
    vector<string> candidateWords;
    string lowerCaseWord;
    
    if (argc == 1){ //if no input file provided show the menu 
        menu();
        return 0;
    }

    HashTable<string> ht;
    ht.load(dictionary);
    
    ifstream fs(checkFile);

    if (!fs){
        cout << "error";
    }
    string line;

    while (getline(fs, line)){
        istringstream iss(line);
        string word;
        while (iss >> word){
            lowerCaseWord = convertWord(word);
            if(!ht.contains(lowerCaseWord)){
                outputSentence(line, word);
                findCandidateWords(ht,candidateWords,lowerCaseWord);
                displayCW(candidateWords);
                chooseCW(ht, line,candidateWords,outputFile);

            }
        }
    }
    
	return 0;
}
