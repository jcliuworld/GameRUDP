#gate make file
INCLUDES=-I$(top_srcdir)/protocol -I$(top_srcdir)/Language -I$(top_srcdir)/sdk -I$(top_srcdir)/Task
AM_CXXFLAGS=@CXXFLAGS_MYSQL@ @CXXFLAGS_XML2@
bin_PROGRAMS=DemoServer
DemoServer_SOURCES = \
			ServerTask.cpp \
			TaskManager.cpp \
			TimeTick.cpp \
			DemoUser.cpp \
			DemoUserM.cpp \
			DemoServer.cpp 

DemoServer_LDFLAGS = @LIBS_MYSQL@ @LIBS_XML2@ 
DemoServer_LDADD = $(top_builddir)/protocol/libprotocol.la $(top_builddir)/sdk/libage.la $(top_builddir)/sdk/Encrypt/libencdec.la
