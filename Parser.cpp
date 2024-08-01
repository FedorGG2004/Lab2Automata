#include "Parser.h"
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <stack>
struct NumberInfo
{
    int order;
    int value;
};

NumberInfo extract_number(const std::string& str)
{
    NumberInfo number_info;
    number_info.order = 0;
    number_info.value = 0;
    bool is_number = false;
    for (char c : str)
    {
        if (isdigit(c))
        {
            number_info.value = number_info.value * 10 + (c - '0');
            is_number = true;
        }
        else {
            break; 
        }
        number_info.order++;
    }
    return number_info;
}

NumberInfo getNumber(const std::string& input)
{
    NumberInfo number_info;
    number_info.order = 0;
    number_info.value = 0;
    size_t pos_open_brace = input.find("{");
    if (pos_open_brace != std::string::npos)
    {
        size_t pos_close_brace = input.find("}", pos_open_brace);
        if (pos_close_brace != std::string::npos)
        {
            std::string sub_str = input.substr(pos_open_brace + 1, pos_close_brace - pos_open_brace - 1);
            number_info = extract_number(sub_str);
        }
    }
    return number_info;
}

std::vector<std::pair<int, std::string>>  extractNestedString(const std::string& str, const std::string& openingBracket, const std::string& closingBracket,
    int nestingLevel, char avoidSymbol = '&')
{
    std::string result;
    int currentNestingLevel = 0;
    std::vector<std::pair<int, std::string>> pairs;
    for (size_t i = 0; i < str.length(); ++i)
    {
        char c = str[i];
        if (c == avoidSymbol && currentNestingLevel>=1)
        {
            result += avoidSymbol;
            result += str[i + 1];
            i++;
            continue;
        }
        if (str.substr(i, openingBracket.length()) == openingBracket)
        {
            currentNestingLevel++;
            if (currentNestingLevel == nestingLevel)
            {
                for (int j = i; j < i + openingBracket.length(); j++)
                    result += str[j];
                i += openingBracket.length() - 1;
                continue;
            }
        }
        else if (str.substr(i, closingBracket.length()) == closingBracket)
        {
            if (currentNestingLevel == nestingLevel)
            {
                for (int j = i; j < i + closingBracket.length(); j++)
                    result += str[j];
                i += closingBracket.length() - 1;
                pairs.push_back(std::make_pair(i, result));
                result = "";
            }
            currentNestingLevel--;
        }

        if (currentNestingLevel >= nestingLevel) {
            result += c;
        }
    }
    return pairs;
}

int find_innermost_depth(const std::string& str, const std::string& openingBracket, const std::string& closingBracket) {
    int max_depth = -1;
    int depth = 0;
    for (size_t i = 0; i < str.length(); ++i)
    {
        char  c = str[i];
        if (str.substr(i, openingBracket.length()) == openingBracket)
        {
            depth++;
            if (depth > max_depth) {
                max_depth = depth;
            }
        }
        else if (str.substr(i, closingBracket.length()) == closingBracket)
        {
            if (depth == max_depth) {
            }
            depth--;
        }
    }
    return max_depth;
}



std::string find_and_replace_brackets_n_times(const std::string& str, const std::string& openingBracket, const std::string& closingBracket)
{
    std::vector<std::pair<int, std::string>>  nestedStrings;
    int nestingLevel = find_innermost_depth(str, openingBracket, closingBracket);
    std::string modifiedStr = str;
    if (nestingLevel == -1)
        return modifiedStr;
    nestedStrings = extractNestedString(str, openingBracket, closingBracket, nestingLevel);

    std::string copyOfnestedString;
    while (nestingLevel != 0)
    {
        nestedStrings = extractNestedString(modifiedStr, openingBracket, closingBracket, nestingLevel);
        for (int i = nestedStrings.size() - 1; i >= 0; i--)
        {
            if (modifiedStr[nestedStrings[i].first + 1] == '{')
            {
                NumberInfo number_info = getNumber(modifiedStr.substr(nestedStrings[i].first + 1));
                copyOfnestedString = nestedStrings[i].second;
                for (int j = 0; j < number_info.value - 1; j++)
                {
                    copyOfnestedString += nestedStrings[i].second;
                }
                modifiedStr.erase(nestedStrings[i].first - nestedStrings[i].second.size() + 1, nestedStrings[i].second.size() + 2 + number_info.order);
                modifiedStr.insert(nestedStrings[i].first - nestedStrings[i].second.size() + 1, copyOfnestedString);
            }
        }
        nestingLevel--;
    }
    std::string copyOfChar;
    for (size_t i = 0; i < modifiedStr.length(); ++i)
    {
        if (modifiedStr[i] == '&')
        {
            i++;
            continue;
        }
        if (modifiedStr[i] == '{')
        {
            NumberInfo number_info = getNumber(modifiedStr.substr(i));
            copyOfChar = modifiedStr[i - 1];
            for (int j = 0; j < number_info.value - 1; j++)
            {
                copyOfChar += modifiedStr[i - 1];
            }
            modifiedStr.erase(i - 1, 3 + number_info.order);
            modifiedStr.insert(i - 1, copyOfChar);
        }

    }
   // std::cout << "yes " << modifiedStr << std::endl;
    return modifiedStr;
}

