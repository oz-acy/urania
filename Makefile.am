# Makefile.am for URANIA

AM_CFLAGS = -O2
AM_CXXFLAGS = -O2

lib_LIBRARIES = liburania.a

liburania_a_SOURCES = \
	dec.h \
	sys.h sys.cpp sys-ws2mbs.cpp sys-mbs2ws.cpp \
	askYNCBox.cpp getCLArgs.cpp getLongPath.cpp \
	task.cpp task3.cpp \
	wbase.h  ctrl.cpp ctrl_sb.cpp \
	bwin.h bwin.cpp \
	win.h win.cpp scroll-h.cpp drop-h.cpp \
	dialog.h dialog.cpp \
	menu.h menu.cpp \
	menudesc.cpp \
	clkpanel.h clkpanel.cpp cmndlg.h cmndlg.cpp \
	pdev.h pdev.cpp pdev2.cpp pmdev.cpp pmdidx.cpp \
	canvas.h \
	registry.h registry.cpp regread.cpp regwrite.cpp


uraniaincludedir = @includedir@/urania
uraniainclude_HEADERS = \
	dec.h sys.h \
	wbase.h bwin.h win.h dialog.h \
	menu.h \
	clkpanel.h cmndlg.h \
	pdev.h \
	canvas.h csr.h \
	registry.h


uraniadir = @libdir@
urania_DATA = \
	csr.res.o wmain.o


EXTRA_DIST = csr.h csr.rc cv.cur wmain.cpp

CLEANFILES = *~ *.res


#
csr.res.o : csr.rc cv.cur csr.h
	windres csr.rc -o $@


#eof
