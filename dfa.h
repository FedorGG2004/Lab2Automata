#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "tree.h"
#include <map>
#include <set>
class State {
public:
    std::vector<int> id_set;
    int id;
    std::unordered_map<std::string, std::vector<int>> transitions;
    bool final;
    int num;
    bool error = false;
    State() = default;
    State(const std::vector<int> id_list, int id, int terminal_id, const std::vector<std::string>& alphabet)
        : id(id), final(false) {
        for (int id : id_list) {
            id_set.push_back(id);
        }
        final = (std::find(id_set.begin(),id_set.end(),terminal_id) != id_set.end());
        for (const std::string& a : alphabet) {
            transitions[a] = {};
        }
    }
    void init(const std::vector<int> id_list, int id, int terminal_id, const std::vector<std::string>& alphabet)
        {
        this->id = id;
        for (int id : id_list) {
            id_set.push_back(id);
        }
        final = (std::find(id_set.begin(), id_set.end(), terminal_id) != id_set.end());
        for (const std::string& a : alphabet) {
            transitions[a] = {};
        }
    }

    State(const State& other) {
        id_set = other.id_set;
        id = other.id;
        final = other.final;
        copyTransitions(other.transitions);
        error = other.error;
        num = other.num;
    }

 
    State& operator=(const State& other) {
        if (this != &other) {
            id_set = other.id_set;
            id = other.id;
            final = other.final;
            num = other.num;
            error = other.error;
            copyTransitions(other.transitions);
        }
        return *this;
    }
    bool operator==(const State& other) const {
        return id_set == other.id_set && id == other.id && transitions == other.transitions && final == other.final && error == other.error;
    }

private:
    void copyTransitions(const std::unordered_map<std::string, std::vector<int>>& otherTransitions) {
        transitions.clear();
        for (const auto& entry : otherTransitions) {
            const std::vector<int> original_vector = entry.second;
            std::vector<int> copied_vector(original_vector.begin(), original_vector.end());
            transitions[entry.first] = copied_vector;
        }
    }
};

class DFA {
private:
    std::map<std::vector<int>, std::string> myMap;
public:
    std::vector<State> states;
    int id_counter;
    int terminal;
    std::vector<std::string> alphabet;

    DFA(const std::vector<std::string>& alphabet, const Tree& tree) : alphabet(alphabet), id_counter(1) {
        terminal = tree.id_counter - 1; 
        compute_states(tree);
    }
    DFA ()= default;

    std::vector<State>& multiply(std::vector<State> &newStates, DFA& d)
    {
        id_counter = 2;
        std::vector<int> index;
        std::unordered_map<std::string, std::vector<int>> transitions;
        for (int i = 0; i < d.states.size(); i++)
        {

            for (int j = 0; j < states.size(); j++)
            {
                index.clear();
                transitions.clear();
                index.push_back(d.states[i].id);
                index.push_back(states[j].id);

                for (auto a : alphabet)
                {
                    transitions[a].push_back(d.states[i].transitions[a][0]);
                    transitions[a].push_back(states[j].transitions[a][0]);
                }
                // for (auto a : alphabet)
                // {
                //     // transitions[a].push_back(d.states[i].transitions[a][0]);
                //     transitions[a].push_back(states[j].transitions[a][0]);
                // }
                // for (auto a : d.alphabet)
                // {
                //     transitions[a].push_back(d.states[i].transitions[a][0]);
                //     // transitions[a].push_back(states[j].transitions[a][0]);
                // }
                State newState;
                if (index[0] == 1 && index[1] == 1)
                {
                     newState.init(index,1, -1, alphabet);
                }
                else
                {
                     newState.init(index, id_counter++, -1, alphabet);
                }
                if (d.states[i].final == true && states[j].final == false)
                {
                    newState.final = true;
                }
                newState.transitions = transitions;
                newStates.push_back(newState);
            }
        }
        terminal = id_counter;
        return newStates;
    }
std::vector<State>& difference(std::vector<State> &newStates, DFA& d, std::vector<std::string> &alphabet2)
    {
        id_counter = 2;
        std::vector<int> index;
        std::unordered_map<std::string, std::vector<int>> transitions;
        for (int i = 0; i < d.states.size(); i++)
        {

            for (int j = 0; j < states.size(); j++)
            {
                index.clear();
                transitions.clear();
                index.push_back(d.states[i].id);
                index.push_back(states[j].id);

                for (auto a : alphabet2)
                {
                    transitions[a].push_back(d.states[i].transitions[a][0]);
                    transitions[a].push_back(states[j].transitions[a][0]);
                }
                // for (auto a : alphabet)
                // {
                //     // transitions[a].push_back(d.states[i].transitions[a][0]);
                //     transitions[a].push_back(states[j].transitions[a][0]);
                // }
                // for (auto a : d.alphabet)
                // {
                //     transitions[a].push_back(d.states[i].transitions[a][0]);
                //     // transitions[a].push_back(states[j].transitions[a][0]);
                // }
                State newState;
                if (index[0] == 1 && index[1] == 1)
                {
                     newState.init(index,1, -1, alphabet2);
                }
                else
                {
                     newState.init(index, id_counter++, -1, alphabet2);
                }
                if (d.states[i].final == true && states[j].final == false)
                {
                    newState.final = true;
                }
                newState.transitions = transitions;
                newStates.push_back(newState);
            }
        }
        terminal = id_counter;
        return newStates;
    }


