/**
 * @file common_words.cpp
 * Implementation of the CommonWords class.
 *
 * @author Zach Widder
 * @date Fall 2014
 */

#include "common_words.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

using std::string;
using std::vector;
using std::ifstream;
using std::cout;
using std::endl;
using std::feof;

string remove_punct(const string& str)
{
    string ret;
    std::remove_copy_if(str.begin(), str.end(), std::back_inserter(ret),
                        [](int c) {return std::ispunct(c);});
    return ret;
}

CommonWords::CommonWords(const vector<string>& filenames)
{
    // initialize all member variables
    init_file_word_maps(filenames);
    init_common();
}

void CommonWords::init_file_word_maps(const vector<string>& filenames)
{
    // make the length of file_word_maps the same as the length of filenames
    file_word_maps.resize(filenames.size());

    // go through all files
    for (size_t i = 0; i < filenames.size(); i++) {
        // get the corresponding vector of words that represents the current
        // file
        vector<string> words = file_to_vector(filenames[i]);
        /* Your code goes here! */
        //at this point, words contains all of the words in a file
                
        //fancy for each loop
        for(std::string & word : words) {
            //check to see if this word is already there
            map<std::string, unsigned int>::iterator relevant = file_word_maps.at(i).find(word);

            //if it's not, set it equal to 1
            if(relevant == file_word_maps.at(i).end()) {
                file_word_maps.at(i).insert( {word, 1} );
            }
            //else increment it
            else{
                file_word_maps.at(i)[word] += 1;
            }
        }
    }
}

void CommonWords::init_common()
{
    /* Your code goes here! */
    //loop through each map
    for(map<string, unsigned int> & shmap : file_word_maps) {
        //loop through each word in shmap
        for(std::pair<const string, unsigned int> & word_p : shmap) {
            //check to see if this word is already in common
            string f = word_p.first;
            std::map<std::string, unsigned int>::iterator found = common.find(f);

            if(found == common.end()) {
                common.insert( {word_p.first, 1} );
            }
            else {
                common[word_p.first] += 1;
            }
        }
    }
}

/**
 * @param n The number of times to word has to appear.
 * @return A vector of strings. The vector contains all words that appear
 * in each file >= n times.
 */
vector<string> CommonWords::get_common_words(unsigned int n) const
{
    vector<string> out;
    /* Your code goes here! */
    //we know that common contains all of the words in all of the files
    for(const std::pair<const string, unsigned int> & word_p : common) {
        //store how many times this word has appeared in files
        unsigned int count = 0;
        string word = word_p.first;
        //loop through each map
        for(const std::map<std::string, unsigned int> & shmap : file_word_maps) {
            
            //check if it's in map
            std::map<std::string, unsigned int>::const_iterator found = shmap.find(word_p.first);

            //if it's there, we add how many times it occurs to count
            if(found != shmap.end() && (*found).second >= n) {
                count += 1;
            }
        }
        if(count == common.at(word)) {
            out.push_back(word);
        }

    }
    return out;
}

/**
 * Takes a filename and transforms it to a vector of all words in that file.
 * @param filename The name of the file that will fill the vector
 */
vector<string> CommonWords::file_to_vector(const string& filename) const
{
    ifstream words(filename);
    vector<string> out;

    if (words.is_open()) {
        std::istream_iterator<string> word_iter(words);
        while (!words.eof()) {
            out.push_back(remove_punct(*word_iter));
            ++word_iter;
        }
    }
    return out;
}
