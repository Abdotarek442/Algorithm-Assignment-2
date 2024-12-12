#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
const int TABLE_SIZE = 10;

class ProbingStrategy {
public:
    virtual int probe(int key, int i, int tableSize) const = 0;
    virtual ~ProbingStrategy() = default;
};

class LinearProbing : public ProbingStrategy {
public:
    int probe(int key, int i, int tableSize) const override {
        return (key + i) % tableSize;
    }
};

class QuadraticProbing : public ProbingStrategy {
public:
    int probe(int key, int i, int tableSize) const override {
        return (key + i * i) % tableSize;
    }
};

class DoubleHashing : public ProbingStrategy {
public:
    int probe(int key, int i, int tableSize) const override {
        int hash1 = key % tableSize;
        int hash2 = 1 + (key % (tableSize - 1));
        return (hash1 + i * hash2) % tableSize;
    }
};

class HashTable {
private:
    vector<int> table;
    vector<bool> isOccupied;
    ProbingStrategy* probingStrategy;

public:
    HashTable(ProbingStrategy* strategy)
            : table(TABLE_SIZE, -1), isOccupied(TABLE_SIZE, false), probingStrategy(strategy) {}

    void insert(int key) {
        int index = key % TABLE_SIZE;
        int i = 0;

        while (isOccupied[index]) {
            i++;
            index = probingStrategy->probe(key, i, TABLE_SIZE);
        }

        table[index] = key;
        isOccupied[index] = true;
    }

    void display() const {
        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << i << ": ";
            if (isOccupied[i])
                cout << table[i];
            else
                cout << "NULL";
            cout << "\n";
        }
    }
};



// Functions as defined in your code
int divisionMethod(int key, int tableSize) {
    return key % tableSize;
}

int multiplicationMethod(int key, int tableSize, float a) {
    float fractionPart = (key * a) - floor(key * a);
    return floor(tableSize * fractionPart);
}

int midSquareMethod(int key, int tableSize) {
    long long squaredNum = static_cast<long long>(pow(key, 2));
    string squaredstr = to_string(squaredNum);
    int len = squaredstr.length();
    int midDigitsCount = to_string(tableSize - 1).length();
    int midStart = (len - midDigitsCount) / 2;
    string midDigits = squaredstr.substr(midStart, midDigitsCount);

    int hashIndex = stoi(midDigits);
    if (hashIndex > tableSize)
        hashIndex %= tableSize;
    return hashIndex;
}

int foldShiftingMethod(int key, int address) {
    string keyStr = to_string(key);
    int len = keyStr.length();
    int numOfDigits = to_string(address - 1).length();
    int sum = 0;
    for (int i = 0; i < len; i += numOfDigits) {
        string size = keyStr.substr(i, numOfDigits);
        sum += stoi(size);
    }
    int hashIndex = sum % address;
    return hashIndex;
}

int foldBoundaryMethod(int key, int address) {
    string keyStr = to_string(key);
    int len = keyStr.length();
    int numOfDigits = to_string(address - 1).length();
    int sum = 0;
    for (int i = 0; i < len; i += numOfDigits) {
        string size = keyStr.substr(i, numOfDigits);
        if (i == 0 || i + numOfDigits >= keyStr.length()) {
            reverse(size.begin(), size.end());
        }
        sum += stoi(size);
    }
    int hashIndex = sum % address;
    return hashIndex;
}

// Main function
int main() {
    LinearProbing linearProbingStrategy;
    QuadraticProbing quadraticProbingStrategy;
    DoubleHashing doubleHashingStrategy;

    cout << "Linear Probing:\n";
    HashTable linearHashTable(&linearProbingStrategy);
    linearHashTable.insert(10);
    linearHashTable.insert(20);
    linearHashTable.insert(30);
    linearHashTable.display();

    cout << "\nQuadratic Probing:\n";
    HashTable quadraticHashTable(&quadraticProbingStrategy);
    quadraticHashTable.insert(10);
    quadraticHashTable.insert(20);
    quadraticHashTable.insert(30);
    quadraticHashTable.display();

    cout << "\nDouble Hashing:\n";
    HashTable doubleHashTable(&doubleHashingStrategy);
    doubleHashTable.insert(10);
    doubleHashTable.insert(20);
    doubleHashTable.insert(30);
    doubleHashTable.display();

    cout << "Division Method: " << divisionMethod(7, 10) << endl;
    cout << "Multiplication Method: " << multiplicationMethod(100, 150, 0.618033) << endl;
    cout << "Mid-Square Method: " << midSquareMethod(110, 200) << endl;
    cout << "Fold Shifting Method: " << foldShiftingMethod(100, 12345678) << endl;
    cout << "Fold Boundary Method: " << foldBoundaryMethod(100, 12345678) << endl;

}