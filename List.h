//
//  List.h
//  List
//
//  Created by Conor Sweeney on 4/21/20.
//  Copyright © 2020 csweeney. All rights reserved.
//

#include <string>
#include <deque>
#include <vector>
#include <unordered_map>

using namespace std;

class List {
public:
    List();
    //Removes and returns the first element of the list stored at key.
    string lpop(string key);
    
    //Removes and returns the last element of the list stored at key.
    string rpop(string key);
    
    //Inserts the value at the head of the list. If a list does not exist for a key then the list is created.
    //Returns the size of the list.
    int lpush(string key, string value);
    
    //Inserts the value at the tail of the list. If a list does not exist for a key then the list is created.
    //Returns the size of the list.
    int rpush(string key, string value);
    
    //Inserts the value at the head of the list only if a list already exists. If a list does not exist then it is ignored.
    //Returns the size of the list.
    int lpushx(string key, string value);
    
    //Inserts the value at the tail of the list only if a list already exists. If a list does not exist then it is ignored.
    //Returns the size of the list.
    int rpushx(string key, string value);
    
    //Inserts the value at at the index before or after the pivot parameter.
    //Returns the size of the list.
    //Returns -1 if the pivot or key is not found.
    int linsert(string key, string pivot, string value, bool before);
    
    //Returns a vector of all string values in the given range.
    //Negative values will be count from the tail.
    vector<string> lrange(string key, int start, int end);
    
    //Sets a value for the given index idx.
    //Negative values will be count from the tail.
    //Returns true if the index is exists and false if it does not.
    bool lset(string key, int idx, string value);
    
    //Removes the first count occurrences of elements equal to element from the list stored at key. The count argument influences the operation in the following ways:
    //count > 0: Remove elements equal to element moving from head to tail.
    //count < 0: Remove elements equal to element moving from tail to head.
    //count = 0: Remove all elements equal to element.
    int lrem(string key, int count, string value);
    
    //Trims a list to the expected range.
    //Negative start or end will start from the tail.
    int ltrim(string key, int start, int end);
    
    //Pops a value from a list with the source as its key and pushes it to the destination list.
    //Returns the popped and pushed value.
    string rpoplpush(string source, string destination);
    
    //Returns a value at a given index.
    //A negative idx will start from the tail.
    string lindex(string key, int idx);
    
    //Returns the size of a list for a given key.
    int llen(string key);
private:
    unordered_map<string, deque<string>> m;
};
