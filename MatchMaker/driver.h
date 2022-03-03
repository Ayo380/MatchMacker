
#include <math.h>
#include <limits>
#include <string>
#include <map>
#include <set>
#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
  string questionText;    // Text of the question
  map<char, int> factors;  // Map from factors to +1 or -1
  friend bool operator<(const Question &lhs, const Question &rhs) {
    return lhs.questionText < rhs.questionText;
  }
  friend bool operator==(const Question &lhs, const Question &rhs) {
    return lhs.questionText == rhs.questionText;
  }
  friend bool operator!=(const Question &lhs, const Question &rhs) {
    return lhs.questionText != rhs.questionText;
  }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
  string name;           // Name of the person
  map<char, int> scores;  // Map from factors to +1 or -1
  friend bool operator<(const Person &lhs, const Person &rhs) {
    return lhs.name < rhs.name;
  }
  friend bool operator==(const Person &lhs, const Person &rhs) {
    return lhs.name == rhs.name;
  }
  friend bool operator!=(const Person &lhs, const Person &rhs) {
    return lhs.name != rhs.name;
  }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
 */
Question randomElement(set<Question> &questions) {
  int ind = randomInteger(0, (int)questions.size() - 1);
  int i = 0;
  for (auto e : questions) {
    if (i == ind) {
      return e;
    }
    i++;
  }
  return {};
}

/* randomQuestionFrom
 *
 * This function returns a Question from the question struct and
 * and returns the question. And randomly remove the question
 * from the set.
 */
Question randomQuestionFrom(set<Question> &questions) {
  if (questions.size() == 0) {
    throw runtime_error("set: empty");
  }
  Question q = randomElement(questions);
  questions.erase(q);
  return q;
}

/* scoresFrom
 *
 * This function selects, recieve a map of the users answer
 * as an argument. Computes each answer(factor) into scores
 * and set them to their respective factors character.
 * This function returns map of scores with the factors characters
 * as key to the scores(int).
 *
 */
map<char, int> scoresFrom(map<Question, int> &answers) {
  map<char, int> score;
  int each_score;
  for (auto &e : answers) {
    // subtract 3 from the answer from the user
    each_score = e.second - 3;
    for (auto &a : e.first.factors) {
      // multiply it with the factor
      score[a.first] += a.second * each_score;
      each_score = e.second - 3;
    }
  }
  return score;
}

/* normalize
 *
 * This function normalizes, by squaring up each factor scores and
 * taking the square root of them. And returns their normalized score
 * as a double.
 */
map<char, double> normalize(map<char, int> &scores) {
  double norm = 0;
  map<char, double> normal;
  // Squaring up each scores
  for (auto a : scores) {
    norm += (a.second * a.second);
  }
  norm = sqrt(norm);
  if (norm == 0) {
    throw runtime_error("score is empty");
  } else {
    for (auto a : scores) {
      normal[a.first] = a.second / norm;
    }
  }
  return normal;
}

/* cosineSimilarityOf
 *
 * This function selects, this function take two normalized
 * score maps as an arguement, and calculates their similarities
 * by multiplying thier normalized score of each facotor and returning
 * it as a double.
 */
double cosineSimilarityOf(const map<char, double> &lhs,
                          const map<char, double> &rhs) {
  double similarity = 0;
  for (auto e : lhs) {
    // check if the key exist on the other side
    if (rhs.count(e.first)) {
      similarity += e.second * rhs.at(e.first);
    }
  }
  return similarity;
}

/* mostSimilarTo
 *
 * This function functions returns the person with the most
 * similarities, function uses normalize, cosineSimilarityOf to get
 * the similarities.
 */
Person mostSimilarTo(map<char, int> &scores, set<Person> &people) {
   struct Person p;
  if (people.empty()) {
    throw runtime_error("People is empty");
  }
      double temp = -999;
     
      double val;
      map <char, double> people_score;
      map<char, double> question_score = normalize(scores);
    for (auto a : people) {
        people_score = normalize(a.scores);
        val = cosineSimilarityOf(question_score, people_score);
      // Trying to get the largest similarities
      if (val > temp) {
         temp =  val;
           p = a;
      }
    }
  
  return p;
}
