
# DFA Evaluator

**Authors**: Silvia Calderón, Jorge González, and David Valverde

This program allows the execution of a deterministic finite automaton (DFA) driver via a GUI. The user is able to define the following characteristics of any given DFA:

- Number of states and symbols in the alphabet
- [Optional] Labels or tags for each state
- Symbols of the alphabet, single characters only
- Delta function: next state for any given state-symbol pair
- Subset of acceptance states

Once the DFA is configured, the user can evaluate strings to view the corresponding sequence of states and the final acceptance or rejection result.

## Requirements

- GTK version 3.24 or greater
- Python 2.7 or 3.X
- pyparsing 1.4.8 or greater
- Graphviz
- preview-latex/AUCTeX
- PGZ/TikZ version 2.0 or later
- dot2tex

## How to compile
In the same directory as the included Makefile, run:

    make

## How to run the program

    ./project.out
