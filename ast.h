#include <string>

using namespace std;

struct AstNode {
    string val;
    AstNode* left{};
    AstNode* right{};
};

class AST {
    public:
    AstNode* root;

    AST(){
        root = new AstNode();
    }
};