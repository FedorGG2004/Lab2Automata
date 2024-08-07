#include "myregex.h"
#include <vector>
#include <iterator>

void create_automata(std::vector<MyReg>& vec){
    std::cout<<"input regex "<<std::endl;
    std::string input;
    std::cin>>input;
    MyReg m(input);  
    m.compile();
    m.kPath();
    m.checkKpath();
    vec.push_back(m);
}
void delete_automata(std::vector<MyReg>& vec){
    std::cout<<"input index "<<std::endl;
    int index;
    std::cin>>index;
    vec.erase(vec.begin() + index);
}
void output_automata(std::vector<MyReg>& vec){
    for(int i=0; i<vec.size(); i++){
        std::cout<<vec[i].input<<std::endl;
    }
}

void output_options(){
        printf("1 - create_automata\n");
        printf("2 - delete_automata\n");
        printf("3 - find_all\n");
        printf("4 - output_automata\n");
        printf("5 - Addition\n");
        printf("6 - Difference\n");
        printf("7 - End\n");
}

int main() 
{
    int in=0;
    int flag=0;
    int index;
    int index1;
    int index2;
    std::string find;
    std::vector<MyReg> vec;
    while(in!=6){
    output_options();
    scanf("%d", &in);
        switch (in)
        {
            case 1:
                create_automata(vec);
                break;
            case 2:
                delete_automata(vec);
                break;
            case 3:
                std::cout<<"input find "<<std::endl;
                std::cin>>find;
                std::cout<<"input index "<<std::endl;
                std::cin>>index;
                vec[index].find_all(find);
                break;
            case 4:
                output_automata(vec);
                break;
            case 5:
                std::cout<<"input index "<<std::endl;
                std::cin>>index;
                vec[index].addition(vec[index].input);
                break;
            case 6:
                std::cout<<"input index1 "<<std::endl;
                std::cin>>index1;
                std::cout<<"input index2 "<<std::endl;
                std::cin>>index2;
                std::cout<<"differ between 2nd and 1st: "<<std::endl;
                vec[index1].difference(vec[index1].input, vec[index2].input);
                break;
            case 7:
                flag=1;
                break;
            default:
                break;
        }
        if(flag==1){
            break;
        }
        in=0;
        scanf("%*[^\n]");
    }

//     std::string input = R"((ab)*)";

// //bc|(bb)* - дополнение
// //(ab)*c 
//     //ababbcac 
//     //R"(((a(b|c))+c)"
//     MyReg m(input);  
                                   
//      m.compile();
//      m.kPath();
//      m.checkKpath();
//     //  m.inverse(); 
//     //m.addition() - но для любого а не (a|b|c)*
//      m.addition();
//     //  m.difference();
//      m.compile();



    // for(auto& i : res){
    //     std::cout << i << std::endl;
    // }

    // a1
    // a2
    // a3=a1-a2
    // a3.match=a1.match && !a2.match
     
    return 0;                                      
}
