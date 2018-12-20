#include "tst_hdr.h"

//In-file declaration
ItemPointerData *constructRelationTuple(int n, char *str);

int main(int argc, char *argv[]) {
    printf("\n\n/////////////////////////////////////////////////////////////");
    printf("Execution beginning. [tst_main.c]\n");
    int fileLoop = 0;
    if (argc > 1) {
        //Uses cat to pipe the file's data in rather than handling files
        fileLoop = atoi(argv[1]);
        printf("Input file submitted for %d entries.\n", fileLoop);
    }

    //Build the empty suffix trie
    sfxbuildempty();

    // char compare[60];
    // strcpy (compare, "##EOF##");
    ItemPointerData *ipd;
    int success = 0;
    int cnt;
    for (cnt = 0; cnt < fileLoop; cnt++) {
        char s[60];
        char grouper[10];
        char value[35];
        int num = -1;
        fgets(s, sizeof(s), stdin);
        sscanf(s, "%d\t%s\t%s", &num, grouper, value);
        // printf("id = %d\n", num);
        // printf("grouper = %s\n", grouper);
        // printf("value = %s\n", value);
        ipd = constructRelationTuple(num, value);
        success += sfxinsert(ipd);
        if (cnt % 5000 == 0) printf("Status: %d\n", cnt);
    }
    printf("Status: %d\nTREE BUILD COMPLETE\n", fileLoop);

    //Spreadsheet test cases
    //sfxscanInterface(input, descent/prefix)
    sfxscanInterface("%n", 0);
    sfxscanInterface("%n", 0);
    sfxscanInterface("%n", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("%o", 0);
    sfxscanInterface("%o", 0);
    sfxscanInterface("%o", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("%e", 0);
    sfxscanInterface("%e", 0);
    sfxscanInterface("%e", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("%one", 0);
    sfxscanInterface("%one", 0);
    sfxscanInterface("%one", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("%three", 0);
    sfxscanInterface("%three", 0);
    sfxscanInterface("%three", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("%four", 0);
    sfxscanInterface("%four", 0);
    sfxscanInterface("%four", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("%one", 0);
    sfxscanInterface("%one", 0);
    sfxscanInterface("%one", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("%oneone", 0);
    sfxscanInterface("%oneone", 0);
    sfxscanInterface("%oneone", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("%oneoneone", 0);
    sfxscanInterface("%oneoneone", 0);
    sfxscanInterface("%oneoneone", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("f%", 0);
    sfxscanInterface("f%", 0);
    sfxscanInterface("f%", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("o%", 0);
    sfxscanInterface("o%", 0);
    sfxscanInterface("o%", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("oneone%", 0);
    sfxscanInterface("oneone%", 0);
    sfxscanInterface("oneone%", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("oneoneone%", 0);
    sfxscanInterface("oneoneone%", 0);
    sfxscanInterface("oneoneone%", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("%one%", 0);
    sfxscanInterface("%one%", 0);
    sfxscanInterface("%one%", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("%oneone%", 0);
    sfxscanInterface("%oneone%", 0);
    sfxscanInterface("%oneone%", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("%oneoneone%", 0);
    sfxscanInterface("%oneoneone%", 0);
    sfxscanInterface("%oneoneone%", 0);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("one", 0);
    sfxscanInterface("one", 1);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("oneone", 0);
    sfxscanInterface("oneone", 1);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface("oneoneone", 0);
    sfxscanInterface("oneoneone", 1);
    printf("---------------------------------------------------------------\n");

    sfxscanInterface(NULL, 0);
    sfxscanInterface("\0", 1);

    printf("///////////////////////////////////////////////////////////////\n");

    printf("\n\nExecution complete. [tst_main.c]\n\n");
    return 0;
}

//Helper function returning ItemPointerData struct (containing a Relation_Tuple)
ItemPointerData *constructRelationTuple(int n, char *str) {
    int m;
    if (str != NULL) m = strlen(str) + 1;
    else m = sizeof(NULL);

    Relation_Tuple *rt = (Relation_Tuple *) malloc(sizeof(Relation_Tuple));
    rt->value = (char *) malloc(m);
    rt->id = n;

    if (str != NULL) strcpy(rt->value, str);
    else rt->value = NULL;

    ItemPointerData *ipd = (ItemPointerData *) malloc(sizeof(int));
    ipd->addr = rt;
    if (rt->value != NULL) ipd->tot_length = strlen(rt->value);
    else ipd->tot_length = -1;

    return ipd;
}
