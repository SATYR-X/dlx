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
}