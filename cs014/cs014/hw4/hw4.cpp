#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<vector>
#include<list>
#include<cmath>
#include<string>

using namespace std;       

//USED CODE FROM TEXTBOOK FOR BST/////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

template <typename Comparable>
class BinarySearchTree
{
  public:
    BinarySearchTree( ) :root( NULL )
    {
    }

    BinarySearchTree( const BinarySearchTree & rhs ) : root( NULL )
    {
        *this = rhs;
    }

    /**
     * Destructor for the tree
     */
    ~BinarySearchTree( )
    {
        makeEmpty( );
    }


    /**
     * Returns true if x is found in the tree.
     */
    bool contains( const Comparable & x ) const
    {
        return contains( x, root );
    }

    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool isEmpty( ) const
    {
        return root == NULL;
    }

    /**
     * Print the tree contents in sorted order.
     */
    void printTree( ofstream &fout ) const
    {
        if( isEmpty( ) )
            fout << "Empty tree";
        else
            printTree( root, fout);
    }

    /**
     * Make the tree logically empty.
     */
    void makeEmpty( )
    {
        makeEmpty( root );
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert( const Comparable & x )
    {
        insert( x, root );
    }
     
    /**
     * Remove x from the tree. Nothing is done if x is not found.
     */
    void remove( const Comparable & x )
    {
        remove( x, root );
    }

    /**
     * Deep copy.
     */
    const BinarySearchTree & operator=( const BinarySearchTree & rhs )
    {
        if( this != &rhs )
        {
            makeEmpty( );
            root = clone( rhs.root );
        }
        return *this;
    }

  public:
    struct BinaryNode
    {
        Comparable element;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode( const Comparable & theElement, BinaryNode *lt, BinaryNode *rt )
            : element( theElement ), left( lt ), right( rt ) { }
    };

    BinaryNode *root;
    

    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void insert( const Comparable & x, BinaryNode * & t )
    {
        if( t == NULL )
            t = new BinaryNode( x, NULL, NULL );
        else if( x < t->element )
            insert( x, t->left );
        else if( t->element < x )
            insert( x, t->right );
        else
            ;  // Duplicate; do nothing
    }

    /**
     * Internal method to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void remove( const Comparable & x, BinaryNode * & t )
    {
        if( t == NULL )
            return;   // Item not found; do nothing
        if( x < t->element )
            remove( x, t->left );
        else if( t->element < x )
            remove( x, t->right );
        else if( t->left != NULL && t->right != NULL ) // Two children
        {
            t->element = findMin( t->right )->element;
            remove( t->element, t->right );
        }
        else
        {
            BinaryNode *oldNode = t;
            t = ( t->left != NULL ) ? t->left : t->right;
            delete oldNode;
        }
    }

    

    /**
     * Internal method to test if an item is in a subtree.
     * x is item to search for.
     * t is the node that roots the subtree.
     */
    bool contains( const Comparable & x, BinaryNode *t ) const
    {
        if( t == NULL )
            return false;
        else if( x < t->element )
            return contains( x, t->left );
        else if( t->element < x )
            return contains( x, t->right );
        else
            return true;    // Match
    }
/****** NONRECURSIVE VERSION*************************
    bool contains( const Comparable & x, BinaryNode *t ) const
    {
        while( t != NULL )
            if( x < t->element )
                t = t->left;
            else if( t->element < x )
                t = t->right;
            else
                return true;    // Match

        return false;   // No match
    }
*****************************************************/

    /**
     * Internal method to make subtree empty.
     */
    void makeEmpty( BinaryNode * & t )
    {
        if( t != NULL )
        {
            makeEmpty( t->left );
            makeEmpty( t->right );
            delete t;
        }
        t = NULL;
    }

    /**
     * Internal method to print a subtree rooted at t in sorted order.
     */
    void printTree( BinaryNode *t, ofstream & fout) const
    {
        if( t != NULL )
        {
            printTree( t->left, fout );
            fout << t->element << " ";
            printTree( t->right, fout );
        }
    }

    /**
     * Internal method to clone subtree.
     */
    BinaryNode * clone( BinaryNode *t ) const
    {
        if( t == NULL )
            return NULL;
        else
            return new BinaryNode( t->element, clone( t->left ), clone( t->right ) );
    }
///////////END IF CODE USED FROM TEXTBOOK///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
public:
void splinterHelper(BinaryNode *a, BinaryNode *b, vector<double> &same,
vector<double> &diff, bool &isSame, BinaryNode *rootA){
    Comparable elementB = b->element;
    if(a != NULL && b != NULL && !isSame){
        double elementA = a->element;
        if(elementB == elementA){
            same.push_back(elementB);
            isSame = true;
        }
        splinterHelper(a->left, b, same, diff, isSame, rootA);
        splinterHelper(a->rightb, same, diff, isSame, rootA);
    }
    if(!isSame){
        diff.push_back(elementB);
    }
    splinterHelper(rootA, b->next, same, diff, false, rootA);
}


void splinter(BinarySearchTree<double> & A, 
BinarySearchTree<double> & B, BinarySearchTree<double> & C,
BinarySearchTree<double> & D){
    vector<Comparable> same, diff;
    BinaryNode *rootA = A.root;
    BinaryNode *rootB = B.root;
   splinterHelper(rootA, rootB, same, diff, false, rootA);
   for(int i = 0; i < same.size(); i++)
        C.insert(same.at(i));
    for(int i = 0; i < diff.size(); i++)
        D.insert(diff.at(i));
}

void prettyprint(BinaryNode *t, ofstream &fout, string linePrefix, string nodeTag, BinaryNode *p){
    prettyprint(t->left, fout, linePrefix + "  ", nodeTag, p = t);
    if(t ==NULL){
    }else{
        if( p == NULL && nodeTag == " <"){
            prettyprint(t->left, fout, linePrefix + "  ", nodeTag, p = t);
            fout<<linePrefix<<nodeTag<<t->element<<endl;
            prettyPrint(t->right, fout, linePrefix + "  ", nodeTag, p = t);
        }
        else if(t->element < p->element){
            if(t->left == NULL && t->right == NULL ){
                nodeTag = " /";
                fout<<linePrefix<<nodeTag<<t->element<<endl;
                remove(t->element, t);
            }else{
                if(nodeTag == " /"){
                    prettyprint(t->left, fout, linePrefix + "  ", nodeTag, p = t);
                    prettyprint(t->right, fout, linePrefix + "| ", nodeTag, p = t);
                }else{
                     prettyprint(t->right, fout, linePrefix + "  ", nodeTag, p = t);
                    prettyprint(t->left, fout, linePrefix + "| ", nodeTag, p = t);
                }
            }
        }
        else if(t->element > p->element){
            if(t->left == NULL && t->right == NULL){
                nodeTag = " \\";
                fout<<linePrefix<<nodeTag<<t->element<<endl;
                remove(t->element, t);
               
            }else{
                if(nodeTag == " \\"){
                    prettyprint(t->left, fout, linePrefix + "| ", nodeTag, p = t);
                    prettyprint(t->right, fout, linePrefix + "  ", nodeTag, p = t);
                }else{
                     prettyprint(t->right, fout, linePrefix + "| ", nodeTag, p = t);
                    prettyprint(t->left, fout, linePrefix + "  ", nodeTag, p = t);
                }
            }
            
        }
}
}
};





int main(int argc, char *argv[]){
    string filename,nums, nums2;
    int num, num2;
    fstream fin;
    BinarySearchTree <double> A;
    BinarySearchTree <double> B;
    BinarySearchTree <double> C;
    BinarySearchTree <double> D;
    
    
    fin.open(argv[1]);
    filename = argv[1];
    
    if(filename.find(".") != string::npos)
            filename = filename.substr(0,filename.find("."));
    filename.append(".out");
    ofstream fout(filename.c_str());
    
    A.printTree(fout); fout<<endl;
    
    while(!fin.eof()){
        int casenum = 1;
        getline(fin, nums, '\n');
        nums += " ";
        string t = "";
        stringstream ss(nums);
        cout<<nums;
        if(nums !=" "){
            while(getline(ss, t, ' ')){
                num = atoi(t.c_str());
                A.insert(num);
                }
        }
            getline(fin, nums2, '\n');
            cout<<nums2;
            nums2 += " ";
            string t2 = "";
            stringstream ss2(nums2);
        if(nums2 != ""){
            while(getline(ss2, t2, ' ')){
                num2 = atoi(t2.c_str());
                B.insert(num2);
                }
        }
        splinter(A, B, C, D);
        cout<<1;
        fout<<"CASE: "<<casenum<<endl;
       
       
        A.printTree(fout); fout<<endl;
        A.prettyprint(A.root, fout, "", " <", A.root);
        cout<<2;
        B.printTree(fout); fout<<endl;
        cout<<3;
        C.printTree(fout);fout<<endl;
        D.printTree(fout);fout<<endl;
        
        casenum++;
     }
     
    
    fout.close();
    
    
    
    
 return 0;   
}
