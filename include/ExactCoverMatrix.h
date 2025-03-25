#ifndef EXACTCOVERMATRIX_H
#define EXACTCOVERMATRIX_H

#include<iostream>
using namespace std;
struct Node
{
    Node* left,*right,*up,*down;
    int col,row;
    Node(){
        left = NULL; right = NULL;
        up = NULL; down = NULL;
        col = 0; row = 0;
    }
    Node( int r,int c )
    {
        left = NULL; right = NULL;
        up = NULL; down = NULL;
        col = c; row = r;
    }
};

struct ColunmHeader : public Node
{
    int size;
    ColunmHeader(){
        size = 0;
    }
};

class ExactCoverMatrix
{
    public:
        int ROWS,COLS;
        int *disjointSubset;
        ExactCoverMatrix( int rows, int cols, int **matrix );
        ~ExactCoverMatrix();
        void insert( int r, int c );
        void printColumnHeaders() const;
        void printRowNodes() const;
        void cover( int c );
        void uncover( int c );
        void searchX( int& count );
        void search( std::vector<int>& solution, int& count );
    private:
        ColunmHeader* root;
        ColunmHeader* ColIndex;
        Node* RowIndex;
};
#endif