    void createAddition( DFA& d)
    {
        std::vector<State> newStates;
        newStates = multiply(newStates, d);
        states = newStates;
    }
    void createDifference( DFA& d, std::vector<std::string> &alphabet2)
    {
        std::vector<State> newStates;
        newStates = difference(newStates, d, alphabet2);
        states = newStates;
    }


    std::string createRegex()
    {
        myMap.clear();
        int num0 = 0;
        std::vector<int> finals;
        std::string temp;
        int i=1;
        for (auto& state : states)
        {
            num0++;
            state.num = num0;
            if (state.final)
                finals.push_back(num0);
            if (state.id == 1)
                i = num0;
        }
        std::string result;
        for (int j : finals)
        {
           temp = createKPath(num0, i, j);
          //  temp = createKPath(2, 1, 3);
           result += "(" + temp + ")|";
        }
        result.pop_back();
        return result;
    }

    
    std::string createKPath(int k, int i, int j)
    {
        std::vector<int> vecIndex = { i,j,k };
        auto it = myMap.find(vecIndex);
        if (it != myMap.end()) 
            return it->second;
        std::string tmp;
        State s1;
        State s2;
        if (k == 0)
        {
            for (auto s : states)
            {
                if (s.num == i)
                {
                    s1 = s;
                }
                if (s.num == j)
                {
                    s2 = s;
                }
            }
            tmp += "(";
            std::string tmp2 = "";
            if (s1.num == s2.num)
                tmp2 += "$|";
            for (auto a : alphabet)
            {     
                if (s1.transitions[a][0] == s2.id) {
                    tmp2 += a;
                    tmp2+= "|";
                }
            }
            if(!tmp2.empty())
                tmp2.pop_back();
            else {
                myMap[vecIndex] = "";
                return "";
            }
            if (tmp2.size() == 1)
            {
                myMap[vecIndex] = tmp2;
                return tmp2;
            }
               
            tmp += tmp2;
            tmp += ")";
            myMap[vecIndex] = tmp;
            return tmp;
        }
        std::string f1 = createKPath(k - 1, i, j);
        std::string f2 = createKPath(k - 1, i, k);
        std::string f3 = createKPath(k - 1, k, k);
        std::string f4 = createKPath(k - 1, k, j);
        if (f2.empty() || f3.empty() || f4.empty()) 
        {
            f4=f3=f2 = "";
        }
        if (!f3.empty())
            f3 ="("+ f3 +")"+ "*";
        if (!f4.empty())
            f4 = "(" + f4 + ")";
        if (!f2.empty())
            f2 = "(" + f2 + ")";
        f2 = f2 + f3 + f4;
        if (k+1==i==j&& !f2.empty() )
            f2 = "(" + f2 + ")*";
        if (!f1.empty() && f2 != "")
        {
            f1 = ""+f1 + "|";
        }
        myMap[vecIndex] = f1 + f2;
       return f1  + f2 ;
       // return  createKPath(k - 1, i, j) +"|(:$" + createKPath(k - 1, i, k)+ "(:" + createKPath(k - 1, k, k) + ")..." + createKPath(k - 1, k, j) + ")";
    }



