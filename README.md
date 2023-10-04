# Finite State Automaton (FSA) Simulator in C++

This C++ project is a Finite State Automaton (FSA) simulator that reads an input file containing information about an FSA, processes test strings, and determines whether each test string is accepted or rejected by the FSA. Here's a brief overview of the project:

## Project Structure

- The project utilizes various C++ standard libraries and functions for file input/output, string manipulation, and data storage.
- It defines functions to extract information about the FSA from the input file and to simulate the FSA on test strings.

## Main Components

1. **getFinals()**: Reads the input file and extracts the final states of the FSA, returning a boolean array where each index represents a state, indicating whether it is a final state or not.

2. **getAlphabet()**: Reads the input file and extracts the FSA's alphabet, represented as a vector of integers.

3. **getTransitions()**: Reads the input file and extracts the FSA's transition function, storing it in a map where keys are pairs of the current state and input symbol, and values are the next state.

4. **nextState()**: Given the current state, an input symbol, and the transition function, calculates the next state according to the FSA's rules.

5. **printInfo()**: Displays information about the FSA, including its set of states, final states, alphabet, and transitions.

6. **main()**: The main program reads the input file, sets up the FSA, and processes test strings using the FSA. It displays whether each test string is accepted or rejected.

## Usage

To use the FSA simulator:

1. Create an input file following a specific format (detailed in the code comments).
2. Run the program, providing the input file as a command-line argument (or use "input.txt" by default).
3. The program reads the input file, sets up the FSA, and processes test strings.
4. For each test string, it displays whether the FSA accepts or rejects it.

This project serves as a tool for simulating the behavior of a Finite State Automaton based on input from an external file and processing test strings. It provides insights into the FSA's operation and its acceptance or rejection of given strings.
