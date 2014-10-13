/*
 * Lab 2B Evil Hangman (TDDD86)
 * liuid: piehe154 & danjo732
 *
*/

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
using namespace std;

const string ALPHABET  = " abcdefghijklmnopqrstuvwxyz";

void initGame();

// Creates a dictionary of all words
void createDict(set<string>& dictionary) {
    string filename = "dictionary.txt";
    ifstream input;
    input.open(filename.c_str());
    while(true) {
        string word;
        input >> word;
        if (input.fail()) {
            break;
        }
        dictionary.insert(word);

    }
}

//Gets all words of users choice of length
void selectWords(set<string>& dictionary, set<string>& wordsRemaining, int& size) {

    while (wordsRemaining.empty()) {
        cout << "Please type word length: ";
        size = 0;
        cin >> size;

        for (set<string>::iterator it = dictionary.begin(); it!=dictionary.end(); ++it) {
            if ((*it).size() == size) {
                wordsRemaining.insert(*it);
            }
        }

        if(wordsRemaining.empty()){
            cout << "No words with that size..." << endl << endl;
        }
    }
}

//Checks whether or not given input is of valid format
bool checkFormat(char& letterGuessed, set<char>& guessedLetters) {
    if (guessedLetters.count(letterGuessed) == 0) {
        if (ALPHABET.find(letterGuessed) != -1) {
                return true;
            }
    }
    return false;
}


// We update our families according to the changes of our key
// which the user has provided us with, and updates our word families
void makeWordFamilies(map<string, set<string>>& wordFamilies, char& letterGuessed, set<string>& wordsRemaining, int& wordSize, string& key) {

    wordFamilies.clear(); // make sure there's no garbage left from other iterations.

    for (set<string>::iterator it= wordsRemaining.begin(); it!=wordsRemaining.end(); ++it) {
        string wordKey = key; // we start our key where we left the last time
        string word = *it;

        //Creates a key for each word and puts it in correct set.
        for (int l = 0; l < wordSize; l++) {
            if (key[l] == '-') { //Only change key if it has -, in key -oo- we want to keep the o's.
                if (letterGuessed != word[l]) {
                    wordKey[l] = '-';
                }
                else {
                    wordKey[l] = letterGuessed;
                }
            }
            else {
                wordKey[l] = key[l];
            }
        }
        wordFamilies[wordKey].insert(word);
    }
}

//Finds famly with largest amount of words in it
void findBiggestFamily(map<string, set<string>>& wordFamilies, string& key, set<string>& wordsRemaining) {
    set<string> biggest;

    for (map<string, set<string>>::iterator it= wordFamilies.begin(); it!=wordFamilies.end(); ++it) {
        if ((it->second).size() > biggest.size()) {
            biggest = it->second;
            key = it->first;
        }
    }
    wordsRemaining = biggest;
}

// (E1)
// Fins the words that has the most "-" dashes in its key.
void findSmallestKey(map<string, set<string>>& wordFamilies, string& key, set<string>& wordsRemaining){
    string smallestKey;         //The key with the most dashes
    int mostDashes = 0;

    for(map<string, set<string>>::iterator it = wordFamilies.begin(); it != wordFamilies.end(); it++){
        string tempKey = it->first;
        int tempMostDashes = 0;

        for(int n = 0; n < tempKey.size(); n++){
            if (tempKey[n] == '-'){
                ++tempMostDashes;
            }
        }
        if(tempMostDashes > mostDashes){
            mostDashes = tempMostDashes;
            smallestKey = tempKey;
        }
    }
    key = smallestKey;
    wordsRemaining = wordFamilies[smallestKey];
}


// (E2)
// We remove words that contains a characters twice or more in a word.
//This way the user has to guess a more number of characters in order to find a word.
void findWordsWithDiffCharacters(set<string>& wordsRemaining) {
    string charactersUsed = "";
    set<string> tempSet;
    bool multipleChars = false;

    for(set<string>::iterator it = wordsRemaining.begin(); it != wordsRemaining.end(); it++) {
        for(int n=0; n< (*it).size(); n++) {
            if (charactersUsed.find((*it)[n]) == -1) {
                charactersUsed = charactersUsed + (*it)[n];
            }
            else {
                multipleChars = true;
            }
        }
        if (!multipleChars) {
            tempSet.insert((*it));
        }

        //Reset comparison data
        multipleChars = false;
        charactersUsed = "";

    }

    //In case of removing all words, we should keep the words we had before.
    if (tempSet.size() > 0) {
        wordsRemaining = tempSet;
    }

}

