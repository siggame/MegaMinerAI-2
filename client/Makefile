FLAGS= -g -O0

client: sexp/sexpbuild muparser/mubuild objects/objectbuild client.o shellAI.o
	g++ -o client client.o shellAI.o objects/*.o muparser/*.o sexp/*.o ${FLAGS}

client.o: client.cpp client.h
	g++ -c client.cpp ${FLAGS}

shellAI.o: shellAI.cpp shellAI.h
	g++ -c shellAI.cpp ${FLAGS}

sexp/sexpbuild: sexp/cstring.o sexp/sexp.o sexp/event_temp.o sexp/faststack.o sexp/io.o sexp/parser.o sexp/sexp_memory.o sexp/sexp_ops.o sexp/sexp_vis.o 
	cd sexp && ${MAKE}
	
objects/objectbuild: objects/baseAI.o objects/BuildZone.o objects/Coordinate.o objects/driver.o objects/MappableObject.o objects/Parser.o objects/Player.o objects/Unit.o objects/Wall.o
	cd objects && ${MAKE}

muparser/mubuild: muparser/muParserBase.o muparser/muParserCallback.o muparser/muParser.o muparser/muParserError.o muparser/muParserTest.o muparser/muParserBytecode.o muparser/muParserComplex.o muparser/muParserDLL.o muparser/muParserInt.o muparser/muParserTokenReader.o
	cd muparser && $(MAKE)


clean:
	rm -f client muparser/mubuild object/objectbuild sexp/sexpbuild
