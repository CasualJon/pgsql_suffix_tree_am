#include "tst_hdr.h"

//Functions to manage the linked list of suffixes (struct sfxNodeTups)

////////////////////////////////////////////////////////////////////////////////
//Name:		canAddTups()
//Returns: 	int
//Params: 	Tuples * node
int canAddTups (Tuples *node) {
	if (node->tupleCount < MAX_VAL - 1) return 1;
	else return 0;
} //END canAddTups()

////////////////////////////////////////////////////////////////////////////////
//Name:		pushTuple()
//Returns: 	void
//Params: 	Tuples * add_to_node
//Params:   ItemPointerData * data
void pushTuple(Tuples *add_to_node, ItemPointerData *data) {
	add_to_node->items[add_to_node->tupleCount] = data;
	add_to_node->tupleCount++;
} //END pushTuple()

////////////////////////////////////////////////////////////////////////////////
//Name:		addTupsLink()
//Returns: 	void
//Params: 	Tuples * extend_from_node
//Params:   ItemPointerData * data
//Assumes:  canAddTups() has been called on head node before being invoked
void addTupsLink(Tuples *extend_from_node, ItemPointerData *data) {
    Tuples *linkedNode = extend_from_node->nextTups;
    Tuples *prevNode = extend_from_node;
    while (linkedNode != NULL) {
        prevNode = linkedNode;
        linkedNode = linkedNode->nextTups;
    }
    
    if (canAddTups(prevNode)) {
        pushTuple(prevNode, data);
    }
    else {
        Tuples *newNode = (Tuples *) malloc(sizeof(Tuples));
        newNode->tupleCount = 1;
        int i;
        for (i = 0; i < MAX_VAL; i++) {
            newNode->items[i] = NULL;
        }
        newNode->items[0] = data;
        newNode->nextTups = NULL;
        prevNode->nextTups = newNode;
    }
} //END addTupsLink()

////////////////////////////////////////////////////////////////////////////////
//Name:		sfxChildNodeExists()
//Returns: 	int
//Params: 	Node * node
//Params:   char * c
//Assumes:  canAddTups() has been called on head node before being invoked
int sfxChildNodeExists(Node *node, char c) {
	int indx = (int)c;
    //-= 96 because of reduced array size for space concern in testing
	if (indx > 0) indx -= 96;
	if (node->children[indx] == NULL) return 0;
	else return 1;
} //END sfxChildNodeExists()
