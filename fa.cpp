#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>
#include <regex>
using namespace std;

//Takes a pointer to the array of final states, the input file's contents, and the number of states
//Returns nothing; instead directly saves to the array of final states
bool* getFinals(vector<string> inputStrings, int stateCount)
{
    string temp = "";
    //Create boolean array
    bool* FINAL = new bool[stateCount]();

    //This sets the everything in the array to false. 
    //I don't know if this is necessary but I did it to be safe
    for (int i=0; i<stateCount-1; i++)
    {
        FINAL[i] = false;
    }
    //Mark states as final or not, based on second line of input file
    for (int i=0; i<inputStrings[1].length()+1; i++)
    {
        char current = inputStrings[1][i];
        if (isdigit(current))
        {
            temp += current;
        }
        else
        {
            int finalSt = stoi(temp);
            FINAL[finalSt] = true;
            temp = "";
        }
    }

    return FINAL;
}

//Takes the input file's contents
//Returns the FA alphabet, by reading its third line
vector<int> getAlphabet(vector<string> inputStrings)
{
    vector<int> alphabet;
    //Reads the third line of the input file, pushes every non-space character to alphabet
    for (int i=0; i<inputStrings[2].length(); i++)
    {
        char current = inputStrings[2][i];
        if (current != ' ')
        {
            alphabet.push_back(current);
        }
    } 
    return alphabet;
}

//Takes the input file's contents and a pointer to int
//Returns a map of transitions, and saves the last index (i) to the given int pointer
map<pair<int,string>, int> getTransitions(vector<string> inputStrings, int* endIndex)
{
    map<pair<int,string>, int> transitions;

    int i;
    for (i=3; i<inputStrings.size(); i++)
    {
        int len = inputStrings[i].length();
        //checks if line is NOT of format "(0 x 1)" where 0 and 1 are digits and x is a string
        if (inputStrings[i][0] != '(' || !isdigit(inputStrings[i][1]) || 
            inputStrings[i][2] != ' ' || inputStrings[i][len-3] != ' ' || 
            !isdigit(inputStrings[i][len-2]) || inputStrings[i][len-1] != ')')
        {
            break;
        } 
        else { //given the format "(0 x 1)", keys are 0,x and values are 1
            string keyStr = "";
            for (int j=3; j<(len-3); j++)
            {
                keyStr += inputStrings[i][j];
            }
            pair<int, string> key = make_pair(inputStrings[i][1], keyStr);
            transitions[key] = inputStrings[i][len-2];
            cout << "\t"; 
            cout << inputStrings[i][1] << ' ' << keyStr << " " << inputStrings[i][len-2];
            cout << '\n';
        }
    } 

    *endIndex = i; //This can be retrieved outside of this function

    return transitions;
}


int nextState(int currentState, string symbol, map<pair<int,string>,int> transitions)
{
    int nextState;
    pair<int,string> inKey;

    for (const auto &entry: transitions)
    {
        pair<int,string> outKey = entry.first;
        if (outKey.first != ('0'+currentState))
        {
            continue;
        //if outKey.second is character
        } else if (outKey.second.length()==1) {
            try{
                inKey = make_pair(('0'+currentState), symbol);
                return (int)transitions.at(inKey)-48;
            }
            catch (const out_of_range& e) {
                continue;
            }

        //if outKey.second is regex
        } else if (outKey.second.length()>1) {
            //and the symbol matches the regex
            if (regex_match(symbol, regex(outKey.second)))
            {
                //return the value corresponding to the regex
                inKey = make_pair(('0'+currentState), outKey.second);
                return (int)transitions.at(inKey)-48;
            } else {
                continue;
            }
        }
    }

    //this is only reached if no match is found
    return -1;
}


//printInfo does NOT handle printing the actual transitions list
//transitions are handled in getTransitions
//this is because it would be really annoying to do it here
void printInfo(int stateCount, bool *FINAL, vector<int> alphabet)
{
    cout << "Inputted Finite State Automaton Info:\n";
    cout << "  1) set of states: {";
    for (int i=0; i<stateCount; i++)
    {
        cout << "state " << i;
        if (i!=stateCount-1)
        {
            cout << ", ";
        }
    }
    cout << "}, initial state is state 0 (default).\n";


    cout << "  2) set of final state(s): {";
    vector<int> printFinal;
    for (int i=0; i<stateCount; i++)
    {
        if (FINAL[i])
        {
            printFinal.push_back(i);
        }
    }
    for (int i=0; i<printFinal.size(); i++)
    {
        cout << "state " << printFinal[i];
        if (i!=printFinal.size()-1)
        {
            cout << ", ";
        }
    }
    cout << "}\n";

    cout << "  3) alphabet set: {";
    for (int i=0; i<alphabet.size(); i++)
    {
        cout << char(alphabet[i]);
        if (i!=alphabet.size()-1)
        {
            cout << ", ";
        }
    }
    cout << "}\n";

    cout << "  4) transitions:\n";
}

int main(int argc, char* argv[])
{
    //these are for the FA
    int stateCount;
    bool* FINAL;
    vector<int> alphabet;
    map<pair<int,string>, int> transitions;
    int* inputIndex = new int(1);
    int currentState = 0;
    bool exit = false;
    bool accept = false;

    ifstream inputFile;
    if (argc == 1)
    {
        inputFile.open("input.txt");
    } else {
        inputFile.open(argv[1]);
    }

    //these are for reading the input file
    vector<string> inputStrings;
    string temp;

    //Read all input from a file as lines, saved to inputStrings
    while (getline(inputFile, temp)){
        inputStrings.push_back(temp);
    }
    inputFile.close();


    //set up the FA based on the given input
    stateCount = stoi(inputStrings[0]);
    FINAL = getFinals(inputStrings, stateCount);
    alphabet = getAlphabet(inputStrings);
    printInfo(stateCount, FINAL, alphabet);
    transitions = getTransitions(inputStrings, inputIndex);
    //the index at which input to the FA begins is now saved in *inputIndex


    cout << "Results of test strings:\n";
    //Universal FA algorithm
    while (*inputIndex < inputStrings.size())
    {
        cout << '\t';
        string str = inputStrings[*inputIndex];
        int i=-1;
        while (!exit)
        {
            i += 1; //current symbol is str[i]
            //if symbol is contained in alphabet AND symbol isn't last in line
            if (count(alphabet.begin(), alphabet.end(), str[i]) > 0 && i < str.length())
            {
                string keyStr(1, str[i]);
                cout << str[i] << ' ';
                currentState = nextState(currentState, keyStr, transitions);
                if (currentState == -1)
                {
                    cout << "\t\tReject: dead end\n";
                    accept = false;
                    exit = true;
                }
            } else {
                //cout << "\ncurrentState: " << currentState << '\n';
                exit = true;
                if (i < str.length()){
                    accept=false;
                    cout << "\t\tReject: invalid symbol\n";
                }
                else if (FINAL[currentState]){
                    //cout << "currentState: " << currentState << "\n";
                    accept=true;
                } else {
                    cout << "\t\tReject: not in final state\n";
                    accept = false;
                }
            } //end if
        }//end while

        if (accept)
        {
            cout << "\t\tAccept\n";
        } else {
            //cout << "\t\tReject\n\n";
        }

        currentState = 0;

        exit = false;

        *inputIndex += 1;
    }//end while
  
    cout << "\n*Note: rejected parts of input strings are not displayed.\n";

    return 0;
}