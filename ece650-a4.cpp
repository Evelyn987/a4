#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <stdbool.h>
#include <memory>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include <iostream>
using namespace std;

int V_count;
vector<int> set1;                              
vector<int> set2;
std::vector<std::string> split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;
    unsigned int size=str.size();
    for(unsigned int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

void cnf()
{
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    int k;
    for(k=1; k<=V_count ;++k)
    {
        Minisat::Lit vk[V_count][k];
        for(int x=0; x<V_count; ++x)
        {
            for(int y=1; y<=k; ++y)
            {
              //Minisat::vec <Minisat::vec<Minisat::Lit>> vk[n][i];
              vk[x][y] = Minisat::mkLit(solver->newVar());
            }
        }

        Minisat::vec<Minisat::Lit> var;
        for(int y=1; y<=k; ++y)
        {
            for(int x=0; x<V_count; ++x)
            {
              var.push(vk[x][y]);
            }
            solver->addClause(var);
            var.clear();
        }

        for(int x=0; x<V_count; ++x)
        {
            if(k==1)
            {
              break;
            }
            if (k!=1)
            {
              for(int i=1; i<=k-1; ++i)
              {
                for(int j=i+1; j<=k; ++j)
                {
                  solver->addClause(~vk[x][i], ~vk[x][j]);
                }
              }
            }   
        }
        for(int y=1; y<=k; ++y)
        {
            if(V_count==1)
            {
              break;
            }
            if (V_count!=1)
            {
              for(int n=0; n<V_count-1; ++n)
              {
                for(int j=n+1; j<V_count; ++j)
                {
                  solver->addClause(~vk[n][y], ~vk[j][y]);
                }
              } 
            }   
        }
        Minisat::vec<Minisat::Lit> vars;
        for(unsigned int i=0; i<set1.size(); ++i)
        {
            int vx = set1[i];
            int vy = set2[i];
            for(int n=1; n<=k; ++n)
            {
              vars.push(vk[vx][n]);
            }
            for(int n=1; n<=k; ++n)
            {
              vars.push(vk[vy][n]);
            }
            solver->addClause(vars);
            vars.clear();
        }
        bool res = solver->solve();
        // std::cout << "The result is: "  << res << std::endl;
        // std::cout << "The k is: " << k << "\n";
        if(res == 1)
        {
          for(int x=0; x<V_count; ++x)
          {
              for(int y=1; y<=k; ++y)
              {
                //std::cout<<Minisat::toInt(solver->modelValue(vk[x][y]))<< " ";
                //if (Minisat::toInt(vk[x][y])== 0);
                 if(Minisat::toInt(solver->modelValue(vk[x][y]))==0)
                {
                  std::cout << x <<" ";
                }
              }
          }
          std::cout << endl;
          break;
        }
        solver.reset (new Minisat::Solver());
    }
}

int main()
{
    while (!std::cin.eof())
    {
        string str;
        getline(cin,str);
        vector<int> setall;  
        std::string e1;
        std::string e2;     
        int allv;
        bool done = false; 
        if (str[0] == 'V')
        {
           int allnumber=0;
           for(int i=0;str[i]!='\0';++i) 
           {
              if(str[i]>='0'&& str[i]<='9') 
              {
                  allnumber *= 10;
                  allnumber += str[i]-'0';                      
              }
           } 
           allv = allnumber;                             //number of vertex
           done = false;
           setall.clear();
           set1.clear();
           set2.clear();
           e1.clear();
           e2.clear();
        }
        if (str[0] == 'E')
        { 
          if ((str == "E { }")||(str == "E {}")==1)
          {
            done = true;
            continue;
          }
          if (done == false)
          {
            std::vector<std::string> result2;
            std::vector<std::string> result3;
            std::vector<std::string> result1;
            std::vector<std::string> result=split(str,">,<");
            result[0].erase (0,4);    
            for(std::size_t i=0; i< result.size(); i++)
            {
              result1 = split(result[i],",");  
              e2 += (result1[1] + ' ');
              e1 += (result1[0] + ' ');    
            }              
            e2.erase(e2.end()-1); 
            e2.erase(e2.end()-1);                         //all the edges  
            result2=split(e1," ");
            result3=split(e2," ");
            for (int i = 0 ; result2[i]!="\0" ; ++i)
            {
              int all1 = atoi(result2.at(i).c_str());                        
              int all2 = atoi(result3.at(i).c_str());                         
              setall.push_back(all1);                                          //all v
              set1.push_back(all1);                                            //first line of v
              setall.push_back(all2);
              set2.push_back(all2);                                           //second line of v
            }
            int max = setall[0];
            for (unsigned int i = 1 ; i<setall.size() ; ++i)
            {
              if(setall[i]> max)
              {
                max = setall[i];
              }
            }
            V_count = max+1;
            for(unsigned int i=0; i<setall.size(); ++i)
            {
              if (setall[i]> allv)
              {
                std::cerr<< "Error: The number of vertexes is wrong." << endl;
                break;
              }
            }
            done = true;
          }
          else
          {
            std::cerr << "Error: Wrong command input\n";
          }
          cnf(); 
        }       
    }  
}
