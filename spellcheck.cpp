#include <iostream>
#include <string> 
#include <fstream>
#include <regex> 
#include "hash.h"

using namespace std;

void writeLengthError(int lineCount, string word, ofstream& outputDocument){
    outputDocument << "Long word at line " << lineCount << ", starts: " << word.substr(0,20) << "\n"; 
}

void writeWordNotFound (int lineCount, string word, ofstream& outputDocument){
    outputDocument << "Unknown word at line " << lineCount << ": " << word << "\n"; 
}

void toLowerCase(string& word){
    for(int i = 0; i<word.length(); i++){
        if(isupper(word[i]))
            word[i] = tolower(word[i]);
    }
}


hashTable buildDictionary(){
    hashTable table(0); 
    cout << "What is the path of dictionary file\n";
    string dictionaryPath; 
    cin >> dictionaryPath; 
    ifstream dictionaryFile(dictionaryPath); 
    if(dictionaryFile.is_open()){
        string word; 
        while(getline(dictionaryFile, word))
            table.insert(word);
    }
    return table; 
}



void checkWord (hashTable dictionary, string word, int lineCount, ofstream& outputDocument){
    toLowerCase(word);
    if(word.size() > 20) return writeLengthError(lineCount,word, outputDocument);
    if(word.find_first_of("0123456789") != string::npos || dictionary.contains(word)) return; 
    return writeWordNotFound(lineCount,word, outputDocument); 
}

void sepAndCheckWords(hashTable dictionary,string documentName, string outputDocumentName){
    ifstream document(documentName);
    ofstream outputDocument(outputDocumentName); 
    if(document.is_open()){
        string line; 
        regex r("([\\w-']+)"); 
        int lineCount = 1;
        while(getline(document,line)){
            smatch match; 
            while(regex_search(line,match,r)){
                checkWord(dictionary,match[0],lineCount, outputDocument); 
                line = match.suffix(); 
            }
            lineCount++;
        }
    }
}

void spellCheck(hashTable dictionary){
    cout << "What is the name of the spellcheck document\n"; 
    string documentName,outputDocumentName;
    cin >> documentName;
    cout << "What is the name of the output document\n"; 
    cin >> outputDocumentName;
    sepAndCheckWords(dictionary,documentName,outputDocumentName); 
}


int main(){
    spellCheck(buildDictionary());
    return 0; 
}