    int  getIndex(const std::vector<std::vector<State>> &partition,int id) 
    {
        for (int i = 0; i < partition.size(); i++)
        {
            for (int j = 0; j < partition[i].size(); j++)
            {
                if (partition[i][j].id == id)
                    return i;
            }
        }
    }

    std::vector<std::vector<State>> getPartition(std::vector<State > vec, const std::vector<std::vector<State>>& partition)
    {
        std::vector<State> vec1;
        std::vector<std::vector<State>> newpartition;
        std::vector<int> remember = {};
        for (int i = 0; i < vec.size(); i++)
        {
            bool flag = false;
            int k = 0;
            for (int j = i+1; j < vec.size(); j++)
            {
                for (auto a : alphabet)
                {
                    int s1 = getIndex(partition, vec[i].transitions[a][0]);
                    int s2 = getIndex(partition, vec[j].transitions[a][0]);
                    if (getIndex(partition, vec[i].transitions[a][0]) != getIndex(partition, vec[j].transitions[a][0]))
                    {
                        flag = true; 
                    }
                }
                if (!flag && std::find(remember.begin(), remember.end(), j) == remember.end())// 1 2 3    2 3  3 
                {
                    if (k == 0)
                    {
                        vec1.push_back(vec[i]);
                        k = 1;
                    }
                    vec1.push_back(vec[j]);
                    remember.push_back(i);
                    remember.push_back(j);
                }
                flag = false;
            }
            if (vec1.empty() && std::find(remember.begin(),remember.end(),i)==remember.end())
                vec1.push_back(vec[i]);
            newpartition.push_back(vec1);
            vec1.clear();
        }
        // vecOfVec.erase(std::remove_if(vecOfVec.begin(), vecOfVec.end(), [](const std::vector<int>& v) { return v.empty(); }), vecOfVec.end());
        newpartition.erase(std::remove_if(newpartition.begin(), newpartition.end(), [](const std::vector<State>& v) { return v.empty(); }), newpartition.end());
        return newpartition;
    }
    using VectorOfVectors = std::vector<std::vector<State>>;

    void insertVectorOfVectors(const VectorOfVectors& source, VectorOfVectors& destination) {
        for (const auto& innerVec : source) {
            destination.push_back(innerVec);
        }
    }

    void recreateTransitions(const std::vector<std::vector<State>>& partition)
    {
        std::vector<int> empty_id_list;
        states.clear();
        id_counter = 2;
        bool flag = false;
        bool  flagforfinal = false;
        for (int i = 0; i < partition.size(); i++)
        {
            for (int j= 0; j < partition[i].size(); j++)
            {
                empty_id_list.push_back(partition[i][j].id);
                if (partition[i][j].id == 1)
                {
                    flag = true;
                }
                if (partition[i][j].final == true)
                {
                    flagforfinal = true;
                }
            }
            if (flag)
            {
                State D1(empty_id_list, 1, terminal, alphabet);
                D1.transitions = partition[i][0].transitions; 
                if (flagforfinal)
                    D1.final = true;
                states.push_back(D1);
            }
            else
            {
                State D1(empty_id_list, id_counter++, terminal, alphabet);
                D1.transitions = partition[i][0].transitions;
                if (flagforfinal)
                    D1.final = true;
                states.push_back(D1);
            }
            flagforfinal = false;
            flag = false;
            empty_id_list.clear();
        }
        for (int i = 0; i < states.size(); i++)
        {
            for (auto a:alphabet)
            {
                int tr = states[i].transitions[a][0];
                for (int k = 0; k < states.size(); k++)
                {
                    if (std::find(states[k].id_set.begin(), states[k].id_set.end(), tr) != states[k].id_set.end())
                    {
                        states[i].transitions[a][0] = states[k].id;
                        break;
                    }
                }
            }
        }
        terminal = id_counter;
    }

    void compile()
    {
        std::vector<std::vector<State>> partition;
        std::vector<std::vector<State>> copy;
        std::vector<State> vec1;
        std::vector<State> vec2;
        for (auto& s1 : states)
        {
            if (s1.final)
                vec1.push_back(s1);
            else
                vec2.push_back(s1);
        }
        partition.push_back(vec2);
        partition.push_back(vec1);
   
        int k = -1;

     while (partition.size() !=k)
        {

        for (auto& vec0 : partition) {
            insertVectorOfVectors(getPartition(vec0, partition), copy);
        }
        k = partition.size();
        partition = copy;
        copy.clear();
 
        }
        recreateTransitions(partition);
    }

