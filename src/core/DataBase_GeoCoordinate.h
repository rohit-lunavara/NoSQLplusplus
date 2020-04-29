#include "HashTable.h"
#include "GeoCoordinate.h"
#include "SphericalRTree.h"
#include "DataBase_Errors.h"

#include <set>

template <>
class DataBase<std::string, Geography::GeoCoordinate>
{
private:
        RTree::SphericalRTree<std::string> rtree_;
        HashTable<std::string, Geography::GeoCoordinate> table_;
public:
        Geography::GeoCoordinate get(const std::string& key);

        bool set(const std::string& key, const Geography::GeoCoordinate& value,
                std::initializer_list<std::string> options);
        
        bool exist(const std::string& key);
        bool remove(const std::string& key);
        bool rename(const std::string& key, const std::string& newkey);

        Geography::GeoCoordinate getset(const std::string& key, const Geography::GeoCoordinate& value,
                std::initializer_list<std::string> options);
        
        double distance(const std::string& key1, const std::string& key2);
        
        std::set<std::string> radius(const std::string& key, double rad);
};


// O (logN)
bool DataBase<std::string, Geography::GeoCoordinate>::set(
        const std::string& key, 
        const Geography::GeoCoordinate& value, 
        std::initializer_list<std::string> options)
{
        // TODO options
        rtree_.insert(key, value, 0.);
        table_[key] = value;
        return true;
}

// O (1)
Geography::GeoCoordinate
DataBase<std::string, Geography::GeoCoordinate>::get(const std::string& key)
{
	if (!exist(key))
        {
		throw ValueNotFoundException();
	}
        return table_[key];
}

// O (N)
bool DataBase<std::string, Geography::GeoCoordinate>::remove(const std::string& key)
{
        if (!exist(key))
        {
                return false;
        }

        rtree_.remove(key, table_[key], 0.001);
        table_.erase(key);
        return true;
}


bool DataBase<std::string, Geography::GeoCoordinate>::exist(const std::string& key)
{
        return table_.find(key) != table_.end();
}


Geography::GeoCoordinate DataBase<std::string, Geography::GeoCoordinate>::getset(
        const std::string& key,
        const Geography::GeoCoordinate& value,
        std::initializer_list<std::string> options)
{
        if (!exist(key))
        {
                throw ValueNotFoundException();
        }

        auto oldvalue = get(key);
        set(key, value, options);
        return oldvalue;
}


// O (1)
double DataBase<std::string, Geography::GeoCoordinate>::distance(
    const std::string& key1, 
    const std::string& key2)
{
        if (!exist(key1) || !exist(key2))
        {
                throw ValueNotFoundException();
        }

        const auto& geo1 = table_[key1];
        const auto& geo2 = table_[key2];
        return Geography::distance(geo1, geo2);
}


// O (logN)
std::set<std::string> DataBase<std::string, Geography::GeoCoordinate>::radius(
        const std::string& key, 
        double rad)
{
        if (!exist(key))
        {
                throw ValueNotFoundException();
        }

        std::set<std::string> results;
        rtree_.search(table_[key], rad, &results);
        return results;
}


bool DataBase<std::string, Geography::GeoCoordinate>::rename(
        const std::string& key, 
        const std::string& newkey)
{
        if (!exist(key))
        {
                return false;
        }

        auto value = get(key);
        remove(key);
        set(newkey, value, {});

        return true;
}
