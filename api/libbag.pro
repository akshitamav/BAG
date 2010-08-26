###############################################################################
#                OpenNS Application Build Project (v1.0)                      #
###############################################################################
#                                                                             
###                       DO NOT EDIT THIS SECTION                          
#                                                                             
### Retrieve the application master project which contains shared OpenNS definitions,
### includes, and other parameters to support platform independent building.
#
include (libmaster.pro)
#
INCLUDEPATH += $$INC_OPENNSALL
###############################################################################
#
###                  LIBRARY SPECIFIC SECTION (Edit Here)                          
#

CONFIG -= qt
CONFIG += shared

QMAKE_CFLAGS_DEBUG = 
QMAKE_CXXFLAGS_DEBUG = $$QMAKE_CFLAGS_DEBUG


HEADERS += stdtypes.h \
        crc32.h \
        ons_xml.h \
        onscrypto.h \
        ons_xml_error_handler.h \
        bag_private.h \
        bag.h
        
SOURCES += crc32.c \
        ons_xml.cpp \
        bag_attr.c \
        bag_crypto.c \
        bag_epsg.c \
        onscrypto.c \
        bag_hdf.c  \
        bag_surfaces.c \
        bag_surface_correct.c \
        bag_opt_surfaces.c \
        bag_tracking_list.c \
        bag_xml.c
           
