#include "hash.h"

// Constructor - initializing table based on chosen hash table variant
HashTable::HashTable(int size, CollisionHandling variant) 
{
    tableSize = size; 
    elementCount = 0; 
    method = variant;
    switch (method) {
        case CHAINING_VECTOR:
            tableVector.resize(size);
            break;
        case CHAINING_LIST:
            tableList.resize(size);
            break;
        case CHAINING_BST:
            tableBST.resize(size);
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            tableProbing.resize(size);
            break;
    }
}

HashTable::~HashTable() {}//deconstructor

int HashTable::hash1(const string& key) const {
    unsigned long hash = 0;
    for (char c : key) {
        hash = (hash * 31 + c);
    }
    return hash % tableSize;
}

int HashTable::hash2(const string& key) const {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c;
    }
    return 1 + (hash % (tableSize - 1));
}

// complete this
int HashTable::probe(int index, int i, const string& key) const { //can modify arguments
    index = index % tableSize; //index always within bounds of table

    if (tableProbing[index].first == "") //base case, slot found
    {
        return index;//retuns index of open slot for element
    }

    switch (method) {
        case LINEAR_PROBING:
            return probe(index + i, i, key);
        case QUADRATIC_PROBING:
            // complete this
            return probe(index * i, i, key);
        case DOUBLE_HASHING:
            // complete this
            return probe(index + (i * hash2(key)), i+1, key); //increase i on each pass
        default:
            return index;
    }
}
void HashTable::insert(const string& key, int value) {
    resizeIfNeeded();
    int index = hash1(key); //same for all variants

    switch (method) {
        case CHAINING_VECTOR:
            for (auto& pair : tableVector[index]) {
                if (pair.first == key) {
                    pair.second = value; // Key already exists, update the value
                    return;
                }
            }
            elementCount++;
            tableVector[index].push_back({key, value});
            break;

        case CHAINING_LIST:
            // complete this
            for (auto& pair : tableList[index]) {
                if (pair.first == key) {
                    pair.second = value; // Key already exists, update the value
                    return;
                }
            }
            elementCount++;
            tableList[index].push_back({key, value});
            break;

        case CHAINING_BST:
            // complete this - use AVL insert method
            tableBST[index].insert(key, value);
            elementCount++;
            break;

        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
        {
            if (tableProbing[index].first == "" && tableProbing[index].second == 0)//slot is open for where it should go
            {
                //printf("found open slot\n");
                tableProbing[index] = {key, value}; //assigns pair to slot in table
                //printf("value: %d\n", value);
            }
            else//collision, we have to probe to find the right spot
            {
                tableProbing[findEmptySlot(key)] = {key, value}; //use findemptyslot method to insert element
                //printf("collison on value: %d\n", value);
            }
            elementCount++;
        }
        break;
    }
}

bool HashTable::search(const string& key, int& value) {
    // complete this
    int index = hash1(key);

    switch (method) {
        case CHAINING_VECTOR:
                for (auto& pair : tableVector[index]) {
                    if (pair.first == key && pair.second == value) {
                        return true;
                    }
                }
                return false;
            break;
        case CHAINING_LIST:
                for (auto& pair : tableList[index]) {
                    if (pair.first == key && pair.second == value) {
                        return true;
                    }
                }
                return false;
            break;
        case CHAINING_BST:
            if (tableBST[index].search(key, value))//if value is in bst at that index
            {
                return true;
            }
            return false;
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            {
                for (auto& pair : tableProbing) {
                    if (pair.first == key && pair.second == value) {
                        //printf("search successful\n");
                        return true;
                    }
                }
                return false;
            }
    }
    return false;
}

bool HashTable::remove(const string& key) {
    // complete this
    int index = hash1(key);

    switch (method) {
        case CHAINING_VECTOR:
        {
            int iterator = 0;
            for (auto& pair : tableVector[index]) {
                    if (pair.first == key) {
                        tableVector[index].erase(tableVector[index].begin() + iterator);
                        return true;
                    }
                    iterator++;
                }
            //return false; //value not in vector
        }
        break;
        
        case CHAINING_LIST:
        {
            list<pair<string, int>>::iterator it = tableList[index].begin();
            for (auto& pair : tableList[index]) {
                    if (pair.first == key) {
                        tableList[index].erase(it);
                        return true;
                    }
                    advance(it, 1);
                }
            }
            break;
        case CHAINING_BST:
            tableBST[index].remove(key); //removes key from index where it should be
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
        {
            int iterator3 = 0;
            for (auto& pair : tableProbing) {
                    if (pair.first == key) {
                        tableProbing.erase(tableProbing.begin() + iterator3);
                        return true;
                    }
                    iterator3++;
                }
                return false;
        }
            break;
    }
    return false;
}

// Function to read data from file
vector<pair<string, int>> readDataFromFile(const string& filename) {
    vector<pair<string, int>> data;
    ifstream file(filename);
    string key;
    int value;
    if (!file) {
        cerr << "Error reading file: " << filename << endl;
        return data;
    }
    while (file >> key >> value) {
        data.push_back({key, value});
    }

    file.close();
    return data;
}

