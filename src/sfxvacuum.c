extern IndexBulkDeleteResult *sfxbulkdelete     (IndexVacuumInfo *info,
			                                           IndexBulkDeleteResult *stats,
                                                 IndexBulkDeleteCallback callback,
                                                 void *callback_state);

extern IndexBulkDeleteResult *sfxvacuumcleanup  (IndexVacuumInfo *info,
                                                 IndexBulkDeleteResult *stats);
