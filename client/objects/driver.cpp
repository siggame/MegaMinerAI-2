
#include <iostream>


#include "Coordinate.h"
#include "MappableObject.h"
#include "Player.h"
#include "BuildZone.h"
#include "Unit.h"


using namespace std;

/* Commented out so our lazy makefile works
int main(int argc, char** argv)
{
    cout << "Creating objects" << endl;
    Coordinate* coordinate = new Coordinate;
    MappableObject* mappableObject = new MappableObject;
    Player* player = new Player;
    BuildZone* buildZone = new BuildZone;
    Unit* unit = new Unit;
    cout << endl;

    cout << "Outputting objects" << endl;
    cout << "\t" << "Coordinate: " << coordinate->toString() << endl;
    cout << "\t" << "Mappable Object: " << mappableObject->toString() << endl;
    cout << "\t" << "Player: " << player->toString() << endl;
    cout << "\t" << "BuildZone: " << buildZone->toString() << endl;
    cout << "\t" << "Unit: " << unit->toString() << endl;
    cout << endl;

    cout << "Destroying objects" << endl;
    delete coordinate;
    delete mappableObject;
    delete player;
    delete buildZone;
    delete unit;
    cout << endl;

    cout << "Finished" << endl;

}
*/
