//THIS PROGRAM DOES NOT WORK REALLY WELL ANYMORE< I MESSED
//IT UP TRYING TO TEST SOME STUFF OUT ON IT

#include<iostream>
#include<assert.h>
#include<math.h>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<cstdlib>
#include<cmath>

using namespace std;

//returns true if testing vector is better than best case found //Test has less coins than best
bool compareVectors(vector<int> test, vector<int> best){
    int sumTest = 0;
    int sumBest = 0;
    for(int i = 0; i < test.size(); i++){
        //absolute value function is needed because some vector elements might
        //-1 as a result of it being a bad choice
        sumTest += abs(test.at(i));
        sumBest += abs(best.at(i));
    }
    
    if(sumTest < sumBest)
        return true;
    return false;
}

//recursive function turning EA to MU
void EarthToMartian(int ea, int earthCurr, int coinPos, vector<int> martianCurr,
    vector<int> &martianBest, vector<vector<int> > &martianCombos){
    
    
    
    if(earthCurr < 0){
        return;
    }else if(coinPos == 1){
        martianCurr[0] = earthCurr;
        if(compareVectors(martianCurr, martianBest))
            martianBest = martianCurr;
        martianCombos.push_back(martianCurr);
        return;
    }else if(earthCurr == 0){
        if(compareVectors(martianCurr, martianBest))
            martianBest = martianCurr;
        martianCombos.push_back(martianCurr);
        return;
    }
    
    if(ea % (coinPos*coinPos) == 0){
        martianCurr[coinPos-1] = ea / (coinPos * coinPos);
        if(compareVectors(martianCurr, martianBest))
            martianBest = martianCurr;
        martianCombos.push_back(martianCurr);
        return;
    }
    
    
    
    
    
    ++martianCurr[coinPos-1];
    EarthToMartian(ea,earthCurr- (static_cast <int>(pow(coinPos,2))), coinPos, 
        martianCurr, martianBest, martianCombos);

    --martianCurr[coinPos-1];
    
    //here we try to check if this is a bad case. if so we give up on the case
    //and we fill the rest of the coindemoninations not checked with a -1 so we
    //will know to put "*" in them in the file output. We push it back into the 
    //2D vector of martian combos and return
    if(!compareVectors(martianCurr, martianBest)){
        for(int i = coinPos-2; i >=0 ; --i)
            martianCurr.at(i)=-1;
        martianCombos.push_back(martianCurr);
        return;
    }
    //this is junk for now, i was trying to do the 3rd optimization "zero remainder"
    //but it failed to work
        // vector<int> v;
        // v.resize(coinPos);
        // v.at(coinPos-1) = earthCurr % coinPos;
        // if(earthCurr/coinPos == 0 && compareVectors(v,martianBest)){
        //     martianBest = v;
        //     martianCombos.push_back(v);
        //     return;
        // }else{
    EarthToMartian(ea,earthCurr, coinPos-1, martianCurr, martianBest, 
        martianCombos);
        // }
}

int main(int argc, char *argv[]){
    string currency;
    bool vPresent;
    
    ifstream fin;
    
    string filename;
    
    //opening the file, naming outputfile, determining verbose/////////
    if(argc > 2){
        vPresent = true;
        fin.open(argv[2]);
        filename = argv[2];
        
    }else{
        vPresent = false;
        fin.open(argv[1]);
        filename = argv[1];
        
    }
    
    //this will get you the filename but will exclude the extension so we can 
    //add our own extension
    if(filename.find(".") != string::npos)
            filename = filename.substr(0,filename.find("."));
        filename.append(".out");
        ofstream fout(filename.c_str());
        
    if(fout.is_open()){
        //looping through the file////////////////////////////////////
        while(!fin.eof()){
            vector<int> martianBest;
            vector< vector <int> > martianCombos;
            string currencyType;
            string currencyNum;
            getline(fin, currencyType,' ');
            getline(fin, currencyNum, '\n');
            
            if(currencyType == "EA"){
            //earthToMartian
                
                vector<int> martianCurr;
                string s;
                stringstream ss(currencyNum);
                ss>>s;
                int coinPos = static_cast<int>(sqrt((atoi(s.c_str()))));
                martianCurr.resize(coinPos);
                martianBest.resize(coinPos);
                //here we fill martianBest with the wocst case scenario, earthPos 1 coins
                martianBest.at(0) = atoi(s.c_str()); 
                //this is our recursive call to figure out the MUs
                EarthToMartian(atoi(s.c_str()),atoi(s.c_str()), coinPos, martianCurr, martianBest, martianCombos);
                //this will take care of verbose
                if(vPresent){
                    fout<<currencyType<<" "<<currencyNum<<" could be represented as:"<<endl;
                    for(int i = 0; i < martianCombos.size(); i ++){
                        fout<<"MU ";
                        //this while loop takes care of trailing zeros
                        while(martianCombos.at(i).at(martianCombos.at(i).size()-1) == 0)
                            martianCombos.at(i).pop_back();
                            //this for loop lates care of the -1s in the bad choice vectors
                            //and will fill the outpuf file
                        for(int j = 0; j < martianCombos.at(i).size(); j++){
                            if(martianCombos.at(i).at(j) == -1)
                                fout<<"*";
                            else
                                fout<<martianCombos.at(i).at(j);
                            if(j<martianCombos.at(i).size()-1){
                                fout<<",";
                            }
                        }
                        fout<<endl;
                    }
                    fout<<"A legal representation of this amount is:"<<endl;
                    fout<<"MU ";
                    while(martianBest.at(martianBest.size()-1)==0)
                        martianBest.pop_back();
                    for(int i = 0; i < martianBest.size(); i++){
                        fout<<martianBest.at(i);
                        if(i<martianBest.size()-1){
                            fout<<",";
                        }
                    }
                    fout<<endl;
                    
                }else{
                    //this is taking care of output file if there is NO VE
                    fout<<currencyType<<" "<<currencyNum<<" = "<<"MU ";
                    //this loop makes sure there are no trailing 0 at the end of mu
                    while(martianBest.at(martianBest.size()-1) == 0)
                        martianBest.pop_back();
                    //this loop prints out each mu coin
                    for(int i = 0; i < martianBest.size(); i++){
                        fout<<martianBest.at(i);
                        if(i<martianBest.size()-1){
                            fout<<",";
                        }
                    }
                    fout<<endl;
                }
                
                
                
            }else{
            //MartianToEarth`
                int earthVal = 0;
                currencyNum += ",";
                int coinPosCounter = 0;
                string s;
                stringstream ss(currencyNum);
                while(getline(ss,s,',')){
                    coinPosCounter++;
                    earthVal+= (atoi(s.c_str()))*(static_cast<int>(pow(coinPosCounter,2)));
                }
                currencyNum = currencyNum.substr(0,currencyNum.size()-1);
                fout<<currencyType<<" "<<currencyNum<<" = "<<"EA "<<earthVal<<'\n';
            }
        }
    }else{
        cout<<"Error Opening File"<<endl;
    }
    
    
    fin.close();
    fout.close();
    return 0;
}
