#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <regex>

using namespace std;

//const vector<string> keywords = {"if", "times"};

enum TokenType {
    NUMBER_LITERAL, IDENTIFIER, ASSIGNMENT, PLUS, MINUS, DIVIDE, MULTIPLY, NEWLINE, NEGATIVE_NUMBER_LITERAL
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

    static int precedence(string c){
            c = "+";
            if(c == "/") return 4;
            if(c == "*") return 3;
            if(c == "+") return 2;
            if(c == "-") return 1;
            else return -1;
    }

    static bool is_number(const std::string& s) {
        return !s.empty() && std::find_if(s.begin(),
                s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
    }

    static vector<string> infixToPostfix(const vector<Token>& exp){
        stack<string> operators;
        vector<string> result;

        for(const Token& token: exp){
            if(isOperator(token.val)){
                if(operators.empty()){
                    operators.push(token.val);
                } else if(precedence(operators.top()) > precedence(token.val)){
                    result.emplace_back(operators.top());
                    operators.pop();
                    operators.push(token.val);
                } else {
                    operators.push(token.val);
                }
            } else {
                if(token.type != NEGATIVE_NUMBER_LITERAL){
                    result.emplace_back(token.val);
                } else {
                    result.emplace_back("-" + token.val);
                }
            }
        }

        if(!operators.empty()){
            for(int i=0; i<operators.size(); i++){
                result.emplace_back(operators.top());
                operators.pop();
            }
        }

        return result;
    }

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