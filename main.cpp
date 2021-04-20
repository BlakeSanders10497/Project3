#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Question
{
    enum SearchElement
    {
        AUTHOR, ASSIGNMENT, DEFAULT
    };


    string title, author, date, questionContent, answerContent;
    int assignment, numSubscribe, numImportant, numHelpful;
    SearchElement searchElement = DEFAULT;


    Question(string csvLine)
    {
        /*
         * title = generate_phrase(words, 3)
            author = generate_phrase(words,1)
            assignment =generate_phrase(words,1)
            date = int(time.time()) + randint(-100000, 0)
            message  = generate_phrase(words,12)
            numSubscribe     = randint(1, 50)
            numImportant     = randint(1, 50)
            answer1content   = generate_phrase(words, 12)
            numHelpful       = randint(1, 50)
         */
        stringstream ss(csvLine);
        string tmp;

        getline(ss, title, ',');
        getline(ss, author, ',');

        getline(ss, tmp, ',');
        assignment = stoi(tmp);


        getline(ss, tmp, ',');
        date = stoi(tmp);

        getline(ss, questionContent, ',');

        getline(ss, tmp, ',');
        numSubscribe = stoi(tmp);
        getline(ss, tmp, ',');
        numImportant = stoi(tmp);

        getline(ss, answerContent, ',');

        getline(ss, tmp, ',');
        numHelpful = stoi(tmp);
    }


    bool matchesSearch(string searchTerm)
    {
        switch(searchElement)
        {
            case AUTHOR:
                return author == searchTerm;
            case ASSIGNMENT:
                return assignment == stoi(searchTerm);
            default:
                return false;
        }
    }

    bool operator<(const Question& rhs)
    {
        switch(searchElement)
        {
            case AUTHOR:
                return author < rhs.author;
            case ASSIGNMENT:
                return assignment < rhs.assignment;
            default:
                return false;
        }
    }

};


int main()
{
    bool running = true;

    ifstream file("Data/questions.csv", fstream::in);

    string tmp;
    getline(file, tmp); // Removes headers

    vector<Question> questions;
    while(getline(file, tmp))
    {
        Question question(tmp);
        questions.push_back(question);
    }

    cout << questions.size() << endl;

    while(running)
    {
        string input;
        cout << "\n\n" << endl;
        cout << "-------------------------------------------" << endl;
        cout << "What do you want to search by?" << endl;
        cout << "1 -> Author content" << endl;
        cout << "2 -> Assignment content" << endl;
        cout << "exit -> Exit program" << endl;

        cin >> input;

        try
        {
            int choice = stoi(input);

            switch(choice)
            {
                case 1:
                    for(Question& q : questions)
                    {
                        q.searchElement = Question::AUTHOR;
                    }
                    break;
                case 2:
                    for(Question& q : questions)
                    {
                        q.searchElement = Question::ASSIGNMENT;
                    }
            }

            cout << "What is your search term?" << endl;
            string searchTerm;
            cin >> searchTerm;

            vector<Question> results;
            for(Question& q : questions)
            {
                if(q.matchesSearch(searchTerm)) results.push_back(q);
            }

            cout << "done" << endl;

        }
        catch(exception e) {
            if(input == "exit")
            {
                running = false;
            }
            else cout << "bad input" << endl;
        }
    }

    return 0;
}
