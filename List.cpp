//
//  List.cpp
//  List
//
//  Created by Conor Sweeney on 4/21/20.
//  Copyright © 2020 csweeney. All rights reserved.
//

#include "List.h"
#include <iostream>


using namespace std;

List::List(){
}

string List::lpop(string key) {
    //I am not check for the key so it will throw a map exception if key does not exist
    //thoughts?
    deque<string> d = m.at(key);
    string s = d.front();
    d.pop_front();
    if(d.empty()){
        m.erase(key);
    }
    else {
        m[key] = d;
    }
    return s;
}

string List::rpop(string key) {
    deque<string> d = m.at(key);
    string s = d.back();
    d.pop_back();
    if(d.empty()){
        m.erase(key);
    }
    else {
        m[key] = d;
    }
    return s;
}

int List::lpush(string key, string value){
    deque<string> d = m[key];
    d.push_front(value);
    m[key] = d;
    return static_cast<int>(d.size());
}

int List::rpush(string key, string value){
    deque<string> d = m[key];
    d.push_back(value);
    m[key] = d;
    return static_cast<int>(d.size());
}

int List::lpushx(string key, string value){
    if(m.count(key) == 1) {
        return lpush(key, value);
    }
    return 0;
}

int List::rpushx(string key, string value){
    if(m.count(key) == 1) {
        return rpush(key, value);
    }
    return 0;
}

int List::linsertbefore(string key, string before, string value){
    deque<string> d = m.at(key);
    deque<string>::iterator it = find(d.begin(),d.end(),before);
    if(it == d.end()) {
        //throw error
        //value not found
    }
    it--;
    d.insert(it, value);
    m[key] = d;
    return static_cast<int>(d.size());
}

int List::linsertafter(string key, string after, string value){
    deque<string> d = m.at(key);
    deque<string>::iterator it = find(d.begin(),d.end(),after);
    if(it == d.end()) {
        //throw error
        //value not found
    }
    d.insert(it, value);
    m[key] = d;
    return static_cast<int>(d.size());
}

vector<string> List::lrange(string key, int start, int end){
    deque<string> d = m.at(key);
    
    if (start < 0) {
        start = static_cast<int>(d.size()) + start;
    }
    start = max(0,start);
    
    if (end < 0) {
        end = static_cast<int>(d.size()) + end;
    }
    end = min(end + 1, static_cast<int>(d.size()));
    
    vector<string> v;
    if (start < end) {
        for(int i = start; i < end; i++){
            v.push_back(d[i]);
        }
    }
    return v;
}

bool List::lset(string key, int idx, string value){
    deque<string> d = m.at(key);
    if(idx < 0) {
        idx = static_cast<int>(d.size()) + idx;
    }
    if(idx > 0 && idx < static_cast<int>(d.size())) {
        d[idx] = value;
        m[key] = d;
        return true;
    }
    return false;
}

int List::lrem(string key, int count, string value){
    deque<string> d = m.at(key);
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
        while(it != d.end() && count < 0){
            d.erase(it);
            it = find(d.begin(),d.end(),value);
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

int List::ltrim(string key, int start, int end){
    deque<string> d = m.at(key);
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
        return static_cast<int>(new_d.size());
    }
    return static_cast<int>(d.size());
}

string List::rpoplpush(string source, string destination){
    string s = rpop(source);
    lpush(destination, s);
    return s;
}

string List::lindex(string key, int idx) {
    deque<string> d = m.at(key);
    if(idx < 0) {
        idx = static_cast<int>(d.size()) + idx;
    }
    //Check index or let crash?
//    if(idx > 0 && idx < static_cast<int>(d.size())) {
//
//    }
    return d[idx];
}

int List::llen(string key){
    deque<string> d = m[key];
    return static_cast<int>(d.size());
}

