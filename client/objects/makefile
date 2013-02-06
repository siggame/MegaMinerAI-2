FLAGS= -g

all: objectbuild


objectbuild: baseAI.o BuildZone.o Coordinate.o driver.o MappableObject.o Parser.o Player.o Unit.o Wall.o
	touch objectbuild

baseAI.o: baseAI.cpp baseAI.h  BuildZone.h  Coordinate.h  MappableObject.h  Order.h  Parser.h  Player.h  Unit.h  Wall.h
	g++ -c ${FLAGS} baseAI.cpp

BuildZone.o: BuildZone.cpp baseAI.h  BuildZone.h  Coordinate.h  MappableObject.h  Order.h  Parser.h  Player.h  Unit.h  Wall.h
	g++ -c ${FLAGS} BuildZone.cpp

Coordinate.o: Coordinate.cpp baseAI.h  BuildZone.h  Coordinate.h  MappableObject.h  Order.h  Parser.h  Player.h  Unit.h  Wall.h
	g++ -c ${FLAGS} Coordinate.cpp

driver.o:  driver.cpp baseAI.h  BuildZone.h  Coordinate.h  MappableObject.h  Order.h  Parser.h  Player.h  Unit.h  Wall.h
	g++ -c ${FLAGS}  driver.cpp

MappableObject.o: MappableObject.cpp baseAI.h  BuildZone.h  Coordinate.h  MappableObject.h  Order.h  Parser.h  Player.h  Unit.h  Wall.h
	g++ -c ${FLAGS} MappableObject.cpp

Parser.o: Parser.cpp baseAI.h  BuildZone.h  Coordinate.h  MappableObject.h  Order.h  Parser.h  Player.h  Unit.h  Wall.h
	g++ -c ${FLAGS} Parser.cpp

Player.o: Player.cpp baseAI.h  BuildZone.h  Coordinate.h  MappableObject.h  Order.h  Parser.h  Player.h  Unit.h  Wall.h
	g++ -c ${FLAGS} Player.cpp

Unit.o: Unit.cpp baseAI.h  BuildZone.h  Coordinate.h  MappableObject.h  Order.h  Parser.h  Player.h  Unit.h  Wall.h
	g++ -c ${FLAGS} Unit.cpp

Wall.o: Wall.cpp baseAI.h  BuildZone.h  Coordinate.h  MappableObject.h  Order.h  Parser.h  Player.h  Unit.h  Wall.h
	g++ -c ${FLAGS} Wall.cpp

clean:
	rm *.o objectbuild -f
