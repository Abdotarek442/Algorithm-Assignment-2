#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <vector>

using namespace std;
class Node {
public:
    int key;
    int value;
    Node** forward;
    Node* backward;
    Node(int key, int value, int level) {
        this->key = key;
        this->value = value;
        forward = new Node*[level + 1];
        for (int i = 0; i <= level; i++)
            forward[i] = nullptr;
        backward = nullptr;
    }


    ~Node() {
        delete[] forward;
    }
};
class SkipList {
private:
    unordered_map<int, int> keyToNodeValue;
    int maxLevel;
    float probability;
    int currentLevel;
    Node* header;

    int randomLevel() {
        int level = 0;
        while ((rand() / double(RAND_MAX)) < probability && level < maxLevel)
            level++;
        return level;
    }
    Node* getLastNode(){
        Node* current = header;
        while (current->forward[0]){
            current = current->forward[0];
        }
        return current;
    }

public:
    SkipList(int maxLevel, float probability) {
        this->maxLevel = maxLevel;
        this->probability = probability;
        this->currentLevel = 0;
        header = new Node(-1, -1, maxLevel);
    }

    bool insert(int key, int value) {
        if (keyToNodeValue.find(key) != keyToNodeValue.end()) {
            return false;
        }

        Node* current = header;
        Node* update[maxLevel + 1];
        for (int i = maxLevel; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->value < value)
                current = current->forward[i];
            update[i] = current;
        }
        current = current->forward[0];

        if (!current || current->value != value) {
            int newLevel = randomLevel();
            if (newLevel > currentLevel) {
                for (int i = currentLevel + 1; i <= newLevel; i++)
                    update[i] = header;
                currentLevel = newLevel;
            }

            Node* newNode = new Node(key, value, newLevel);
            for (int i = 0; i <= newLevel; i++) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
                if (i==0 && newNode->forward[i]){
                    newNode->forward[i]->backward = newNode;
                }
            }
            newNode->backward = update[0];
            keyToNodeValue[key] = newNode->value;
            return true;
        }
    }

    bool search(int key, int& value) {
        Node* current = header;
        int val = keyToNodeValue [key] ;
        for (int i = currentLevel; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->value < val)
                current = current->forward[i];
        }
        current = current->forward[0];
        if (current && current->value == val) {
            value = current->value;
            return true;
        } else {
            return false;
        }
    }

    bool remove(int key) {
        if (keyToNodeValue.find(key) == keyToNodeValue.end()) {
            return false;
        }
        int value = keyToNodeValue [key] ;

        Node* current = header;
        Node* update[maxLevel + 1];

        for (int i = currentLevel; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->value < value)
                current = current->forward[i];
            update[i] = current;
        }
        current = current->forward[0];

        if (current && current->value == value) {
            for (int i = 0; i <= currentLevel; i++) {
                if (update[i]->forward[i] != current)
                    break;
                update[i]->forward[i] = current->forward[i];
            }
            current->forward[0]->backward = update[0];
            keyToNodeValue.erase(key);
            delete current;

            while (currentLevel > 0 && header->forward[currentLevel] == nullptr)
                currentLevel--;

            return true;

        } else {
            return false;
        }
    }

    void updateScore(int key, int newValue) {
        int oldValue;
        if (search(key, oldValue)) {
            remove(key);
            insert(key, newValue);
            cout << "Updated: Player " << key << " score to " << newValue << endl;
        } else {
            cout << "Player not found: " << key << endl;
        }
    }

    void display() {
        for (int i = 0; i <= currentLevel; i++) {
            Node* current = header->forward[i];
            cout << "Level " << i << ": ";
            while (current) {
                cout << "(" << current->key << ", " << current->value << ") ";
                current = current->forward[i];
            }
            cout << endl;
        }
    }

    void displayTopN(int N) {
        Node* current = getLastNode();
        int count = 0;
        while (current!=header && count < N) {
            cout << "Player " << current->key << ": " << current->value << endl;
            current = current->backward;
            count++;
        }
    }

};
class Game {
private:
    SkipList skiplist;

public:
    Game(int maxLevel, float probability) : skiplist(maxLevel, probability) {}

    void addPlayer(int playerId, int score) {
        if (skiplist.insert(playerId, score)) {
            cout << "Inserted: Player " << playerId << " with score " << score << endl;
        }
    }

    void removePlayer(int playerId) {
        if(skiplist.remove(playerId)){
            cout << "Deleted: Player " << playerId << endl;
        } else {
            cout << "Player not found: " << playerId << endl;
        }
    }

    void updatePlayerScore(int playerId, int newScore) {
        skiplist.updateScore(playerId, newScore);
    }

    void displayTopPlayers(int N) {
        cout << "Top " << N << " Players:\n";
        skiplist.displayTopN(N);
    }

    void displayPlayerScore(int playerId) {
        int score;
        if (skiplist.search(playerId, score)) {
            cout << "Player " << playerId << " score: " << score << endl;
        } else {
            cout << "Player " << playerId << " not found\n";
        }
    }
    void displayPlayers(){
        skiplist.display();
    }
};
int main() {

    SkipList skiplist(2, 0.5); // Create a skip list with maxLevel = 4 and probability = 0.5

    // Insert nodes
    skiplist.insert(1, 50);
    skiplist.insert(2, 70);
    skiplist.insert(3, 40);
    skiplist.insert(4, 10);

    // Display the skip list
    cout << "Skip List after insertion:\n";
    skiplist.display();


        Game game(4, 0.5); // Create a game with a skip list of maxLevel = 4 and probability = 0.5

        // Add players
        game.addPlayer(1, 50);
        game.addPlayer(2, 70);
        game.addPlayer(3, 40);
        game.addPlayer(4, 90);

        // Display all players
        cout << "\nAll Players:\n";
        game.displayPlayers();

        // Display top 3 players
        cout << "\nTop 3 Players:\n";
        game.displayTopPlayers(3);

        // Update player scores
        game.updatePlayerScore(1, 100);
        game.updatePlayerScore(3, 60);

        // Display updated leaderboard
        cout << "\nLeaderboard after score updates:\n";
        game.displayTopPlayers(3);

        // Remove a player
        game.removePlayer(2);

        // Display all players after removal
        cout << "\nAll Players after removal of Player 2:\n";
        game.displayPlayers();


    return 0;
}