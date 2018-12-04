extern IndexScanDesc sfxbeginscan (Relation rel,
                                   int nkeys,
                                   int norderbys);

extern void sfxendscan            (IndexScanDesc scan);

extern void sfxrescan             (IndexScanDesc scan,
                                   ScanKey scankey,
                                   int nscankeys,
		                               ScanKey orderbys,
                                   int norderbys);
