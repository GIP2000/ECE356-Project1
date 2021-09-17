#include "./hash.h"
#include <vector>
#include <tuple>
#include <functional>

using namespace std;

hashTable::hashTable(int size){
    this->capacity = hashTable::getPrime(size); 
    this->data = vector<hashTable::hashItem>(this->capacity); 
}

int hashTable::insert(const string &key, void *pv){
    if(filled/capacity > .5){
        const bool success = this->rehash(); 
        if(!success) return 2; 
    }
    const int initalSpace = this->hash(key); 
    tuple<bool,int> result = this->loop(initalSpace,key,[](hashTable::hashItem item, int index, string key){
        if(!item.isDeleted){
            if(item.isOccupied && item.key == key) return make_tuple(1,true,false); //return 1; 
            if(!item.isOccupied) return make_tuple(index,false,true);
        }
        return make_tuple(0,false,false);
    });
    if(get<0>(result)) return get<1>(result);
    const int index = get<1>(result); 
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
    tuple<bool,int> result = this->loop(initalSpace,key,[](hashTable::hashItem item, int index, string key){
        if(!item.isDeleted && item.isOccupied && item.key == key)
            return make_tuple(index,true,false);
        return make_tuple(-1,false,false);
    });
    return get<1>(result); 
    
    
}
tuple<bool,int> hashTable::loop(const int initalSpace,const string& key, function<tuple<int,bool,bool>(hashTable::hashItem,int,string)> func){
    bool wrap = false;
    int index = initalSpace;
    tuple<int,bool,bool> values;
    while(!wrap || index < initalSpace){
        hashTable::hashItem item = this->data[index];
        values = func(item,index,key);
        if(get<1>(values)) return make_tuple(true,get<0>(values));
        if(get<2>(values)) return make_tuple(false,get<0>(values));
        index = (index + 1) % this->capacity;
        if (index <= initalSpace) wrap = true;
    }
    return make_tuple(true,-1); 
}

bool hashTable::rehash(){
    vector<hashTable::hashItem> oldData = {this->data.begin(), this->data.begin() + this->capacity};
    const int newSize = hashTable::getPrime(this->capacity);
    if(newSize == -1) return false;
    this->capacity = newSize;
    this->data = vector<hashTable::hashItem>(this->capacity);
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