std::string replaceOrBrackets(const std::string& str, char avoidSymbol = '&')
{
    int k = 0;
    std::string modifiedStr = "";
    int index;
    if (str.size() == 1) 
        return str;
    for (size_t i = 0; i < str.length() - 1; ++i)
    {
        char c = str[i];
        if (c == '&')
        {
            modifiedStr += '&';
            modifiedStr += str[i + 1];
            i++;
            continue;
        }
        if (c == '&' && k >= 1)
        {
            modifiedStr += '&';
            modifiedStr += str[i + 1];
            if(str[i+2]!=']')
            modifiedStr += '|';
            i++;
            continue;
        }
    
        if (str[i] == '[')
        {
            modifiedStr += "(";
            k += 1;
            continue;
        }
        if (str[i] == ']')
        {
            k -= 1;
            modifiedStr += ")";
            if (str[i + 1] != ']' && k >= 1)
            {
                modifiedStr += '|';
            }
            continue;
        }
        if (k >= 1) {
            modifiedStr += str[i];
            if (str[i + 1] != ']')
                modifiedStr += '|';
            continue;
        }
        modifiedStr += c;
    }
    if (str[str.size() - 1] == ']' )
    {
        modifiedStr += ")";
    }
    else {
        if (str.size()>1)
        {
            if (str[str.size() - 2] != '&')
                modifiedStr += str[str.size() - 1];
        }
      
    }
    //std::cout << "yes " << modifiedStr << std::endl;
    return modifiedStr;
}

std::vector<std::string> Parser::create_token_queue() {
    std::string modifiedStr = replaceOrBrackets(INPUT);
    modifiedStr = find_and_replace_brackets_n_times(modifiedStr, "(", ")");
    std::string id = "";
    std::string tmp = "";
    std::string INPUT = modifiedStr;
    INPUT += " ";
    for (std::size_t i = 0; i < INPUT.size() - 1; ++i)
    {
        char c = INPUT[i];

        if (c == '&')
        {
            tmp = R"(&)";
            tmp += INPUT[i + 1];
            tokens.push_back(tmp);
            char b = INPUT[i + 1];
            if (b != ')' && b != '|' && b != '.' && b != '+' && b != '?' && b != '*' && i != INPUT.size() - 3)
                tokens.push_back(".");
            i++;
            continue;
        }
        if (c == '(' || c == ')' || c == '|' || c == '.' || c == '+' || c == '?' || c == '*')
        {
            tmp = c;
            if (c == '+')
            {
                tmp = "+";
                char b = INPUT[i + 1];
                tokens.push_back(tmp);
                if ( b != ')' && b != '|' && b != '.' && b != '+' && b != '?' && b != '*' && i + 2 != INPUT.size()) 
                    tokens.push_back(".");
                continue;
            }
            if (c == '?')
            {
                tmp = "?";
                char b = INPUT[i + 1];
                tokens.push_back(tmp);
                if ( b != ')' && b != '|' && b != '.' && b != '+' && b != '?' && b != '*' && i + 2 != INPUT.size()) 
                    tokens.push_back(".");
                continue;
            }
            if (c == '*')
            {
                tmp = "*";
                char b = INPUT[i + 1];
                tokens.push_back(tmp);
                if ( b != ')' && b != '|' && b != '.' && b != '+' && b != '?' && b != '*' && i + 2 != INPUT.size()) 
                    tokens.push_back(".");
                continue;
            }
            if (c == '(')
            {
                tmp = "(";
                tokens.push_back(tmp);
                continue;
            }
            if (c == ')')
            {
                char b = INPUT[i + 1];
                tokens.push_back(tmp);
                if (b != ')' && b != '|' && b != '.' && b != '+' && b != '?' && b != '*' && i + 2 != INPUT.size())
                    tokens.push_back(".");
                continue;
            }
            tokens.push_back(tmp); 
        }
        else
        {
            id = c;
            tokens.push_back(id);
            char b = INPUT[i + 1];
            if (b != ')' && b != '|' && b != '.' && b != '+' && b != '?' && b != '*' && i != INPUT.size() - 2)
                tokens.push_back(".");
        }
    }
    return tokens;
}
void Parser::printTokens()
{
    for (const auto& token : tokens) {
        std::cout << token << " ";
    }
    std::cout << "\n";
}

std::vector<std::string> Parser::createAlphabet()
{
    std::vector<std::string> alphabet;
    for (std::size_t i = 0; i < INPUT.size() - 1; ++i)
    {
        char c = INPUT[i];
        if (c == '(' || c == ')' || c == '|' || c == '.' || c == '+' || c == '?' || c == '*')
        {
            continue;
        }
        auto it = std::find(alphabet.begin(), alphabet.end(), std::string(1,c));
        if (it == alphabet.end() && c != '$') {
            alphabet.push_back(std::string(1,c));
        }
    }
    return alphabet;
}

void Parser::printQueue()
{
    for (const auto& token : output_queue) {
        std::cout << token << " ";
    }
    std::cout << "\n";
}

std::vector<std::string> Parser::create_postfix_token_queue()
    {
    std::stack<std::string> stack;
    for (const auto& token : tokens) {
        if (token == "(")
        {
            stack.push("(");
        }
        else if (token == ")")
        {
            while (!stack.empty() && stack.top() != "(")
            {
                output_queue.push_back(stack.top());
                stack.pop();
            }
            stack.pop(); 
        }
        else if (token == "+")
        {
            stack.push(token);
        }
        else if (token == "?")
        {
            stack.push(token);
        }
        else if (token == "*")
        {
            stack.push(token);
        }
        else if (token == ".")
        {
            while (!stack.empty() && (stack.top() == "+" || stack.top() == "?" || stack.top() == "*"))
            {
                output_queue.push_back(stack.top());
                stack.pop();
            }
            stack.push(token);
        }
        else if (token == "|") {

            while (!stack.empty() && (stack.top() == "+" || stack.top() == "?" ||stack.top() == "*" || stack.top() == "."))
            {
                output_queue.push_back(stack.top());
                stack.pop();
            }
            stack.push(token);
        }
        else
        {
            output_queue.push_back(token);
        }
    }
    while (!stack.empty())
    {
        output_queue.push_back(stack.top());
        stack.pop();
    }
    return output_queue;
}