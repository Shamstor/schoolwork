#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<vector>
#include<list>
#include<cmath>
#include<string>
#include<iterator>

using namespace std;

//fraction class was given to us by professor Molle
class Fraction
{
    int gcd(int a, int b) {return b==0 ? a : gcd(b,a%b); }
    int n, d;
 public:
    Fraction(int n, int d = 1) : n(n/gcd(n,d)), d(d/gcd(n,d)) {}
    int num() const { return n; }
    int den() const { return d; }
    Fraction& operator*=(const Fraction& rhs) {
        int new_n = n*rhs.n / gcd(n*rhs.n, d*rhs.d);
        d = d*rhs.d / gcd(n*rhs.n, d*rhs.d);
        n = new_n;
        return *this;
    }
};
std::ostream& operator<<(std::ostream& out, const Fraction& f){
   return out << f.num() << '/' << f.den() ;
}
bool operator==(const Fraction& lhs, const Fraction& rhs) {
    return ((lhs.num() * rhs.den()) == (rhs.num() * lhs.den()));
}
bool operator<(const Fraction& lhs, const Fraction& rhs) {
    return (lhs.num() * rhs.den() < rhs.num() * lhs.den());
}
bool operator>(const Fraction& lhs, const Fraction& rhs) {
    return (lhs.num() * rhs.den() > rhs.num() * lhs.den());
}
Fraction operator*(Fraction lhs, const Fraction& rhs)
{
    return lhs *= rhs;
}




template<typename T>
typename list<T>::iterator merge(typename list<T>::iterator lIter, int leftSize, 
  typename list<T>::iterator rIter, int rightSize, list<T> &l){
    int lCount= 0;
    int rCount= 0;
    typename list<T>::iterator tempIter;

    if(*lIter < *rIter){
    
        tempIter = lIter;
        ++lIter;
        ++lCount;
    }else{
        lIter = l.insert(lIter, *rIter);
        rIter = l.erase(rIter);
        tempIter = lIter;
        rCount++;
    }
    while (lCount <= leftSize && rCount < rightSize &&  rIter != lIter){
        if(*lIter < *rIter){
            lIter++;
            lCount++;
        }else{
            lIter = l.insert(lIter, *rIter);
            rIter = l.erase(rIter);
            rCount++;
        }
    }
    return tempIter;

}

template<typename T>
typename list<T>::iterator mergeSort(list<T> &l,  typename list<T>::iterator iter, int size){
    int leftSize = size/2;
    int rightSize = size-leftSize;
    typename list<T>::iterator lIter = iter;
    typename list<T>::iterator rIter = iter;
    advance(rIter, leftSize);

    if(size <= 1){
        return lIter;
    }
    lIter  = mergeSort(l, lIter, leftSize);
    rIter =  mergeSort(l, rIter, rightSize);
    return merge(lIter, leftSize, rIter, rightSize, l);
}




int main(int argc, char *argv[]){
    string filename, type, nums;
    int num, n, d;
    //cout<<"inside main 0"<<endl;
    list<int> l1;
    list<Fraction> l2;
    fstream fin;
    
   //cout<<"inside main 1"<<endl;
    fin.open(argv[1]);
    //cout<<1;
    filename = argv[1];
    //cout<<2;
    if(filename.find(".") != string::npos)
            filename = filename.substr(0,filename.find("."));
    filename.append(".out");
    //cout<<3;
    ofstream fout(filename.c_str());
    //cout<<4;
   // cout<<"inside main 2"<<endl;
    
    while(!fin.eof()){
        getline(fin, type, '\n');
        l1.clear();
        l2.clear();
      //  cout<<"inside main 3"<<endl;
        if(type == "i"){
            getline(fin, nums, '\n');
            nums += " ";
            string t = "";
            stringstream ss(nums);
            while(getline(ss, t, ' ')){
                
                num = atoi(t.c_str());
                l1.push_back(num);
                   
            }
        //        cout<<"inside main 4"<<endl;
            
        }
        else{
            getline(fin, nums, '\n');
            nums += " ";
            string t2 = "";
            stringstream ss(nums);
            while(getline(ss, t2, ' ')){
                
                n = atoi(t2.c_str());
                getline(ss, t2, ' ');
                d = atoi(t2.c_str());
                Fraction f = Fraction(n,d);
                l2.push_back(f);
            }
            
        }
       
        
        if(type == "i"){
           
            //cout<<"inside main 5"<<endl;
            mergeSort(l1,l1.begin(),l1.size());
            fout<<type<<endl;
            for(list<int>::iterator i= l1.begin(); i != l1.end(); i++){
                fout<<*i<<" ";
            }
            fout<<endl;
            
        }else if(type == "f"){
            
            mergeSort(l2, l2.begin(), l2.size());
            fout<<type<<endl;
            for(list<Fraction>::iterator i = l2.begin(); i != l2.end(); i++){
                fout<<*i<<" ";
            }
         
            fout<<endl;
            
            
        }
         
            
            
    }

    
    
    
    return 0;
}