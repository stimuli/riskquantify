if enable_gtk_apps
TARGET = riskquantify
else
TARGET =
endif

bin_PROGRAMS = $(TARGET)

riskquantify_SOURCES = \
	rqui_dialog.c \
	rqui_dialog.h \
	rqui_dlg_about.c \
	rqui_dlg_about.h \
	rqui_system_sel.c \
	rqui_system_sel.h \
	rqui.c \
	rqui.h \
	main.c

CFLAGS = `gtk-config --cflags` -I../../rq
riskquantify_LDFLAGS = `gtk-config --libs`
LDADD = ../../rq/librq.a

