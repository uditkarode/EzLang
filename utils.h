#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <regex>

using namespace std;

//const vector<string> keywords = {"if", "times"};

enum TokenType {
    NUMBER_LITERAL, IDENTIFIER, ASSIGNMENT, PLUS, MINUS, DIVIDE, MULTIPLY, NEWLINE, NEGATIVE_NUMBER_LITERAL,
    OPENING_BRACKET, CLOSING_BRACKET, HASHTAG
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
    static bool exists_in(const vector<string> &check, const string &target) {
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

            case 12:
                return "HASHTAG";

            default:
                return "UNKNOWN";
        }
    }

    static int prec(const string& c){
        if(c == "^")
            return 3;
        else if(c == "*" || c == "/")
            return 2;
        else if(c == "+" || c == "-")
            return 1;
        else
            return -1;
    }

    static vector<string> infixToPostfix(vector<Token> s){
        stack<string> st;
        st.push("N");
        int l = s.size();
        vector<string> ns;
        for(int i = 0; i < l; i++){
            if(s[i].type == IDENTIFIER || s[i].type == NUMBER_LITERAL)
                ns.emplace_back(s[i].val);

            else if(s[i].type == NEGATIVE_NUMBER_LITERAL)
                ns.emplace_back("-" + s[i].val);
            else if(s[i].type == OPENING_BRACKET)
                st.push(s[i].val);
            else if(s[i].type == CLOSING_BRACKET){
                while(st.top() != "N" && st.top() != "("){
                    string c = st.top();
                    st.pop();
                    ns.emplace_back(c);
                }
                if(st.top() == "("){
                    st.pop();
                }
            }

            else {
                while(st.top() != "N" && prec(s[i].val) <= prec(st.top())){
                    string c = st.top();
                    st.pop();
                    ns.emplace_back(c);
                }
                st.push(s[i].val);
            }
        }
        while(st.top() != "N"){
            string c = st.top();
            st.pop();
            ns.emplace_back(c);
        }

        return ns;
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