#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;


// from https://www.learncpp.com/cpp-tutorial/8-16-timing-your-code/
class Timer
{
private:
    // Type aliases to make accessing nested type easier
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;

    std::chrono::time_point<clock_t> m_beg;

public:
    Timer() : m_beg(clock_t::now())
    {
    }

    void reset()
    {
        m_beg = clock_t::now();
    }

    double elapsed() const
    {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }
};

struct Question
{
    enum SearchElement
    {
        AUTHOR, ASSIGNMENT, SEARCH_DEFAULT
    };
    enum SortElement
    {
        SUBSCRIBE, IMPORTANT, HELPFUL, SORT_DEFAULT
    };


    string title, author, date, questionContent, answerContent;
    int assignment, numSubscribe, numImportant, numHelpful;

    SearchElement searchElement = SEARCH_DEFAULT;
    SortElement sortElement = SORT_DEFAULT;


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
        switch(sortElement)
        {
            case SUBSCRIBE:
                return numSubscribe < rhs.numSubscribe;
            case IMPORTANT:
                return numImportant < rhs.numImportant;
            case HELPFUL:
                return numHelpful < rhs.numHelpful;
        }
    }
    bool operator>(const Question& rhs)
    {
        switch(sortElement)
        {
            case SUBSCRIBE:
                return numSubscribe > rhs.numSubscribe;
            case IMPORTANT:
                return numImportant > rhs.numImportant;
            case HELPFUL:
                return numHelpful > rhs.numHelpful;
        }
    }
    bool operator==(const Question& rhs)
    {
        switch(sortElement)
        {
            case SUBSCRIBE:
                return numSubscribe == rhs.numSubscribe;
            case IMPORTANT:
                return numImportant == rhs.numImportant;
            case HELPFUL:
                return numHelpful == rhs.numHelpful;
        }
    }
    bool operator!=(const Question& rhs) { return !(operator==(rhs)); }

};


void quickSort(vector<Question>& array, int low, int high);
int partition(vector<Question>& array,int low, int high);

void shellSort(vector<Question>& array);


bool compare(vector<Question>& v1, vector<Question>& v2)
{
    if(v1.size() != v2.size())
    {
        cout << "Size of arrays don't match!!!" << endl;
        return false;
    }

    for(int i = 0; i < v1.size(); i++)
    {
        if(v1[i] != v2[i]) return false;
    }

    return true;
}

int main()
{

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

    while(true)
    {
        string input;
        cout << "\n\n" << endl;
        cout << "-------------------------------------------" << endl;
        cout << "What do you want to search by?" << endl;
        cout << "1 -> Author content" << endl;
        cout << "2 -> Assignment content" << endl;
        cout << "exit -> Exit program" << endl;

        cin >> input;

        if(input == "exit") break;

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

        cout << "What would you like to sort by?" << endl;
        cout << "1 -> subscribe" << endl;
        cout << "2 -> important" << endl;
        cout << "3 -> helpful" << endl;

        string sSortTerm;
        cin >> sSortTerm;
        int sortTerm = stoi(sSortTerm);
        for(Question& q : results)
        {
            switch(sortTerm)
            {
                case 1:
                    q.sortElement = Question::SUBSCRIBE;
                    break;
                case 2:
                    q.sortElement = Question::IMPORTANT;
                    break;
                case 3:
                    q.sortElement = Question::HELPFUL;
                    break;
            }
        }

        std::sort(results.begin(), results.end());

        vector<Question> resultsQuicksort = results;
        quickSort(resultsQuicksort, 0, resultsQuicksort.size() - 1);

        vector<Question> resultsShellSort = results;
        shellSort(resultsShellSort);

        cout << compare(results, resultsQuicksort) << endl;
        cout << compare(resultsQuicksort, resultsShellSort) << endl;

    }

    return 0;
}

void quickSort(vector<Question>& array, int low, int high)
{
    if(low < high)
    {
        // Index of partitioning
        int p = partition(array, low, high);
        quickSort(array, low, p);
        quickSort(array, p + 1, high);
    }
}
int partition(vector<Question>& array,int low, int high)
{
    Question& pivot = array[(high + low) / 2];
    int i  = low - 1;
    int j = high + 1;

    while(true)
    {
        do
        {
            i++;
        } while(array[i] < pivot);

        do
        {
            j--;
        } while(array[j] > pivot);

        if(i >= j) return j;
        swap(array[i], array[j]);
    }
}

void shellSort(vector<Question>& array)
{
    vector<int> gaps = {1750, 701, 301, 132, 57, 23, 10, 4, 1}; // Credit to Marcin Ciura for the gaps; https://en.wikipedia.org/wiki/Shellsort

    for(int gap : gaps)
    {
        // Gapped insertion sort
        for(int i = gap; i < array.size(); i++)
        {
            Question& temp = array[i];

            int j = i;
            for(; j >= gap; j -= gap)
            {
                if(array[j - gap] < temp) break;

                array[j] = array[j - gap];
            }

            array[j] = temp;
        }
    }
}
