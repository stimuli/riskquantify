#ifndef rqui_dialog_h
#define rqui_dialog_h

#include <rq_defs.h>

RQ_DECLARE_OBJECT(rqui_dialog_t);

rqui_dialog_t rqui_dialog_alloc(const char *title, const char *message);

void rqui_dialog_show_modal(rqui_dialog_t dlg);

void rqui_dialog_free(rqui_dialog_t dlg);

#endif
