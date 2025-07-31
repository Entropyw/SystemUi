#ifndef SPLAY_H
#define SPLAY_H
//define basic features of the element in splay tree
struct _Node{
    void* val;  //The data
    struct _Node *ls, *rs; // two sons
    struct _Node *fa; //its father node
    int cnt; //the same values it has
    int size; //size of the sub_tree with a root of the current node
};
typedef struct _Node _Node;
//define the type of a whole tree
typedef struct{
    _Node* rt;   //root
    int(* cmp)(const void* const,const void* const); //comparison strategy  
}SplayTree;

//define the function of dealing with tree size when a node is inserted or deleted
void update(_Node * const x);
//define the function of rotation to ensure the balance of the tree
void rotate(_Node * const x);
//define the core operation of splay 
void splay(_Node* const x, SplayTree* const t);
//define the inserting operation
int insert(void* const x, SplayTree* const t);
//define the function to find the rank of x
int findRank(const void* const x, SplayTree* const t);
//define the function to find the k-th number
void* findKmem(int k, SplayTree* const t);
//define the function to find the pre of x
_Node* preMem(const void* const x, SplayTree* const t);
//define the function to find the next number of x
_Node* nxtMem(const void* const x, SplayTree* const t);
//define the function to remove x
int Remove(const void *const x, SplayTree* const t);
//build a tree
void build(SplayTree* const t, int (* cmp)(const void* const, const void* const));
// user friendly functions
void * Find(const void* const x, SplayTree* const t);
void * PreMem(const void* const x, SplayTree* const t);
void * NxtMem(const void* const x, SplayTree* const t);
void DeleteTree(_Node* t);
void DeleteAllElements(_Node* t);
#endif