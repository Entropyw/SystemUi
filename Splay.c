#include "Splay.h"
#include <stdlib.h>
void update(_Node* const x) {
    if(x == NULL) return;   
    //The size is the sum of the size of its left,right tree and its same elements
    x -> size = x -> cnt;
    if(x -> ls != NULL) x -> size += x -> ls -> size;
    if(x -> rs != NULL) x -> size += x -> rs -> size;
}
//define the function of rotation to ensure the balance of the tree
void rotate(_Node* const x) {
    _Node *f = x -> fa;
  // swap the son and father while maintaining the balance,we rotate as follows
    if(f -> ls == x) {
        f -> ls = x -> rs; //The current _Node's right son becomes the left son of father 
        if(x -> rs != NULL) x -> rs -> fa = f; 
        x -> rs = f; //The right son of current _Node becomes its father
        x -> fa = f -> fa; // The father of current node becomes its grandfather
        f -> fa = x;
        if(x -> fa != NULL){
            if(f == x -> fa -> ls) x -> fa -> ls = x;
            else x -> fa -> rs = x;
        } //note that we should change the sons as well as fathers
    } else {
        //same as above
        f -> rs = x -> ls;
        if(x -> ls != NULL) x -> ls -> fa = f;
        x -> ls = f;
        x -> fa = f -> fa;
        f -> fa = x;
        if(x -> fa != NULL) {
            if(f == x -> fa -> ls) x -> fa -> ls = x;
            else x -> fa -> rs = x;
        }
    }
    update(f); update(x);
    //As the relation has changed, updating is needed.
}

