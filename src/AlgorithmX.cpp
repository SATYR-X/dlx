#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<chrono>
#include<filesystem>
#include "../include/ExactCoverMatrix.h"

namespace fs = std::filesystem;

void extractNM( const std::string& line, int& n, int& m ){
    std::istringstream iss(line);
    std::string token;
    // 读取 c
    iss >> token;
    // 读取 n
    iss >> token;
    if(token=="n"){
        iss >> token;
        iss >> n;
    }else{
        throw std::runtime_error("格式错误：未找到'n'");
    }

    iss >> token;
    iss >> token;
    if(token=="m"){
        iss >> token;
        iss >> m;
    }else{
        throw std::runtime_error("格式错误：未找到'n'");
    }
}

void extractCR( const std:: string& line, int &c, int& r){
    std::istringstream iss(line);
    std::string token;

    iss >> c;
    iss >> r;
}

bool startsWith(const std::string& str, char ch){
    return !str.empty() && str[0] == ch;
}

int** processFileToMatrix(const std::string& filename,int& r,int& c){
    std::ifstring file(filename);
    if(!file.is_open()){
        throw std::runtime_error("无法打开文件");
    }
    std:string line;
    std::getline(file,line);
    int n, m;
    extractNM(line, n, m);
    r = m;
    c = n;

    std::getline(file, line);

    int** matrix = new int*[r];
    for(int i = 0;i < r;i++){
        matrix[i]=new int[c];
        for(int j = 0;j < c;j++){
            matrix[i][j] = 0;
        }
    }

    int row=0;
    while(std::getline(file, line)){
        if(line.empty() || line[0] != 's')continue;
        std::istringstream iss(line.substr(2));
        int col;
        while(iss >> col){
            if(col > 0 && col <= n){
                matrix[row][col - 1] = 1;
            }
        }
        ++row;
    }
    file.close();
    return matrix;
}

int **proFileToMat(const fs::path& filename, int& r, int& c){
    std::ifstream file(filename.string());
    if(!file.is_open()){
        throw std::runtime_error("无法打开文件夹");
    }

    std::string line;
    std::getline(file, line);

    int n, m;
    extractCR(line, n, m);
    r = m;
    c = n;

    int **matrix = new int*[r];
    for(int i = 0; i < r;++i){
        matrix[i] = new int[c];
        for(int j = 0; j < c;++j){
            matrix[i][j] = 0;
        }
    }

    fs::path fileNmae = filename.filename();
    if(startsWith(fileNmae.string(), 's')){
        std::cout<<"该文件名以s开头"<<std::endl;
        int row = 0;
        while(std::getline(file, line)){
            int col;
            while(iss >> col){
                if(col > 0 && col <= n){
                    matrix[row][col - 1] = 1;
                }
            }
            ++row;
        }
        file.close();
    }else{
        std::cout<<"该文件名不以s开头"<<std::endl;
        int row = 0;
        while(std::getline(file, line)){
            std::istringstream iss(line);
            std::string token;
            iss >> token;
            iss >> token;
            int col;
            while(iss >> col){
                if(col > 0 && col <=n){
                    matrix[row][col - 1] = 1;
                }
            }
            ++row;
        }
        file.close();
    }
    return matrix;
}

void freeMatrix(int** matrix, int rows){
    for(int i = 0;i < rows; i++){
        delete[] matrix[i];
    }
    delete[] matrix;
}

int main(){
    try{
        const std::string folderPath = "../set_partitioning_benchmarks";
        for(const auto& entry : fs::directory_iterator(folderPath)){
            if(entry.is_regular_file() && enrty.path().extension() == ".txt"){
                fs::path filepath = entry.path();
                int rows,cols;
                int **matrix = proFileToMat(filepath, rows, cols);
                std::cout<< rows << " " << cols << std::endl;

                ExactCoverMatrix ecm(rows, cols, matrix);

                std::vector<int> solution;
                int count = 0;
                auto start =  std::chrono::high_resolution_clock::now();
                ecm.search(solution, count);
                
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

                std::cout<< "File name: " << entry.path().filename().string() << std::endl;
                std::cout<< "Search Compulation Time: " << duration.count() << " seconds.";
                std::cout<< "Solution Numbers: " << count << std::endl;
                std::cout<< std::endl;

                freeMatrix(matrix, rows);
                std::cout<<std::endl;
            }
         }
    } catch(const std::exception& e){
        std::cerr << "错误：" << e.what() << '\n';
    }

    return 0;
}