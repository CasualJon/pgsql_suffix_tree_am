#include "tst_hdr.h"

//Struct declarations
typedef struct Matching_Tuple_Nodes {
    Tuples * matches;
    struct Matching_Tuple_Nodes *nextMatches;
} Matches;
typedef struct Node_To_Inspect {
    Node *toInspect;
    struct Node_To_Inspect *nextInspect;
} Inspect;
//SeenTups might be better as a tree, but non-random entry skews here
typedef struct Seen_Tuples {
    int tid;
    struct Seen_Tuples *right;
} SeenTups;

//Function declarations
void addAllTuplesSubRooted(Node *currNode, Inspect *review, Matches *result);
int hasUnderscore(char *input);
int checkSeen(Tuples *t, SeenTups *head, int seek);

////////////////////////////////////////////////////////////////////////////////
//Name:		sfxscanExactDescent()
//Returns: 	Matches *
//Params: 	char * input
Matches *sfxscanExactDescent(char *input) {
    Matches *result = (Matches *) malloc(sizeof(Matches));
    result->matches = NULL;
    result->nextMatches = NULL;
    if (input == NULL) {
        result->matches = sfx_root->sfxTups;
        return result;
    }
    else if (input[0] == STR_TERM) {
        result->matches = sfx_root->children[0]->sfxTups;
        return result;
    }

    int found = 1;
    int len = strlen(input);
    Node *currNode = sfx_root;
    int i;
    //Chase the string down the tree
    for (i = 0; i < len; i++) {
        int indx = (int)input[i];
        //-= 96 because of reduced array size for space concern in testing
        if (indx > 0) indx -= 96;
        if (currNode->children[indx] != NULL) {
            currNode = currNode->children[indx];
        }
        else {
            //If a child node in the array is NULL, break out
            found = 0;
            break;
        }
    }
    //If found, we've reached last input char, advance to it's string term child
    if (found && currNode->children[0] != NULL) {
        currNode = currNode->children[0];
    }
    else {
        //If the terminator is NULL, no string has ended at this node
        found = 0;
    }

    if (found) {
        result->matches = currNode->sfxTups;
        return result;
    }
    else {
        free(result);
        return (Matches *) NULL;
    }
} //END sfxscanExactDescent()

////////////////////////////////////////////////////////////////////////////////
//Name:		sfxscanExactPrefix()
//Returns: 	Matches *
//Params: 	char * input
Matches *sfxscanExactPrefix(char *input) {
    Matches *result = (Matches *) malloc(sizeof(Matches));
    result->matches = NULL;
    result->nextMatches = NULL;
    if (input == NULL) {
        result->matches = sfx_root->sfxTups;
        return result;
    }
    else if (input[0] == STR_TERM) {
        result->matches = sfx_root->children[0]->sfxTups;
        return result;
    }

    int found = 1;
    int len = strlen(input);
    Node *currNode = sfx_root;
    int i;
    //Chase the string down the tree
    for (i = 0; i < len; i++) {
        int indx = (int)input[i];
        //-= 96 because of reduced array size for space concern in testing
        if (indx > 0) indx -= 96;
        if (currNode->children[indx] != NULL) {
            currNode = currNode->children[indx];
        }
        else {
            //If a child node in the array is NULL, break out
            found = 0;
            break;
        }
    }

    if (found && currNode->pfxTups == NULL) {
        found = 0;
    }

    if (found) {
        result->matches = currNode->pfxTups;
        return result;
    }
    else {
        free(result);
        return (Matches *) NULL;
    }
} //END sfxscanExactPrefix()

