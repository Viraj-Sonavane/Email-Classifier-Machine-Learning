#include<iostream>
#include<string>
#include<map>
#include<set>
#include<vector>
#include<fstream>
#include<sstream>
#include<iterator>
#include <math.h>   
using namespace std;

void split(string &s, char delimiter,vector<string>&v2)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (tokenStream >> token)
   {
      v2.push_back(token);
   }
}

void prob_h(double n, double t,string f, map<string, pair<double, double>>&all, int k)
{
    double p = 0;
    p = k/(t+(k*n));
    all[f].first=p;     
}

//Calculating p(wi|ham) prob of spam if word occurs in spam and not in ham
void prob_s(double n,double ts, string f, map<string, pair<double, double>>&all, int k)
{   
    double p = 0;
    map<string, pair<double,double>>::iterator it;
    p = k/(ts+(k*n));
    it = all.find(f);
    if(it == all.end()) 
        cout<<"not find"<<endl;
    else
        all[f].second=p;
}

double prob1(double t,double tb,double k)
{
    return (t+k)/(tb+(k*2));
}

void prob2(double n, double t, string f,int c, map<string, pair<double, double> >&all, int k)
{
    double p = 0;
    p = (c+k)/(t+(k*n));
    all[f].first=p;  
}

void prob3(double n, double t,double th,string f,int c, map<string, pair<double, double>>&all, int k)
{
    double p = 0;
    map<string, pair<double,double>>::iterator it; 
    p = (c+k)/(t+(k*n));
    it = all.find(f);
    if(it == all.end())
    { 
        all[f].second=p; 
        prob_h(n,th,f,all,k);
    }    
    else
        all[f].second=p; 
}

double find_h(string f,map<string, pair<double, double>>&all,double total)
{
    map<string, pair<double,double>>::iterator it; 
    it = all.find(f);
    if(it == all.end()) 
        return log(1/(total));
    else
        return (log(it->second.first));         
}

double find_s(string f,map<string, pair<double, double>>&all,double total)
{
    map<string, pair<double,double>>::iterator it; 
    it = all.find(f);
    if(it == all.end()) 
        return log(1/(total));
    else
        return (log(it->second.second));         
}

