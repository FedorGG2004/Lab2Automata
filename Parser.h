#include <iostream>
#include <vector>
#include <string>
#include <stack>
class Parser
{
	private:
		 std::string INPUT;
public:
	Parser(const std::string INPUT) :INPUT(INPUT)
	{
	}
	void setINPUT(std::string str)
	{
		INPUT = str;
	}
	std::vector<std::string> tokens;
	std::vector<std::string> output_queue;
	std::vector<std::string> create_postfix_token_queue();
	std::vector<std::string> create_token_queue();
	std::vector<std::string> createAlphabet();
	void printTokens();
	void printQueue();
};