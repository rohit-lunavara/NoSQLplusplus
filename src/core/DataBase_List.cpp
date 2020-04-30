// TODO need to copy to header file

//
//  List.cpp
//  List
//
//  Created by Conor Sweeney on 4/21/20.
//  Copyright © 2020 csweeney. All rights reserved.
//

#include "DataBase_List.h"
#include "DataBase_Errors.h"

using namespace std;

bool DataBase<string, deque<string>>::exist(const string& key) {
    return m.count(key);
}

deque<string> DataBase<string, deque<string>>::get(const string& key){
    if(!exist(key)){
        throw KeyNotFoundException();
    }
    return m[key];
}

bool DataBase<string, deque<string>>::remove(const string& key){
    if(!exist(key)){
        return false;
    }
    m.erase(key);
    keys.erase(key);
    return true;
}

bool DataBase<string, deque<string>>::rename(const string& key, const string& newkey){
    if(exist(newkey) || !exist(key)){
        return false;
    }
    deque<string>d = m[key];
    m.erase(key);
    keys.erase(key);
    m[newkey] = d;
    keys.insert(newkey);
    return true;
}

unordered_set<string> DataBase<string, deque<string>>::getKeys(){
    return keys;
}

string DataBase<string, deque<string>>::lpop(string key) {
    if(!exist(key)) {
        throw KeyNotFoundException();
    }
    deque<string> d = m[key];
    string s = d.front();
    d.pop_front();
    if(d.empty()){
        m.erase(key);
        keys.erase(key);
    }
    else {
        m[key] = d;
    }
    return s;
}

string DataBase<string, deque<string>>::rpop(string key) {
    if(!exist(key)) {
        throw KeyNotFoundException();
    }
    deque<string> d = m[key];
    string s = d.back();
    d.pop_back();
    if(d.empty()){
        m.erase(key);
        keys.erase(key);
    }
    else {
        m[key] = d;
    }
    return s;
}

bool DataBase<string, deque<string>>::set(const string& key, const string& value, initializer_list<string> options){
    for ( auto& option : options ) {
        std::for_each(option.begin(), option.end(), ::toupper);
    }
    initializer_list<string>::iterator it=options.begin();
    if(options.size() == 2){
        if(*it == "X"){
            it++;
            if(*it == "R"){
                return rpushx(key, value);
            }
            else if(*it == "L"){
                return lpushx(key, value);
            }
        }
        else if(*it == "R"){
            it++;
            if(*it == "X"){
                return rpushx(key, value);
            }
        }
        else if(*it == "L"){
            it++;
            if(*it == "X"){
                return lpushx(key, value);
            }
        }
    }
    else if(options.size() == 1){
        if(*it == "R"){
            return rpush(key, value);
        }
        else if(*it == "L"){
            return lpush(key, value);
        }
    }
    return false;
}

int DataBase<string, deque<string>>::lpush(string key, string value){
    deque<string> d;
    if(exist(key)) {
        d = m[key];
    }
    d.push_front(value);
    m[key] = d;
    keys.insert(key);
    return static_cast<int>(d.size());
}

int DataBase<string, deque<string>>::rpush(string key, string value){
    deque<string> d;
    if(exist(key)) {
        d = m[key];
    }
    d.push_back(value);
    m[key] = d;
    keys.insert(key);
    return static_cast<int>(d.size());
}

int DataBase<string, deque<string>>::lpushx(string key, string value){
    if(exist(key)) {
        return lpush(key, value);
    }
    return 0;
}

int DataBase<string, deque<string>>::rpushx(string key, string value){
    if(exist(key)) {
        return rpush(key, value);
    }
    return 0;
}

int DataBase<string, deque<string>>::linsert(string key, string pivot, string value, bool before){
    if(m.count(key) == 0) {
        return -1;
    }
    deque<string> d = m[key];
    deque<string>::iterator it = find(d.begin(),d.end(),pivot);
    if(it == d.end()) {
        return -1;
    }
    if(before){
        if(it == d.begin()){
            return lpush(key, value);
        }
        it--;
    }
    d.insert(it, value);
    m[key] = d;
    return static_cast<int>(d.size());
}

vector<string> DataBase<string, deque<string>>::lrange(string key, int start, int end){
    vector<string> v;
    if(!exist(key)) {
        //key does not exist so just return empty vector
        return v;
    }
    deque<string> d = m[key];
    if (start < 0) {
        start = static_cast<int>(d.size()) + start;
    }
    start = max(0,start);
    
    if (end < 0) {
        end = static_cast<int>(d.size()) + end;
    }
    end = min(end + 1, static_cast<int>(d.size()));
    
    if (start < end) {
        v.reserve(end - start);
        for(int i = start; i < end; i++){
            v.push_back(d[i]);
        }
    }
    return v;
}

bool DataBase<string, deque<string>>::lset(string key, int idx, string value){
    if(!exist(key)) {
        //key does not exist so just return false
        return false;
    }
    deque<string> d = m[key];
    if(idx < 0) {
        idx = static_cast<int>(d.size()) + idx;
    }
    if(idx >= 0 && idx < static_cast<int>(d.size())) {
        d[idx] = value;
        m[key] = d;
        return true;
    }
    return false;
}

int DataBase<string, deque<string>>::lrem(string key, int count, string value){
    if(!exist(key)) {
        //key does not exist so just return 0
        return 0;
    }
    deque<string> d = m[key];
    if(count > 0) {
        deque<string>::iterator it = find(d.begin(),d.end(),value);
        while(it != d.end() && count > 0){
            d.erase(it);
            it = find(d.begin(),d.end(),value);
            count --;
        }
    }
    else if (count < 0){
        deque<string>::iterator it = find(d.end(),d.begin(),value);
        while(it != d.begin() && count < 0){
            d.erase(it);
            it = find(d.end(),d.begin(),value);
            count ++;
        }
    }
    else {
        //0 erase all
        deque<string>::iterator it = find(d.begin(),d.end(),value);
        while(it != d.end()){
            d.erase(it);
            it = find(d.begin(),d.end(),value);
        }
    }
    m[key] = d;
    return static_cast<int>(d.size());
}

int DataBase<string, deque<string>>::ltrim(string key, int start, int end){
    if(!exist(key)) {
        //key does not exist so just return 0
        return 0;
    }
    deque<string> d = m[key];
    if (start < 0) {
        start = static_cast<int>(d.size()) + start;
    }
    start = max(0,start);
    
    if (end < 0) {
        end = static_cast<int>(d.size()) + end;
    }
    end = min(end + 1, static_cast<int>(d.size()));
    deque<string> new_d;
    if (start < end) {
        for(int i = start; i < end; i++) {
            new_d.push_back(d[i]);
        }
        m[key] = new_d;
        return static_cast<int>(new_d.size());
    }
    return static_cast<int>(d.size());
}

string DataBase<string, deque<string>>::rpoplpush(string source, string destination){
    if(!exist(source)) {
        throw KeyNotFoundException();
    }
    string s = rpop(source);
    lpush(destination, s);
    return s;
}

string DataBase<string, deque<string>>::lindex(string key, int idx) {
    if(!exist(key)) {
        throw KeyNotFoundException();
    }
    deque<string> d = m.at(key);
    if(idx < 0) {
        idx = static_cast<int>(d.size()) + idx;
    }
    if(idx < 0 || idx > static_cast<int>(d.size())) {
        throw OutOfIndexException();
    }
    return d[idx];
}

int DataBase<string, deque<string>>::llen(string key){
    if(!exist(key)) {
        return 0;
    }
    deque<string> d = m[key];
    return static_cast<int>(d.size());
}
