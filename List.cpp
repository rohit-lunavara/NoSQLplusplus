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
    deque<string> d;
    if(m.count(key) == 1) {
        d = m[key];
    }
    d.push_front(value);
    m[key] = d;
    return static_cast<int>(d.size());
}

int List::rpush(string key, string value){
    deque<string> d;
    if(m.count(key) == 1) {
        d = m[key];
    }
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


int List::linsert(string key, string pivot, string value, bool before){
    if(m.count(key) == 0) {
        return -1;
    }
    deque<string> d = m[key];
    deque<string>::iterator it = find(d.begin(),d.end(),pivot);
    if(it == d.end()) {
        return -1;
    }
    if(before){
        it--;
    }
    d.insert(it, value);
    m[key] = d;
    return static_cast<int>(d.size());
}

vector<string> List::lrange(string key, int start, int end){
    vector<string> v;
    if(m.count(key) == 0) {
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

bool List::lset(string key, int idx, string value){
    if(m.count(key) == 0) {
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

int List::lrem(string key, int count, string value){
    if(m.count(key) == 0) {
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

int List::ltrim(string key, int start, int end){
    if(m.count(key) == 0) {
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
        return static_cast<int>(new_d.size());
    }
    return static_cast<int>(d.size());
}

string List::rpoplpush(string source, string destination){
    if(m.count(source) == 0) {
        throw "rpoplpush: source key does not exist in the list.";
    }
    string s = rpop(source);
    lpush(destination, s);
    return s;
}

string List::lindex(string key, int idx) {
    if(m.count(key) == 0) {
        throw "lindex: key does not exist in the list.";
    }
    deque<string> d = m.at(key);
    if(idx < 0) {
        idx = static_cast<int>(d.size()) + idx;
    }
    if(idx > 0 && idx < static_cast<int>(d.size())) {
        throw "lindex: idx is out of index.";
    }
    return d[idx];
}

int List::llen(string key){
    if(m.count(key) == 0) {
        return 0;
    }
    deque<string> d = m[key];
    return static_cast<int>(d.size());
}
