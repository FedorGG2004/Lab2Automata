#pragma once
#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <unordered_map> 
class Node {
public:
    std::string type;
    std::string label;
    int id;
    Node* left_child;
    Node* right_child;
    bool nullable;
    std::unordered_set<int> firstpos;
    std::unordered_set<int> lastpos;

    Node(std::string type, std::string label, int id = -1, Node* left_child = nullptr, Node* right_child = nullptr)
        : type(type), label(label), id(id), left_child(left_child), right_child(right_child), nullable(false) {}

    std::string info() {
        int childrenCount = (right_child != nullptr) + (left_child != nullptr);
        return "Type:\t" + type + "\n"
            + "Label: \t" + label + "\n"
            + "Children count:\t" + std::to_string(childrenCount) + "\n"
            + "Nullable:\t" + (nullable ? "True" : "False") + "\n"
            + "Firstpos:\t" + setToString(firstpos) + "\n"
            + "Lastpos:\t" + setToString(lastpos);
    }

    std::string setToString(const std::unordered_set<int>& s) {
        std::string result = "{";
        for (auto it = s.begin(); it != s.end(); ++it) {
            result += std::to_string(*it);
            if (std::next(it) != s.end()) {
                result += ", ";
            }
        }
        result += "}";
        return result;
    }

    std::string printSubtree(int level = 0, std::vector<int> linelist = {}, bool rightchild = false, bool instar = false) {
        std::string ret;
        bool star;
        if (type == "star-node" || type == "plus-node" || type == "que-node")
            star = 1;
        star = 0;
        std::string N = "\n";
        std::string T = "\t";
        std::string L = "|";
        if (level == 0) {
            ret = label + N;
        }
        else {
            std::string s = "";
            if (!instar) {
                for (int k = 0; k < 2; ++k) {
                    for (int i = 0; i < level; ++i) {
                        if (std::find(linelist.begin(), linelist.end(), i) != linelist.end()) {
                            s += ((i != 0) ? "   " : "!") + L;
                        }
                        else {
                            s += T;
                        }
                    }
                    if (k == 0) {
                        s += N;
                    }
                }
            }
            ret = s + "___" + label + (star ? "" : N);
        }

        if (rightchild) {
            linelist.pop_back();
        }
        if (left_child != nullptr) {
            linelist.push_back(level);
            ret += left_child->printSubtree(level + 1, linelist, rightchild = false, instar = star);
        }
        if (right_child != nullptr) {
            if (!star)
                linelist.push_back(level);
            ret += right_child->printSubtree(level + 1, linelist, rightchild = true);
        }
        return ret;
    }
};

class Tree {
public:
    Node* root;
    std::unordered_map<int, std::string> leaves;
    int id_counter;
    std::vector<std::unordered_set<int>> followpos;
    std::vector<std::string> alphabet;
    bool inverse = false;
    Tree(std::vector<std::string> post, bool inverse = false) : root(nullptr), id_counter(1) , inverse(inverse){
        root = new Node("cat-node", ".");
        createTree(post);
        followpos.resize(id_counter); 
        followFirstLadtPosNullableCalculate(root);
    }
    void init(std::vector<std::string> post, bool inverse = false)
    {
        id_counter = 1;
        root = nullptr;
        this->inverse = inverse;
        root = new Node("cat-node", ".");
        createTree(post);
        followpos.resize(id_counter); 
        followFirstLadtPosNullableCalculate(root);
    }

    Tree() = default;

