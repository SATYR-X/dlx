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

void ExactCoverMatrix::insert( int r, int c)
{
    Node* cur = &ColIndex[c];
    ColIndex[c].size++;
    Node* newNode = new Node( r, c );
    while( cur->down != &ColIndex[c] && cur->down->row < r)
        cur = cur->down;
    newNode->down = cur->down;
    newNode->up = cur;
    cur->down->up = newNode;
    cur->down = newNode;
    if( RowIndex[r].right == NULL ){
        RowIndex[r].right = newNode;
        newNode->left = newNode;
        newNode->right = newNode;
    }
    else{
        Node* rowHead = RowIndex[r].right;
        cur = rowHead;
        while( cur->right != rowHead && cur->right->col < c)
            cur = cur->right;
        newNode->right = cur->right;
        newNode->left = cur;
        cur->right->left = newNode;
        cur->right = newNode;
    }                                       
}

void ExactCoverMatrix::cover( int c )
{
    ColunmHeader* col = &ColIndex[c];
    col->right->left = col->left;
    col->left->right = col->right;
    Node* curR,*curC;
    curC = col->down;
    while( curC != col )
    {
        Node* noteR = curC;
        curR = noteR->right;
        while( curR != noteR )
        {
            curR->down->up = curR->up;
            curR->up->down = curR->down;
            ColIndex[curR->col].size--;
            curR = curR->right;
        }
        curC = curC->down;
    }
}

void ExactCoverMatrix::uncover( int c )
{
    Node* curR, *curC;
    ColunmHeader* col = &ColIndex[c];
    curC = col->up;
    while( curC != col )
    {
        Node* noteR = curC;
        curR = curC->left;
        while( curR != noteR )
        {
            ColIndex[curR->col].size++;
            curR->down->up = curR;
            curR->up->down = curR;
            curR = curR->left;
        }
        curC = curC->up;
    }
    col->right->left = col;
    col->left->right = col;
}

void ExactCoverMatrix::printColumnHeaders() const {
    std::cout << "Column Headers:" << std::endl;
    ColunmHeader* current = static_cast<ColunmHeader*>(root->right);
    while (current != root){
        std::cout << "Col: " << current->col <<" Size: " << current->size << " ";
        current = static_cast<ColunmHeader*>(current->right);
    }
    std::cout << std::endl;
}

void ExactCoverMatrix::printRowNodes() const {
    std::cout << "Row Nodes:" << std::endl;
    for ( int i = 0; i < ROWS; i++)
    {
        Node* current = RowIndex[i].right;
        if(current != nullptr){
            Node* Note = current;
            std::cout << "Row: " << current->row << " Col: " << current->col << " ";
            current = current->right;
            while ( current != Note ){
                std::cout << "Row: " << current->row << " Col: " << current->col << " ";
                current = current-> right;
            }
            std::cout << std::endl;
        }
        else{
            break;
        }
    }
}

void ExactCoverMatrix::search(std::vector<int>& solution, int& count)
{
    // 如果根节点的右指针指向自身，说明所有列都被覆盖，找到一个解
    if (root->right == root) {
        count++;  // 计数器增加，表示找到一个解
        return;   // 返回上层递归
    }

    // 选择一个列头节点，初始选择为根节点右边的第一个列头
    ColunmHeader* choose = (ColunmHeader*)root->right, *cur = choose;
    // 遍历所有列头节点，选择包含最少节点的列
    while (cur != root) {
        if (choose->size > cur->size)
            choose = cur;  // 更新选择的列头为当前列头
        cur = (ColunmHeader*)cur->right;  // 移动到下一个列头
    }

    // 如果选择的列头节点的大小小于等于0，说明没有可行解，返回
    if (choose->size <= 0) {
        return;
    }

    // 覆盖选择的列
    cover(choose->col);
    Node* curC = choose->down;  // 从选择的列头向下遍历

    // 遍历选择列中的每个节点
    while (curC != choose) {
        solution.push_back(curC->row + 1);  // 将当前行加入解中
        Node* noteR = curC;
        Node* curR = curC->right;

        // 覆盖当前行中的所有列
        while (curR != noteR) {
            cover(curR->col);
            curR = curR->right;
        }

        // 递归搜索下一个解
        search(solution, count);

        // 回溯：从解中移除当前行
        solution.pop_back();
        curR = noteR->left;

        // 取消覆盖当前行中的所有列
        while (curR != noteR) {
            uncover(curR->col);
            curR = curR->left;
        }

        curC = curC->down;  // 移动到选择列中的下一个节点
    }

    // 取消覆盖选择的列
    uncover(choose->col);
    return;
}