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
#include "DataBase.h"

template<>
class DataBase<std::string, std::deque<std::string>> {
public:
    template<>
    DataBase<std::string, std::deque<std::string>>();

    // Returns if a list key exists in the database.
    template<>
    bool exist(const std::string& key);

    // Returns the deque list for the given key.
    // Throws an error if the key does not exist.
    template<>
    std::deque<std::string> get(const std::string& key);

    // Removes a list from the database.
    // Returns true if the key is in the list and successfully removed.
    template<>
    bool remove(const std::string& key);

    // Renames a list in the database.
    // Returns false if the key does not exist or the newkey already exists.
    // Returns true for a successful rename.
    template<>
    bool rename(const std::string& key, const std::string& newkey);
    
    //Removes and returns the first element of the list stored at key.
    template<>
    std::string lpop(std::string key);
    
    //Removes and returns the last element of the list stored at key.
    template<>
    std::string rpop(std::string key);
    
    // Pushes a value to the list at the key.
    // initialzer list options are as follows:
    // set(k, v, {'l'}) -> lpush
    // set(k, v, {'r'}) -> rpush
    // set(k, v, {'x', 'l'}) -> lpushx
    // set(k, v, {'x', 'r'}) -> rpushx
    // x must always be the first option followed by a direction
    // Invalid options will always return a false.
    // Returns if the value is successfully added to the key.
    template<>
    bool set(const std::string& key, const std::string& value, std::initializer_list<std::string> options);
    
    //Inserts the value at at the index before or after the pivot parameter.
    //Returns the size of the list.
    //Returns -1 if the pivot or key is not found.
    template<>
    int linsert(std::string key, std::string pivot, std::string value, bool before);
    
    //Returns a vector of all string values in the given range.
    //Negative values will be count from the tail.
    template<>
    std::vector<std::string> lrange(std::string key, int start, int end);
    
    //Sets a value for the given index idx.
    //Negative values will be count from the tail.
    //Returns true if the index is exists and false if it does not.
    template<>
    bool lset(std::string key, int idx, std::string value);
    
    //Removes the first count occurrences of elements equal to element from the list stored at key. The count argument influences the operation in the following ways:
    //count > 0: Remove elements equal to element moving from head to tail.
    //count < 0: Remove elements equal to element moving from tail to head.
    //count = 0: Remove all elements equal to element.
    template<>
    int lrem(std::string key, int count, std::string value);
    
    //Trims a list to the expected range.
    //Negative start or end will start from the tail.
    template<>
    int ltrim(std::string key, int start, int end);
    
    //Pops a value from a list with the source as its key and pushes it to the destination list.
    //Returns the popped and pushed value.
    template<>
    std::string rpoplpush(std::string source, std::string destination);
    
    //Returns a value at a given index.
    //A negative idx will start from the tail.
    template<>
    std::string lindex(std::string key, int idx);
    
    //Returns the size of a list for a given key.
    template<>
    int llen(std::string key);
    
private:
    std::unordered_map<std::string, std::deque<std::string>> m;
    
    //Inserts the value at the head of the list. If a list does not exist for a key then the list is created.
    //Returns the size of the list.
    template<>
    int lpush(std::string key, std::string value);
    
    //Inserts the value at the tail of the list. If a list does not exist for a key then the list is created.
    //Returns the size of the list.
    template<>
    int rpush(std::string key, std::string value);

    //Inserts the value at the head of the list only if a list already exists. If a list does not exist then it is ignored.
    //Returns the size of the list.
    template<>
    int lpushx(std::string key, std::string value);

    //Inserts the value at the tail of the list only if a list already exists. If a list does not exist then it is ignored.
    //Returns the size of the list.
    template<>
    int rpushx(std::string key, std::string value);
};
