

__start__: obj __lines_for_space__ ../libs/libInterp4Move.so ../libs/libPreProc.so
	@echo "  Tworzenie wtyczki zakonczone."
	@echo

obj:
	mkdir obj

CPPFLAGS=-Wall -fPIC -pedantic -std=c++17 -Iinc -I../inc
LDFLAGS=-Wall -shared

__lines_for_space__:
	@echo
	@echo
	@echo
	@echo
	@echo

../libs/libInterp4Move.so: obj/Interp4Move.o obj/PreProc.o
	g++ ${LDFLAGS} -o ../libs/libInterp4Move.so obj/Interp4Move.o
	@echo
	@echo "  Wtyczka dla polecenia 'Move' zostala utworzona."
	@echo
	
../libs/libPreProc.so: obj/PreProc.o
	g++ ${LDFLAGS} -o ../libs/libPreProc.so obj/PreProc.o
	@echo
	@echo "  Wtyczka PreProc została utworzona."
	@echo


obj/Interp4Move.o: src/Interp4Move.cpp ../inc/AbstractInterp4Command.hh\
                   ../inc/AbstractScene.hh ../inc/AbstractComChannel.hh\
                   inc/Interp4Move.hh 
	g++ -c ${CPPFLAGS} -o obj/Interp4Move.o src/Interp4Move.cpp
	
	
obj/PreProc.o: src/PreProc.cpp inc/PreProc.hh
	g++ -c ${CPPFLAGS} -o obj/PreProc.o src/PreProc.cpp


clean:
	rm -f obj/*

cleanall: clean
	find . -name \*~ -print -exec rm {} \;
