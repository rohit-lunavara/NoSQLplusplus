#pragma once

// DEBUG
#include <iostream>

//DATA STRUCTURES
#include <string>
#include <deque>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// OPTIONS
#include <initializer_list>

//Base
// #include "DataBase.h"

template<>
class DataBase<std::string, std::deque<std::string>>
{
public :
    /**
     Returns if a list key exists in the database.
     */
    bool exist(const std::string& key);

     /**
      Returns the deque list for the given key.
      Throws an error if the key does not exist.
      */
    std::deque<std::string> get(const std::string& key);

    /**
     Removes a list from the database.
     Returns true if the key is in the list and successfully removed.
     */
    bool remove(const std::string& key);
    
    /**
     Renames a list in the database.
     Returns false if the key does not exist or the newkey already exists.
     Returns true for a successful rename.
     */
    bool rename(const std::string& key, const std::string& newkey);
    
    /**
     Returns an unordered_set of key names.
     */
    std::unordered_set<std::string> getKeys();
    
    /**
     Removes and returns the first element of the list stored at key.
     */
    std::string lpop(std::string key);
    
    /**
     Removes and returns the last element of the list stored at key.
     */
    std::string rpop(std::string key);
    
    /**
     Pushes a value to the list at the key.
     initialzer list options are as follows:
     set(k, v, {'l'}) -> lpush
     set(k, v, {'r'}) -> rpush
     set(k, v, {'x', 'l'}) -> lpushx
     set(k, v, {'x', 'r'}) -> rpushx
     x must always be the first option followed by a direction
     Invalid options will always return a false.
     Returns if the value is successfully added to the key.
     */
    bool set(const std::string& key, const std::string& value, std::initializer_list<std::string> options);
    
    /**
     Inserts the value at at the index before or after the pivot parameter.
     Returns the size of the list.
     Returns -1 if the pivot or key is not found.
     */
    int linsert(std::string key, std::string pivot, std::string value, bool before);
    
    /**
     Negative values will be counted from the tail.
     Returns a vector of all string values in the given range.
     */
    std::vector<std::string> lrange(std::string key, int start, int end);
    
    /**
     Sets a value for the given index idx.
     Negative values will be counted from the tail.
     Returns true if the index is exists and false if it does not.
     */
    bool lset(std::string key, int idx, std::string value);
    
    /**
     Removes the first count occurrences of elements equal to element from the list stored at key. The count argument influences the operation in the following ways:
     count > 0: Remove elements equal to element moving from head to tail.
     count < 0: Remove elements equal to element moving from tail to head.
     count = 0: Remove all elements equal to element.
     Returns the size of the list after the remove.
     */
    int lrem(std::string key, int count, std::string value);
    
    /**
     Trims a list to the expected range.
     Negative start or end will start from the tail.
     Returns the size of the list after the trim.
     */
    
    int ltrim(std::string key, int start, int end);
    
    /**
     Pops a value from a list with the source as its key and pushes it to the destination list.
     Returns the popped and pushed value.
     */
    std::string rpoplpush(std::string source, std::string destination);
    
    /**
     A negative idx will start from the tail.
     Returns a value at a given index.
     */
    std::string lindex(std::string key, int idx);
    
    /**
     Returns the size of a list for a given key.
     */
    int llen(std::string key);
    
private:
    std::unordered_map<std::string, std::deque<std::string>> m;
    std::unordered_set<std::string> keys;
    
    /**
     Inserts the value at the head of the list. If a list does not exist for a key then the list is created.
     Returns the size of the list.
     */
    int lpush(std::string key, std::string value);
    
    /**
     Inserts the value at the tail of the list. If a list does not exist for a key then the list is created.
     Returns the size of the list.
     */
    int rpush(std::string key, std::string value);

    /**
     Inserts the value at the head of the list only if a list already exists. If a list does not exist then it is ignored.
     Returns the size of the list.
     */
    int lpushx(std::string key, std::string value);

    /**
     Inserts the value at the tail of the list only if a list already exists. If a list does not exist then it is ignored.
     Returns the size of the list.
     */
    int rpushx(std::string key, std::string value);
};

bool DataBase<std::string, std::deque<std::string>>::exist(const std::string& key) {
    return m.count(key);
}

std::deque<std::string> DataBase<std::string, std::deque<std::string>>::get(
        const std::string& key){
    if(!exist(key)){
        throw KeyNotFoundException();
    }
    return m[key];
}

bool DataBase<std::string, std::deque<std::string>>::remove(
        const std::string& key){
    if(!exist(key)){
        return false;
    }
    m.erase(key);
    keys.erase(key);
    return true;
}

std::unordered_set<std::string> DataBase<std::string, std::deque<std::string>>::getKeys()
{
    return keys;
}

bool DataBase<std::string, std::deque<std::string>>::rename(
        const std::string& key, 
        const std::string& newkey){
    if(exist(newkey) || !exist(key)){
        return false;
    }
    auto d = m[key];
    m.erase(key);
    keys.erase(key);
    m[newkey] = d;
    keys.insert(newkey);
    return true;
}

