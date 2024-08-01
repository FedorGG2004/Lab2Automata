#include <string>
#include "dfa.h"
#include "Parser.h"
void printVectorOfSets(const std::vector<std::unordered_set<int>>& vec) {
    for (const auto& set : vec) {
        std::cout << "{ ";
        for (int num : set) {
            std::cout << num << " ";
        }
        std::cout << "} ";
    }
    std::cout << std::endl;
}
class MyReg {
private:
    DFA dfa;
    Tree t;
public:
	std::string input;
	bool optimize;
    std::string kpath;
	MyReg(std::string input) :input(input){
        Parser parser(input);
        parser.create_token_queue();
        parser.create_postfix_token_queue();
        parser.printTokens();
        parser.printQueue();

        t.init(parser.output_queue);
        std::cout << t.print() << std::endl; 
        printVectorOfSets(t.followpos);
        dfa.init(t.alphabet, t);
        std::cout << "yes";
        dfa.post_processing();
        std::cout << dfa.to_string();
	}

    void compile()
    {
        dfa.compile();
        std::cout << std::endl;
        std::cout << "min dfa" << std::endl;
        std::cout << dfa.to_string();
      
    }

    void kPath()
    {
        std::cout << std::endl;
        std::cout << "KPath" << std::endl;
        kpath = dfa.createRegex();
        std::cout << kpath << std::endl;

    }

    void checkKpath()
    {
        Parser parser(kpath);
        parser.create_token_queue();
        // parser.printTokens();
        parser.create_postfix_token_queue();
        Tree t1;
        t1.init(parser.output_queue);
        //    printVectorOfSets(t.followpos);
        dfa.init(t1.alphabet, t1);
        dfa.post_processing();
        //  std::cout << dfa.to_string();
        dfa.compile();
        std::cout << "min dfa to kPath" << std::endl;
        std::cout << dfa.to_string();
    }

    // void inverse()
    // {
    //     Parser parser(kpath);
    //     parser.create_token_queue();
    //     // parser.printTokens();
    //     parser.create_postfix_token_queue();
    //     Tree t1;
    //     t1.init(parser.output_queue,true);
    //     //    printVectorOfSets(t.followpos);
    //     dfa.init(t1.alphabet, t1);
    //     std::cout << "tree inverse" << std::endl;        
    //     dfa.post_processing();
    //     //  std::cout << dfa.to_string();
    //     dfa.compile();
    //     std::cout << "min dfa to inverse" << std::endl;
    //     std::cout << dfa.to_string();
    // }

    void difference()
    {
        std::string input2 = R"(abcd|bac)";s
        Parser parser(input2);
        parser.create_token_queue();
        parser.create_postfix_token_queue();
        Tree t1;
        t1.init(parser.output_queue);
        DFA completeDfa(t1.alphabet, t1);
        completeDfa.post_processing();
        // completeDfa.compile();
        std::vector<std::string> alphabet2=dfa.alphabet;
        for(auto& a: completeDfa.alphabet){
            auto it = std::find(alphabet2.begin(), alphabet2.end(), a);
            if(it == alphabet2.end()){
                alphabet2.push_back(a);
            }
        }
        // for(auto a: alphabet2){
        //     std::cout<<a<<" ";
        // }
        for(auto& al: alphabet2){
            auto it = std::find(dfa.alphabet.begin(), dfa.alphabet.end(), al);
            if(it == dfa.alphabet.end()){
                int id = 0;
                for(auto& st: dfa.states){
                    if(st.error==true){
                        id=st.id;
                    }
                }
                std::cout<<id << "  ";
                for(auto& st: dfa.states){
                    st.transitions[al] = {id};
                }
            }
        }
        for(auto& al: alphabet2){
            auto it = std::find(completeDfa.alphabet.begin(), completeDfa.alphabet.end(), al);
            if(it == completeDfa.alphabet.end()){
                int id = 0;
                for(auto& st: completeDfa.states){
                    // std::cout<<st.error;
                    if(st.error==true){
                        id=st.id;
                    }
                }
                // std::cout<<"second" << id << "  ";
                for(auto& st: completeDfa.states){
                    st.transitions[al] = {id};
                }
            }
        }
        //std::cout << "min complete dfa" << std::endl;
        //std::cout << completeDfa.to_string();
        // std::cout << dfa.to_string();
        // std::cout << completeDfa.to_string();
        dfa.createDifference(completeDfa, alphabet2);
        dfa.post_processing();
        std::cout << dfa.to_string();

    }
    void addition()
    {
        Parser parser(kpath);
  
        std::string completeStr="(";
        for (int i = 0; i < dfa.alphabet.size(); i++)
        {
            completeStr += dfa.alphabet[i] + "|";
        }
        if (completeStr.size() == 1)
            return;
        completeStr.pop_back();
        completeStr += ")*";
        // completeStr = "ab|ba";
        parser.setINPUT(completeStr);
        parser.create_token_queue();
        parser.create_postfix_token_queue();
        Tree t1;
        t1.init(parser.output_queue);
        DFA completeDfa(t1.alphabet, t1);
        completeDfa.post_processing();
        completeDfa.compile();
        //std::cout << "min complete dfa" << std::endl;
        //std::cout << completeDfa.to_string();

        dfa.createAddition(completeDfa);
        dfa.post_processing();
        std::cout << dfa.to_string();

    }

