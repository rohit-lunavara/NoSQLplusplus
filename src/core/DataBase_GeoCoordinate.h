#include "HashTable.h"
#include "GeoCoordinate.h"
#include "SphericalRTree.h"
#include "DataBase_Errors.h"

#include <set>
#include <locale>
/**
 * Specialized class for Geography Coodinates (latitude and longitude) handling
 */
template <>
class DataBase<std::string, Geography::GeoCoordinate>
{
private:
        /**
         * data structure for spherical rtree
         */
        RTree::SphericalRTree<std::string> rtree_;

        /**
         * a hashtable keep the record of geography coordinates
         * for each key
         */
        HashTable<std::string, Geography::GeoCoordinate> table_;
public:
        /**
         * Gets the geography coordinates for a key
         * @param key key for coordinates, such as city name
         * @return geography coordinates, if not exist throw an error
         */
        Geography::GeoCoordinate get(const std::string& key);

        /**
         * Sets the coordinate for a key
         * with options "NX", only set if the key does not exist
         *              "XX", only set if the key exists
         * @param key key for coordinates
         * @param value coordinates
         * @param options different options {opt1, opt2}
         * @return true if successful
         */
        bool set(const std::string& key, const Geography::GeoCoordinate& value,
                std::initializer_list<std::string> options = {});
        
        /**
         * Checks if a key exist
         * @param key key for coordinates
         * @return true if existed
         */
        bool exist(const std::string& key);

        /**
         * Removes a key and the corresponding record
         * @return true if the key exist
         */
        bool remove(const std::string& key);

        /**
         * Renames a key
         @param key key for a coordinate
         @param newKey the new key for this coordinate
         @return true if the old key exist
         */
        bool rename(const std::string& key, const std::string& newkey);

        /**
         * Gets the coordinates for a key and set them to be value
         * with options
         * @param key key for the coordinate
         * @param value the new value for the key
         * @param options options
         * @return old geography coordinates
         */
        Geography::GeoCoordinate getset(const std::string& key, const Geography::GeoCoordinate& value,
                std::initializer_list<std::string> options);
        
        /**
         * Gets the distance between two keys: key1 and key2
         * @param key1 one key
         * @param key2 another key
         * @return true if both key exist or throw an error
         */
        double distance(const std::string& key1, const std::string& key2);
        
        /** 
         * Queries the keys within distance rad from key
         * @param key the center key
         * @param rad the query radiance, in kilometers
         * @return the set for all keys located in the range
         */
        std::set<std::string> radius(const std::string& key, double rad);
};


// O (logN)
bool DataBase<std::string, Geography::GeoCoordinate>::set(
        const std::string& key, 
        const Geography::GeoCoordinate& value, 
        std::initializer_list<std::string> options)
{
        for (auto& option : options) {
		std::for_each(option.begin(), option.end(), ::toupper);
	}

	for (const auto& option : options) {
		// NX -> Set if it does not exist
		// XX -> Set if it exists
		if ((option == "NX" && !exist(key)) ||
                        (option == "XX" && exist(key)) ) {
                        rtree_.insert(key, value, 0.);
                        table_[key] = value;			
			return true;
		}
	}

	if ( options.size() == 0 ) {
                rtree_.insert(key, value, 0.);
                table_[key] = value;
		return true ;
	}
	else {
		return false ;
	}
}

// O (1)
Geography::GeoCoordinate
DataBase<std::string, Geography::GeoCoordinate>::get(const std::string& key)
{
	if (!exist(key))
        {
		throw KeyNotFoundException();
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
                throw KeyNotFoundException();
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
                return -1.;
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
        std::set<std::string> results;
        if (!exist(key))
        {
               return results;
        }

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
