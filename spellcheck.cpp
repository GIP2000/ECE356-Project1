#include <iostream>
#include <string> 
#include <fstream>
#include <regex> 
#include "hash.h"
#include <chrono>

using namespace std;

//Writes error to otuput file if the word is too long 
void writeLengthError(int lineCount, string word, ofstream& outputDocument){
    outputDocument << "Long word at line " << lineCount << ", starts: " << word.substr(0,20) << "\n"; 
}

//Writes error to the output file if the word is not int he dictionary 
void writeWordNotFound (int lineCount, string word, ofstream& outputDocument){
    outputDocument << "Unknown word at line " << lineCount << ": " << word << "\n"; 
}


// converts a string word to all lower case
void toLowerCase(string& word){
    for(int i = 0; i<word.length(); i++){
        if(isupper(word[i]))
            word[i] = tolower(word[i]);
    }
}

//returns dictionary (in hash table ) from a file where every line is an entery in the hash table
hashTable buildDictionary(){
    hashTable table(0); 
    cout << "What is the path of dictionary file\n";
    string dictionaryPath; 
    cin >> dictionaryPath; 
    auto start = chrono::steady_clock::now(); 
    ifstream dictionaryFile(dictionaryPath); 
    if(dictionaryFile.is_open()){
        string word; 
        while(getline(dictionaryFile, word)){
            toLowerCase(word); 
            table.insert(word);
        }
    }
    dictionaryFile.close(); 
    auto end = chrono::steady_clock::now(); 
    chrono::duration<double> timeInSeconds = end-start; 
    cout << "Time it took to read dictionary is " << timeInSeconds.count() << "s\n"; 
    return table; 
}


// checks if an individual word is valid.
// If not it will use one of the above error funcitons to indicate so
void checkWord (hashTable& dictionary, string word, int lineCount, ofstream& outputDocument){
    toLowerCase(word);
    if(word.size() > 20) return writeLengthError(lineCount,word, outputDocument);
    if(word.find_first_of("0123456789") != string::npos || dictionary.contains(word)) return; 
    return writeWordNotFound(lineCount,word, outputDocument); 
}


// seperates an input document into valid words and checks if the word is valid or not
void sepAndCheckWords(hashTable& dictionary,string documentName, string outputDocumentName){
    auto start = chrono::steady_clock::now(); 
    ifstream document(documentName);
    ofstream outputDocument(outputDocumentName); 
    if(document.is_open()){
        string line; 
        regex r("([\\w'-]+)");
        int lineCount = 1;
        while(getline(document,line)){
            // sepAndCheckLine(dictionary,line,lineCount, outputDocument);
            smatch match; 
            while(regex_search(line,match,r)){
                checkWord(dictionary,match[0],lineCount, outputDocument); 
                line = match.suffix(); 
            }
            lineCount++;
        }
    }
    document.close(); 
    outputDocument.close(); 
    auto end = chrono::steady_clock::now(); 
    chrono::duration<double> timeInSeconds = end-start; 
    cout << "Time it took to spell check file is " << timeInSeconds.count() << "s\n"; 
}

// gets the input and otuput file names and initates spell check 
void spellCheck(hashTable& dictionary){
    cout << "What is the name of the spellcheck document\n"; 
    string documentName,outputDocumentName;
    cin >> documentName;
    cout << "What is the name of the output document\n"; 
    cin >> outputDocumentName;
    sepAndCheckWords(dictionary,documentName,outputDocumentName); 
}


int main(){
    hashTable table = buildDictionary(); 
    spellCheck(table);
    return 0; 
}