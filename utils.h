#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <regex>

using namespace std;

//const vector<string> keywords = {"if", "times"};

enum TokenType {
    NUMBER_LITERAL, IDENTIFIER, ASSIGNMENT, PLUS, MINUS, DIVIDE, MULTIPLY, NEWLINE, NEGATIVE_NUMBER_LITERAL, OPENING_BRACKET, CLOSING_BRACKET
};

class Token {
public:
    explicit Token(char val, TokenType type) {
        this->type = type;
        this->val = string(1, val);
    }

    explicit Token(string val, TokenType type){
        this->type = type;
        this->val = std::move(val);
    }

    TokenType type;
    string val;
};

class Utils {
    public:
    /*static bool exists_in(const vector<string> &check, const string &target) {
        for (auto &str: check) if (str == target) return true;
        return false;
    }

    static string getTokenTypeString(int a) {
        switch (++a) {
            case 0:
                return "KEYWORD";

            case 1:
                return "NUMBER_LITERAL";

            case 2:
                return "IDENTIFIER";

            case 3:
                return "ASSIGNMENT";

            case 4:
                return "PLUS";

            case 5:
                return "MINUS";

            case 6:
                return "DIVIDE";

            case 7:
                return "MULTIPLY";

            case 8:
                return "NEWLINE";

            case 9:
                return "NEGATIVE_NUMBER_LITERAL";

            default:
                return "UNKNOWN";
        }
    }*/

    // todo okstart

    static int inPrec(string input){
        char input_char = input.c_str()[0];
        switch (input_char) {
            case '+':
            case '-':
                return 2;
            case '*':
            case '%':
            case '/':
                return 4;
            case '^':
                return 5;
            case '(':
                return 0;
            default: return -1;
        }
    }

// function to return precedence value
// if operator is present outside stack.
    static int outPrec(const string& input){
        char input_char = input.c_str()[0];
        switch (input_char) {
            case '+':
            case '-':
                return 1;
            case '*':
            case '%':
            case '/':
                return 3;
            case '^':
                return 6;
            case '(':
                return 100;

            default: return -1;
        }
    }

// function to convert infix to postfix
    static vector<string> infixToPostfix(vector<Token> input) {
        stack<string> s;
        vector<string> out;

        // while input is not NULL iterate
        int i = 0;
        while (i < input.size()) {

            // if character an operand
            if (input[i].type == IDENTIFIER || input[i].type == NUMBER_LITERAL || input[i].type == NEGATIVE_NUMBER_LITERAL) {
                out.emplace_back(input[i].val);
            }

                // if input is an operator, push
            else if (isOperator(input[i].val)) {
                if (s.empty() || outPrec(input[i].val) > inPrec(s.top()))
                    s.push(input[i].val);
                else {
                    while (!s.empty() && outPrec(input[i].val) < inPrec(s.top())) {
                        out.emplace_back(s.top());
                        s.pop();
                    }
                    s.push(input[i].val);
                }
            }

                // condition for opening bracket
            else if (input[i].type == CLOSING_BRACKET) {
                while (s.top() != "(") {
                    out.emplace_back(s.top());
                    s.pop();

                    // if opening bracket not present
                    if (s.empty()) {
                        cout << "Wrong input\n";
                        exit(1);
                    }
                }

                // pop the opening bracket.
                s.pop();
            }
            i++;
        }

        // pop the remaining operators
        while (!s.empty()) {
            if (s.top() == "(") {
                printf("\n Wrong input\n");
                exit(1);
            }
            out.emplace_back(s.top());
            s.pop();
        }
    }

    // todo okend

    static bool isOperator(const string& c){
        return c == "+" || c == "-" ||
               c == "*" || c == "/" ||
               c == "^";
    }

    static int toInt(string s){
        int num = 0;

        // Check if the integral value is
        // negative or not
        // If it is not negative, generate the number
        // normally
        if(s[0]!='-')
            for (char i : s)
                num = num*10 + (int(i)-48);
            // If it is negative, calculate the +ve number
            // first ignoring the sign and invert the
            // sign at the end
        else
            for (int i=1; i<s.length(); i++){
                num = num*10 + (int(s[i])-48);
                num = num*-1;
            }

        return num;
    }
};

#endif // UTILS_H