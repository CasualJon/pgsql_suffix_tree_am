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
