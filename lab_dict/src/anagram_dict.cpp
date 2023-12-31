/**
 * @file anagram_dict.cpp
 * Implementation of the AnagramDict class.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "anagram_dict.h"

#include <algorithm> /* I wonder why this is included... */
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;

/**
 * Constructs an AnagramDict from a filename with newline-separated
 * words.
 * @param filename The name of the word list file.
 */
AnagramDict::AnagramDict(const string& filename)
{
    /* Your code goes here! */

    //gameplan: for each word, make a copy
    //sort the copy
    //place the word in the vector behind the key of sorted 

    ifstream wordsFile(filename);
    string word;
    if (wordsFile.is_open()) {
        /* Reads a line from `wordsFile` into `word` until the file ends. */
        while (getline(wordsFile, word)) {
            string temp = word;
            //temp is sorted
            sort(temp.begin(), temp.end());
            
            if(dict.find(temp) == dict.end()) {
                vector<string> tempV;
                tempV.push_back(word);
                dict.insert( { temp, tempV });
            }
            else {
                dict.at(temp).push_back(word);
            }  
        }
    }
}

/**
 * Constructs an AnagramDict from a vector of words.
 * @param words The vector of strings to be used as source words.
 */
AnagramDict::AnagramDict(const vector<string>& words)
{
    /* Your code goes here! */
    for(const string & word : words) {
        string temp = word;
        //temp is sorted
        sort(temp.begin(), temp.end());
        
        if(dict.find(temp) == dict.end()) {
            vector<string> tempV;
            tempV.push_back(word);
            dict.insert( { temp, tempV });
        }
        else {
            dict.at(temp).push_back(word);
        }
    }
}

/**
 * @param word The word to find anagrams of.
 * @return A vector of strings of anagrams of the given word. Empty
 * vector returned if no anagrams are found or the word is not in the
 * word list.
 */
vector<string> AnagramDict::get_anagrams(const string& word) const
{
    /* Your code goes here! */
    string temp = word;
    sort(temp.begin(), temp.end());
    
    if(dict.find(temp) == dict.end()) {
        return vector<string>();
    }
    else {
        return dict.at(temp);
    }
    
}

/**
 * @return A vector of vectors of strings. Each inner vector contains
 * the "anagram siblings", i.e. words that are anagrams of one another.
 * NOTE: It is impossible to have one of these vectors have less than
 * two elements, i.e. words with no anagrams are ommitted.
 */
vector<vector<string>> AnagramDict::get_all_anagrams() const
{
    /* Your code goes here! */
    vector<vector<string>> ret;

    for(const std::pair<const string, vector<string>> & annies : dict) {
        if(annies.second.size() > 1) {
            ret.push_back(annies.second);
        }
    }

    return ret;
}
