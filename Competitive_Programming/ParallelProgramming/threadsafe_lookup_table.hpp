#include "parallel_header.hpp"

template<typename Key, typename Value, typename Hash=std::hash<Key> >
class threadsafe_lookup_table
{
private:
    class bucket_type
    {
    private:
        using bucket_value=std::pair<Key, Value>;
        using bucket_data=std::list<bucket_value>;
        using bucket_iterator=typename bucket_data::iterator;
        bucket_data data;
        mutable std::shared_mutex mutex;
        bucket_iterator find_entry_for(Key const& key) const {
            return std::find_if(data.begin(), data.end(), [&](bucket_value const& item){
                return item.first==key;
            });
        }
    public:
        Value value_for(Key const& key, Value const& default_value) const {
            std::shared_lock<std::shared_mutex> lock(mutex);
            bucket_iterator const found_entry=find_entry_for(key);
            return (found_entry==data.end())?default_value:found_entry->second;
        }
        void add_or_update_mapping(Key const& key, Value const& value)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);
            bucket_iterator const found_entry=find_entry_for(key);
            if(found_entry==data.end())
            {
                data.push_back(bucket_value(key, value));
            }
            else
            {
                found_entry->second=value;
            }
        }
        void remove_mapping(Key const& key)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);
            bucket_iterator const found_entry=find_entry_for(key);
            if(found_entry!=data.end())
            {
                data.erase(found_entry);
            }
        }
    };
    std::vector<std::unique_ptr<bucket_type> > buckets;
    Hash hasher;
    bucket_type& get_bucket(Key const& key) const
    {
        std::size_t const bucket_index=hasher(key)%buckets.size();
        return *buckets(bucket_index);
    }
public:
    using key_type=Key;
    using mapped_type=Value;
    using hash_type=Hash;
    threadsafe_lookup_table(unsigned num_buckets=19, Hash const& hasher_=Hash())
        : buckets(num_buckets), hasher(hasher_)
    {
        for(unsigned i=0;i<num_buckets;++i){
            buckets[i].reset(new bucket_type);
        }
    }
    threadsafe_lookup_table(threadsafe_lookup_table const& another)=delete;
    threadsafe_lookup_table& operator=(threadsafe_lookup_table const& another)=delete;
    Value value_for(Key const& key, Value const& default_value=Value()) const
    {
        return get_bucket(key).value_for(key, default_value);
    }
    void add_or_update_mapping(Key const& key, Value const& value)
    {
        get_bucket(key).add_or_updata_mapping(key, value);
    }
    void remove_mapping(Key const& key)
    {
        get_bucke(key).remove_mapping(key);
    }
};
