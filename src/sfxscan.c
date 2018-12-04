#include "postgres.h"

#include "access/relscan.h"
#include "pgstat.h"
#include "utils/memutils.h"

#include "sfx_hdr.h"

////////////////////////////////////////////////////////////////////////////////
//Name:     sfxbeginscan()
//Returns:  IndexScanDesc
//Params:   Relation rel
//          int nkeys
//          int norderbys
IndexScanDesc sfxbeginscan (Relation rel, int nkeys, int norderbys) {

}

////////////////////////////////////////////////////////////////////////////////
//Name:     sfxendscan()
//Returns:  void
//Params:   IndexScanDesc scan
void sfxendscan(IndexScanDesc scan) {

}

////////////////////////////////////////////////////////////////////////////////
//Name:     sfxrescan()
//Returns:  void
//Params:   IndexScanDesc scan
//          ScanKey scankey
//          int nscankeys
//          ScanKey orderbys
//          int norderbys
void sfxrescan(IndexScanDesc scan, ScanKey scankey, int nscankeys,
               ScanKey orderbys, int norderbys) {

}
