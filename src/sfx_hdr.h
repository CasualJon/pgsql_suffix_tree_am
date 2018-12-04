////////////////////////////////////////////////////////////////////////////////
//Suffix Tree Implementation Header File
//James Goodman, Jon Cyrus
//UW-Madison, CS764, Fall 2018
////////////////////////////////////////////////////////////////////////////////

#ifndef __SFX_HDR__
#define __SFX_HDR__

#include "access/amapi.h"
#include "access/generic_xlog.h"
#include "access/gin.h"
#include "access/itup.h"
#include "access/sdir.h"
#include "lib/rbtree.h"
#include "storage/bufmgr.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//Hold to include any additional headers required

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//Hold to define magic numbers
#define MAX_CHAR 128 // was 256
#define MAX_VAL 256

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//Hold to define structs for node details
typedef struct sfxNodeTups {
    int tupleCount;
    struct ItemPointerData *items[MAX_VAL]
    struct sfxTreeTups *nextTups
} Tuples;
typedef struct sfxTreeNode {
    char val;
    struct sfxTreeNode *children[MAX_CHAR];
    struct sfxNodeTups sfxTups;
} Node;

//Functions to manage the linked list of sfxNodeTups
int canAddTups    (Tuples *node);

void pushTuple    (Tuples *add_to_node,
                   ItemPointerData *data);

void addTupsLink  (Tuples *extend_from_node,
                   ItemPointerData *data);

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//Function Declarations
//File: sfxscan.c
extern IndexScanDesc sfxbeginscan (Relation rel,
                                   int nkeys,
                                   int norderbys);

extern void sfxendscan            (IndexScanDesc scan);

extern void sfxrescan             (IndexScanDesc scan,
                                   ScanKey scankey,
                                   int nscankeys,
		                               ScanKey orderbys,
                                   int norderbys);

//File: sfxinsert.c
extern IndexBuildResult *sfxbuild (Relation heap,
                                   Relation index,
                                   struct IndexInfo *indexInfo);

extern void sfxbuildempty         (Relation index);

extern bool sfxinsert             (Relation index,
                                   Datum *values,
                                   bool *isnull,
                                   ItemPointer ht_ctid,
                                   Relation heapRel,
                                   IndexUniqueCheck checkUnique,
                                   struct IndexInfo *indexInfo);

//File: sfxvacuum.c
extern IndexBulkDeleteResult *sfxbulkdelete     (IndexVacuumInfo *info,
			                                           IndexBulkDeleteResult *stats,
                                                 IndexBulkDeleteCallback callback,
                                                 void *callback_state);

extern IndexBulkDeleteResult *sfxvacuumcleanup  (IndexVacuumInfo *info,
                                                 IndexBulkDeleteResult *stats);

//File: sfxvalidate.c
extern bool sfxvalidate(Oid opclassoid);

//File: rumutilities0.c
extern bytea *sfxoptions  (Datum reloptions,
                           bool validate);

extern Datum sfxhandler    (PG_FUNCTION_ARGS);



//__SFX_HDR__
#endif