////////////////////////////////////////////////////////////////////////////////
//Name:		sfxscanLike()
//Returns: 	Matches *
//Params: 	char * input
Matches *sfxscanLike(char *input) {
    Matches *result = (Matches *) malloc(sizeof(Matches));
    result->matches = NULL;
    result->nextMatches = NULL;
    if (input == NULL) {
        result->matches = sfx_root->sfxTups;
        return result;
    }
    else if (input[0] == STR_TERM) {
        result->matches = sfx_root->children[0]->sfxTups;
        return result;
    }

    int len = strlen(input);
    Node *currNode = sfx_root;
    Inspect *review = NULL;
    int i;
    for (i = 0; i < len; i++) {
        if (input[i] != '%') {
            int indx = (int)input[i];
            //-= 96 because of reduced array size for space concern in testing
            if (indx > 0) indx -= 96;
            currNode = currNode->children[indx];
            if (currNode == NULL) {
                free(result);
                return (Matches *) NULL;
            }
        }

        //If last char and a "%" wildcard
        if (i == (len - 1)) {
            if (input[i] == '%') {
                addAllTuplesSubRooted(currNode, review, result);
            }
            else if (currNode->children[0] != NULL) {
                currNode = currNode->children[0];
                result->matches = currNode->sfxTups;
            }
        }
    }
    if (result->matches != NULL) return result;
    else {
        free(result);
        return (Matches *) NULL;
    }
} //END sfxscanLike()

////////////////////////////////////////////////////////////////////////////////
//Name:     sfxscanDoubleWC()
//Returns: 	void
//Params: 	char * input
void sfxscanDoubleWC(char *input) {
    printf("************************************************************\n");
    printf("Tuples (prefix-WILDCARD) Matching Search Input \"%%%s\":\n", input);
    int total = 0;
    clock_t startTime = clock();
    Matches *result = sfxscanLike(input);
    if (result == NULL) {
        clock_t endTime = clock();
        printf("# Results Found: %d\n", total);
        printf("TOTAL TIME: %f\n", ((double) (endTime - startTime)) / CLOCKS_PER_SEC);
    }
    else {
        Matches *m = result;
        Tuples *t = m->matches;
        SeenTups *head = NULL;
        total += t->tupleCount;
        int i;
        for (i = 0; i < t->tupleCount; i++) {
            //Definiton here to save the eval every other loop has to checkSeen
            if (i == 0) {
                head = (SeenTups *) malloc(sizeof(SeenTups));
                head->tid = t->items[i]->addr->id;
                head->right = NULL;
            }
            else if (checkSeen(t, head, t->items[i]->addr->id) == 1) {
                total--;
            }
//UNCOMMENT BELOW FOR SCREEN OUTPUT
            // else printf("Tuple ID|VALUE|addr: %d|%s|%p\n", t->items[i]->addr->id, t->items[i]->addr->value, t->items[i]->addr);
        }
        while (t->nextTups != NULL) {
            t = t->nextTups;
            total += t->tupleCount;
            for (i = 0; i < t->tupleCount; i++) {
                if (checkSeen(t, head, t->items[i]->addr->id) == 1) {
                    total--;
                }
//UNCOMMENT BELOW FOR SCREEN OUTPUT
                // else printf("Tuple ID|VALUE|addr: %d|%s|%p\n", t->items[i]->addr->id, t->items[i]->addr->value, t->items[i]->addr);
            }
        }
        while (m->nextMatches != NULL) {
            Matches *old = m;
            m = m->nextMatches;
            free(old);
            t = m->matches;
            total += t->tupleCount;
            for (i = 0; i < t->tupleCount; i++) {
                if (checkSeen(t, head, t->items[i]->addr->id) == 1) {
                    total--;
                }
//UNCOMMENT BELOW FOR SCREEN OUTPUT
                // else printf("Tuple ID|VALUE|addr: %d|%s|%p\n", t->items[i]->addr->id, t->items[i]->addr->value, t->items[i]->addr);
            }
            while (t->nextTups != NULL) {
                t = t->nextTups;
                total += t->tupleCount;
                for (i = 0; i < t->tupleCount; i++) {
                    if (checkSeen(t, head, t->items[i]->addr->id) == 1) {
                        total--;
                    }
//UNCOMMENT BELOW FOR SCREEN OUTPUT
                // else printf("Tuple ID|VALUE|addr: %d|%s|%p\n", t->items[i]->addr->id, t->items[i]->addr->value, t->items[i]->addr);
                }
            }
        }
        clock_t endTime = clock();
        printf("# Results Found: %d\n", total);
        printf("TOTAL TIME: %f\n", ((double) (endTime - startTime)) / CLOCKS_PER_SEC);
        if (m != NULL) free(m);

        SeenTups *oldSeen;
        while (head != NULL && head->right != NULL) {
            oldSeen = head;
            head = head->right;
            free(oldSeen);
        }
    }
} //END sfxscanDoubleWC()

