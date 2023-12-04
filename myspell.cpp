#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
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
    istringstream iss(line);
    vector<string> words;
    string element;

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

    line = "";
    for (const auto & element : words){
        line += element + " ";
    }
    cout << line;
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

            word[i] = previousChar; //reset to character
        }
    }

}

void displayCW(vector<string>& cw){
    cout << "====================================" << endl;

    for (int i = 0; i < cw.size(); ++i){
        cout << i << "): " << cw[i] << endl;
    }
    cout << "n (no change):" << endl; 

}

void changeWord(const int& c, vector<string>& cw){

}

void chooseCW(vector<string>& cw){
    int choice;
    cout << "====================================" << endl;
    cout << "Your choice: ";
    cin >> choice;

    if (choice >= cw.size()){
        cout << "invalid choice" << endl;
        chooseCW(cw);
    }
    else{
        changeWord(choice,cw);
    }
}


int main(int argc, char* argv[]){
    if (argc != 4){
        cout << "Error please provide dictionary file, the file to be checked, and the output file" << endl;
        return 1;
    }
    const char* dictionary = argv[1];
    const char* checkFile = argv[2];
    const char* outputFile = argv[3];
    vector<string> candidateWords;
    string lowerCaseWord;
    char choice;
    
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
                chooseCW(candidateWords);

            }
        }
    }

    // do{
    //     menu();
    // }while(choice != 'x');
      
	return 0;
}
