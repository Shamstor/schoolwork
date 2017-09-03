#include<iostream>
#include<math.h>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<cstdlib>
#include<cmath>

using namespace std;

//returns true if testing vector is better than best case found //Test has less coins than best
bool compareVectors(vector<int> test, vector<int> best){
    int sumTest = 0;
    int sumBest = 0;
    for(int i = 0; i < test.size(); i++){
        //absolute value function is needed because some vector elements might
        //-1 as a result of it being a bad choice//this is not really needed this time
        sumTest += abs(test.at(i));
        sumBest += abs(best.at(i));
    }
    
    if(sumTest < sumBest)
        return true;
    return false;
}
void displayVector(vector<vector<int> > v){
    for(int i = 0; i < v.size(); i++){
        for(int j = 0; j < v.at(i).size(); j++){
            cout<<v.at(i).at(j)<<" ";
        }
        cout<<endl;
    }
}
//for this program, we know v is always going to be bigger in size than b
void addVectors(vector<int> &v, vector<int> b){
        for(int i = 0; i < b.size(); i++){
            v.at(i) += b.at(i);
        }
}
void clearVectorCells(vector<int> &v){
    for(int i = 0; i <v.size(); i++)
        v.at(i) = 0;
}
int coinsInVector(vector<int> v){
    int sum = 0;
    for(int i = 0; i < v.size(); i ++)
        sum += v.at(i);
    return sum;
}

//brute force algorithim
void dumb(int earthCurr, int coinPos, vector<int> martianCurr,
    vector<int> &martianBest, int &ticks){
        if(earthCurr < 0){
        return;
    }else if(coinPos == 1){
        ticks++;
        martianCurr[0] = earthCurr;
        if(compareVectors(martianCurr, martianBest))
            martianBest = martianCurr;
        return;
    }else if(earthCurr == 0){
        ticks++;
        if(compareVectors(martianCurr, martianBest))
            martianBest = martianCurr;
        
        return;
    }
    
    ++martianCurr[coinPos-1];
    dumb(earthCurr- (static_cast <int>(pow(coinPos,2))), coinPos, 
        martianCurr, martianBest, ticks);
        
    --martianCurr[coinPos-1];
    dumb(earthCurr, coinPos-1, martianCurr, martianBest, ticks);
    ticks++;
}
    
//optimized brute force algoritim: lagrange 4 square theorem, giving up early
//on a bad choice, using bottom upoik+
void smart(int earthCurr, int coinPos, vector<int> martianCurr,
    vector<int> &martianBest, int &ticks){
    
    if(earthCurr < 0){
        return;
    }else if(coinPos == 1){
        ticks++;
        martianCurr[0] = earthCurr;
        if(compareVectors(martianCurr, martianBest))
            martianBest = martianCurr;
        
        return;
    }else if(earthCurr == 0){
        ticks++;
        if(compareVectors(martianCurr, martianBest))
            martianBest = martianCurr;
        return;
    }
    //implementing lagrange 4 square theorem
    if((coinPos*coinPos) < earthCurr/4){
        return;
    }
    
    ++martianCurr[coinPos-1];
    smart(earthCurr- (static_cast <int>(pow(coinPos,2))), coinPos, 
        martianCurr, martianBest, ticks);

    --martianCurr[coinPos-1];
    
    //lagarnge implementation again here
    if(coinsInVector(martianCurr)>=4 && earthCurr != 0){
        return;
    }
    //here we try to check if this is a bad case. 
    if(!compareVectors(martianCurr, martianBest)){
        return;
    }
    
    smart(earthCurr, coinPos-1, martianCurr, martianBest, ticks);
    ticks++;
}

void cache(int earthCurr, vector<vector<int> > &cache, vector<int> &coins,
    vector<int> &ticks){
        
    for(int i = 1; i <=earthCurr; i++){
        vector<int> martianCurr;
        martianCurr.resize(sqrt(i));
        int largestCoinPos = sqrt(i);
        
        if((largestCoinPos*largestCoinPos == i)){
        
            martianCurr.at(largestCoinPos-1) =1;
            cache.push_back(martianCurr);
            coins.push_back(1);
           
            ticks.at(i-1)++; 
            
        }else{
            
            martianCurr.at(0) = i;
            cache.push_back(martianCurr);
            coins.push_back(coinsInVector(martianCurr));
            
            for(int j = largestCoinPos; j >=1; j--){
                
                ticks.at(i-1)++;
                //if((j*j) < earthCurr/4)
                //    break;
                clearVectorCells(martianCurr);
                martianCurr.at(j-1) = 1;
                int remainder = i - (j*j);;
                addVectors(martianCurr, cache.at(remainder-1));
             
                if(coins.at(i-1)>coinsInVector(martianCurr)){
                    coins.at(i-1) = coinsInVector(martianCurr);
                    cache.at(i-1) = martianCurr;
                }
                //if(coins.at(i-1) <=2)
                  //  break;
                    
                    
            }
        } 
    }
}

