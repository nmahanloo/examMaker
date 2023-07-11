// NimaMahanloo_qzOOP.cpp
// CISP 400 - Extra Credit HW
// FLC - Professor Fowler
#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <string>
#include <cassert>
#include <iomanip>
using namespace std;
// Program structure as a node
struct qSet
{
        qSet();
        string question;
        string A;
        string B;
        string C;
        string D;
        char key;
        char answer;
        double point;
        int qNum;
        bool displayed;
        qSet* next;
        qSet* prev;
};
// qSet structure default construction
qSet::qSet()
{
    question = "N/A";
    A = "N/A";
    B = "N/A";
    C = "N/A";
    D = "N/A";
    key = '?';
    answer = '?';
    point = 1;
    qNum = 1;
    displayed = false;
    next = nullptr;
    prev = nullptr;
}
// Program main class
class qTester
{
    public:
        void loadQSet(qSet*&);
        void pushQSet(qSet*&, string);
        double testTime(qSet*);
        bool testCounter(clock_t&, clock_t, double&, double);
        void convertTime(double, int&, int&);
        double randomSet(qSet*&, qSet*, vector<int>&, double);
        void doExam(qSet*&, int);
        void displayQue(qSet*, int);
        void result(qSet*, vector<int>, double);
        void display(qSet*);
};
// Feature 2 : Randomized multiple choices order
// A module to push inserted data from input file to linked list in randomized multiple choice order
void qTester::pushQSet(qSet* (&last), string line)
{
    bool filled = false;
    int fillChoice = -1;
    if ((last->A != "N/A") && (last->B != "N/A") && (last->C != "N/A") && (last->D != "N/A"))
    {
        last->next = new qSet;
        last->next->prev = last;
        last->next->qNum = last->qNum + 1;
        last = last->next;
    }
    if (line[line.size()-1] == '?')
    {
        last->question = line;
    }
    else
    {
        while (filled == false)
        {
            fillChoice = rand() %4;
            if ((fillChoice == 0) && (last->A == "N/A"))
            {
                if (line[0] == '*')
                {
                    line = line.substr(1, line.size()-1);
                    last->key = 'A';
                }
                last->A = line;
                filled = true;
            }
            else if ((fillChoice == 1) && (last->B == "N/A"))
            {
                if (line[0] == '*')
                {
                    line = line.substr(1, line.size()-1);
                    last->key = 'B';
                }
                last->B = line;
                filled = true;
            }
            else if ((fillChoice == 2) && (last->C == "N/A"))
            {
                if (line[0] == '*')
                {
                    line = line.substr(1, line.size()-1);
                    last->key = 'C';
                }
                last->C = line;
                filled = true;
            }
            else if ((fillChoice == 3) && (last->D == "N/A"))
            {
                if (line[0] == '*')
                {
                    line = line.substr(1, line.size()-1);
                    last->key = 'D';
                }
                last->D = line;
                filled = true;
            }
        }
    }
}
// Feature 1 : Load data from input file
// A module to read question set data from input file
void qTester::loadQSet(qSet* (&last))
{
    string line;
    ifstream readFile ("question.txt");
    if (readFile.is_open())
    {
        while (!(readFile.eof()))
        {
            getline(readFile, line);
            if (line.size() > 0)
            {
                pushQSet(last, line);
            }
        }
    }
    else
    {
        cout << "Error: File is not open!" << endl << endl;
    }
    readFile.close();
}
// A module to display a current question data from linked list
void qTester::displayQue(qSet* curr, int dNum)
{
    cout << dNum << ". " << curr->question << endl
    << "A) " << curr->A << endl << "B) " << curr->B << endl
    << "C) " << curr->C << endl << "D) " << curr->D << endl;
}
// Feature 5 : Time counter and time limitation
// A double function to calculate test time limit
double qTester::testTime(qSet* last)
{
    // Seconds per question
    int questionTime = 54;
    double testDuration = questionTime * last->qNum;
    return testDuration;
}
// Program greeting and interface
// A module to make the quiz
void qTester::doExam(qSet* (&curr), int dNum)
{
    char ans = '?';
    bool error = false;
    displayQue(curr, dNum);
    while ((ans != 'A') && (ans != 'a') && (ans != 'B') && (ans != 'b') && (ans != 'C') && (ans != 'c') && (ans != 'D') && (ans != 'd'))
    {
        if (error == true)
        {
            cout << "Error: Invalid choice!" << endl;
        }
        cout << "Enter your choice > ";
        cin >> ans;
        error = true;
    }
    // Assert feature as backup input validation
    assert((ans == 'A') || (ans == 'a') || (ans == 'B') || (ans == 'b') || (ans == 'C') || (ans == 'c') || (ans == 'D') || (ans == 'd'));
    cout << endl;
    if (ans == 'a')
    {
        ans = 'A';
    }
    if (ans == 'b')
    {
        ans = 'B';
    }
    if (ans == 'c')
    {
        ans = 'C';
    }
    if (ans == 'd')
    {
        ans = 'D';
    }
    curr->answer = ans;
}
//A boolean function to check test passed time with time limitation
bool qTester::testCounter(clock_t &timePassed, clock_t testStart, double &secondsPassed, double testDuration)
{
    bool deadTime = false;
    timePassed = clock() - testStart;
    secondsPassed = timePassed / (double)CLOCKS_PER_SEC;
    if (secondsPassed >= testDuration)
        {
            deadTime = true;
            cout << "Test time finished!" << endl;
        }
    return deadTime;
}
// A module to convert seconds to minutes and seconds
void qTester::convertTime(double totalSec, int &minutes, int &seconds)
{
    minutes = totalSec / 60;
    seconds = totalSec - (minutes * 60);
    if (seconds > 59)
    {
        minutes++;
        seconds -= 60;
    }
}
// Feature 3 : Display questions and make quiz in randomized orders
// A module to display questions and make the quiz in randomize order set
double qTester::randomSet(qSet* (&first), qSet* last, vector <int> (&thisSet), double testDuration)
{
    cout << "Test >" << endl << endl;
    int randCounter, selectedQue, minutes, seconds, dNum = 1;
    int setSize = last->qNum;
    qSet* curr;
    double secondsPassed;
    clock_t testStart = clock();
	clock_t timePassed;
    bool deadTime = false;
    convertTime(testDuration, minutes, seconds);
    cout << "Test time limit : " << minutes << " min " << seconds << " sec" << endl << endl;
    for (randCounter = 0;(randCounter < setSize) && (deadTime == false); randCounter++)
    {
        retry :
        deadTime = testCounter(timePassed, testStart, secondsPassed, testDuration);
        curr = first;
        selectedQue = rand() %setSize + 1;
        while ((curr->qNum != selectedQue) && (curr->next != nullptr))
        {
            curr = curr->next;
        }
        if (curr->displayed == true)
        {
            goto retry;
        }
        else
        {
            thisSet.push_back(curr->qNum);
            curr->displayed = true;
            deadTime = testCounter(timePassed, testStart, secondsPassed, testDuration);
            convertTime(testDuration - secondsPassed, minutes, seconds);
            cout << "Available time : " << minutes << " min " << seconds << " sec" << endl << endl;
            doExam(curr, dNum);
            dNum++;
            deadTime = testCounter(timePassed, testStart, secondsPassed, testDuration);
        }
    }
    return secondsPassed;
}
// Feature 4 : Calculate total score as questions points summary, percentage and grade letter
//  A module to calculate grade and show result
void qTester::result(qSet* first, vector <int> thisSet, double finishTime)
{
    cout << "Result >" << endl << endl;
    int counter, i, minutes, seconds;
    bool ansFlag;
    double scorePerc;
    double score = 0, total = 0;
    char grade = '?';
    int queSize = thisSet.size();
    qSet* curr;
    for (counter = 0; counter < queSize; counter++)
    {
        curr = first;
        while((curr->qNum != thisSet.at(counter)) && (curr != nullptr))
        {
            curr = curr->next;
        }
        displayQue(curr, counter+1);
        cout << "Correct answer : " << curr->key << endl
        << "Your answer : " << curr->answer << endl << endl;
        if (curr->answer == curr->key)
        {
            score += curr->point;
        }
        total += curr->point;
    }
    curr = first;
    while (curr != nullptr)
    {
        ansFlag = false;
        for (i = 0; i < queSize; i++)
        {
            if (curr->qNum == thisSet.at(i))
            {
                ansFlag = true;
            }
        }
        if (ansFlag == false)
        {
            counter++;
            displayQue(curr, counter);
            cout << "Correct answer : " << curr->key << endl
            << "Your answer : " << curr->answer << endl << endl;
            total += curr->point;
        }
        curr = curr->next;
    }
    scorePerc = (score * 100) /  total;
    if (scorePerc >= 90)
    {
        grade = 'A';
    }
    else if (scorePerc >= 80)
    {
        grade = 'B';
    }
    else if (scorePerc >= 70)
    {
        grade = 'C';
    }
    else if (scorePerc >= 60)
    {
        grade = 'D';
    }
    else
    {
        grade = 'F';
    }
    convertTime(finishTime, minutes, seconds);
    cout << "Your Score is : " << score << " from " << total << endl
    << "%" << fixed << setprecision(2) << scorePerc << " (" << grade << ")" << endl
    << "Test duration : " << minutes << " min " << seconds << " sec" << endl;
}

int main()
{
    vector <int> thisSet;
    double testDuration, finishTime;;
    qSet* questionSet = new qSet;
    qSet* first = questionSet;
    qSet* last = first;
    qTester test;
    srand(time(0));
    test.loadQSet(last);
    testDuration = test.testTime(last);
    cout << "<Quiz Tester OOP>" << endl << endl;
    if (last->key != '?')
    {
        finishTime = test.randomSet(first, last, thisSet, testDuration);
        test.result(first, thisSet, finishTime);
    }
    return 0;
}
