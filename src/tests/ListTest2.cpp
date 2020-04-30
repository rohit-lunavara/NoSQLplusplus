//
//  main.cpp
//  List
//
//  Created by Conor Sweeney on 4/21/20.
//  Copyright © 2020 csweeney. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "DataBase.h"

using namespace std;
using namespace std::chrono;

void test_time(int keyCount){
    string line;
    ifstream myfile ("../src/bible.txt");
    int actualKeyCount = 0;
    if (myfile.is_open()){
        string key = "Old Testament";
        auto start = chrono::steady_clock::now();
        DataBase<string, deque<string>> db;
        while ( getline (myfile,line) ){
            char c;
            if (!line.empty()){
                c = line[0];
                if(isdigit(c)){
                    db.set(key, line, {"R"});
                }
                else {
                    key = line;
                    if (actualKeyCount == keyCount) {
                        break;
                    }
                    actualKeyCount ++;
                }
            }
        }
        for(auto k : db.getKeys()){
            while (db.exist(k)) {
                db.rpop(k);
            }
        }
        auto end = chrono::steady_clock::now();
        cout << "Elapsed time in seconds for "
        << actualKeyCount
        << " : "
        << chrono::duration_cast<chrono::milliseconds>(end - start).count()
        << " milliseconds"
        << endl;
        myfile.close();
    }

    else {
        cout << "Unable to open file" <<endl;;
    }
    
}


int main(int argc, const char * argv[]) {
    test_time(10);
    test_time(100);
    test_time(1000);
    test_time(5000);
    test_time(10000);
    test_time(50000);
    test_time(100000);
    return 0;
}