//Comparing old key with new key.
bool compareNewOldKey(string& newKey, string& oldKey){
    return (newKey.compare(oldKey) == 0);

}

//Is our current key equal to the last word. If so the user has won the game
bool keyEqualsLastWord(string& key, set<string>& wordsRemaining){
    set<string>::iterator it;
    it = wordsRemaining.begin();
    //Only checks if there is one word left
    if(wordsRemaining.size() == 1){
        if (*it == key){
            return true;
        }
        else{
            return false;
        }
    }
}

// Losing message
void printLosingMessage(){
    cout << "Hahaha, I have won over you!!!" << endl;
}

// Wining message
void printWinningMessage(string& key){
    cout << "You have won over me, the right word was: " << key << endl;
}

// Restart game options
void endPrompt(){
    char answer;
    cout << "Would you like to try again(y/n): ";
    cin >> answer;
    switch(answer){
        case 'y':
            initGame();
            break;
        case 'n':
            break;
        default:
            break;
    }

}

// Make key empty
void makeEmptyKey(int& size, string& key){
    for (int n = 0; n < size; n++) {
        key.append("-");
    }
}

// Retrieving character
void promptGuessLetter(char& letterGuessed, set<char>& guessedLetters){
    while (true) {
        cout << "\nGuess a letter: ";
        cin >> letterGuessed;
        if (checkFormat(letterGuessed, guessedLetters)) {
                guessedLetters.insert(letterGuessed);
                break;
            }
        else {
                cout << "Please type a LETTER" << endl;
            }
    }
}

// Option to see amount of words left.
void promptWordsLeft(set<string>& wordsRemaining){
    string yes_no;
    cout << "Do you want to see amount of words left? y/n: ";
    cin >> yes_no;

    if (yes_no == "y") {
        cout << "There are: " << wordsRemaining.size() << " words left" << endl;
    }
}

// Prints current key
void printKey(string& key){
    cout << "\nCurrent key: " << key << endl;
}

// Our main game loop.
void runGame(set<string> dictionary){
    int size;
    int guesses;
    string key;
    string oldKey;
    char letterGuessed;
    set<string> wordsRemaining;
    set<char> guessedLetters;
    map<string, set<string>> wordFamilies;
    bool filterDuplicateLetterWords = false;

    selectWords(dictionary, wordsRemaining, size);

    makeEmptyKey(size, key);
    oldKey = key;

    cout << "\nHow many guesses do you want?: ";
    cin >> guesses;

    while (guesses != 0 && !keyEqualsLastWord(key,wordsRemaining)) {
        printKey(key);
        cout << "You have " << guesses << " guesses left" << endl;
        promptWordsLeft(wordsRemaining);

        promptGuessLetter(letterGuessed, guessedLetters);
        makeWordFamilies(wordFamilies,letterGuessed, wordsRemaining, size, key);

        if(guesses != 1) {
            findBiggestFamily(wordFamilies, key, wordsRemaining);
        }
        else {
            // E1
            findSmallestKey(wordFamilies, key, wordsRemaining);
        }

        // E2
        // When we have less than 100 words left, we remove word with dupplicate characters.
        // And we do this only once, hence the bool.
        if ((wordsRemaining.size() < 100) && !filterDuplicateLetterWords){
            findWordsWithDiffCharacters(wordsRemaining);
            filterDuplicateLetterWords = true;
        }

        if(compareNewOldKey(key, oldKey)){  //If any changes to key means user has guessed a correct character.
            --guesses;
        }
        oldKey = key;
    }

    if(keyEqualsLastWord(key,wordsRemaining)){
        printWinningMessage(key);
        endPrompt();
    }
    else{
        printLosingMessage();
        endPrompt();
    }
}

void initGame(){
    set<string> dictionary;

    cout << "Loading... \n";
    createDict(dictionary);
    runGame(dictionary);
}

int main(){

    initGame();

    return 0;
}
