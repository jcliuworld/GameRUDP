AM_LDFLAGS = @LIBS_MYSQL@ @LIBS_XML2@
AM_CXXFLAGS = @CXXFLAGS_MYSQL@ @CXXFLAGS_XML2@
INCLUDES = -I./ -I$(top_srcdir)/sdk -I$(top_srcdir)/Language 
noinst_LTLIBRARIES = libprotocol.la
libprotocol_la_SOURCES =	 \
						Base.pb.cc \
						UDPTaskProtoBuf.cpp \
						DemoServer.pb.cc 
