#include "./hash.h"
#include <vector>

using namespace std;

hashTable::hashTable(int size){
    this->capacity = hashTable::getPrime(size); 
    this->data = vector<hashTable::hashItem>(this->capacity); 
}

int hashTable::insert(const string &key, void *pv){
      if(capacity/2 <= filled){
        const bool success = this->rehash(); 
        if(!success) return 2; 
    }
    const int initalSpace = this->hash(key); 
    int index = initalSpace; 
    bool wrap = false;
    while(!wrap || index < initalSpace){
        hashTable::hashItem item = this->data[index];
        if(!item.isDeleted){
            if(item.isOccupied && item.key == key) return 1; 
            if(!item.isOccupied) break;
        }
        index = (index +1) % this->capacity;
        if (index <= initalSpace) wrap = true;
    }
    if(wrap && index >= initalSpace) return 4; // no available space (should never happen if we rehash correctly)
    this->data[index].key = key;
    this->data[index].isOccupied = true; 
    this->data[index].pv = pv; 
    this->filled++; 
    return 0; 
} 

bool hashTable::contains(const string &key){
    return this->findPos(key) != -1; 
}

void* hashTable::getPointer(const string &key, bool *b) {

    int pos = this->findPos(key); 
    if(b != nullptr) *b = pos != -1; 
    return pos == -1 ? nullptr : this->data[pos].pv; 
     
}

int hashTable::setPointer(const string &key, void *pv){
    int pos = this->findPos(key);
    if(pos == -1) return 1; 
    this->data[pos].pv = pv; 
    return 0; 
}

bool hashTable::remove(const string &key){
    int pos = this->findPos(key);
    if(pos == -1) return false; 
    this->data[pos].isDeleted = true; 
    return true; 
}

int hashTable::hash(const std::string &key){
    unsigned int hash = 0;
    for(char c: key){
        hash = hash*37 + (int)c; 
    }
    return hash % this->capacity; 
}

int hashTable::findPos(const std::string &key){
    const int initalSpace = this->hash(key); 
    int index = initalSpace; 
    bool wrap = false;
    while(!wrap || index < initalSpace){
        hashTable::hashItem item = this->data[index]; 
        if(!item.isDeleted && item.isOccupied && item.key == key)
            return index;
        if(!item.isOccupied)
            return -1; 
        index = (index + 1) % this->capacity;
        if (index <= initalSpace) wrap = true;
    }
    return -1; 
    
}


bool hashTable::rehash(){
    vector<hashTable::hashItem> oldData = this->data;
    const int newSize = hashTable::getPrime(this->capacity);
    if(newSize == -1) return false;
    this->capacity = newSize;
    this->data = vector<hashTable::hashItem>(this->capacity);
    this->filled = 0;
    for(auto item: oldData){
        if(item.isOccupied && !item.isDeleted)
            this->insert(item.key,item.pv); 
    }
    return true; 
    
}

unsigned int hashTable::getPrime(int size){
    const int primes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593,49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};
    for(int prime : primes){
        if(prime > size)
            return prime; 
    }
    return -1; // list is too large maybe ask if I should throw an exspetion 
}