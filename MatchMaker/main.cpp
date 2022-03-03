
#include "driver.h"
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
// Function #1: Load_Question *Helper function*
// Parameters: set<Question>& q and string s
// This function get a string from Read_Question
// parse get teach characters in their appropriate data structure
void Load_Question(set<Question>& q, string s) {
    int i = 0;
    int j = 0;
    Question questions;
    map<char, int> factor;
    string question;
    // parse through until it encounters "."
    while (s[i] != '.') {
        if (s[i] == '.') {
            question += s[i];
            break;
        }
        question += s[i];
        i++;
    }
    questions.questionText = question + '.';
    j = i + 1;
    string numbers;
    // parse through until the end of the line
    while (s[j] != '\0') {
        // checkes for negative factors
        if (s[j + 1] == ':' && s[j + 2] == '-') {
            factor.emplace(s[j], (s[j + 3] - '0') * -1);
        }
        else if (s[j + 1] == ':') {
            factor.emplace(s[j], s[j + 2] - '0');
        }
        j++;
    }
    // insert data of struct question into the set
    questions.factors = factor;
    q.insert(questions);
}
// Function #2: Read_Question
// Parameters: set<Question>& p
// This function opens the file get each line from the questions.txt file
// then it loads it into the helper function "Load_Question".
void Read_Question(set<Question>& q) {

    ifstream openfile;
    openfile.open("questions.txt");
    string file_data;
    if (openfile.is_open()) {
        while (getline(openfile, file_data)) {
            if (!openfile.fail()) {
                Load_Question(q, file_data);
            }
        }
    }
    else {
        cout << "could not open file" << endl;
    }
    openfile.close();
}

// Function : Menu()
// It contains the menu for the questions
void Menu() {
    cout << "1. Strongly disagree" << endl;
    cout << "2. Disagree" << endl;
    cout << "3. Neutral" << endl;
    cout << "4. Agree" << endl;
    cout << "5. Strongly agree" << endl << endl;
    cout << "Enter your answer here (1-5): ";
}

int main() {
    // TODO:  Write your code here.  You should have lots of functions.
    set<Question> Q;
    set<Person> p;
    vector <Question> questions_asked;// to hold the question asked
    Question q;
    vector<map<Question, int>> people_answers;// to hold each person answers
    vector< map<char, int>> people_score;// to hold people score
    map<char, int> Scores;
    map<Question, int > answers;
    map<char, double> normal;
    vector<map<char, double>> norm;
    int questions;
    int user_answers;
    int people = 0;
    int num_questions;
    string Person1, Person2;
    vector<string> person;
    cout << "Welcome to the Personality Quiz!" << endl << endl;
    cout << "Choose number of questions: ";
    cin >> questions;
    cout << "Name of the first person: ";
    cin >> Person1;
    cout << "Name of the second person: ";
    cin >> Person2;
    person.push_back(Person1);
    person.push_back(Person2);
    Read_Question(Q);
    num_questions = questions;
    // Stores the questions in a set so that the same question can be asked to everyone
    while (num_questions > 0) {
        q = randomQuestionFrom(Q);
        questions_asked.push_back(q);
        num_questions--;
    }
    while (people < 2) {
        int i = 0;
        cout << endl << person[people] << " answer the question " << endl;
        // asking the question to each person
        for (auto e : questions_asked) {
            char c = '"';
            cout << "How much do you agree with this statement?";
            cout << endl << c << e.questionText << c << endl << endl;
            Menu();
            cin >> user_answers;
            answers.emplace(e, user_answers);
            i++;
            if (i == questions)
            {
                break;
            }
        }
        //saving their answers
        people_answers.push_back(answers);
        people++;
    }
    //compute the score for each person
    for (auto e : people_answers) {
        Scores = scoresFrom(e);
        norm.push_back(normalize(Scores));
    }
    int number; 
    int i = 0;
    number = (int)cosineSimilarityOf(norm[i], norm[i + 1])*100;
    cout << person[0] << " is " << number << "% compatible with " << person[1];
    return 0;
}

