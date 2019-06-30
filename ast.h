#include <string>

using namespace std;

struct AstNode {
    string val;
    AstNode* left{};
    AstNode* right{};
};

class AST {
    public:
    static AstNode* newNode(char val){
        auto node = new AstNode();
        node->val = val;
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
};