std::string DataBase<std::string, std::deque<std::string>>::lpop(std::string key) {
    if(!exist(key)) {
        throw KeyNotFoundException();
    }
    std::deque<std::string> d = m[key];
    std::string s = d.front();
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

std::string DataBase<std::string, std::deque<std::string>>::rpop(std::string key) {
    if(!exist(key)) {
        throw KeyNotFoundException();
    }
    std::deque<std::string> d = m[key];
    std::string s = d.back();
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

bool DataBase<std::string, std::deque<std::string>>::set(
        const std::string& key, 
        const std::string& value, 
        std::initializer_list<std::string> options){
    for ( auto& option : options ) {
        std::for_each(option.begin(), option.end(), ::toupper);
    }
    auto it=options.begin();
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

int DataBase<std::string, std::deque<std::string>>::lpush(
        std::string key, 
        std::string value){
    std::deque<std::string> d;
    if(exist(key)) {
        d = m[key];
        keys.insert(key);
    }
    d.push_front(value);
    m[key] = d;
    return static_cast<int>(d.size());
}

int DataBase<std::string, std::deque<std::string>>::rpush(
        std::string key, std::string value){
    std::deque<std::string> d;
    if(exist(key)) {
        d = m[key];
    }
    d.push_back(value);
    m[key] = d;
    keys.insert(key);
    return static_cast<int>(d.size());
}

int DataBase<std::string, std::deque<std::string>>::lpushx(
        std::string key, 
        std::string value){
    if(exist(key)) {
        return lpush(key, value);
    }
    return 0;
}

int DataBase<std::string, std::deque<std::string>>::rpushx(
        std::string key, 
        std::string value){
    if(exist(key)) {
        return rpush(key, value);
    }
    return 0;
}

int DataBase<std::string, std::deque<std::string>>::linsert(
        std::string key, 
        std::string pivot, 
        std::string value, bool before){
    if(m.count(key) == 0) {
        return -1;
    }
    std::deque<std::string> d = m[key];
    auto it = std::find(d.begin(),d.end(),pivot);
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

std::vector<std::string> DataBase<std::string, std::deque<std::string>>::lrange(
        std::string key, int start, int end){

    using std::min;
    using std::max;
    std::vector<std::string> v;
    if(!exist(key)) {
        //key does not exist so just return empty vector
        return v;
    }
    std::deque<std::string> d = m[key];
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

bool DataBase<std::string, std::deque<std::string>>::lset(
        std::string key, 
        int idx, 
        std::string value){
    if(!exist(key)) {
        //key does not exist so just return false
        return false;
    }
    std::deque<std::string> d = m[key];
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

int DataBase<std::string, std::deque<std::string>>::lrem(
        std::string key, int count, std::string value){
    if(!exist(key)) {
        //key does not exist so just return 0
        return 0;
    }
    std::deque<std::string> d = m[key];
    if(count > 0) {
        auto it = std::find(d.begin(),d.end(),value);
        while(it != d.end() && count > 0){
            d.erase(it);
            it = find(d.begin(),d.end(),value);
            count --;
        }
    }
    else if (count < 0){
        auto it = find(d.end(),d.begin(),value);
        while(it != d.begin() && count < 0){
            d.erase(it);
            it = find(d.end(),d.begin(),value);
            count ++;
        }
    }
    else {
        //0 erase all
        auto it = find(d.begin(),d.end(),value);
        while(it != d.end()){
            d.erase(it);
            it = find(d.begin(),d.end(),value);
        }
    }
    m[key] = d;
    return static_cast<int>(d.size());
}

int DataBase<std::string, std::deque<std::string>>::ltrim(
        std::string key, int start, int end){

    using std::min;
    using std::max;
    if(!exist(key)) {
        //key does not exist so just return 0
        return 0;
    }
    std::deque<std::string> d = m[key];
    if (start < 0) {
        start = static_cast<int>(d.size()) + start;
    }
    start = max(0,start);
    
    if (end < 0) {
        end = static_cast<int>(d.size()) + end;
    }
    end = min(end + 1, static_cast<int>(d.size()));
    std::deque<std::string> new_d;
    if (start < end) {
        for(int i = start; i < end; i++) {
            new_d.push_back(d[i]);
        }
        m[key] = new_d;
        return static_cast<int>(new_d.size());
    }
    return static_cast<int>(d.size());
}

std::string DataBase<std::string, std::deque<std::string>>::rpoplpush(
        std::string source, 
        std::string destination){
    if(!exist(source)) {
        throw KeyNotFoundException();
    }
    auto s = rpop(source);
    lpush(destination, s);
    return s;
}

std::string DataBase<std::string, std::deque<std::string>>::lindex(
        std::string key, int idx) {
    if(!exist(key)) {
        throw KeyNotFoundException();
    }
    std::deque<std::string> d = m.at(key);
    if(idx < 0) {
        idx = static_cast<int>(d.size()) + idx;
    }
    if(idx < 0 || idx > static_cast<int>(d.size())) {
        throw OutOfIndexException();
    }
    return d[idx];
}

int DataBase<std::string, std::deque<std::string>>::llen(std::string key){
    if(!exist(key)) {
        return 0;
    }
    std::deque<std::string> d = m[key];
    return static_cast<int>(d.size());
}
