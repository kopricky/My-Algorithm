#include "../header.hpp"

// 使う文字は 31 文字以下の文字コード上で連続する文字の列とする.
template<unsigned int CHARACTER_SIZE, char START_CHARACTER>
class CompressedTrie {
private:
    struct node {
        string *s;
        node **to;
        // sub: 部分木に含まれる要素の数, adj: 子の bit 表現
        uint32_t sub, adj;
        node() : s(nullptr), to(nullptr),  sub(1u), adj(0u){}
        node(string&& _s, node *v, unsigned int index, uint32_t _sub)
         : s(new string[CHARACTER_SIZE]()), to(new node*[CHARACTER_SIZE]()),
            sub(_sub), adj(1u << index){
            s[index] = move(_s), to[index] = v;
        }
        // ~node(){ delete[] s, delete[] to; }
        #define lsb(v) (__builtin_ctz(v))
        inline unsigned int begin() const { return adj ? lsb(adj) : CHARACTER_SIZE; }
        inline unsigned int next(unsigned int cur) const {
            cur = adj & ~((1u << (cur + 1u)) - 1u);
            return cur ? lsb(cur) : CHARACTER_SIZE;
        }
        inline static unsigned int end(){ return CHARACTER_SIZE; }
        inline bool exist(const unsigned int v) const { return adj >> v & 1u; }
        inline bool isExist(const unsigned int v) const { return adj >> v & 1u; }
        inline bool isFinal() const { return !s; }
        void direct_push(string&& _s, unsigned int index){
            if(!s) s = new string[CHARACTER_SIZE](), to = new node*[CHARACTER_SIZE]();
            s[index] = move(_s), to[index] = new node(), ++sub, adj |= (1u << index);
        }
    };
    void make_node(string& orgs, unsigned int start, node*& to){
        string tmp = orgs.substr(0, start);
        orgs.erase(orgs.begin(), orgs.begin() + start);
        to = new node(move(orgs), to, orgs[0] - START_CHARACTER, to->sub);
        orgs = move(tmp);
    }
    void new_push(const string& s, unsigned int index, node *to){
        string _s(s.substr(index, s.size() - index));
        to->direct_push(move(_s), s[index] - START_CHARACTER);
    }
    void new_push(string&& s, unsigned int index, node *to){
        s.erase(s.begin(), s.begin() + index);
        to->direct_push(move(s), s[0] - START_CHARACTER);
    }
    template<typename String>
    void push(node *cur, String&& news){
        const unsigned int _ls = news.size();
        unsigned int index = 0u, prefix;
        while(true){
            const unsigned int num = news[index] - START_CHARACTER;
            if(cur->isExist(num)){
                ++cur->sub;
                string& orgs = cur->s[num];
                const unsigned int ls = orgs.size();
                for(prefix = 0u; prefix < ls && index < _ls; ++prefix, ++index){
                    if(orgs[prefix] == news[index]) continue;
                    make_node(orgs, prefix, cur->to[num]);
                    new_push(forward<String>(news), index, cur->to[num]);
                    return;
                }
                if(index == _ls){
                    if(prefix == ls) return;
                    make_node(orgs, prefix, cur->to[num]);
                    return;
                }else{
                    cur = cur->to[num];
                }
            }else{
                new_push(forward<String>(news), index, cur);
                return;
            }
        }
    }
    // void clear_dfs(node *cur){
    //     if(!cur) return;
    //     for(unsigned int i = 0u; i < CHARACTER_SIZE; ++i) if(cur->to) clear_dfs(cur->to[i]);
    //     delete cur;
    // }
public:
    node* root;
    CompressedTrie() : root(new node()){}
    // ~CompressedTrie(){ clear_dfs(root); }
    // CompressedTrie 木に s を加える
    void add(const string& s){ push(root, s); }
    void add(string&& s){ push(root, move(s)); }
    //何らかのクエリ
    int recur_query(node *cur, unsigned int d, const string& s){
        int res = 0;
        while(true){
            // 終点ノードなら break
            if(cur->isFinal()) break;
            const unsigned int next = s[d] - START_CHARACTER;
            // cur から出る存在する辺のみを以下の for 文でなめることができる.
            for(unsigned int i = cur->begin(); i != cur->end(); i = cur->next(i)){
                // 例えば cur->s[i] は cur から出る i 番目の文字から始まる文字列
            }
            d += cur->s[next].size();
            cur = cur->to[next];
        }
        return res;
    }
    int query(const string& s){
        return recur_query(root, 0u, s);
    }
};
