#include "parallel_header.hpp"

// -std=c++17 をつける
template<typename Key, typename Value, typename Hash=std::hash<Key> >
class ThreadsafeUnorderedMap
{
private:
    struct noexist_read : std::exception
    {
        const char* what() const noexcept
        {
            return "no element to read!!";
        };
    };
    struct noexist_erase : std::exception
    {
        const char* what() const noexcept
        {
            return "no element to erase!!";
        };
    };
    class bucket_type
    {
    private:
        using bucket_value = std::pair<Key, Value>;
        using bucket_data = std::list<bucket_value>;
        using bucket_iterator = typename bucket_data::iterator;
        using c_bucket_iterator = typename bucket_data::const_iterator;
        bucket_data data;
        mutable std::shared_mutex mutex;
        bucket_iterator find_entry_for(const Key& key)
        {
            return std::find_if(data.begin(), data.end(), [&](const bucket_value& item)
            {
                return item.first == key;
            });
        }
        c_bucket_iterator c_find_entry_for(const Key& key) const
        {
            return std::find_if(data.begin(), data.end(), [&](const bucket_value& item)
            {
                return item.first == key;
            });
        }
    public:
        const Value& value_for(const Key& key) const
        {
            std::shared_lock<std::shared_mutex> lock(mutex);
            const c_bucket_iterator found_entry = c_find_entry_for(key);
            if(found_entry == data.end())
            {
                throw noexist_read();
            }
            else
            {
                return found_entry->second;
            }
        }
        Value& add_or_update_mapping(const Key& key)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);
            const bucket_iterator found_entry = find_entry_for(key);
            if(found_entry == data.end())
            {
                data.push_back(bucket_value(key, Value()));
                return data.back().second;
            }
            else
            {
                return found_entry->second;
            }
        }
        void remove_mapping(const Key& key)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);
            const bucket_iterator found_entry = find_entry_for(key);
            if(found_entry == data.end())
            {
                throw noexist_erase();
            }
            else
            {
                data.erase(found_entry);
            }
        }
    };
    std::vector<std::unique_ptr<bucket_type> > buckets;
    Hash hasher;
    bucket_type& get_bucket(const Key& key) const
    {
        const std::size_t bucket_index = hasher(key) % buckets.size();
        return *buckets[bucket_index];
    }
public:
    using key_type = Key;
    using mapped_type = Value;
    using hash_type = Hash;
    // num_buckets は素数に設定する
    ThreadsafeUnorderedMap(unsigned num_buckets, const Hash& hasher_ = Hash())
        : buckets(num_buckets), hasher(hasher_)
    {
        for(unsigned i = 0; i < num_buckets; ++i)
        {
            buckets[i].reset(new bucket_type);
        }
    }
    ThreadsafeUnorderedMap(const ThreadsafeUnorderedMap& another) = delete;
    ThreadsafeUnorderedMap& operator= (const ThreadsafeUnorderedMap& another) = delete;
    const Value& at(const Key& key) const
    {
        try
        {
            return get_bucket(key).value_for(key);
        }
        catch(noexist_read& err)
        {
            std::cout << err.what() << std::endl;
            assert(false);
        }
        // in order to handle a warning (to be ignored)
        return get_bucket(key).value_for(key);
    }
    Value& operator[](const Key& key) noexcept
    {
        return get_bucket(key).add_or_update_mapping(key);
    }
    void erase(const Key& key)
    {
        try
        {
            get_bucket(key).remove_mapping(key);
        }
        catch(noexist_erase& err)
        {
            std::cout << err.what() << std::endl;
            assert(false);
        }
    }
};
