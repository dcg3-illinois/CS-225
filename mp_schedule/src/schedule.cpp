/**
 * @file schedule.cpp
 * Exam scheduling using graph coloring
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <unordered_map>
#include <string>

#include "schedule.h"
#include "utils.h"
#include <algorithm>

using namespace std;

/**
 * Given a filename to a CSV-formatted text file, create a 2D vector of strings where each row
 * in the text file is a row in the V2D and each comma-separated value is stripped of whitespace
 * and stored as its own string. 
 * 
 * Your V2D should match the exact structure of the input file -- so the first row, first column
 * in the original file should be the first row, first column of the V2D.
 *  
 * @param filename The filename of a CSV-formatted text file. 
 */
V2D file_to_V2D(const std::string & filename){
    // Your code here!
    //get the file input in a string
    string fstring = file_to_string(filename);
    vector<string> rowString;
    //make it so that rows contains lines
    SplitString(fstring, '\n', rowString);
    //create the 
    vector<vector<string>> rows;
    rows.resize(rowString.size());
    //split each row on commas
    for(size_t i = 0; i < rowString.size(); i++) {
        SplitString(rowString.at(i), ',', rows.at(i));
        for(size_t k = 0; k < rows.at(i).size(); k++) {
            rows.at(i).at(k) = Trim(rows.at(i).at(k));
        }
    }

    return rows;
}

/**
 * Given a course roster and a list of students and their courses, 
 * perform data correction and return a course roster of valid students (and only non-empty courses).
 * 
 * A 'valid student' is a student who is both in the course roster and the student's own listing contains the course
 * A course which has no students (or all students have been removed for not being valid) should be removed
 * 
 * @param cv A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param student A 2D vector of strings where each row is a student ID followed by the courses they are taking
 */
V2D clean(const V2D & cv, const V2D & student){
    //create map which maps student name to their registered classes
    vector<vector<string>> retVect;
    retVect.resize(cv.size());
    
    map<string, std::vector<string>> studentClasses;
    for(size_t k = 0; k < student.size(); k++) {
        //store student name
        string sName = student.at(k).at(0);
        //insert name as key and vector as value
        studentClasses.insert({sName, student.at(k)});
        //remove name from vector
        studentClasses[sName].erase(studentClasses[sName].begin());
    }
    
    for(int i = (cv.size() - 1); i >= 0; i--) {
        //store the classname
        string cName = cv.at(i).at(0);

        retVect.at(i).push_back(cName);
        //traverse through every student in class
        for(size_t j = 1; j < cv.at(i).size(); j++) {
            string studName = cv.at(i).at(j);
            //if student exists
            if(studentClasses[studName].size() != 0) {
                if(std::find(studentClasses[studName].begin(), 
                    studentClasses[studName].end(), cName) != studentClasses[studName].end()) {
                    
                    retVect.at(i).push_back(studName);
                }
            }
        }
        //if there are no students (sad) then begone
        if(retVect.at(i).size() == 1) {
            retVect.erase(retVect.begin()+i);
        }
    }

    return retVect;
}

/**
 * Given a collection of courses and a list of available times, create a valid scheduling (if possible).
 * 
 * A 'valid schedule' should assign each course to a timeslot in such a way that there are no conflicts for exams
 * In other words, two courses who share a student should not share an exam time.
 * Your solution should try to minimize the total number of timeslots but should not exceed the timeslots given.
 * 
 * The output V2D should have one row for each timeslot, even if that timeslot is not used.
 * 
 * As the problem is NP-complete, your first scheduling might not result in a valid match. Your solution should 
 * continue to attempt different schedulings until 1) a valid scheduling is found or 2) you have exhausted all possible
 * starting positions. If no match is possible, return a V2D with one row with the string '-1' as the only value. 
 * 
 * @param courses A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param timeslots A vector of strings giving the total number of unique timeslots
 */
V2D schedule(const V2D &courses, const std::vector<std::string> &timeslots){
    // Your code here!
    vector<vector<int>> neighbors = adjacencyMatrix(courses);

    //create a vector to indicate which color each class has and resize it
    vector<vector<string>> classesAtTime;
    classesAtTime.resize(timeslots.size());

    //fill each vector with the time name for the first value
    for(size_t i = 0; i < classesAtTime.size(); i++) {
        classesAtTime.at(i).push_back(timeslots.at(i));
    }

    //go through each class as the first time slot
    for(size_t class1 = 0; class1 < courses.size(); class1++) {
        //flag to indicate whether or not a combination has succeeded
        bool failed = false;

        //create a map that maps class indices to their time index + 1
        //0 indicates that it has been unassigned
        std::map<int, int> nodeTime;
        nodeTime.clear();
        
        //give the first class the earliest time slot
        nodeTime[class1] = 1;

        //loop through every other class in the adjancency matrix
        for(size_t next = class1 + 1; next < class1+ courses.size(); next++) {
            //used to indicate whether or not a time for this class has been found
            bool found = false;
            
            //get the index of the next class we need to check
            size_t nextClass = next%courses.size();
            
            //***DETERMINE THE LOWEST INDEX TIME NO NEIGHBORS SHARE***//

            //a vector to help us determine the smallest time not yet used by neighbors
            vector<int> determineSmallest;
            determineSmallest.clear();
            determineSmallest.resize(timeslots.size());

            //iterate through the neighbors of the class we're trying to find a spot for
            for(int neighbor : neighbors.at(nextClass)) {
                //if a neighbor already has a time, indicate which in the determineSmallest vector
                if(nodeTime[neighbor] != 0) {
                    //times.push_back(nodeTime[neighbor] - 1);
                    determineSmallest.at(nodeTime[neighbor] - 1) = 1;
                }
            }

            //set this node to the smallest available color
            for(size_t k = 0; k < determineSmallest.size(); k++) {
                //this means that this is the smallest time not occupied by neighbors
                if(determineSmallest.at(k) != 1) {
                    nodeTime[nextClass] = k + 1;
                    //show that we've found a spot for this class
                    found = true;
                }
            }
            //if we are unable to find a spot for any class, end this loop and continue
            //to the next starting node
            if(!found) { 
                failed = true;
                break;
            }
        }
        //if we were able to place every class somewhere, we are done!
        if(!failed) {
            //key is the index of the class, value is the index in time + 1
            for(std::pair<int, int> keyVal : nodeTime) {
                classesAtTime.at(keyVal.second - 1).push_back(courses.at(keyVal.first).at(0));
            }
            return classesAtTime;
        }
    }

    V2D retVect;
    retVect.resize(1);
    retVect.at(0) = {"-1"};
    return retVect;
}

// cleanCourses has className at 0 index of each interior vector and a list of students following 
vector<vector<int>> adjacencyMatrix(const V2D& cleanCourses) {
    //neighbors will contain the indices of neighbors for each node
    vector<vector<int>> neighbors;

    //resize neighbors vector
    neighbors.resize(cleanCourses.size());

    //loop through every class
    for(size_t row = 0; row < cleanCourses.size(); row++) {
        for(size_t otherClass = 0; otherClass < cleanCourses.size(); otherClass++) {
            for(size_t col = 1; col < cleanCourses.at(row).size(); col++) {
                if(otherClass != row && std::find(cleanCourses.at(otherClass).begin(), cleanCourses.at(otherClass).end(), 
                                        cleanCourses.at(row).at(col)) != cleanCourses.at(otherClass).end()) {
                    neighbors.at(row).push_back(otherClass);
                    //break because we don't want duplicates
                    break;
                }
            }
        }          
    }

    return neighbors;
}