////////////////////////////////////////////////////////////////////////////////
//Name:     sfxscanInterfaceLike()
//Returns: 	void
//Params: 	char * input
void sfxscanInterfaceLike(char *input) {
    //Redirect if ending in wildcard
    if (input != NULL && input[strlen(input) - 1] == '%') {
        sfxscanDoubleWC(input);
        return;
    }
    printf("************************************************************\n");
    printf("Tuples (prefix-WILDCARD) Matching Search Input \"%%%s\":\n", input);
    int total = 0;
    clock_t startTime = clock();
    Matches *result = sfxscanLike(input);
    if (result == NULL) {
        clock_t endTime = clock();
        printf("# Results Found: %d\n", total);
        printf("TOTAL TIME: %f\n", ((double) (endTime - startTime)) / CLOCKS_PER_SEC);
    }
    else {
        Matches *m = result;
        Tuples *t = m->matches;
        total += t->tupleCount;
//UNCOMMENT BELOW FOR SCREEN OUTPUT
        // int i;
        // for (i = 0; i < t->tupleCount; i++) {
            // printf("Tuple ID|VALUE|addr: %d|%s|%p\n", t->items[i]->addr->id, t->items[i]->addr->value, t->items[i]->addr);
        // }
        while (t->nextTups != NULL) {
            t = t->nextTups;
            total += t->tupleCount;
//UNCOMMENT BELOW FOR SCREEN OUTPUT
            // for (i = 0; i < t->tupleCount; i++) {
                // printf("Tuple ID|VALUE|addr: %d|%s|%p\n", t->items[i]->addr->id, t->items[i]->addr->value, t->items[i]->addr);
            // }
        }
        while (m->nextMatches != NULL) {
            Matches *old = m;
            m = m->nextMatches;
            free(old);
            t = m->matches;
            total += t->tupleCount;
//UNCOMMENT BELOW FOR SCREEN OUTPUT
            // for (i = 0; i < t->tupleCount; i++) {
                // printf("Tuple ID|VALUE|addr: %d|%s|%p\n", t->items[i]->addr->id, t->items[i]->addr->value, t->items[i]->addr);
            // }
            while (t->nextTups != NULL) {
                t = t->nextTups;
                total += t->tupleCount;
//UNCOMMENT BELOW FOR SCREEN OUTPUT
                // for (i = 0; i < t->tupleCount; i++) {
                    // printf("Tuple ID|VALUE|addr: %d|%s|%p\n", t->items[i]->addr->id, t->items[i]->addr->value, t->items[i]->addr);
                // }
            }
        }
        printf("# Results Found: %d\n", total);
        clock_t endTime = clock();
        printf("TOTAL TIME: %f\n", ((double) (endTime - startTime)) / CLOCKS_PER_SEC);
        if (m != NULL) free(m);
    }
} //END sfxscanInterfaceLike()

