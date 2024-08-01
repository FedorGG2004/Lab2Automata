#include "myregex.h"
#include <vector>
#include <iterator>
int main() 
{
    std::string input = R"(abcd)";

//bc|(bb)* - дополнение
//(ab)*c 
    //ababbcac 
    //R"(((a(b|c))+c)"
    MyReg m(input);  
                                   
     m.compile();
     m.kPath();
     m.checkKpath();
    //  m.inverse(); 
    //m.addition() - но для любого а не (a|b|c)*
    //  m.addition();
     m.difference();
     m.compile();

    std::vector<std::string> res = m.find_all("abbcdddabc");
    auto vo = res;
    std::sort(vo.begin(), vo.end());
    auto end = std::unique(vo.begin(), vo.end());
    std::copy(vo.begin(), end, std::ostream_iterator<std::string>{std::cout, "\n"});


    // for(auto& i : res){
    //     std::cout << i << std::endl;
    // }

    // a1
    // a2
    // a3=a1-a2
    // a3.match=a1.match && !a2.match
     
    return 0;                                      
}
