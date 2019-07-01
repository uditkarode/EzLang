#ifndef UTILS_H
#define UTILS_H

#include <regex>

using namespace std;

//const vector<string> keywords = {"if", "times"};

enum TokenType {
    NUMBER_LITERAL, IDENTIFIER, ASSIGNMENT, PLUS, MINUS, DIVIDE, MULTIPLY, NEWLINE
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
        switch (a) {
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

            default:
                return "UNKNOWN";
        }
    }*/

    static int precedence(char c){
        if(c == '^')
            return 3;
        else if(c == '*' || c == '/')
            return 2;
        else if(c == '+' || c == '-')
            return 1;
        else
            return -1;
    }

    static string infixToPostfix(string s){
        stack<char> st;
        st.push('N');
        int l = s.length();
        string ns;
        for(int i = 0; i < l; i++){
            // If the scanned character is an operand, add it to output string.
            if(isdigit(s[i]))
                ns+=s[i];

            // If the scanned character is an ‘(‘, push it to the stack.
            else if(s[i] == '(')
                st.push('(');

            // If the scanned character is an ‘)’, pop and to output string from the stack
            // until an ‘(‘ is encountered.

            else if(s[i] == ')'){
                while(st.top() != 'N' && st.top() != '('){
                    char c = st.top();
                    st.pop();
                    ns += c;
                }

                if(st.top() == '('){
                    char c = st.top();
                    st.pop();
                }
            }

                //If an operator is scanned
            else {
                while(st.top() != 'N' && precedence(s[i]) <= precedence(st.top())){
                    char c = st.top();
                    st.pop();
                    ns += c;
                }
                st.push(s[i]);
            }
        }

        //Pop all the remaining elements from the stack
        while(st.top() != 'N'){
            char c = st.top();
            st.pop();
            ns += c;
        }

        return ns;
    }

    static bool isOperator(char c){
        return c == '+' || c == '-' ||
               c == '*' || c == '/' ||
               c == '^';
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