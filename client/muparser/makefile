all: mubuild

mubuild: muParserBase.o muParserCallback.o muParser.o muParserError.o muParserTest.o muParserBytecode.o muParserComplex.o muParserDLL.o muParserInt.o muParserTokenReader.o
	touch mubuild

muParserBase.o: muParserBase.cpp
	g++ -c -g muParserBase.cpp

muParserCallback.o: muParserCallback.cpp
	g++ -c -g muParserCallback.cpp

muParser.o: muParser.cpp
	g++ -c -g muParser.cpp

muParserError.o: muParserError.cpp
	g++ -c -g muParserError.cpp

muParserTest.o: muParserTest.cpp
	g++ -c -g muParserTest.cpp

muParserBytecode.o: muParserBytecode.cpp
	g++ -c -g muParserBytecode.cpp

muParserComplex.o: muParserComplex.cpp
	g++ -c -g muParserComplex.cpp

muParserDLL.o: muParserDLL.cpp
	g++ -c -g muParserDLL.cpp

muParserInt.o: muParserInt.cpp
	g++ -c -g muParserInt.cpp

muParserTokenReader.o: muParserTokenReader.cpp
	g++ -c -g muParserTokenReader.cpp


clean:
	rm -f *.o mubuild
