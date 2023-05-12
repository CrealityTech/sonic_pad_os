
include $(top_srcdir)/Makefile.inc

lib_LTLIBRARIES = libOmxCore.la

libOmxCore_la_SOURCES = \
                        tablelist/linux/aw_registry_table.c \
                        aw_omx_component.c \
                        aw_omx_core.c

libOmxCore_la_CFLAGS = $(CFLAGS_CDCG)
libOmxCore_la_CPPFLAGS = $(CPPFLAGS_CDCG)
LOCAL_INCLUDE = -I../include \
                -I./include \
                -I$(top_srcdir)/ \
                -I$(top_srcdir)/include \
                -I$(top_srcdir)/openmax/omxcore \
                -I$(top_srcdir)/base/include \
                -I$(top_srcdir)/ve/include

libOmxCore_la_CFLAGS += $(LOCAL_INCLUDE)
libOmxCore_la_CPPFLAGS += $(LOCAL_INCLUDE)

libOmxCore_la_LDFLAGS = $(LDFLAGS_CDCG)

libOmxCore_la_LIBADD = $(top_srcdir)/base/libcdc_base.la \
                       $(top_srcdir)/vdecoder/libvdecoder.la \
                       $(top_srcdir)/memory/libMemAdapter.la
LOCAL_LIB = -ldl -lpthread 
libOmxCore_la_LDFLAGS += $(LOCAL_LIB)