int main(int argc, char *argv[]){
    vector<vector<int> >dumbTable; 
    vector<vector<int> >smartTable; 
    vector<vector<int> >cacheTable;
    vector<vector<int> >outputTable;
    int dTick, sTick; dTick = sTick = 0; 
    vector<int> cTick; vector<int> coins;
    int flagD, flagS, flagC; flagD = flagC = flagS = 0;
    bool flagError = false;
    string filename1 = ""; string filename2 = "";
    int eaNum = 0;
    stringstream ss;
   
    string d = "-dumb";
    string s = "-smart";
    string c = "-cache";
    
            filename1 = argv[argc-1];
            filename1.append(".csv");
           
    
    for(int i = 1; i < argc -1; i++){

        
        string test = argv[i];
        if(i == 1){
            ss<<argv[i];
            ss>>eaNum;
            
        }else if(test == "-dumb"){
            
            flagD++;
        }else if(test == "-smart"){
            
            flagS++;
        }else if(test== "-cache"){
            flagC++;
            
        }else{
            flagError = true;
            
        }
        }
    
    
    if(flagC > 1 || flagD > 1 || flagS > 1 || (flagS+flagD+flagC == 0)){
        flagError = true;
    }
    cout<<0;
    if(!flagError){
        ofstream fout(filename1.c_str());
        // ea,coinPos, vector<int> martianCurr, vector<int> &martianBest, int &ticks){
        if(flagD==1){
            outputTable.clear();
            fout<<"dumb"<<endl;
            for(int i = 0; i < eaNum; i ++){
                fout<<i+1;
                if(i+1 < eaNum)
                    fout<<",";
            }
            fout<<endl;
            for(int i = 1; i <=eaNum; i++){
                vector<int> martianCurr;
                martianCurr.resize(sqrt(i));
                vector<int> martianBest;
                martianBest.resize(sqrt(i));
                martianBest.at(0) = i;
                dumb(i,  static_cast<int>(sqrt(i)), martianCurr, martianBest, dTick);
                dumbTable.push_back(martianBest);
                fout<<dTick;
                if(i < eaNum)
                    fout<<",";
            }
            fout<<endl;
            for(int i= 0; i < dumbTable.size(); i++){
                while(dumbTable.at(i).at(dumbTable.at(i).size()-1) == 0)
                    dumbTable.at(i).pop_back();
                outputTable.push_back(dumbTable.at(i));
                
            }
        }
        
        
        if(flagS == 1){
            outputTable.clear();
            fout<<"smart"<<endl;
            for(int i = 0; i < eaNum; i ++){
                fout<<i+1;
                if(i+1 < eaNum)
                    fout<<",";
            }
            fout<<endl;
            //int earthCurr, int coinPos, vector<int> martianCurr, vector<int> &martianBest, int &ticks
            for(int i = 1; i <=eaNum; i++){
                vector<int> martianCurr;
                martianCurr.resize(sqrt(i));
                vector<int> martianBest;
                martianBest.resize(sqrt(i));
                martianBest.at(0) = i;
                smart(i,  static_cast<int>(sqrt(i)), martianCurr, martianBest, sTick);
                smartTable.push_back(martianBest);
                fout<<sTick;
                if(i < eaNum)
                    fout<<",";
            }
            fout<<endl;
            for(int i= 0; i < smartTable.size(); i++){
                while(smartTable.at(i).at(smartTable.at(i).size()-1) == 0)
                   smartTable.at(i).pop_back();
                outputTable.push_back(smartTable.at(i));
                
            }
        }
        if(flagC == 1){
            
            outputTable.clear();
            int cTick2 = 0;
            fout<<"cache"<<endl;
            for(int i = 0; i < eaNum; i++){
                fout<<i+1;
                if(i+1<eaNum)
                    fout<<",";
            }
            
            fout<<endl;
            
            cTick.resize(eaNum);
            
            //int earthCurr, vector<vector<int> > &cache, vector<int> &coins,vector<int> &ticks
            cache(eaNum, cacheTable,coins, cTick);
            
            for(int i = 0; i <cTick.size(); i++){
                cTick2+= cTick.at(i);
                fout<<cTick2;
                if(i +1 < cTick.size()){
                    fout<<",";
                }
            }
            cout<<3;
            fout<<endl;
            for(int i= 0; i < cacheTable.size(); i++){
                while(cacheTable.at(i).at(cacheTable.at(i).size()-1) == 0)
                   cacheTable.at(i).pop_back();
                 outputTable.push_back(cacheTable.at(i));
                 
                 
            }
        }
        
        fout.close();
        filename1 = argv[argc-1];
        filename1.append(".out");
        fout.open(filename1.c_str());
        for(int i = 0; i < outputTable.size(); i++){
            fout<<"EA "<< i +1<<" = MU ";
            for(int j = 0; j < outputTable.at(i).size(); j++){
                fout<<outputTable.at(i).at(j);
                if(j+1 < (outputTable.at(i).size()))
                    fout<<",";
            }
            fout<<endl;
        }
    }else{
        cout<<"Error was made!";
    }
    
    
    return 0;
}








