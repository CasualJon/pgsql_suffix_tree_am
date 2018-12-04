#include "postgres.h"
////////////////////////////////////////////////////////////////////////////////
#include "access/htup_details.h"
#include "access/reloptions.h"
#include "catalog/pg_collation.h"
#include "catalog/pg_opclass.h"
#include "catalog/pg_type.h"
#include "miscadmin.h"
#include "storage/indexfsm.h"
#include "storage/lmgr.h"
#include "utils/builtins.h"
#include "utils/guc.h"
#include "utils/index_selfuncs.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"
#include "utils/typcache.h"

#include "sfx_hdr.h"

PG_MODULE_MAGIC;

/*
 * SFX Tree handler function: return IndexAmRoutine with access method parameters
 * and callbacks.
 */
Datum sfxhandler(PG_FUNCTION_ARGS) {
	IndexAmRoutine *amroutine = makeNode(IndexAmRoutine);

  //Total # of strategies (operators) by which we can traverse/search
  //this AM.  Zero indicates there is NOT a fixed set of strategy assignments.
	amroutine->amstrategies = 0;
  //number of support functions that this AM uses
	amroutine->amsupport = 10;
  //support ORDER BY indexed column's value?
	amroutine->amcanorder = false;
  //support ORDER BY result of an operator on indexed column?
	amroutine->amcanorderbyop = false;
  //support backward scanning?
	amroutine->amcanbackward = false;
  //support UNIQUE indexes?
	amroutine->amcanunique = false;
  //support multi-column indexes?
	amroutine->amcanmulticol = false;
  //require scans to have a constraint on the first index column?
	amroutine->amoptionalkey = true;
  //handle ScalarArrayOpExpr quals?
	amroutine->amsearcharray = false;
  //handle IS NULL/IS NOT NULL quals?
	amroutine->amsearchnulls = true;
  //can index storage data type differ from column data type?
	amroutine->amstorage = true;
  //can an index of this type be clustered on?
	amroutine->amclusterable = false;
  //handle predicate locks?
	amroutine->ampredlocks = false;
  //support parallel scan?
	amroutine->amcanparallel = false;
  //support columns included with clause INCLUDE?
  amroutine->amcaninclude = false;
  //type of data stored in index, or InvalidOid if variable
	amroutine->amkeytype = InvalidOid;

  //INTERFACE FUNCTIONS
	amroutine->ambuild = sfxbuild;
	amroutine->ambuildempty = sfxbuildempty;
	amroutine->aminsert = sfxinsert;
	amroutine->ambulkdelete = sfxbulkdelete;
	amroutine->amvacuumcleanup = sfxvacuumcleanup;
	amroutine->amcanreturn = NULL;
	amroutine->amcostestimate = gincostestimate;
	amroutine->amoptions = sfxoptions;
	amroutine->amproperty = NULL;
	amroutine->amvalidate = sfxvalidate;
	amroutine->ambeginscan = sfxbeginscan;
	amroutine->amrescan = sfxrescan;
	amroutine->amgettuple = NULL;
	amroutine->amgetbitmap = NULL;
	amroutine->amendscan = sfxendscan;
	amroutine->ammarkpos = NULL;
	amroutine->amrestrpos = NULL;

  //INTERFACE FUNCTIONS: Prallel index scan
	amroutine->amestimateparallelscan = NULL;
	amroutine->aminitparallelscan = NULL;
	amroutine->amparallelrescan = NULL;

	PG_RETURN_POINTER(amroutine);
}

//Functions to manage the linked list of sfxNodeTups
int canAddTups(Tuples *node) {
    if (node->tupleCount < MAX_VAL - 1) return 1;
    else return 0;
}
void pushTuple(Tuples *add_to_node, ItemPointerData *data) {
    add_to_node->items[add_to_node->tupleCount] = data;
    add_to_node->tupleCount++;
}
void addTupsLink(Tuples *extend_from_node, ItemPointerData *data) {
    Tuples *newSfxNodeTups = (Tuples *) palloc(sizeof(Tuples));
    newSfxNodeTups->tupleCount = 1;
    newSfxNodeTups->items[0] = data;
    newSfxNodeTups->nextTups = NULL;
    extend_from_node->nextTups = newSfxNodeTups;
}
