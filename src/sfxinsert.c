#include "postgres.h"

#include "access/generic_xlog.h"
#include "storage/predicate.h"
#include "catalog/index.h"
#include "miscadmin.h"
#include "utils/memutils.h"
#include "utils/datum.h"

#include "sfx_hdr.h"

////////////////////////////////////////////////////////////////////////////////
//Name:     sfxbuild()
//Returns:  IndexBuildResult *
//Params:   Relation heap
//          Relation index
//          Struct IndexInfo * indexInfo
IndexBuildResult *sfxbuild(Relation heap, Relation index, struct IndexInfo *indexInfo) {

}

////////////////////////////////////////////////////////////////////////////////
//Name:     sfxbuildempty()
//Returns:  void
//Params:   Relation index
void sfxbuildempty(Relation index) {

}

////////////////////////////////////////////////////////////////////////////////
//Name:     sfxinsert()
//Returns:  bool
//Params:   Relation index
//          Datum * values
//          bool * isnull
//          ItemPointer ht_ctid
//          Relation heapRel
//          IndexUniqueCheck checkUnique
//          struct IndexInfo * indexInfo
bool sfxinsert(Relation index, Datum *values, bool *isnull, ItemPointer ht_ctid,
               Relation heapRel, IndexUniqueCheck checkUnique, struct IndexInfo *indexInfo) {

}
