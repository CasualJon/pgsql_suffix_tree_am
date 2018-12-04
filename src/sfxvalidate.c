#include "postgres.h"

#include "access/amvalidate.h"
#include "access/htup_details.h"
#include "catalog/pg_amop.h"
#include "catalog/pg_amproc.h"
#include "catalog/pg_opclass.h"
#include "catalog/pg_opfamily.h"
#include "catalog/pg_type.h"
#include "utils/builtins.h"
#include "utils/catcache.h"
#include "utils/regproc.h"
#include "utils/syscache.h"

#include "sfx_hdr.h"

////////////////////////////////////////////////////////////////////////////////
//Name:     sfxvalidate()
//Returns:  bool
//Params:   Oid opclassoid
bool sfxvalidate(Oid opclassoid) {

}
