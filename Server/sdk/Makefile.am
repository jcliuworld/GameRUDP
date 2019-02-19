SUBDIRS = Encrypt
INCLUDES = -I$(top_srcdir)/protocol -I$(top_srcdir)/Language  
AM_LDFLAGS = @LIBS_MYSQL@ @LIBS_XML2@
AM_CXXFLAGS = @CXXFLAGS_MYSQL@ @CXXFLAGS_XML2@

noinst_LTLIBRARIES = libage.la
#libage_la_LIBADD = $(top_builddir)/protocol/libprotocol.la 
libage_la_SOURCES =	 \
		Age.cpp	\
		aArg.cpp \
		aVec3.cpp \
		aPos.cpp \
		aGlobal.cpp \
		aLogger.cpp \
		aConfig.cpp \
		aCond.cpp \
		aMsgCallBack.cpp \
		aService.cpp \
		aTime.cpp \
		aXMLReader.cpp \
		aThread.cpp \
		aUserM.cpp \
		aBase64.cpp \
		aMemStream.cpp \
		aTimeWarp.cpp \
		aUDPNetServer.cpp \
		aUDPServer.cpp \
		aUDPTask.cpp \
		aUDPSocket.cpp \
		aUDPTaskPool.cpp  
		 
