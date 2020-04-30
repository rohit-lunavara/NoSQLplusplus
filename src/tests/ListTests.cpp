//
//  main.cpp
//  List
//
//  Created by Conor Sweeney on 4/21/20.
//  Copyright © 2020 csweeney. All rights reserved.
//

#include <iostream>
#include <string>
#include "DataBase.h"

using namespace std;

string print_dq(deque <string> g) {
    string str;
    deque <string> :: iterator it;
    for (it = g.begin(); it != g.end(); ++it){
        str.append(*it);
        str.append(" ");
        cout << ' ' << *it;
    }
    cout << endl;
    return str;
}

string print_vector(vector <string> g) {
    string str;
    vector <string> :: iterator it;
    for (it = g.begin(); it != g.end(); ++it){
        str.append(*it);
        str.append(" ");
        cout << ' ' << *it;
    }
    cout << endl;
    return str;
}

int main(int argc, const char * argv[]) {
    bool all_tests_pass = true;
    DataBase<string, deque<string>> db ;
    //lpush
    if(db.set("First", "Hello", {"L"}) == 1) {
        cout << "Success: lpush" <<endl;
    }
    else {
        cout << "Failure: lpush" <<endl;
        all_tests_pass = false;
    }
    
    //lpop
    if(db.lpop("First") == "Hello"){
        cout << "Success: lpop" <<endl;
    }
    else {
        cout << "Failure: lpop" <<endl;
        all_tests_pass = false;
    }
    
    //lpushx
    if(db.set("First", "Hello", {"L","X"}) == 0) {
        cout << "Success: lpushx" <<endl;
    }
    else {
        cout << "Failure: lpushx" <<endl;
        all_tests_pass = false;
    }
    
    //remove
    db.set("Remove", "Help! I'm being created to be deleted!!!!!", {"L"});
    if (db.exist("Remove") && db.remove("Remove") && !db.exist("Remove")) {
        cout << "Success: remove" <<endl;
    }
    else{
        cout << "Failure: remove" <<endl;
        all_tests_pass = false;
    }
    
    //get
    db.set("First", "Hello", {"L"});
    db.set("First", "World", {"R"});
    
    auto dq = db.get("First");
    if(print_dq(dq) == "Hello World "){
        cout << "Success: get" <<endl;
    }
    else {
        cout << "Failure: get" <<endl;
        all_tests_pass = false;
    }
    
    //rename
    db.rename("First", "Worst");
    if (!db.exist("First") && db.exist("Worst")) {
        cout << "Success: rename" <<endl;
    }
    else {
        cout << "Failure: rename" <<endl;
        all_tests_pass = false;
    }
    
    
    //linsert
    int count = db.linsert("Worst","World","Smelly",false);
    dq = db.get("Worst");
    if(print_dq(dq) == "Hello Smelly World " && count == 3){
        cout << "Success: after linsert" <<endl;
    }
    else {
        cout << "Failure: after linsert" <<endl;
        all_tests_pass = false;
    }
    
    count = db.linsert("Worst","Hello","Hi",true);
    dq = db.get("Worst");
    if(print_dq(dq) == "Hi Hello Smelly World " && count == 4){
        cout << "Success: before linsert" <<endl;
    }
    else {
        cout << "Failure: before linsert" <<endl;
        all_tests_pass = false;
    }
    
    //lrange
    auto v = db.lrange("Worst", 0, 2);
    if(print_vector(v) == "Hi Hello Smelly "){
        cout << "Success: lrange" <<endl;
    }
    else {
        cout << "Failure: lrange" <<endl;
        all_tests_pass = false;
    }
    
    v = db.lrange("Worst", 0, -2);
    if(print_vector(v) == "Hi Hello Smelly "){
        cout << "Success: lrange -2" <<endl;
    }
    else {
        cout << "Failure: lrange -2" <<endl;
        all_tests_pass = false;
    }
    
    //lset
    bool set = db.lset("Worst", 2, "Stinky");
    dq = db.get("Worst");
    if(print_dq(dq) == "Hi Hello Stinky World " && set == true){
        cout << "Success: lset" <<endl;
    }
    else {
        cout << "Failure: lset" <<endl;
        all_tests_pass = false;
    }
    
    //lrem
    int remove = db.lrem("Worst", 0, "Hi");
    dq = db.get("Worst");
    if(print_dq(dq) == "Hello Stinky World " && remove == 3){
        cout << "Success: lrem" <<endl;
    }
    else {
        cout << "Failure: lrem" <<endl;
        all_tests_pass = false;
    }
    
    //ltrim
    int trim = db.ltrim("Worst", 0, 1);
    dq = db.get("Worst");
    if(print_dq(dq) == "Hello Stinky " && trim == 2){
        cout << "Success: ltrim" <<endl;
    }
    else {
        cout << "Failure: ltrim" <<endl;
        all_tests_pass = false;
    }
    
    //rpoplpush
    db.set("Remove", "Hi", {"L"});
    string s = db.rpoplpush("Remove", "Worst");
    if(s == "Hi" && print_dq(db.get("Worst")) == "Hi Hello Stinky " && !db.exist("Remove") ){
        cout << "Success: rpoplpush" <<endl;
    }
    else {
        cout << "Failure: rpoplpush" <<endl;
        all_tests_pass = false;
    }
    
    //lindex
    if(db.lindex("Worst", 2) == "Stinky") {
        cout << "Success: lindex" <<endl;
    }
    else {
        cout << "Failure: lindex" <<endl;
        all_tests_pass = false;
    }
    
    //llen
    if(db.llen("Worst") == 3) {
        cout << "Success: llen" <<endl;
    }
    else {
        cout << "Failure: llen" <<endl;
        all_tests_pass = false;
    }
    
    //errors
    try {
        db.get("Fake");
        cout << "Failure: get Error" <<endl;
        all_tests_pass = false;
    }
    catch (exception& e){
        cout << "Success: get Error" <<endl;
        cout << e.what()<< endl;
    }
    
    try {
        db.lpop("Fake");
        cout << "Failure: lpop Error" <<endl;
        all_tests_pass = false;
    }
    catch (exception& e){
        cout << "Success: lpop Error" <<endl;
        cout << e.what()<< endl;
    }
    
    try {
        db.rpop("Fake");
        cout << "Failure: rpop Error" <<endl;
        all_tests_pass = false;
    }
    catch (exception& e){
        cout << "Success: rpop Error" <<endl;
        cout << e.what()<< endl;
    }
    
    try {
        db.rpoplpush("Fake", "Worst");
        cout << "Failure: rpoplpush Error" <<endl;
        all_tests_pass = false;
    }
    catch (exception& e){
        cout << "Success: rpoplpush Error" <<endl;
        cout << e.what()<< endl;
    }
    
    try {
        db.lindex("Fake", 0);
        cout << "Failure: lindex Error" <<endl;
        all_tests_pass = false;
    }
    catch (exception& e){
        cout << "Success: lindex Error" <<endl;
        cout << e.what()<< endl;
    }
    
    try {
        db.lindex("Worst", 100);
        cout << "Failure: lindex Error" <<endl;
        all_tests_pass = false;
    }
    catch (exception& e){
        cout << "Success: lindex Error" <<endl;
        cout << e.what()<< endl;
    }
    
    if(all_tests_pass){
        cout << "SUCESSS: ALL TESTS PASS!!!!!!!!" <<endl;
    }
    else {
        cout << "FAILURE: REVIEW YOUR TEST RESULTS" <<endl;
    }

    return 0;
}
