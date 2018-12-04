#include "postgres.h"

#include "commands/vacuum.h"
#include "postmaster/autovacuum.h"
#include "storage/indexfsm.h"
#include "storage/lmgr.h"
#include "storage/predicate.h"

#include "sfx_hdr.h"

////////////////////////////////////////////////////////////////////////////////
//Name:     sfxbulkdelete()
//Returns:  IndexBulkDeleteResult *
//Params:   IndexVacuumInfo * info
//          IndexBulkDeleteResult * stats
//          IndexBulkDeleteCallback callback
//          void * callback_state
IndexBulkDeleteResult *sfxbulkdelete(IndexVacuumInfo *info, IndexBulkDeleteResult *stats,
                                     IndexBulkDeleteCallback callback, void *callback_state) {

}

////////////////////////////////////////////////////////////////////////////////
//Name:     sfxvacuumcleanup()
//Returns:  IndexBulkDeleteResult *
//Params:   IndexVacuumInfo * info
//          IndexBulkDeleteResult * stats
IndexBulkDeleteResult *sfxvacuumcleanup (IndexVacuumInfo *info, IndexBulkDeleteResult *stats) {

}
