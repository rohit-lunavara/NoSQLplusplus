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

template<>
class DataBase<string, deque<string>> {
public:
    DataBase<string, deque<string>>();

    // Returns if a list key exists in the database.
    bool exist(const string& key);

    // Returns the deque list for the given key.
    // Throws an error if the key does not exist.
    deque<string> get(const string& key);

    // Removes a list from the database.
    // Returns true if the key is in the list and successfully removed.
    bool remove(const string& key);

    // Renames a list in the database.
    // Returns false if the key does not exist or the newkey already exists.
    // Returns true for a successful rename.
    bool rename(const string& key, const string& newkey);
    
    //Removes and returns the first element of the list stored at key.
    string lpop(string key);
    
    //Removes and returns the last element of the list stored at key.
    string rpop(string key);
    
    // Pushes a value to the list at the key.
    // initialzer list options are as follows:
    // set(k, v, {'l'}) -> lpush
    // set(k, v, {'r'}) -> rpush
    // set(k, v, {'x', 'l'}) -> lpushx
    // set(k, v, {'x', 'r'}) -> rpushx
    // x must always be the first option followed by a direction
    // Invalid options will always return a false.
    // Returns if the value is successfully added to the key.
    bool set(const std::string& key, const string& value, initializer_list<string> options);

    // TODO make it private
    //Inserts the value at the head of the list. If a list does not exist for a key then the list is created.
    //Returns the size of the list.
    private int lpush(string key, string value);
    
    // TODO make it private
    //Inserts the value at the tail of the list. If a list does not exist for a key then the list is created.
    //Returns the size of the list.
    private int rpush(string key, string value);

    // TODO make it private    
    //Inserts the value at the head of the list only if a list already exists. If a list does not exist then it is ignored.
    //Returns the size of the list.
    private int lpushx(string key, string value);

    // TODO make it private   
    //Inserts the value at the tail of the list only if a list already exists. If a list does not exist then it is ignored.
    //Returns the size of the list.
    private int rpushx(string key, string value);
    
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
