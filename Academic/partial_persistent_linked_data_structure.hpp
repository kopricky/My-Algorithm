#include "header.hpp"

template<unsigned int INDEGREE, unsigned int OUTDEGREE, typename DataType>
class PartialPersistentLinkedDataStructure
{
private:
    static const unsigned int THRESHOLD = INDEGREE;
    enum CHANGE { INSERT, ERASE };
public:
    struct base_pointer;
    struct pointer;
    struct info;
    struct node
    {
        DataType data;
        const unsigned int version;
        unsigned int extra_count;
        base_pointer original[OUTDEGREE];
        pointer inverse[INDEGREE];
        struct node *copy_pointer;
        forward_list<info> extra[OUTDEGREE];
        node(const DataType& _data, const unsigned int _version)
            : data(_data), version(_version), extra_count(0)
        {}
        inline bool isLive() const
        {
            return !copy_pointer;
        }
    };
    struct base_pointer
    {
        node *to;
        unsigned int inverse_index;
        base_pointer() : to(nullptr){}
        base_pointer(node* const _to, const unsigned int _inverse_index)
            : to(_to), inverse_index(_inverse_index)
        {}
        inline bool isExist() const
        {
            return to;
        }
    };
    static inline void toNULL(base_pointer& u)
    {
        u.to = nullptr;
    }
    struct pointer : base_pointer
    {
        unsigned int version;
        pointer() : base_pointer()
        {}
        pointer(node* const _to, const unsigned int _inverse_index, const unsigned int _version)
            : base_pointer(_to, _inverse_index), version(_version)
        {}
    };
    struct info
    {
        const CHANGE change;
        const pointer detail;
        info(const CHANGE _change) : change(_change){}
        info(const CHANGE _change, const pointer& _detail) : change(_change), detail(_detail)
        {}
    };
private:
    queue<node*> copied_nodes;
    unsigned int set_inverse_pointer(node* const u, const unsigned int index,
        node* const v, const unsigned int version) const
    {
        for(unsigned int i = 0; i < INDEGREE; ++i)
        {
            if(!v->inverse[i].isExist())
            {
                v->inverse[i] = (pointer){u, index, version};
                return i;
            }
        }
        assert(false);
    }
    void erase_inverse_pointer(const node* u, const unsigned int index,
        node* const v) const
    {
        for(unsigned int i = 0; i < INDEGREE; ++i)
        {
            if(v->inverse[i].to == u && v->inverse[i].inverse_index == index)
            {
                toNULL(v->inverse[i]);
                return;
            }
        }
        assert(false);
    }
    void init_node(node* const u, node* const newu)
    {
        u->copy_pointer = newu;
        for(unsigned int i = 0; i < OUTDEGREE; ++i)
        {
            base_pointer cur = current_base_pointer(u, i);
            node *v = (newu->original[i] = cur).to;
            if(v)
                v->inverse[cur.inverse_index].to = newu;
        }
        copied_nodes.push(u);
    }
    void update_link(node * const u, const unsigned int index,
        node * const v, const unsigned int version, const CHANGE type)
    {
        if(u->version == version)
        {
            if(type == INSERT)
            {
                u->original[index] = (base_pointer){v, set_inverse_pointer(u, index, v, version)};
            }
            else
            {
                toNULL(u->original[index]);
                erase_inverse_pointer(u, index, v);
            }
        }
        else if(u->extra_count < THRESHOLD)
        {
            if(type == INSERT)
            {
                u->extra[index].emplace_front(type,
                    pointer(v, set_inverse_pointer(u, index, v, version), version));
            }
            else
            {
                erase_inverse_pointer(u, index, v);
                u->extra[index].emplace_front(type);
            }
            ++u->extra_count;
        }
        else
        {
            node *newu = new node(u->data, version);
            init_node(u, newu);
            update_link(newu, index, v, version, type);
        }
    }
    base_pointer current_base_pointer(const node *u, const unsigned int out_index) const
    {
        base_pointer result = u->original[out_index];
        if(!u->extra[out_index].empty())
        {
            const info& value = u->extra[out_index].front();
            if(value.change == INSERT)
            {
                result = (base_pointer){value.detail.to, value.detail.inverse_index};
            }
            else
            {
                toNULL(result);
            }
        }
        return result;
    }
    void _update_data(node* const u, const DataType new_data, const unsigned int version)
    {
        if(u->version == version)
        {
            u->data = new_data;
        }
        else
        {
            init_node(u, new node(new_data, version));
        }
    }
    void erase_outdegree(node *u, const unsigned int version)
    {
        for(unsigned int i = 0; i < OUTDEGREE; ++i)
        {
            node *v = current_base_pointer(u, i).to;
            if(v)
                erase_link(u = toLive(u), i, v, version);
        }
    }
    void erase_indegree(node *u, const unsigned int version)
    {
        for(unsigned int i = 0; i < INDEGREE; ++i){
            pointer in = u->inverse[i];
            if(in.isExist())
                erase_link(in.to, in.inverse_index, u = toLive(u), version);
        }
    }
    void update_inverse_pointer(node *u, const unsigned int version)
    {
        node *newu = toLive(u);
        for(unsigned int i = 0; i < INDEGREE; ++i)
        {
            pointer inverse_pointer = u->inverse[i];
            if(inverse_pointer.isExist())
                update_link(inverse_pointer.to, inverse_pointer.inverse_index, newu, version, INSERT);
        }
    }
    void finish_update(const unsigned int version)
    {
        while(!copied_nodes.empty())
        {
            node *u = copied_nodes.front();
            copied_nodes.pop();
            update_inverse_pointer(u, version);
        }
    }
    node *_forward(const node *u, const unsigned int out_index, const unsigned int version) const
    {
        node *res = u->original[out_index].to;
        for(const info& value : u->extra[out_index]){
            if(value.detail.version <= version)
            {
                if(value.change == INSERT)
                {
                    res = value.detail.to;
                }
                else
                {
                    res = nullptr;
                }
                break;
            }
        }
        return res;
    }
public:
    PartialPersistentLinkedDataStructure(){}
    // u の out_index 番目に v を指す link を追加(u, v は最新版(isLive) であることを仮定)
    void insert_link(node* const u, const unsigned int out_index,
        node* const v, const unsigned int version)
    {
        assert(u->isLive() && v->isLive());
        update_link(u, out_index, v, version, INSERT);
        finish_update(version);
    }
    // u の out_index 番目の v を指す link を削除(u, v は最新版(isLive) であることを仮定)
    void erase_link(node* const u, const unsigned int out_index,
        node* const v, const unsigned int version)
    {
        assert(u->isLive() && v->isLive());
        update_link(u, out_index, v, version, ERASE);
        finish_update(version);
    }
    // u の データを new_data に更新(u は最新版(isLive) であることを仮定)
    void update_data(node* const u, const DataType new_data, const unsigned int version)
    {
        assert(u->isLive());
        _update_data(u, new_data, version);
        finish_update(version);
    }
    // データ _data を持つノードを挿入
    node *insert_node(const DataType _data, const unsigned int version) const
    {
        return new node(_data, version);
    }
    // 正確には u に入る辺, 出る辺をすべて削除
    void erase_node(node* const u, const unsigned int version)
    {
        erase_outdegree(u, version);
        erase_indegree(u, version);
    }
    // バージョン version の更新後における u の out_index 番目のリンクが指す先の node
    node *forward(const node *u, const unsigned int out_index, const unsigned int version) const
    {
        return _forward(u, out_index, version);
    }
    // u を最新のバージョンのノードにする.
    static node *toLive(node *u)
    {
        while(u->copy_pointer)
        {
            u = u->copy_pointer;
        }
        return u;
    }
};