//define the core operation of splay 
void splay(_Node* const x, SplayTree* const t) {
    //rotate x to the root
    while(x -> fa != NULL) {
        _Node *f = x -> fa;
        _Node *g = f -> fa;
        //if x,fa,gfa are in a line,rotate f,else rotate
        if(g != NULL) {   
            if(f -> ls == x && g -> ls == f || f -> rs == x && g -> rs == f) 
                rotate(f);
            else rotate(x);
        }
        rotate(x);
    }
    t -> rt = x;
}
//define the inserting operation
int insert(void* const x, SplayTree* const t) {
    _Node *now = t -> rt;
    _Node *f = NULL;
    if(now != NULL) f = now -> fa;
    int flag = 0;
    // go to empty nodes
    while(now != NULL) {
        if(t -> cmp(x, now -> val) > 0) {
            f = now;
            now = now -> rs;
            flag = 0;
        }else if(t -> cmp(x, now -> val) == 0) {
            now -> cnt++;
            update(now);
            splay(now, t);
            return 1; 
        }else if(t -> cmp(x, now -> val) < 0) {
            f = now;
            now = now -> ls;
            flag = 1;
        }
    }
    now = (_Node *)malloc(sizeof(_Node));
    if(now == NULL) return 0; //failed malloc
    now -> val = x;
    now -> size = now -> cnt = 1;
    now -> ls = NULL;
    now -> rs = NULL;    
    now -> fa = f;
    if(flag && f != NULL) f -> ls = now;
    else if(f != NULL) f -> rs = now;
    splay(now, t);
    return 1;
}
//define the function to find the rank of x
int findRank(const void* const x, SplayTree* const t) {
    _Node *i = t -> rt;
    int sum = 0;
    //find how many numbers are below x
    while(i != NULL) {
        if(t -> cmp(x, i -> val) > 0 && i -> ls != NULL) sum += i -> ls -> size + i -> cnt;
        else if(t -> cmp(x, i -> val) > 0) sum += i -> cnt;
        if(t -> cmp(x, i -> val) > 0 && i -> rs != NULL) i = i -> rs;
        else if(t -> cmp(x, i -> val) <= 0 && i -> ls != NULL) i = i -> ls;
        else break;
    }
    if(i != NULL) splay(i, t);
    return sum + 1;
}
_Node* find(const void* const x, SplayTree* const t) {
    _Node* i = t -> rt;
    while(i != NULL) {
        if(t -> cmp(x, i -> val) == 0) {
            splay(i, t);  //splay to the root
            return i;
        }
        if(t -> cmp(x, i -> val) > 0) i = i -> rs;
        else i = i -> ls;
    }
    return NULL;
}
//define the function to find the k-th number
void* findKmem(int k, SplayTree* const t) {
    _Node* i = t -> rt;
    while(i != NULL) {
        if(i -> ls == NULL) {
            if(i -> cnt >= k) {
                splay(i, t);
                return i -> val;
            }else {
                k -= i -> cnt;
                i = i -> rs;
            }
        }else {
            if(i -> ls -> size + i -> cnt >= k && k > i -> ls -> size) {
                splay(i, t);
                return i -> val;
            }else if(k <= i -> ls -> size) {
                i = i -> ls;
            }else {
                k -= i -> ls -> size + i -> cnt;
                i = i -> rs;
            }
        }
    }
    return NULL;
}
//define the function to find the pre of x
_Node *preMem(const void* const x, SplayTree* const t) {
    //find the largest feasible number
    _Node* i = t -> rt, *now = NULL;
    while(i != NULL) {
        if(t -> cmp(x, i -> val) > 0) {
            now = i;
            i = i -> rs;
        }else i = i -> ls;
    }
    if(now != NULL) splay(now, t);
    return now;
}
//define the function to find the next number of x
_Node* nxtMem(const void* const x, SplayTree* const t) {
    //find the smallest feasible number
    _Node* i = t -> rt, *now = NULL;
    while(i != NULL) {
        if(t -> cmp(x, i -> val) < 0) {
            now = i;
            i = i -> ls;
        }else i = i -> rs;
    }
    if(now != NULL) splay(now, t);
    return now;
}
//define the function to remove x
int Remove(const void* const x, SplayTree* const t) {
    _Node *pos = find(x, t);
    if(pos == NULL) return 0; 
    if(pos -> cnt > 1) {
        pos -> cnt--;
        update(pos);
        splay(pos, t);
        return 1;
    }//remove directly
    if(pos -> ls != NULL && pos -> rs != NULL) {
        preMem(x, t);
        t -> rt -> rs = pos -> rs;
        pos -> rs -> fa = t -> rt;
        free(pos);
        update(t->rt); //pre is root, ls is pos
    }else if(pos -> ls == NULL && pos -> rs == NULL){
        free(t -> rt);
        t -> rt = NULL; 
    }else if(pos -> ls != NULL){
        t -> rt = pos -> ls;
        t -> rt -> fa = NULL;
        //change the root
        free(pos);
    }else {
        t -> rt = pos -> rs;
        t -> rt -> fa = NULL; //change the root
        free(pos);
    }
    return 1;
}
//build a tree
void build(SplayTree* const t, int(* cmp)(const void* const, const void* const)) {
    t -> rt = NULL;
    t -> cmp = cmp;
} 
// user oriented functions
//find the address of the element equal to the given template
void* Find(const void* const x, SplayTree* const t) {
    _Node* pos = find(x, t);
    if(pos == NULL) return NULL;
    return pos -> val;
}
//find the address of the pre element
void* PreMem(const void* const x, SplayTree* const t) {
    _Node* pos = preMem(x, t);
    if(pos == NULL) return NULL;
    return pos -> val;
}
//find the address of the next element
void* NxtMem(const void* const x, SplayTree* const t) {
    _Node* pos = nxtMem(x, t);
    if(pos == NULL) return NULL;
    return pos -> val;
}
//Delete and free storage
void DeleteTree(_Node* t) {
    if(t == NULL) return;
    if(t -> rs == NULL && t -> ls == NULL) {
        free(t); return;
    }
    if(t -> ls != NULL) DeleteTree(t -> ls);
    if(t -> rs != NULL) DeleteTree(t -> rs);
    free(t);
}
//Delete all elements in the tree
void DeleteAllElements(_Node* t) {
    if(t == NULL) return;
    if(t -> rs == NULL && t -> ls == NULL) {
        free(t -> val);
        free(t);
        return;
    }
    if(t -> ls != NULL) DeleteAllElements(t -> ls);
    if(t -> rs != NULL) DeleteAllElements(t -> rs);
    free(t -> val);
    free(t);
}
