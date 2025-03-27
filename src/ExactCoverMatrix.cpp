#include "ExactCoverMatrix.h"

ExactCoverMatrix::ExactCoverMatrix( int rows, int cols, int** matrix)
{
    ROWs = rows;
    COLS = cols;
    disjointSubset = new int[rows+1];
    ColIndex = new ColunmHeader[cols+1];
    RowIndex = new Node[rows];
    root = &ColIndex[0];
    ColIndex[0].left = &ColIndex[COLS];
    ColIndex[0].right = &ColIndex[1];
    ColIndex[COLS].right = &ColIndex[0];
    ColIndex[COLS].left = &ColIndex[COLS-1];
    for( int i=1; i<cols; i++){
        ColIndex[i].left = &ColIndex[i-1];
        ColIndex[i].right = &ColIndex[i+1];
    }

    for( int i=0; i<=cols; i++){
        ColIndex[i].up = &ColIndex[i];
        ColIndex[i].down = &ColIndex[i];
        ColIndex[i].col = i;
    }
    ColIndex[0].down = &RowIndex[0];

    for( int i=0; i<rows; i++){
        for( int j=0; j<cols; j++){
            if(matrix[i][j] == 1){
                insert( i, j+1 );
            }
        }
    }
}

ExactCoverMatrix::~ExactCoverMatrix()
{
    delete[] disjointSubset;
    for( int i=1; i<=COLS; i++){
        Node* cur = ColIndex[i].down;
        Node* del = cur->down;
        while( cur != &ColIndex[i] )
        {
            delete cur;
            cur = del;
            del = cur->down;
        }
    }
    delete[] RowIndex;
    delete[] ColIndex;
}