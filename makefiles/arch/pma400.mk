include $(ROOTDIR)/makefiles/arch/unix.mk

INSTALLDIR = /opt/QtPalmtop
IMAGEDIR = $(INSTALLDIR)/pics
APPIMAGEDIR = $(IMAGEDIR)/%APPLICATION_NAME%
HOMEDIR = ~/Documents

QTPATH = /opt/Qtopia/sharp
TOOLSPATH = /opt/Embedix/tools/arm-linux/bin
MOC = $(QTPATH)/bin/moc
RM = rm -rvf
RM_QUIET = rm -rf
QTINCLUDE = -I $(QTPATH)/include
UILIBS = -L $(QTPATH)/lib -lqte -lqpe
CC = $(TOOLSPATH)/gcc
AR = $(TOOLSPATH)/ar rsu
LD = $(TOOLSPATH)/g++

CFLAGS = -pipe -DQT_QWS_EBX -DQT_QWS_CUSTOM -DQWS -fno-exceptions -fno-rtti -Wall -Wno-ctor-dtor-privacy -W -ftemplate-depth-30
DEPGEN = $(CC) -MM

XML_LIB = -lexpat
ENCA_LIB = -lenca
ARCHIVER_LIB = -lbz2