int main(int argc,char* argv[])
{
    string s1;
    string s2;
    string line;
    double count1=0;
    double count2=0;
    double ham_TruePoisitve = 0;
    double ham_FalseNegative = 0;
    double spam_TrueNegative = 0;
    double spam_FalsePositive = 0;
    double distinct=0;
    double total=0;
    double m=0;
    double n=0;
    double logh1=0;
    double logs1=0;
    double logh2=0;
    double logs2=0;
    double sensitivity=0;
    double specificity=0;
    double accuracy=0;
    double Cwordham = 0;
    double Cwordspam = 0;

    set<string>dictonary;
    set<int>:: iterator it3;

    map<string,int>wordsHam_train;
    map<string,int>wordsSpam_train;
    map<string, pair<double, double>> both_train;
    
    map<string, int>::iterator it1; 
    map<string, int>::iterator it2; 
    map<string, pair<double, double>>::iterator it4;

    
    vector<string>vh_train;
    vector<string>vs_train;
    vector<string>vh_test;
    vector<string>vs_test;

    int k = atoi(argv[argc-1]);
    for(int i=1;i<3;i++)
    {
        ifstream infile(argv[i]);
        string filename(argv[i]);
        while(getline(infile,line))
        {
            if(filename.find("ham")!=string::npos||filename.find("Ham")!=string::npos){
                count1++;
                split(line,' ',vh_train);
            }
            else if(filename.find("spam")!=string::npos||filename.find("Spam")!=string::npos)
            {
                count2++;
                split(line,' ',vs_train);  
            }
       
            double a=0;
            double b=0;    
  
            //Putting values of ham in wordHam map
            for (auto & i : vh_train)
            {
                if(!vh_train.empty())
                    Cwordham++;
                    dictonary.insert(vh_train[a]);
                auto result = wordsHam_train.insert(pair<string, int>(i, 1));
                if (result.second == false)
                {
                    result.first->second++;
                }
                a++;
            }
            vh_train.clear();

            //Putting values of ham in wordSpam map
            for (auto & j : vs_train)
            {
                if(!vs_train.empty())
                    Cwordspam++;
                    dictonary.insert(vs_train[b]);
                auto result = wordsSpam_train.insert(pair<string, int>(j, 1));
                if (result.second == false)
                {
                    result.first->second++;
                }
                b++;    
            }
            vs_train.clear();
        }
       
    }
        
    /*------------------------------------------------------------------------*/    
    // Calculating first probability    
    m = prob1(count1,(count1+count2),k);
    n = prob1(count2,(count1+count2),k);
    /*-----------------------------------------------------------------------*/       

    //total size of dictonary
    distinct = dictonary.size();
    //total number of words
    total  = count1+count2;

    //Calculating P(wi|ham) second probability for ham
    for(it1=wordsHam_train.begin(); it1!=wordsHam_train.end();it1++)
    {
        prob2(distinct,Cwordham,it1->first,it1->second,both_train,k);
    }   

    //Calculating P(wi|spam) second probability for spam
    for(it2=wordsSpam_train.begin(); it2!=wordsSpam_train.end();it2++)
    {
        prob3(distinct,Cwordspam,Cwordham,it2->first,it2->second,both_train,k);
    }

    //Calculating P(wi|ham) prob of ham if word occurs in ham and not in spam    
    for(it1=wordsHam_train.begin(); it1!=wordsHam_train.end();it1++)
    {
        if(wordsSpam_train.find(it1->first)==wordsSpam_train.end())
        {
            prob_s(distinct,Cwordspam,it1->first,both_train,k);
        }
    }   
    
    /*----------------------------------------------------------------------------------*/
    logh1 = log(m);
    logh2 = log(m);
    logs1 = log(n);
    logs2 = log(n);

    for(int i=3;i<argc-1;i++)
    {
        ifstream infile(argv[i]);
        string filename(argv[i]);
        while(getline(infile,line)) 
        {
            if(filename.find("ham")!=string::npos||filename.find("Ham")!=string::npos)
            {
                split(line,' ',vh_test);
                    
                //calculate the unnormalized posterior probabilities of ham
                for(int i=0;i<vh_test.size();i++)
                {
                    logh1=logh1+find_h(vh_test[i],both_train,(Cwordspam+Cwordham));
                    logs1=logs1+find_s(vh_test[i],both_train,(Cwordspam+Cwordham));   
                }
                    
                if(logh1>logs1)
                {   
                    ham_TruePoisitve++;
                    cout<<"ham "<<logh1<< " " <<logs1<<endl;
                }
                else 
                {
                    ham_FalseNegative++;
                    cout<<"spam "<<logh1<< " " <<logs1<<endl;
                }
                    
                vh_test.clear();
                logh1 = log(m);
                logs1 = log(n);
            }
                
            else if (filename.find("spam")!=string::npos||filename.find("Spam")!=string::npos)
            {
                split(line,' ',vs_test);
                
                 //calculate the unnormalized posterior probabilities of spam
                for(int i=0;i<vs_test.size();i++)
                {  
                    logh2=logh2+find_h(vs_test[i],both_train,(Cwordspam+Cwordham));
                    logs2=logs2+find_s(vs_test[i],both_train,(Cwordspam+Cwordham));              
                }

                if(logh2>logs2)
                {
                    spam_FalsePositive++;
                    cout<<"ham "<<logh2<< " " <<logs2<<endl;
                }
    
                else 
                {
                    spam_TrueNegative++;
                    cout<<"spam "<<logh2<< " " <<logs2<<endl;
                }
                
                vs_test.clear();
                logh2 = log(m);
                logs2 = log(n);
            }
        }
    }  
    specificity = (spam_TrueNegative/(spam_TrueNegative+spam_FalsePositive));    
    sensitivity = (ham_TruePoisitve/(ham_TruePoisitve+ham_FalseNegative));
    accuracy = ((ham_TruePoisitve+spam_TrueNegative)/(ham_TruePoisitve+spam_TrueNegative+ham_FalseNegative+spam_FalsePositive));
    cout<<specificity<< " " <<sensitivity<< " " << accuracy << endl;
    return 0;
}