    void init(const std::vector<std::string>& alphabet, const Tree& tree)
    {
        this->alphabet = alphabet;
        id_counter = 1;
        terminal= tree.id_counter - 1;
        compute_states(tree);
    }

   
    void compute_states(const Tree& tree) {
        states.clear();
        std::vector<int> my_vector;
        for (auto it = tree.root->firstpos.begin(); it != tree.root->firstpos.end(); ++it) {
            my_vector.push_back(*it);
        }
        std::vector<State> queue;
        State D1(my_vector, id_counter++, tree.id_counter - 1, alphabet);
        queue.push_back(D1);
        std::vector<std::vector<int>> new_states;
        while (!queue.empty()) {
            State st = queue.back(); //states.back
            states.push_back(st);
            new_states = Dtran(&st, tree, queue);
            queue.pop_back();
            states.pop_back();
            //final
            states.push_back(st);
            for (const auto& s : new_states) {
                State newState(s, id_counter++, tree.id_counter - 1, alphabet);
                queue.push_back(newState); //&copy
            }
        }
    }

    std::vector<std::vector<int>> Dtran(State* state, const Tree& tree, std::vector<State>& queue) {
        std::vector<std::vector<int>> new_states;
        for (int i : state->id_set) {
            if (i == terminal) {
                continue;
            }
            std::string label;
            for (const auto& pair : tree.leaves) { 
                if (pair.first == i)
                    label = pair.second;
            }
           
            if (state->transitions[label].empty()) {
                for (auto it = tree.followpos[i].begin(); it != tree.followpos[i].end(); ++it) {
                    state->transitions[label].push_back(*it);
                }
            }
            else {
                for (auto it = tree.followpos[i].begin(); it != tree.followpos[i].end(); ++it) {
                    if(std::find(state->transitions[label].begin(), state->transitions[label].end(),*it)== state->transitions[label].end())
                    state->transitions[label].push_back(*it);
                }
            }
        }
        for ( std::string& a : alphabet) {
            if (!state->transitions[a].empty()) {
                bool is_new_state = true;
                for (auto& s : states) {
                    if (s.id_set == state->transitions[a] ) {
                        is_new_state = false;
                        break;
                    }
                } 
                for (auto& s : queue) {
                    if (s.id_set == state->transitions[a]) {
                        is_new_state = false;
                        break;
                    }
                } 

                for (auto& s : new_states) {
                    if ( state->transitions[a]==s) {
                        is_new_state = false;
                        break;
                    }
                }
                if (is_new_state) {
                    new_states.push_back(state->transitions[a]);
                }
            }
        }
        return new_states;
    }

    void post_processing() {

        bool has_none_state = false;
        for (auto& state : states) {

            for (const std::string& a : alphabet) {
              
                if (state.transitions[a].empty()) {
                    has_none_state = true;
                    state.transitions[a] = {};
                    state.transitions[a].push_back(terminal+1); 
                }
            }
            
        }
        for (auto& state : states) {

            for (const std::string& a : alphabet) {
                for (auto& s : states) {
                    if (s.id_set == state.transitions[a]) {
                        state.transitions[a] = {};
                        state.transitions[a].push_back(s.id); 
                       break;

                    }
                }
            }
        }
        if (has_none_state) {
            std::vector<int> empty_id_list;
            State new_state(empty_id_list, terminal+1, terminal, alphabet);
            new_state.error = true;
            states.push_back(new_state);
            for (const std::string& a : alphabet) {
                states.back().transitions[a] = {};
                states.back().transitions[a].push_back(states.back().id); //clear
            }
        }
    }

    std::string to_string() {
     
        std::string s;
        int k = 1;
        for ( auto& state : states) {
            if (state.id == 1) {
                s += "->\t";
            }
            else {
                s += "\t";
            }
            if (state.id == terminal + 1)
            {
                s += std::to_string(id_counter) + "\t";
            }
            else
            s += std::to_string(state.id) + "\t";
            for (const std::string& a : alphabet) {
                s += a;
                s += " : ";
                if ((state.transitions[a])[0] == terminal + 1)
                {
                    s += std::to_string(id_counter);
                }
                else
                    s += std::to_string((state.transitions[a])[0]);
                s += " ";
            }
            if (state.final) {
                s += "Final State";
            }
            s += "\n";
        }
        return s;
    }
};
