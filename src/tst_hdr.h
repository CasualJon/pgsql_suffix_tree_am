////////////////////////////////////////////////////////////////////////////////
//Prefixed-Suffix Trie Implementation - Header File
//James Goodman, Jon Cyrus
//UW-Madison, CS764, Fall 2018
////////////////////////////////////////////////////////////////////////////////

#ifndef __SFX_HDR__
#define __SFX_HDR__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//Define magic numbers
#define MAX_CHAR 27    //ASCII NUL (0) through DEL (127)
//UPDATE - memory considerations in our test environment require minimizing 
//unused space to conserve virtual memory

#define MAX_VAL 100     //256 is reasonable, but larger arrays will improve perf
//UPDATE - memory considerations in our test envionment require minimizing
//unused space to conserve virtual memory

#define STR_TERM '\0'   //Simplified handling of null terminator in c strings

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//Declare structs for node details
typedef struct Relation_Tuple {
    int id;
    char *value;
} Relation_Tuple;

typedef struct ItemPointerData {
    Relation_Tuple *addr;
    int tot_length;
} ItemPointerData;

typedef struct sfxNodeTups {
    int tupleCount;
    struct ItemPointerData *items[MAX_VAL];
    struct sfxNodeTups *nextTups;
} Tuples;

typedef struct sfxTreeNode {
    char val;
    struct sfxTreeNode *children[MAX_CHAR];
    struct sfxNodeTups *sfxTups;
    struct sfxNodeTups *pfxTups;
} Node;

//Tree root declaration
Node *sfx_root;

//Functions to manage the linked list of sfxNodeTups
extern int canAddTups           (Tuples *node);

extern void pushTuple           (Tuples *add_to_node,
                                 ItemPointerData *data);

extern void addTupsLink         (Tuples *extend_from_node,
                                 ItemPointerData *data);

extern int sfxChildNodeExists   (Node *node,
                                 char c);

extern void sfxbuildempty       ();

extern int sfxinsert            (ItemPointerData *ipd);

extern void sfxscanInterface    (char *input,
                                 int prefix);

//__SFX_HDR__
#endif