    bool match(const std::string& str)
    {
        int stateId=1;
        bool isfinal = 0;
        for (size_t i = 0; i < str.length(); ++i) 
        {
            const std::string c (1, str[i]);
            auto it = std::find(dfa.alphabet.begin(), dfa.alphabet.end(), c);
            if (it == dfa.alphabet.end())
                return false;
            for (size_t j = 0; j < dfa.states.size(); ++j)
            {
                if (stateId == dfa.states[j].id)
                {
                    stateId = dfa.states[j].transitions[c][0];
                    break;
                }
                  
            }
            
        }
        for (size_t k = 0; k < dfa.states.size(); ++k)
        {
            if (stateId == dfa.states[k].id)
            {
                isfinal = dfa.states[k].final;
            }
        }
        return isfinal;
    }

    // std::string match(const std::string& str)
    // {
    //     int stateId=1;
    //     std::vector<int> statesId;
    //     std::string ret;
    //     bool isfinal = 0;
    //     for (size_t i = 0; i < str.length(); ++i) 
    //     {
    //         const std::string c (1, str[i]);
    //         auto it = std::find(dfa.alphabet.begin(), dfa.alphabet.end(), c);
    //         if (it == dfa.alphabet.end())
    //             return "";
    //         for (size_t j = 0; j < dfa.states.size(); ++j)
    //         {
    //             if (stateId == dfa.states[j].id)
    //             {
    //                 stateId = dfa.states[j].transitions[c][0];
    //                 ret+=c;
    //                 break;
    //             }
                  
    //         }
            
    //     }
    //     for (size_t k = 0; k < dfa.states.size(); ++k)
    //     {
    //         if (stateId == dfa.states[k].id)
    //         {
    //             isfinal = dfa.states[k].final;
    //         }
    //     }
    //     if(isfinal==true)
    //         return ret;
    //     else
    //         return "";
    // }
    std::vector<std::string> find_all(const std::string& str)
    {
        std::vector<std::string> matches;
        for (int j=0; j < str.size(); j++) {
            for (int i=1; i <= str.size() - j; i++) {
                std::string sub = str.substr(j, i);
                if (match(sub)) {
                    matches.push_back(sub);
                }
            }
        }
        return matches;
    }
    // std::string find_all(const std::string& str)
    // {
    //     // int stateId=1;
    //     std::vector<int> statesId(dfa.states.size());
    //     for(int i = 0; i < dfa.states.size(); i++){
    //         statesId[i]=i+1;
    //     }
    //     std::vector<std::string> ret(dfa.states.size());
    //     std::vector<bool> isfinal(dfa.states.size());
    //     std::string r;
    //     for(size_t k = 0; k < dfa.states.size(); k++){
    //         for (size_t i = 0; i < str.length(); ++i) 
    //         {
    //             const std::string c (1, str[i]);
    //             auto it = std::find(dfa.alphabet.begin(), dfa.alphabet.end(), c);
    //             if (it == dfa.alphabet.end())
    //                 break;
    //             for (size_t j = 0; j < dfa.states.size(); ++j)
    //             {
    //                 if (statesId[k] == dfa.states[j].id)
    //                 {
    //                     statesId[k] = dfa.states[j].transitions[c][0];
    //                     ret[k]+=c;
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    //     for (size_t k = 0; k < dfa.states.size(); ++k)
    //     {
    //         std::cout<<ret[k]<<std::endl;
    //     }

    //     for (size_t k = 0; k < dfa.states.size(); ++k)
    //     {
    //         if (statesId[k] == dfa.states[k].id)
    //         {
    //             isfinal[k] = dfa.states[k].final;
    //         }
    //     }
    //     for(size_t k = 0; k < dfa.states.size(); k++){
    //         if(isfinal[k]==true){
    //             r+=ret[k];
    //             r+=" ";
    //         }
    //     }
    //     return r;
    // }
};