#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
using namespace std;

const string alphabet  = "abcdefghijklmnopqrstuvwxyz";


void createDict(set<string>& allWords) {
    string filename = "di.txt";
    ifstream input;
    input.open(filename.c_str());
    while(true) {
        string word;
        input >> word;
        if (input.fail()) {
            break;
        }
        allWords.insert(word);

    }
}

void selectWords(set<string> allWords, set<string>& userWords, int size) { //Gets all words of users choice of length
    for (set<string>::iterator it= allWords.begin(); it!=allWords.end(); ++it) {
        if ((*it).size() == size) {
            userWords.insert(*it);
        }
    }
}

bool checkFormat(char letter_guessed, set<char> guessedLetters) {
    if (guessedLetters.count(letter_guessed) == 0) {
        int n = 0;
        while (n < alphabet.size()) {
            if (alphabet.find(letter_guessed)) {
                return true;
                cout << "hey" << endl;
            }
            ++n;
        }
    }
    return false;
}

void makeWordFamilies(map<string, set<string>>& wordFamilies, char letter_guessed, set<string> userWords, int wordSize, string& key) {
    wordFamilies.clear(); // make sure there no garbage.
    for (set<string>::iterator it= userWords.begin(); it!=userWords.end(); ++it) {
        string wordKey = key;
        string word = *it;

        for (int l = 0; l < wordSize; l++) {
            if (key[l] == '-') { //Only change key if it has -, in key -oo- we want to keep the o's.
                if (letter_guessed != word[l]) {
                    wordKey[l] = '-';
                }
                else {
                    wordKey[l] = letter_guessed;
                }
            }
            else {
                wordKey[l] = key[l];
            }
        }
        cout << wordKey << endl;
        wordFamilies[wordKey].insert(word);
    }
}

void findBiggestFamily(map<string, set<string>> wordFamilies, string& key, set<string>& userWords) {
    set<string> biggest;

    cout << wordFamilies.size() << endl;
    for (map<string, set<string>>::iterator it= wordFamilies.begin(); it!=wordFamilies.end(); ++it) {
        if ((it->second).size() > biggest.size()) {
            biggest = it->second;
            key = it->first;
            //cout << "bigger" << key << endl;
        }
    }
    userWords = biggest;
    cout << "Current key is: " << key << endl;
}


int main() {
    int size;
    int guesses;
    string yes_no;
    char letter_guessed;
    set<string> allWords;
    set<string> userWords;
    set<char> guessedLetters;
    map<string, set<string>> wordFamilies;

    cout << "Loading... \n";
    createDict(allWords);

    while (userWords.empty()) {
        cout << "Please type word length: ";
        size = 0;
        cin >> size;
        selectWords(allWords, userWords, size);
    }

    cout << "How many guesses do you want?: ";
    cin >> guesses;

    string key; // current key example: --e--;

    //make empty key

    for (int n = 0; n < size; n++) {
        key.append("-");
    }

    while (guesses != 0) {
        cout << "Do you want to see amount of words left? y/n";
        cin >> yes_no;

        if (yes_no == "y") {
            cout << userWords.size() << endl;
        }
        cout << "You have " << guesses << " guesses left" << endl;

        while (true) {
            cout << "Guess a letter: ";
            cin >> letter_guessed;
            int possible_words_left = userWords.size();
            if ( possible_words_left != 1) {
                if (checkFormat(letter_guessed, guessedLetters)) {
                    guessedLetters.insert(letter_guessed);
                    makeWordFamilies(wordFamilies,letter_guessed, userWords, size, key);
                    findBiggestFamily(wordFamilies, key, userWords);
                    break;
                }
                else {
                    cout << "Please type a LETTER" << endl;
                }
            }
            else {
                guessedLetters.insert(letter_guessed);
                makeWordFamilies(wordFamilies,letter_guessed, userWords, size, key);
                findBiggestFamily(wordFamilies, key, userWords);


            }
        }
        //cout << guessedLetters.size() << endl;
        --guesses;

    }
    return 0;
}
