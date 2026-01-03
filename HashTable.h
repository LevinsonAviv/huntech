#pragma once

template <class Value>
class HashTable{

public:

HashTable();

// delete copy constructor
HashTable(const HashTable& hashTable) = delete;

// delete assignment operator
HashTable& operator=(const HashTable& hashTable) = delete;

~HashTable();

bool insert(int key, const Value& value);

Value* find(int key);

private:

static const int arrSizes[24];

struct Node {
    /**
     * @brief key of the value stored in the node.
    */
    int key;

    /**
     * @brief value stored in the node.
    */
    Value value;
};
Node **arr;

int arrSize;

int currentSize;

int arrSizesIndex;

int initialHash(int key);

int step(int key);

int hash(int key);

void resizeArr(int newSize);
};

template <class Value>
const int HashTable<Value>::arrSizes[] = {3 ,7, 17, 37, 79, 163, 331, 673, 1361, 2729, 
    5471, 10949, 21911, 43853, 87719, 175447, 350899, 
    701819, 1403641, 2807303, 5614657, 11229331, 22458671, 
    44917381};

template <class Value>
HashTable<Value>::HashTable(): arr(new Node* [arrSizes[0]]), arrSize(arrSizes[0]), currentSize(0), arrSizesIndex(0){
    for(int i = 0; i < arrSize; i++){
        arr[i] = nullptr;
    }
}

template <class Value>
HashTable<Value>::~HashTable(){
    for(int i = 0; i < arrSize; i++){
        delete arr[i];
    }
    delete[] arr;
}

template <class Value>
bool HashTable<Value>::insert(int key, const Value& value){
    if(find(key)){
        return false;
    }
    if(currentSize + 1 >= arrSize){ // think if should resize before
        resizeArr(arrSizes[++arrSizesIndex]);
    }
    arr[hash(key)] = new Node{key, value};
    currentSize++;
    return true;
}

template <class Value>
Value* HashTable<Value>::find(int key){
    int index = initialHash(key);
    int stepSize = step(key);
    while(arr[index] != nullptr){
        if(arr[index]->key == key){
            return &(arr[index]->value);
        }
        index = (index + stepSize) % arrSize;
    }
    return nullptr;
}

template <class Value>
int HashTable<Value>::initialHash(int key){
    return key % arrSize;
}

template <class Value>
int HashTable<Value>::step(int key){
    return 1 + key % (arrSize - 2);
}

template <class Value>
int HashTable<Value>::hash(int key){
    int index = initialHash(key);
    int stepSize = step(key);
    while(arr[index] != nullptr){
        index = (index + stepSize) % arrSize;
    }
    return index;
}

template <class Value>
void HashTable<Value>::resizeArr(int newArrSize){
    Node **oldArr = arr;
    int oldArrSize = arrSize;
    arr = new Node* [newArrSize];
    arrSize = newArrSize;
    for(int i = 0; i < arrSize; i++){
        arr[i] = nullptr;
    }
    for(int i = 0; i < oldArrSize; i++){
        if(oldArr[i] != nullptr){
            arr[hash(oldArr[i]->key)] = oldArr[i];
        }
    }
    delete[] oldArr;
}