    void createTree(std::vector<std::string> post) {
        std::vector<Node*> stack;
        for (auto& token : post) {
            if (token == ".") {
                if (inverse)
                {
                    Node* left = stack.back();
                    stack.pop_back();
                    Node* right = stack.back();
                    stack.pop_back();
                    Node* temp = new Node("cat-node", token, -1, left, right);
                    stack.push_back(temp);
                }
                else
                {
                    Node* right = stack.back();
                    stack.pop_back();
                    Node* left = stack.back();
                    stack.pop_back();
                    Node* temp = new Node("cat-node", token, -1, left, right);
                    stack.push_back(temp);
                }
               
            }
            else if (token == "|") {
                if (inverse)
                {
                    Node* left = stack.back();
                    stack.pop_back();
                    Node* right = stack.back();
                    stack.pop_back();
                    Node* temp = new Node("or-node", token, -1, left, right);
                    stack.push_back(temp);
                }
                else
                {
                    Node* right = stack.back();
                    stack.pop_back();
                    Node* left = stack.back();
                    stack.pop_back();
                    Node* temp = new Node("or-node", token, -1, left, right);
                    stack.push_back(temp);
                }
               
            }
            else if (token == "*") {
                Node* left = stack.back();
                stack.pop_back();
                Node* temp = new Node("star-node", token, -1, left);
                stack.push_back(temp);
            }
            else if (token == "+") {
                Node* left = stack.back();
                stack.pop_back();
                Node* temp = new Node("plus-node", token, -1, left);
                stack.push_back(temp);
            }
            else if (token == "?") {
                Node* left = stack.back();
                stack.pop_back();
                Node* temp = new Node("que-node", token, -1, left);
                stack.push_back(temp);
            }
            else {
               
                if (token[0] == '&')
                {
                    char tmp = token[1];
                    token.clear();
                    token += tmp;
                }
                auto it = std::find(alphabet.begin(), alphabet.end(), token);
            
                if (it == alphabet.end() && token!="$") {
                    alphabet.push_back(token);
                }
                Node* temp = new Node("a-node", token, giveNextId());
              //  std::cout << "Token " << token << " " << temp->id << std::endl;
                leaves[temp->id] = temp->label;
                stack.push_back(temp);
               // std::cout << "token " <<token;
            }
        }
        Node* temp = new Node("a-node", "#", giveNextId());
        leaves[temp->id] = temp->label;
        root->left_child = stack.back();
        stack.pop_back();
        root->right_child = temp;
        std::cout << "Token " << temp->label << " " << temp->id << std::endl;
    }

    int giveNextId() {
        return id_counter++;
    }

    void  followFirstLadtPosNullableCalculate(Node* node) {
        if (!node) return;
        followFirstLadtPosNullableCalculate(node->left_child);
        followFirstLadtPosNullableCalculate(node->right_child);
        if (node->type == "a-node") {
            if (node->label == "$") {
                node->nullable = true;
            }
            else {
                node->firstpos.insert(node->id);
                node->lastpos.insert(node->id);
            }
        }
        else if (node->type == "star-node") {
            node->nullable = true;
            node->firstpos = node->left_child->firstpos;
            node->lastpos = node->left_child->lastpos;
            calculateFollows(node);
        }
        else if (node->type == "plus-node") {
            node->nullable = false;
            node->firstpos = node->left_child->firstpos;
            node->lastpos = node->left_child->lastpos;
            calculateFollows(node);
        }
        else if (node->type == "que-node") {
            node->nullable = true;
            node->firstpos = node->left_child->firstpos;
            node->lastpos = node->left_child->lastpos;
            // calculateFollows(node);
        }
        else if (node->type == "cat-node") {
            node->nullable = node->left_child->nullable && node->right_child->nullable;
            if (node->left_child->nullable) {
                node->firstpos = setUnion(node->left_child->firstpos, node->right_child->firstpos);
            }
            else {
                node->firstpos = node->left_child->firstpos;
            }
            if (node->right_child->nullable) {
                node->lastpos = setUnion(node->left_child->lastpos, node->right_child->lastpos);
            }
            else {
                node->lastpos = node->right_child->lastpos;
            }
            calculateFollows(node);
        }
        else if (node->type == "or-node") {
            node->nullable = node->left_child->nullable || node->right_child->nullable;
            node->firstpos = setUnion(node->left_child->firstpos, node->right_child->firstpos);
            node->lastpos = setUnion(node->left_child->lastpos, node->right_child->lastpos);
        }
  
       // std::cout << node->info() << std::endl;
       // std::cout << std::endl;
    }


    std::unordered_set<int> setUnion(const std::unordered_set<int>& s1, const std::unordered_set<int>& s2) {
        std::unordered_set<int> result = s1;
        result.insert(s2.begin(), s2.end());
        return result;
    }
 
    void calculateFollows(Node* n) {
        if (n->type == "cat-node") {
            for (int i : n->left_child->lastpos) {
    
                followpos[i].insert(n->right_child->firstpos.begin(), n->right_child->firstpos.end());
            }
        }
        else if (n->type == "star-node") {
            for (int i : n->left_child->lastpos) {
                
                followpos[i].insert(n->left_child->firstpos.begin(), n->left_child->firstpos.end());
            }
        }
        else if (n->type == "plus-node") {
            for (int i : n->left_child->lastpos) {
                followpos[i].insert(n->left_child->firstpos.begin(), n->left_child->firstpos.end());
            }
        }
        // else if (n->type == "que-node") {
        //     for (int i : n->left_child->lastpos) {
        //         followpos[i].insert(n->left_child->firstpos.begin(), n->left_child->firstpos.end());
        //     }
        // }
    
    }

    std::string print() {
        return root->printSubtree();
    }
};