////////////////////////////////////////////////////////////////////////////////
//Name:     sfxscanInterfaceExactEndWC()
//Returns: 	void
//Params: 	char * input
//Params:   int prefix
void sfxscanInterfaceExactEndWC(char *input) {
    //This is effectively the same as sfxscanInterface with an exact string
    //match; however, we don't cull the results whose lenght != descent length
    printf("************************************************************\n");
    printf("Tuples (prefix-EXACT) Matching Search Input \"%s%%\":\n", input);
    int total = 0;
    clock_t startTime = clock();

    Matches *result = sfxscanExactPrefix(input);

    if (result == NULL || result->matches->tupleCount == 0) {
        printf("# Results Found: %d\n", total);
        clock_t endTime = clock();
        printf("TOTAL TIME: %f\n", ((double) (endTime - startTime)) / CLOCKS_PER_SEC);
    }
    else {
        Tuples *t = result->matches;
        total += t->tupleCount;
//UNCOMMENT BELOW FOR SCREEN OUTPUT
        // int i;
        // for (i = 0; i < t->tupleCount; i++) {
            // printf("Tuple ID|VALUE|addr: %d|%s|%p\n", t->items[i]->addr->id, t->items[i]->addr->value, t->items[i]->addr);
        // }
        while (t->nextTups != NULL) {
            t = t->nextTups;
            total += t->tupleCount;
//UNCOMMENT BELOW FOR SCREEN OUTPUT
           // for (i = 0; i < t->tupleCount; i++) {
                 // printf("Tuple ID|VALUE|addr: %d|%s|%p\n", t->items[i]->addr->id, t->items[i]->addr->value, t->items[i]->addr);
            // }
        }
        clock_t endTime = clock();
        printf("# Results Found: %d\n", total);
        printf("TOTAL TIME: %f\n", ((double) (endTime - startTime)) / CLOCKS_PER_SEC);
    }
    free(result);
} //END sfxscanInterfaceExactEndWC()

////////////////////////////////////////////////////////////////////////////////
//Name:     sfxscanInterface()
//Returns: 	void
//Params: 	char * input
//Params:   int prefix
void sfxscanInterface(char *input, int prefix) {
    //Reject scans with unsupported _ wildcard
    if (hasUnderscore(input)) {
        printf("************************************************************\n");
        printf("This Access Method Does Not Support Wildcard Type \"_\".\n");
        printf("Input string: %s\n", input);
        return;
    }

    int inputSize = -1;
    if (input != NULL) inputSize = strlen(input);

    //Unimplemented: first check for middle wildcard: xyz%xyz
    if (1 == 0) {
        //Implement stub
        return;
    }
    //If first char is %, redirect
    if (inputSize > -1 && input[0] == '%') {
        char newInput[inputSize];
        memcpy(newInput, &input[1], inputSize - 1);
        newInput[inputSize - 1] = '\0';
        sfxscanInterfaceLike(newInput);
        return;
    }
    //Ending wildcard check
    if (inputSize > -1 && input[inputSize - 1] == '%') {
        char newInput[inputSize];
        memcpy(newInput, &input[0], inputSize - 1);
        newInput[inputSize - 1] = '\0';
        sfxscanInterfaceExactEndWC(newInput);
        return;
    }

    printf("************************************************************\n");
    printf("Tuples (prefix-EXACT) Matching Search Input \"%s\":\n", input);
    int total = 0;
    clock_t startTime = clock();

    //Descent length vs. Prefix
    Matches *result;
    if (prefix == 0) result = sfxscanExactDescent(input);
    else result = sfxscanExactPrefix(input);

    if (result == NULL || result->matches->tupleCount == 0) {
        clock_t endTime = clock();
        printf("# Results Found: %d\n", total);
        printf("TOTAL TIME: %f\n", ((double) (endTime - startTime)) / CLOCKS_PER_SEC);
    }
    else {
        Tuples *t = result->matches;
        total += t->tupleCount;
        int i;
        for (i = 0; i < t->tupleCount; i++) {
            if (t->items[i]->tot_length != inputSize) {
                total--;
                continue;
            }
//UNCOMMENT BELOW FOR SCREEN OUTPUT
            // printf("Tuple ID|VALUE|addr: %d|%s|%p\n", t->items[i]->addr->id, t->items[i]->addr->value, t->items[i]->addr);
        }
        while (t->nextTups != NULL) {
            t = t->nextTups;
            total += t->tupleCount;
            for (i = 0; i < t->tupleCount; i++) {
                if (t->items[i]->tot_length != inputSize) {
                    total--;
                    continue;
                }
//UNCOMMENT BELOW FOR SCREEN OUTPUT
                // printf("Tuple ID|VALUE|addr: %d|%s|%p\n", t->items[i]->addr->id, t->items[i]->addr->value, t->items[i]->addr);
            }
        }
        clock_t endTime = clock();
        printf("# Results Found: %d\n", total);
        printf("TOTAL TIME: %f\n", ((double) (endTime - startTime)) / CLOCKS_PER_SEC);
    }
    free(result);
} //END sfxscanInterface()

