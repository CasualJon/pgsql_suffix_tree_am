#include "tst_hdr.h"

//In-file declarations
void buildNodesAllASCIIChildren(Node *node);
void buildUniqueChild(Node *node, int pos);
void pushStringToTree(char* str, int off, Node *node, ItemPointerData *ipd);

////////////////////////////////////////////////////////////////////////////////
//Name:		sfxbuildempty()
//Returns: 	void
//Params: 	none
void sfxbuildempty() {
    //Build the empty tuple list for the root tree node
    Tuples *tuples = (Tuples *) malloc(sizeof(Tuples));
    tuples->tupleCount = 0;
    //Tuples->items sized via array definition
    int j;
    for (j = 0; j < MAX_VAL; j++) {
        tuples->items[j] = NULL;
    }
    tuples->nextTups = NULL;

    //Build the root tree node
    sfx_root = (Node *) malloc(sizeof(Node));
    sfx_root->val = (char)0;
    //Node->children sized via array definition
    sfx_root->sfxTups = tuples;
    sfx_root->pfxTups = NULL;

    //Add child nodes to root
    buildNodesAllASCIIChildren(sfx_root);
} //END sfxbuildempty()

////////////////////////////////////////////////////////////////////////////////
//Name:		buildNodesAllASCIIChildren()
//Returns: 	void
//Params: 	Node * node
//Assumes:  Only called by sfxbuildempty()
void buildNodesAllASCIIChildren(Node *node) {
    int i;
    for (i = 0; i < MAX_CHAR; i++) {
        //Build the empty suffix tuple list for the given node
        Tuples *sfxs = (Tuples *) malloc(sizeof(Tuples));
        sfxs->tupleCount = 0;
        int j;
        for (j = 0; j < MAX_VAL; j++) {
            sfxs->items[j] = NULL;
        }
        sfxs->nextTups = NULL;
        //Build the empty prefix tuple list for the given node
        Tuples *pfxs = (Tuples *) malloc(sizeof(Tuples));
        pfxs->tupleCount = 0;
        for (j = 0; j < MAX_VAL; j++) {
            pfxs->items[j] = NULL;
        }
        pfxs->nextTups = NULL;

        //Build the empty children list for this node
        Node *child = (Node *) malloc(sizeof(Node));
        child->val = (char)i;
        for (j = 0; j < MAX_CHAR; j++) {
            child->children[j] = NULL;
        }
        child->sfxTups = sfxs;
        child->pfxTups = pfxs;

        //Plug this node in as the apporopriate child of sfx_root
        node->children[i] = child;
    }
} //END buildNodesAllASCIIChildren()

////////////////////////////////////////////////////////////////////////////////
//Name:		buildUniqueChild()
//Returns: 	void
//Params: 	Node * node
//Params:   int pos
void buildUniqueChild(Node *node, int pos) {
    //Build the empty suffix tuple list for the given node
    Tuples *sfxs = (Tuples *) malloc(sizeof(Tuples));
    sfxs->tupleCount = 0;
    int j;
    for (j = 0; j < MAX_VAL; j++) {
        sfxs->items[j] = NULL;
    }
    //Tuples->items sized via array definition
    sfxs->nextTups = NULL;
    //Build the empty prefix tuple list for the given node
    Tuples *pfxs = (Tuples *) malloc(sizeof(Tuples));
    pfxs->tupleCount = 0;
    for (j = 0; j < MAX_VAL; j++) {
        pfxs->items[j] = NULL;
    }
    pfxs->nextTups = NULL;

    //Build the node for this child
    Node *child = (Node *) malloc(sizeof(Node));
    child->val = (char)pos;
    for (j = 0; j < MAX_CHAR; j++) {
        child->children[j] = NULL;
    }
    child->sfxTups = sfxs;
    child->pfxTups = pfxs;
    node->children[pos] = child;
} //END buildUniqueChild()

////////////////////////////////////////////////////////////////////////////////
//Name:		sfxinsert()
//Returns: 	bool
//Params: 	ItemPointerData * ipd
int sfxinsert(ItemPointerData *ipd) {
    Relation_Tuple *rt = ipd->addr;
    char *str = rt->value;

    if (str == NULL) {
        //Add to tracker for null
        if (canAddTups(sfx_root->sfxTups)) pushTuple(sfx_root->sfxTups, ipd);
        else {
            Tuples *tups = sfx_root->sfxTups;
            while (!canAddTups(tups)) {
                tups = tups->nextTups;
            }
            addTupsLink(tups, ipd);
        }
    }
    else if (str[0] == STR_TERM) {
        //Add to empty string node
        Node *emptyChild = sfx_root->children[0];
        if (canAddTups(emptyChild->sfxTups)) pushTuple(emptyChild->sfxTups, ipd);
        else {
            Tuples *tups = emptyChild->sfxTups;
            while (!canAddTups(tups)) {
                tups = tups->nextTups;
            }
            addTupsLink(tups, ipd);
        }
    }
    else {
        pushStringToTree(str, strlen(str), sfx_root, ipd);
    }

    return 1;
} //END sfxinsert()

////////////////////////////////////////////////////////////////////////////////
//Name:		pushStringToTree()
//Returns: 	bool
//Params: 	ItemPointerData * ipd
void pushStringToTree(char* str, int off, Node *node, ItemPointerData *ipd) {
    off--;
    if (off >= 0) {
    //Add suffix extending to tree via for loop
    int j;
    Node *currNode = node;
    for (j = off; j < strlen(str); j++) {
        char c = str[j];
        int indx = (int) c;
        //-= 96 because of reduced array size for space concern in testing
        if (indx > 0) indx -= 96;
        if (currNode->children[indx] == NULL) buildUniqueChild(currNode, indx);
        currNode = currNode->children[indx];

        //If this is a full string entry (off == 0), add as a prefix down path
        if (off == 0) {
            if (canAddTups(currNode->pfxTups)) pushTuple(currNode->pfxTups, ipd);
            else {
                Tuples *prefixes = currNode->pfxTups;
                while (!canAddTups(prefixes)) {
                    if (prefixes->nextTups == NULL) {
                        prefixes->nextTups = (Tuples *) malloc(sizeof(Tuples));
                        prefixes = prefixes->nextTups;
                        prefixes->tupleCount = 0;
                        int p;
                        for (p = 0; p < MAX_VAL; p++) {
                            prefixes->items[p] = NULL;
                        }
                        prefixes->nextTups = NULL;
                    }
                    else prefixes = prefixes->nextTups;
                }
                addTupsLink(prefixes, ipd);
            }
        }
    }

    //Now down to the bottom of this suffix, push into terminus
    if (currNode->children[0] == NULL) buildUniqueChild(currNode, 0);
    currNode = currNode->children[0];
    if (canAddTups(currNode->sfxTups)) pushTuple(currNode->sfxTups, ipd);
    else {
        Tuples *tups = currNode->sfxTups;
        while (!canAddTups(tups)) {
            if (tups->nextTups == NULL) {
                tups->nextTups = (Tuples *) malloc(sizeof(Tuples));
                tups = tups->nextTups;
                tups->tupleCount = 0;
                for (j = 0; j < MAX_VAL; j++) {
                    tups->items[j] = NULL;
                }
                tups->nextTups = NULL;
            }
            else tups = tups->nextTups;
        }
        addTupsLink(tups, ipd);
    }

    //Recursive call to this function
    pushStringToTree(str, off, node, ipd);
    }
} //END pushStringToTree()