// Benchmark function for custom HashTable
void HashTable::benchmarkHashTable(HashTable& table, const vector<pair<string, int>>& data, int numSearch, int numDelete) {
    auto start = chrono::high_resolution_clock::now();

    // Insertions
    for (const auto& pair : data) {
        table.insert(pair.first, pair.second);
    }

    auto insertEnd = chrono::high_resolution_clock::now();

    // Searches
    int value;
    for (int i = 0; i < numSearch; ++i) {
        table.search(data[rand() % data.size()].first, value);
    }

    auto searchEnd = chrono::high_resolution_clock::now();

    // Deletions
    for (int i = 0; i < numDelete; ++i) {
        table.remove(data[rand() % data.size()].first);
    }

    auto end = chrono::high_resolution_clock::now();

    auto insertTime = chrono::duration_cast<chrono::microseconds>(insertEnd - start);
    auto searchTime = chrono::duration_cast<chrono::microseconds>(searchEnd - insertEnd);
    auto deleteTime = chrono::duration_cast<chrono::microseconds>(end - searchEnd);

    cout << "Insertion time: " << insertTime.count() << " microseconds" << endl;
    cout << "Search time: " << searchTime.count() << " microseconds" << endl;
    cout << "Deletion time: " << deleteTime.count() << " microseconds" << endl;
}

void HashTable::displayStats() {
    int totalElements = 0;
    int maxChainLength = 0;
    int emptyBuckets = 0;

    switch (method) {
        case CHAINING_VECTOR:
            for (const auto& bucket : tableVector) {
                if (bucket.empty()) emptyBuckets++;
                maxChainLength = max(maxChainLength, (int)bucket.size());
                totalElements += bucket.size();
            }
            break;
        case CHAINING_LIST:
            for (const auto& bucket : tableList) {
                if (bucket.empty()) emptyBuckets++;
                maxChainLength = max(maxChainLength, (int)bucket.size());
                totalElements += bucket.size();
            }
            break;
        case CHAINING_BST:
            for (const auto& bucket : tableBST) {
                vector<pair<string, int>> elements = bucket.inOrderTraversal();
                if (elements.empty()) {
                    emptyBuckets++;
                } else {
                    maxChainLength = max(maxChainLength, (int)elements.size());
                    totalElements += elements.size();
                }
            }
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            for (const auto& entry : tableProbing) {
                if (!entry.first.empty()) totalElements++;
                else emptyBuckets++;
            }
            break;
    }

    cout << "Total elements: " << totalElements << endl;
    cout << "Load factor: " << (double)totalElements / tableSize << endl;
    cout << "Empty buckets: " << emptyBuckets << endl;
    if (method == CHAINING_VECTOR || method == CHAINING_LIST || method == CHAINING_BST) {
        cout << "Max chain length: " << maxChainLength << endl;
    }
}


void HashTable::rehash() {
    // complete this
    switch (method) {
        case CHAINING_VECTOR:

            break;
        case CHAINING_LIST:

            break;
        case CHAINING_BST:

            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
  
            break;
    }
}


int HashTable::findEmptySlot(const string& key) {
    // complete this
    int index = hash1(key);
    return probe(index, 1, key);
    
}

// try experimenting with different thresholds for each technique
void HashTable::resizeIfNeeded() {
    // complete this
    int loadfactor = elementCount/tableSize;

    switch (method) {
        case CHAINING_VECTOR:
            if (loadfactor > 2)
            {
                tableSize *= 2;
                tableVector.resize(tableSize);
                rehash();
            }
            break;
        case CHAINING_LIST:
            if (loadfactor > 2)
            {
                tableSize *= 2;
                tableList.resize(tableSize);
                rehash();
            }
            break;
        case CHAINING_BST:
            if (loadfactor > 5)
            {
                tableSize *= 2;
                tableBST.resize(tableSize);
                rehash();
            }
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            if (loadfactor > 1)
            {
                tableSize *= 2;
                tableProbing.resize(tableSize);
                rehash();
            }
            break;
    }
    
}

// Benchmark function for built-in hash table in C++
void benchmarkStdUnorderedMap(const vector<pair<string, int>>& data, int numSearch, int numDelete) {
    unordered_map<string, int> stdMap;

    auto start = chrono::high_resolution_clock::now();

    // Insertions
    for (const auto& pair : data) {
        stdMap[pair.first] = pair.second;
    }

    auto insertEnd = chrono::high_resolution_clock::now();

    // Searches
    int value;
    for (int i = 0; i < numSearch; ++i) {
        auto it = stdMap.find(data[rand() % data.size()].first);
        if (it != stdMap.end()) {
            value = it->second;
        }
    }

    auto searchEnd = chrono::high_resolution_clock::now();

    // Deletions
    for (int i = 0; i < numDelete; ++i) {
        stdMap.erase(data[rand() % data.size()].first);
    }

    auto end = chrono::high_resolution_clock::now();

    auto insertTime = chrono::duration_cast<chrono::microseconds>(insertEnd - start);
    auto searchTime = chrono::duration_cast<chrono::microseconds>(searchEnd - insertEnd);
    auto deleteTime = chrono::duration_cast<chrono::microseconds>(end - searchEnd);

    cout << "std::unordered_map Benchmark Results:" << endl;
    cout << "Insertion time: " << insertTime.count() << " microseconds" << endl;
    cout << "Search time: " << searchTime.count() << " microseconds" << endl;
    cout << "Deletion time: " << deleteTime.count() << " microseconds" << endl;
}