////////////////////////////////////////////////////////////////////////////////
//Name:     addAllTuplesSubRooted()
//Returns: 	void
//Params: 	Node * currNode
//Params:   Inspect * review
//Params:   Matches * result
void addAllTuplesSubRooted(Node *currNode, Inspect *review, Matches *result) {
    Matches *r = result;
    //Add the current node to the review list by creating list if it's NULL
    //We ended up under-utilizing this code... Leaving much of it unnecessary
    int addCurrToTail = 1;
    if (review == NULL) {
        review = (Inspect *) malloc(sizeof(Inspect));
        review->toInspect = currNode;
        review->nextInspect = NULL;
        addCurrToTail = 0;
    }
    //Get addr of last review list node to add more as we traverse tree
    Inspect *reviewTail = review;
    while (reviewTail->nextInspect != NULL) {
        reviewTail = reviewTail->nextInspect;
    }
    //If review list wasn't NULL, add currNode to tail
    if (addCurrToTail) {
        reviewTail->nextInspect = (Inspect *) malloc(sizeof(Inspect));
        reviewTail = reviewTail->nextInspect;
        reviewTail->toInspect = currNode;
        reviewTail->nextInspect = NULL;
    }

    //Do while to add Tuples of NULL terminator at this node to result
    //then check children of node to see if populated (and add to review list).
    //Advance through review list until done
    do {
        //If this review node's tuple list > 0, add it to r
        if (review->toInspect->sfxTups->tupleCount > 0) {
            if (r->matches == NULL) r->matches = review->toInspect->sfxTups;
            else {
                r->nextMatches = (Matches *) malloc(sizeof(Matches));
                r = r->nextMatches;
                r->matches = review->toInspect->sfxTups;
                r->nextMatches = NULL;
            }
        }
        int i;
        for (i = 0; i < MAX_CHAR; i++) {
            //Add any valid children of the current inspection node to review list
            if (review->toInspect->children[i] != NULL) {
                reviewTail->nextInspect = (Inspect *) malloc(sizeof(Inspect));
                reviewTail = reviewTail->nextInspect;
                reviewTail->toInspect = review->toInspect->children[i];
                reviewTail->nextInspect = NULL;
            }
        }

        Inspect *oldReview = review;
        review = review->nextInspect;
        free(oldReview);
    } while (review != NULL);
} //END addAllTuplesSubRooted()

////////////////////////////////////////////////////////////////////////////////
//Name:     hasUnderscore()
//Returns: 	int
//Params: 	char * input
int hasUnderscore(char *input) {
    int out = 0;
    if (input == NULL) return out;
    int i;
    for (i = 0; i < strlen(input); i++) {
        if (input[i] == '_') {
            out = 1;
            break;
        }
    }

    return out;
} //END hasUnderscore()

////////////////////////////////////////////////////////////////////////////////
//Name:     checkSeen()
//Returns: 	int
//Params: 	Tuple * t
//Params:   SeenTups * head
//Params:   int seek
int checkSeen(Tuples *t, SeenTups *head, int seek) {
    int found = 0;
    SeenTups *s = head;

    if (s->tid == seek) return 1;
    while (s->right != NULL) {
        s = s->right;
        if (s->tid == seek) {
            found = 1;
            break;
        }
    }

    if (!found) {
        s->right = (SeenTups *) malloc(sizeof(SeenTups));
        s = s->right;
        s->tid = seek;
        s->right = NULL;
    }

    return found;
} //END checkSeen()
