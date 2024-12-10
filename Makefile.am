AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = monitor_products monitor_products_child

monitor_products_SOURCES = products.h products.c products.h callbacks.c \
                           monitor_products.c send_to_monitor.c \
                           comm.c comm.h ports.h

monitor_products_child_SOURCES = monitor_products_child.c \
                                 comm.c comm.h \
                                 send_to_monitor.c \
                                 pipe.c ports.h

docdir = $(prefix)/doc/$(PACKAGE)
doc_DATA = README CHANGES

products.c products.h: products.fd
	fdesign -convert products.fd

CLEANFILES = products.c products.h  # Yes, fdesign makes them.

EXTRA_DIST = CHANGES products.fd