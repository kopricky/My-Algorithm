#include "../header.hpp"

// 使う文字は 31 文字以下の文字コード上で連続する文字の列とする.
template<unsigned int CHARACTER_SIZE, char START_CHARACTER>
class CompressedTrie {
private:
    struct node {
        string s[CHARACTER_SIZE];
        node *to[CHARACTER_SIZE] = {};
        // sub: 部分木に含まれる要素の数, adj: 子の bit 表現
        uint32_t sub, adj;
        node() : sub(1u), adj(0u){}
        node(string&& _s, node *v, unsigned int index, uint32_t _sub)
         : sub(_sub), adj(1u << index){
            s[index] = move(_s), to[index] = v;
        }
        #define lsb(v) (__builtin_ctz(v))
        inline unsigned int begin() const { return adj ? lsb(adj) : CHARACTER_SIZE; }
        inline unsigned int next(unsigned int cur) const {
            cur = adj & ~((1u << (cur + 1u)) - 1u);
            return cur ? lsb(cur) : CHARACTER_SIZE;
        }
        inline static unsigned int end(){ return CHARACTER_SIZE; }
        inline bool exist(const unsigned int v) const { return adj >> v & 1u; }
        void direct_push(string&& _s, unsigned int index){
            s[index] = move(_s), to[index] = new node(), ++sub, adj |= (1u << index);
        }
    };
    void make_node(string& orgs, const string& news, unsigned int start, unsigned int newend, node*& to){
        string tmp = orgs.substr(0, start);
        orgs.erase(orgs.begin(), orgs.begin() + start);
        to = new node(move(orgs), to, orgs[0] - START_CHARACTER, to->sub);
        orgs = move(tmp);
        if(start == newend) return;
        to->direct_push(news.substr(start, newend - start), news[start] - START_CHARACTER);
    }
    template<typename String>
    void push(node *cur, String&& _s){
        string news = forward<String>(_s);
        while(true){
            const unsigned int num = news[0] - START_CHARACTER;
            if(cur->exist(num)){
                ++cur->sub;
                string& orgs = cur->s[num];
                const unsigned int ls = orgs.size(), _ls = news.size();
                for(unsigned int i = 0u; i < min(ls, _ls); ++i){
                    if(orgs[i] == news[i]) continue;
                    return make_node(orgs, news, i, _ls, cur->to[num]);
                }
                if(ls > _ls) return make_node(orgs, news, _ls, _ls, cur->to[num]);
                else if(ls < _ls) cur = cur->to[num], news.erase(news.begin(), news.begin() + ls);
                else return;
            }else{
                return cur->direct_push(move(news), num);
            }
        }
    }
    // void clear_dfs(node *cur){
    //     if(!cur) return;
    //     for(unsigned int i = 0u; i < 27u; ++i) clear_dfs(cur->to[i]);
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
    int recur_query(node *cur, unsigned int d, int *index, const string& s){
        int res = 0;
        while(true){
            if(!cur || d >= s.size()) break;
            const unsigned int next = s[d] - START_CHARACTER;
            for(unsigned int i = cur->begin(); i != cur->end(); i = cur->next(i)){
                if(index[i] < index[next]) res += cur->to[i]->sub;
            }
            d += cur->s[next].size();
            cur = cur->to[next];
        }
        return res;
    }
    int query(int *index, const string& s){
        return recur_query(root, 0u, index, s);
    }
};
