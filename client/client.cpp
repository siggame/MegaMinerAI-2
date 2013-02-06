//Make sure your class is called 'myAI'
#include "shellAI.h"


/////////////////////////////////////////////////////
///       DO NOT EDIT BELOW THIS POINT!!!         ///
/////////////////////////////////////////////////////

#include "client.h"
#include "sexp/sexp.h"
#include "objects/Parser.h"

#include "objects/Unit.h"
#include "objects/Coordinate.h"
#include "objects/Player.h"
#include "objects/BuildZone.h"
#include "objects/Order.h"
#include "objects/baseAI.h"

#include <errno.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <deque>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <time.h>

#include <sys/types.h>

#include "muparser/muParserDLL.h"

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define SOCKET_ERROR -1
#endif

//#define DEBUG
//#define SHOW_NETWORK
#define CLIENT_VERSION 2


string my_user_id = "";
int myID = 1;
int enemyId = 2;
ofstream log_file;
ostringstream log_stream;

// Game utilities
bool parse_ident_sexpr        (myAI& ai, sexp_t* sexp);
bool parse_constants_sexpr    (myAI& ai, sexp_t* sexp);
bool parse_formulas_sexpr     (sexp_t* sexp);

bool parse_status_sexpr       (myAI& ai, sexp_t* sexp);
bool parse_units_sexpr        (myAI& ai, sexp_t* myUnits, sexp_t* enemyUnits);
bool parse_walls_sexpr        (myAI& ai, sexp_t* sexp);
bool parse_buildzones_sexpr   (myAI& ai, sexp_t* sexp);


// SEXPR utilities
sexp_t* extract_sexpr(string sexpr);
string create_sexpr(deque<string> parts);
bool check_sexp(sexp_t* sexp, string value);
bool sexp_sanity(sexp_t* sexp);
sexp_t* rec_sexp(int socket);
string sexp_part(sexp_t* sexp, int part);
sexp_t* sexp_sub(sexp_t* sexp, int part);

// Network utilities
#define GLOBAL_BUFFER_SIZE 512
#define PORT "19000"
int open_server_connection(char* host, char* port);
int send_string(int socket, string payload);
string rec_string(int socket);
string login_to_server(int sock_server, string username, string password);
string create_game(int sock_server);

/////////////////////////////////////////////////////
/// MAIN
/////////////////////////////////////////////////////

int main(int argc, char** argv)
{
   //connect to server, given as the first argument
	string usage = string(argv[0]) + " <server> [game #]";
	if(argc < 2) {
		cerr <<  usage << endl;
		return 0;
	}

	bool end_game = false;
	bool start_game = true;
	if(argc == 3) {
		// they specified a game number,
		// do something different?
		start_game = false;
		cout << "Trying to join game #" << argv[2] << endl;
	}

	// we got enough command-line arguments,
	// start tying to connect:
	int sock_server = open_server_connection(argv[1], PORT);
	if(sock_server == -1) {
		return 0;
	}

	// create an instance of the player's AI class
	myAI gameAI;

	string comm_buffer;
	sexp_t* sexp;

	// log into the server, using the AI's team name
	string whos_turn;
	my_user_id = login_to_server(sock_server, gameAI.playerName(), gameAI.playerPassword());
	myID = atoi(my_user_id.c_str());


	// setup the logging filename
	ostringstream stream;
   tm* myTime;
   time_t theTime = time(NULL);
   myTime = localtime(&theTime);
	stream << myTime->tm_hour << "." << myTime->tm_min;

   string name = string(argv[0]);
   for(int i=static_cast<int>(name.size()-1); i >=0; i--)
   {
      if(name[i] == ' ' || name[i] == '/' || name[i] == '\\' || name[i] == '.' || name[i] == ':')
      {
         name.erase(i,1);
      }
   }
   string log_filename = stream.str() + "-" + name + string(".gamelog");
	cout << "Log Filename: " << log_filename << endl;

   #ifdef DEBUG
	cout << "DEBUG: logged in as user #" << my_user_id << endl;
	#endif

	// okay, we've succesfully logged in,
	// now start or join a game

	if(start_game) {
		// create a new game:
		string game_number = create_game(sock_server);

		//gameAI.playerNumber = 1;
		//gameAI.myBase = coordinate(0,0);
		//gameAI.enemyBase = coordinate(25,25);

	} else {

		//gameAI.playerNumber = 2;
		//gameAI.myBase = coordinate(0,0);
		//gameAI.enemyBase = coordinate(25,25);

		// join an existing game
		cout << "Trying to join game " << argv[2] << "..." << endl;
		string join_string = "(join-game " + string(argv[2]) + ")";
		send_string(sock_server, join_string);
		sexp = rec_sexp(sock_server);
		if(!check_sexp(sexp, "join-accepted")) {
			cout << "Unabled to join game " << argv[2] << endl;
			goto GAME_SHUTDOWN;
		}

		// we should only try to start the game if we're the
		// player that just joined; otherwise, the server
		// will sorta freak out...
		send_string(sock_server, "(game-start)");
	}

	// the server is going to spit a ton of SEXP's at us;
	// wade through them until we get a "new-turn" msg.
	sexp = rec_sexp(sock_server);
	while(!check_sexp(sexp, "new-turn")) {
		// okay, what kind of data have we gotten from the server,
		// and what are we supposed to do about it?

		if(check_sexp(sexp, "game-start-accepted")) {
			// the server accepted our reqeust to begin the game;
			// we don't care, because earlier we just assumed
			// that it worked :|
		}

		if(check_sexp(sexp, "unit-types")){
			// this should be sent only during this initialization:
			// it contains the details of each type of unit
			#ifdef DEBUG
			cout << "DEBUG: updating unit types" << endl;
			#endif
			//update_unit_types((myAI*)&gameAI, sexp);
		}

      //Identifies player id's and names
      if(check_sexp(sexp, "ident")){
			#ifdef DEBUG
			cout << "DEBUG: Receiving ident info" << endl;
			#endif
			parse_ident_sexpr(gameAI, sexp);
		}

      //Contains the constants for the various unit formulas - we won't be using these
      if(check_sexp(sexp, "constants")){
			#ifdef DEBUG
			cout << "DEBUG: Receiving constant info" << endl;
			#endif
			parse_constants_sexpr(gameAI, sexp);
		}

      //Contains the formulas for the unit stats - muParser will use these
      if(check_sexp(sexp, "formulas")){
			#ifdef DEBUG
			cout << "DEBUG: Receiving formulas" << endl;
			#endif
			parse_formulas_sexpr(sexp);
		}

      if(check_sexp(sexp, "walls"))
      {
			#ifdef DEBUG
			cout << "DEBUG: Receiving walls" << endl;
			#endif
			parse_walls_sexpr(gameAI, sexp);
      }

      if(check_sexp(sexp, "buildZones"))
      {
			#ifdef DEBUG
			cout << "DEBUG: Receiving buildZones" << endl;
			#endif
			parse_buildzones_sexpr(gameAI, sexp);
      }

		if(check_sexp(sexp, "status")) {
			// An actual game-status message: this might not
			// mean much right now, but try to parse it anyway.
			// Don't bail if it fails, but warn us.

			if(!parse_status_sexpr(gameAI, sexp))
         {
				cout << "WARN: error parsing game status" << endl;
			}
		}

		// get another S-expression to test:
		sexp = rec_sexp(sock_server);
	}

	// okay, now sexp should contain a "new-turn" message
	// pull out who's turn it is, then start the main game loop
	whos_turn = string(sexp->list->next->val);

	#ifdef DEBUG
	cout << "DEBUG: got all our pregame stuff, starting main loop!" << endl;
	#endif

	while(!end_game) {
		// ask the server for an update:
		//send_string(sock_server, "(game-status)");
		//sexp = rec_sexp(sock_server);

		// we're not guaranteed to get a "status" message,
		// so handle anything unexpected before we get to
		// the main game loop
		while(!check_sexp(sexp, "status")) {

			#ifdef DEBUG
			cout << "DEBUG: waiting for game status, got \"" << sexp->list->val << "\" message" << endl;
			#endif

			if(check_sexp(sexp, "game-over")) {
				cout << "Game Over: ";
				string winner = string( sexp->list->next->val );
				if( winner == my_user_id )
					cout << "You Win!";
				else
					cout << "You Lose...";
				cout << endl;

				end_game = true;
				goto GAME_SHUTDOWN;
			}

			if(check_sexp(sexp, "new-turn")) {
				whos_turn = string(sexp->list->next->val);
				#ifdef DEBUG
				cout << "DEBUG: It's player " << whos_turn << "'s turn" << endl;
				#endif
			}

                        if(check_sexp(sexp, "records")){
                            #ifdef DEBUG
                            cout << "DEBUG: Receiving records" << endl;
                            #endif
                            cout << sexp->list->next->val;
                         }


			sexp = rec_sexp(sock_server);
		}

		if(!check_sexp(sexp, "status")) {
			cerr << "WARN: expected 'status', got '" << sexp->list->val << "'" << endl;
		}

		if(!parse_status_sexpr(gameAI, sexp)) {
			cerr << "WARN: couldn't parse game status!" << endl;
		}

		// update the turn number
		gameAI.turnNumber = atoi( sexp->list->next->list->val);
		if(gameAI.turnNumber == 1)
      {
         gameAI.init();
      }

		if(whos_turn == my_user_id && !end_game) {

			//cout << endl << "Starting my turn... ";

			// 4a. Run their custom 'Play()' function
			gameAI.play();

			// 4b. Send their orders to the server
			while( gameAI.orders.size() != 0){

				string myOrder = gameAI.orders.front().s_expression;
				gameAI.orders.pop();

				// send this command to the server
				send_string(sock_server, myOrder);
				// there's no need to listen for an incoming message;
				// we assume errors will be handled at the beginning
				// of the next loop.
			}

			// let the server know we've finished out turn
			send_string(sock_server, "(end-turn)");
			//cout << "done with my turn" << endl;

		} else {
			// it's not your turn, so take it easy for a second
			//cout << "Not my turn, waiting...." << endl;
			#ifndef WIN32 // the usleep function insn't avaible in Windows
			//usleep(100000); // this helps prevent climbing to 100% CPU usage
			#endif
		}

		// get another sexp to jump-start the next iteration of the loop
		sexp = rec_sexp(sock_server);

	}

	GAME_SHUTDOWN:
	cout << "Shutting down..." << endl;
	send_string(sock_server, "(leave-game)");
	sexp = rec_sexp(sock_server);
	send_string(sock_server, "(logout)");
	sexp = rec_sexp(sock_server);

	//End of game
	#ifdef WIN32
	closesocket(sock_server);
	#else
	close(sock_server);
	#endif

   log_file.open(log_filename.c_str());
   if(log_file.fail())
   {
      cout << "Error writing to log file" << endl;
   }
   else
   {
      log_file << log_stream.str();
      log_file.close();
   }
	return 0;
}

/////////////////////////////////////////////////////
/// GAME UTILITIES
/////////////////////////////////////////////////////

/*
bool update_unit_types(myAI* ai, sexp_t* sexp) {
	// make sure 'sexp' is actually a status message
	if(!check_sexp(sexp, "unit-types")) {
		cerr << "ERROR: update_unit_types given a non-unit-types SEXP" << endl;
		return false;
	}

	sexp_t* cur = sexp->list->next->list;
	// 'cur' should now point to the first unit in the list
	int unit_type_id = 0;
	while(cur != NULL) {
		unitInfo info;
		#ifdef DEBUG
		cout << "       added unit type " << cur->list->val << endl;
		#endif
		info.setUnitType(unit_type_id);
		info.setName( cur->list->val );
		info.setCost( atoi(cur->list->next->val) );
		info.setMaxHealth( atoi(cur->list->next->next->val) );
		info.setAttackPower( atoi(cur->list->next->next->next->val) );
		info.setSpeed( atoi(cur->list->next->next->next->next->val) );
		info.setAttackRange( atoi(cur->list->next->next->next->next->next->val) );
		ai->unitTypes.push_back( info );

		unit_type_id++;
		cur = cur->next;
	}

	return true;
}
*/

/*
bool game_status(myAI& ai, sexp_t* sexp, string my_user_id) {

	// make sure 'sexp' is actually a status message
	if(!check_sexp(sexp, "status")) {
		cerr << "ERROR: game_status given a non-status SEXP" << endl;
		return false;
	}

	// extract the sexps about each player
	sexp_t* player1;
	sexp_t* player2;
	sexp_t* player_me;
	sexp_t* player_enemy;

	player1 = sexp->list->next->list->next; // yes, it's funky. but it works.
	player2 = sexp->list->next->list->next->next;

	if( string(player1->list->val) == my_user_id ) {
		player_me = player1;
		player_enemy = player2;
	} else
	if( string(player2->list->val) == my_user_id ) {
		player_me = player2;
		player_enemy = player1;
	}

	// 1. Get the players' current scores (resources)
	//ai.myScore = atoi( player_me->list->next->val );
	//ai.enemyScore = atoi( player_enemy->list->next->val );

	// 2. For each player, populate their unit vectors.
	parse_unit_data( player_me->list->next->next, player_enemy->list->next->next, ai );

	// 3. Parse resource node info.
	//parse_resource_data( sexp->list->next->list->next->next->next, ai);

	return true;
}
*/

/*
void parse_unit_data(sexp_t* my_units, sexp_t* enemy_units, myAI& ai) {
	// each sub-sexp is of the format:
	// ( id# HP X Y buildpoints poisonDmg red blue green yellow)

	// MY UNITS ////////////////////////////

	sexp_t* cur = my_units->list;
	deque<Unit> units;
	while(cur != NULL) {
		Coordinate coords( atoi(cur->list->next->next->next->val), atoi(cur->list->next->next->next->next->val) );
		Unit theUnit;
		//theUnit.constants = consthash[ string(cur->list->val) ];
		//theUnit.setMoves((consthash[ string(cur->list->val) ])->getSpeed());
		theUnit.setLocation(coords);
		theUnit.setId(atoi( cur->list->next->val ));
		theUnit.setCurHealth(atoi( cur->list->next->next->val ));
		units.push_back(theUnit);
		cur = cur->next;
	}
	ai.myUnits = units;
	// ENEMY UNITS /////////////////////////

	cur = enemy_units->list;
	units.clear();
	while(cur != NULL) {
		Coordinate coords( atoi(cur->list->next->next->next->val), atoi(cur->list->next->next->next->next->val) );
		Unit theUnit;
		//theUnit.constants = consthash[ string(cur->list->val) ];
		//theUnit.setMoves((consthash[ string(cur->list->val) ])->getSpeed());
		theUnit.setLocation(coords);
		theUnit.setId(atoi( cur->list->next->val ));
		theUnit.setCurHealth(atoi( cur->list->next->next->val ));
		units.push_back(theUnit);
		cur = cur->next;
	}
	ai.enemyUnits = units;
}
*/


bool parse_ident_sexpr    (myAI& ai, sexp_t* sexp)
{
   #ifdef DEBUG
      cout << "DEBUG: Parsing ident sexpr" << endl;
   #endif

   //ident format:
   //("ident" ((player) (player)))
   //player format:
   //(id name)

   ai.players.clear();

   int id;
   string name;

   sexp_t* cur = sexp->list->next->list;
   while(cur != NULL)
   {
      id = atoi(cur->list->val);
      name =    cur->list->next->val;

      Player player(id, 0, name);
      ai.players.push_back(player);

      //Set the playerID of the player's me object
      if(myID == player.id())
      {
         ai.me = player;
      }
      if(myID != player.id())
      {
         enemyId = player.id();
      }

      //cout << "My id is " << myID << endl;
      //cout << "Enemy is " << enemyId << endl;

      cur = cur->next;
   }

   #ifdef DEBUG
      cout << "DEBUG: Finished parsing ident sexpr" << endl;
   #endif
   return true;
}

bool parse_constants_sexpr    (myAI& ai, sexp_t* sexp)
{
   #ifdef DEBUG
      cout << "DEBUG: Parsing constants sexpr" << endl;
   #endif
   return false;
}

bool parse_formulas_sexpr     (sexp_t* sexp)
{
   #ifdef DEBUG
      cout << "DEBUG: Parsing formulas sexpr" << endl;
   #endif

   //Expression is:
   //("formulas" (formula) (formula))
   //Where (formula) is:
   //(("formulaname" "formulaString")

   sexp_t* formula;
   Parser* parser = Parser::instance();

   formula = sexp->list->next->list;
   while(formula != NULL)
   {
      if( strcmp(formula->list->val, "maxHP") == 0 )
      {
         parser->setMaxHealth( string(formula->list->next->val) );
      }
      else if( strcmp(formula->list->val, "range") == 0 )
      {
         parser->setAttackRange( string(formula->list->next->val) );
      }
      else if( strcmp(formula->list->val, "attack") == 0 )
      {
         parser->setAttackPower( string(formula->list->next->val) );
      }
      else if( strcmp(formula->list->val, "move") == 0 )
      {
         parser->setMaxMoves( string(formula->list->next->val) );
      }
      else if( strcmp(formula->list->val, "heal") == 0 )
      {
         parser->setHealPower( string(formula->list->next->val) );
      }
      else if( strcmp(formula->list->val, "splash") == 0 )
      {
         parser->setSplashDamage( string(formula->list->next->val) );
      }
      else if( strcmp(formula->list->val, "poison") == 0 )
      {
         parser->setPoisonPower( string(formula->list->next->val) );
      }
      else if( strcmp(formula->list->val, "regen") == 0 )
      {
         parser->setMaxRegen( string(formula->list->next->val) );
      }
      else
      {
         cout << "Got formula of type \"" << formula->list->val << "\" but didn't know what to do with it." << endl;
      }
      formula = formula->next;
   }


   #ifdef DEBUG
      cout << "DEBUG: Finished parsing formulas sexpr" << endl;
   #endif
   return true;
}

bool parse_status_sexpr       (myAI& ai, sexp_t* sexp)
{
   //("status" (turnNumber (playerID ((unit) (unit))) (playerID ((unit) (unit)))))

   #ifdef DEBUG
      cout << "DEBUG: Parsing status sexpr" << endl;
   #endif

   if(!check_sexp(sexp, "status"))
   {
      cerr << "ERROR: parse_status_sexpr given a non-status SEXP" << endl;
      return false;
   }

   //UNITS
   //Get the (playerID ((unit) (unit))) sub expressions
   sexp_t* player1 = sexp->list->next->list->next;
   sexp_t* player2 = sexp->list->next->list->next->next;

   sexp_t* myUnits;
   sexp_t* enemyUnits;

   //Set player scores
   for (unsigned int i = 0; i < ai.players.size(); i++)
   {
      if (ai.players[i].id() == atoi(player1->list->val))
         ai.players[i].setScore(atoi(player1->list->next->val));
      if (ai.players[i].id() == atoi(player2->list->val))
         ai.players[i].setScore(atoi(player2->list->next->val));
   }


   //Get the ((unit) (unit)) sub expressions
   if(atoi(player1->list->val) == myID)
   {
      myUnits = player1->list->next->next;
      enemyUnits = player2->list->next->next;
   }
   else
   {
      myUnits = player2->list->next->next;
      enemyUnits = player1->list->next->next;
   }

   //Parse the unit subexpressions and load them into the ai
   if(!parse_units_sexpr(ai, myUnits, enemyUnits))
   {
      cerr << "Error parsing unit s-expresion" << endl;
      return false;
   }


   #ifdef DEBUG
      cout << "DEBUG: Finished parsing status sexpr" << endl;
   #endif

   return true;
}


bool parse_units_sexpr(myAI& ai, sexp_t* myUnits, sexp_t* enemyUnits)
{
	// each sub-sexp is of the format:
   //((unit) (unit))
   //Where units are of the format:
	// ( id# HP X Y buildpoints poisonDmg red blue green yellow)

   #ifdef DEBUG
      cout << "DEBUG: Parsing units sexprs" << endl;
   #endif

	// MY UNITS ////////////////////////////
	sexp_t* cur = myUnits->list;
	deque<Unit> units;

   //This should speed things up a bit
   int id;
   int curHealth;
   Coordinate location;
   int buildPoints;
   int poisonDmg;
   int red;
   int blue;
   int green;
   int yellow;

   int actions;
   int moves;

   //Step through each unit sub expression
	while(cur != NULL)
   {
      id =           atoi(cur->list->val);
      curHealth =    atoi(cur->list->next->val);
      location = Coordinate( atoi(cur->list->next->next->val), atoi(cur->list->next->next->next->val) );
      buildPoints =  atoi(cur->list->next->next->next->next->val);
      poisonDmg =    atoi(cur->list->next->next->next->next->next->val);
      red =          atoi(cur->list->next->next->next->next->next->next->val);
      blue =         atoi(cur->list->next->next->next->next->next->next->next->val);
      green =        atoi(cur->list->next->next->next->next->next->next->next->next->val);
      yellow =       atoi(cur->list->next->next->next->next->next->next->next->next->next->val);

      actions = 1;
      moves = -1;

      Unit theUnit(id, myID, curHealth, poisonDmg, location, red, yellow, blue, green, buildPoints, actions, moves);
		theUnit.setMoves( theUnit.maxMoves() );

      units.push_back(theUnit);
		cur = cur->next;
	}

   //Put the units into the ai
   ai.units = units;

	// ENEMY UNITS /////////////////////////
	cur = enemyUnits->list;
	units.clear();
	while(cur != NULL)
   {
      id =           atoi(cur->list->val);
      curHealth =    atoi(cur->list->next->val);
      location = Coordinate( atoi(cur->list->next->next->val), atoi(cur->list->next->next->next->val) );
      buildPoints =  atoi(cur->list->next->next->next->next->val);
      poisonDmg =    atoi(cur->list->next->next->next->next->next->val);
      red =          atoi(cur->list->next->next->next->next->next->next->val);
      blue =         atoi(cur->list->next->next->next->next->next->next->next->val);
      green =        atoi(cur->list->next->next->next->next->next->next->next->next->val);
      yellow =       atoi(cur->list->next->next->next->next->next->next->next->next->next->val);

      //TODO: Calculate actual values somewhere
      actions = 1;
      moves = 1;

      Unit theUnit(id, enemyId, curHealth, poisonDmg, location, red, yellow, blue, green, buildPoints, actions, moves);
		theUnit.setMoves( theUnit.maxMoves() );

      units.push_back(theUnit);
		cur = cur->next;
	}

   //push the enemy's units into the vector also
   for(unsigned int i=0; i < units.size(); ++i)
   {
      ai.units.push_back(units[i]);
   }

   #ifdef DEBUG
      cout << "DEBUG: Finished parsing unit sexprs" << endl;
   #endif

   return true;
}

bool parse_walls_sexpr        (myAI& ai, sexp_t* sexp)
{
   #ifdef DEBUG
      cout << "DEBUG: Parsing walls sexpr" << endl;
   #endif

   //Walls format:
   //((wall) (wall))
   //Wall Format:
   //(id x y)

   ai.walls.clear();

   int id;
   int x;
   int y;

   sexp_t* cur = sexp->list->next;
   while(cur != NULL)
   {
      id = atoi(cur->list->val);
      x  = atoi(cur->list->next->val);
      y  = atoi(cur->list->next->next->val);

      Wall wall(Coordinate(x,y), id);
      ai.walls.push_back(wall);
      cur = cur->next;
   }

   #ifdef DEBUG
      cout << "DEBUG: Finished parsing walls sexpr" << endl;
   #endif
   return true;
}

bool parse_buildzones_sexpr   (myAI& ai, sexp_t* sexp)
{
   #ifdef DEBUG
      cout << "DEBUG: Parsing buildzones sexpr" << endl;
   #endif

   //Buildzones format:
   //((buildzone) (buildzone))
   //Buildzone Format:
   //(id x y)

   ai.buildZones.clear();

   int id;
   int x;
   int y;

   sexp_t* cur = sexp->list->next;
   while(cur != NULL)
   {
      id = atoi(cur->list->val);
      x  = atoi(cur->list->next->val);
      y  = atoi(cur->list->next->next->val);

      BuildZone buildZone(Coordinate(x,y), id);
      ai.buildZones.push_back(buildZone);
      cur = cur->next;
   }

   #ifdef DEBUG
      cout << "DEBUG: Finished parsing buildzones sexpr" << endl;
   #endif
   return true;
}

/////////////////////////////////////////////////////
/// S-EXPRESSION UTILITIES
/////////////////////////////////////////////////////

// Takes in a string with the s-expression and breaks it
// up into it's pieces, putting them in a deque<string>.
// Example: input   "(game-move-denied 'no game')"
//          returns <"game-move-denied", "no game">
sexp_t* extract_sexpr(string sexpr) {
	// STUB

	sexp_t* st = parse_sexp( (char *)sexpr.c_str(), sexpr.length() );
	return st;
}

// Takes in a deque<string>, and uses those strings
// to build an s-expression.
// Example: input   <"foobar", "1", "3">
//          returns "(foobar 1 3)"
string create_sexpr(deque<string> parts) {

	string buffer;
	buffer = "(";
	for(unsigned int i = 0; i < parts.size(); i++) {
		buffer += parts.at(i);
		if(i != parts.size()-1)
			buffer += " ";
	}
	buffer += ")";

	return buffer;

}

// checks to make sure that the first value of the given
// sexp is the string 'value'
bool check_sexp(sexp_t* sexp, string value) {
	if(!sexp_sanity(sexp)) {
		#ifdef DEBUG
		cerr << "DEBUG: check_sexp: sexp failed sanity test" << endl;
		#endif
		return false;
	}


	if(sexp->ty != SEXP_VALUE) {
		// this means that we got the head of a list,
		// so should we recurse and descend into the next list?
		return check_sexp(sexp->list, value);
		// sure, why not?
	}

	char* v = NULL;
	if(sexp->aty == SEXP_DQUOTE || sexp->list->aty == SEXP_SQUOTE)
		v = sexp->val;
	else if(sexp->aty == SEXP_BINARY)
		v = sexp->bindata;
	else {
		cerr << "check_sexp: unknown sexp type" << endl;
		return false;
	}

	if( strcmp(value.c_str(), v) == 0 ) {
		return true;
	} else {
		return false;
	}
}


//If something needs to go into the gamelog from the server, do it here
sexp_t* rec_sexp(int socket) {
	string buffer;
	sexp_t* sexp = NULL;

	while( sexp == NULL )
   {
		buffer = rec_string(socket);
		sexp = extract_sexpr(buffer);
	}

   if(check_sexp(sexp, "ident"))
   {
      //log_stream << buffer << endl;
   }

   if(check_sexp(sexp, "buildZones"))
   {
      //log_stream << buffer << endl;
   }

   if(check_sexp(sexp, "walls"))
   {
      //log_stream << buffer << endl;
   }

   if(check_sexp(sexp, "animations"))
   {
      //log_stream << buffer << endl;
   }

   if(check_sexp(sexp, "status")) {
	   // dump this to a file, for the visualizer
	   // my_user_id is a global
	   //log_stream << buffer << endl;
   }

	if( check_sexp(sexp, "game-move-denied") ||
	    check_sexp(sexp, "game-attack-denied") ||
		check_sexp(sexp, "game-build-denied") ||
		check_sexp(sexp, "game-combine-denied") )
   {
			cerr << "There was an error: " << buffer << endl;
         //log_stream << buffer << endl;
	}

   /*
   static int count = 0;
   count++;

   if(count > 10)
   {
      if(log_file.fail())
      {
         cout << "Error writing to log file" << endl;
      }
      else
      {
         log_file << log_stream.str();
         log_stream.str("");
         log_file.flush();
      }

      count = 0;
   }
	*/

	return sexp;
}

bool sexp_sanity(sexp_t* sexp) {
	if(sexp == NULL) {
		#ifdef DEBUG
		cerr << "sexp_sanity: sexp is NULL" << endl;
		#endif
		return false;
	}

	if(sexp->aty == SEXP_BINARY) {
		#ifdef DEBUG
		cerr << "sexp_sanity: sexp is SEXP_BINARY" << endl;
		#endif
		return false;
	}

	return true;
}

string sexp_part(sexp_t* sexp, int part) {
	if(!sexp_sanity(sexp)) {
		return NULL;
	}

	if(sexp->ty != SEXP_VALUE) {
		// this means that we got the head of a list,
		// so we recurse and descend into the next list
		#ifdef DEBUG
		//cout << "sexp_part: sexp is list head, recursing..." << endl;
		cerr << "sexp_part: sexp is list head, bailing..." << endl;
		#endif
		//return sexp_part(sexp->list, part);
		return NULL;
	}

	// at this point, we need to find the Nth part of the sexp
	sexp_t* cur = sexp;
	for(int i = 0; i < part; i++) {
		cur = cur->next;
		if(cur == NULL) {
			#ifdef DEBUG
			cerr << "check_sexp: unexpectedly reached end of list" << endl;
			#endif
			return NULL;
		}
	}
	// now we should have a pointer to the desired part,
	// so attemp to extract it:
	if(!sexp_sanity(cur)) {
		return NULL;
	}

	string value( cur->val );
	return value;
}

sexp_t* sexp_sub(sexp_t* sexp, int part) {
	if(!sexp_sanity(sexp)) {
		return NULL;
	}

	if(sexp->ty != SEXP_LIST) {
		// we want to get a piece of a list:
		// if the node we're given isn't a list, then what are we doing?
		#ifdef DEBUG
		cout << "sexp_sub: sexp is not list head!" << endl;
		#endif
		return NULL;
	}

	// at this point, we need to find the Nth part of the sexp
	sexp_t* cur = sexp->list;
	for(int i = 0; i < part; i++) {
		cur = cur->next;
		if(cur == NULL) {
			#ifdef DEBUG
			cerr << "sexp_sub: unexpectedly reached end of list" << endl;
			#endif
			return NULL;
		}
	}
	return cur;
}

/////////////////////////////////////////////////////
/// NETWORK UTILITIES
/////////////////////////////////////////////////////

int open_server_connection(char* host, char* port) {
	int sock_server;
	struct sockaddr_in addr;

	#ifdef WIN32
	WSADATA wsaData;
	WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
	#endif

	if((sock_server = static_cast<int>(socket(AF_INET, SOCK_STREAM, 0))) == SOCKET_ERROR ) {
		cerr << "Error creating socket" << endl;
		return -1;
	}

	// cover our DNS lookup stuff:
	struct hostent *h;
	if((h = gethostbyname( host )) == NULL) {
		cerr << "Unable to lookup host: " << host << endl;
		return -1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr = *((struct in_addr *)h->h_addr);
	memset(addr.sin_zero, '\0', sizeof(addr.sin_zero) );

	if( connect(sock_server, (struct sockaddr *)&addr, sizeof(addr) ) == SOCKET_ERROR)
	{
		cerr <<  "Unable to open socket!" << endl;
		cerr << "Couldn't connect to " << host << endl;
#ifdef WIN32
		cerr << "Windows Error " << WSAGetLastError() << endl;
#endif
		return -1;
	}

	rec_string(sock_server);
	send_string(sock_server, "n");

	return sock_server;
}

int send_string(int socket, string payload) {

	#ifdef SHOW_NETWORK
	cerr << "C: " << payload << endl;
	#endif

	// this function only supports the fake 'no compression' scheme
	payload = "n" + payload;

	char buffer[GLOBAL_BUFFER_SIZE];
	int numbytes;
	const char * cstr = payload.c_str();
	int offset = 0;

	// add our packet header
	int msg_len = static_cast<int>(payload.size());

	int n_msg_len = htonl(msg_len);

	// first, send the payload size (4 bytes)
   int val = send(socket, (char*)&n_msg_len, 4, 0);
	if( val != 4) {
		cerr << "Network too slow to send 4 bytes: EPIC FAIL!" << endl;
      cerr << "Sent: " << val << endl;
		return -1;
	}

	// now, keep sending chunks of the payload
	// until we send the whole thing

	while(offset < msg_len) {
		// we can't copy GLOBAL_BUFFER_SIZE bytes each time,
		// because that might push us past the end of cstr.
		// figure out which is smaller, GLOBAL_BUFFER_SIZE
		// or (payload.length() - offset)
		int bytes_to_copy = min(GLOBAL_BUFFER_SIZE, (int)(payload.length() - offset) );
		memcpy((void*)&buffer, (void*)(cstr+offset), bytes_to_copy);

		if((numbytes = send(socket, buffer, bytes_to_copy, 0)) == -1) {
			cerr << "Error sending data to  server!" << endl;
			return -1;
		}

		// update the offset to copy from
		offset += numbytes;
	}

	return 0;

}

// Takes the socket connected to the server, reads
// the network packet format, and extracts the payload
// as a string.
string rec_string(int socket) {

	char buffer[GLOBAL_BUFFER_SIZE];
	int numbytes = 0;
	string result = "";
	int msg_len = -1;
	int bytes_to_read = 0;
	int bytes_left_to_read = 0;

	// first, receive the payload size (4 bytes)
	numbytes = recv(socket, (char*)&msg_len, 4, 0);
	if( numbytes == 0) {
		cerr << "Disconnected from server!" << endl;
		// we've been disconnected, there's no point in going on
		exit(0);
	}
	// get this out of network byte order
	//memcpy((void*)&msg_len, (void*)&buffer, 4);
	msg_len = ntohl(msg_len);

	bytes_left_to_read = msg_len;


	while(bytes_left_to_read > 0) {
		numbytes = 0;
		bytes_to_read = min(GLOBAL_BUFFER_SIZE-1, bytes_left_to_read);

		if((numbytes = recv(socket, buffer, bytes_to_read, 0)) == -1) {
			cerr << "Error reading data from server!" << endl;
			return 0;
		}

		if(numbytes == 0) {
			cerr << "Disconnected from server!" << endl;
			// we've been disconnected, there's no point in going on
			exit(0);
		}

		bytes_left_to_read -= numbytes;

		buffer[numbytes] = '\0';
		result += buffer;

	}

	char compression = result.at(0);
	if(compression != 'n') {
		//cout << "Unsupported compression algorithm: " << compression << endl;
		return string("");
	}
	result = result.substr(1);

	#ifdef SHOW_NETWORK
	cout << "S: " << result << endl;
	#endif

	log_stream << result << endl;

	return result;
}

string login_to_server(int sock_server, string username, string password) {

	string login_sexp = "(login \"" + username + "\" \"" + password + "\")";
	send_string(sock_server, login_sexp);
	sexp_t* sexp = rec_sexp(sock_server);
	if(! check_sexp(sexp, "login-accepted")) {
		cerr << "Login error!" << endl;
		exit(0);
	}

   //Verify client version... useful during the tournament
   int expectedVersion = atoi(sexp->list->next->val);
   if(CLIENT_VERSION == -1)
   {
      #ifdef DEBUG
      cout << "Client version set to -1: not checking to see if updates are needed" << endl;
      #endif
   }
   else
   {
      if(expectedVersion > CLIENT_VERSION)
      {
         cerr << "ERROR! Your client is out of date - pull in the latest update package!" << endl;
         exit(0);
      }
      else if(expectedVersion < CLIENT_VERSION)
      {
         cerr << "Warning! Your client has a higher version than the server thinks exists - welcome to the future!" << endl;
      }
      else
      {
         #ifdef DEBUG
         cout << "Client version verified!" << endl;
         #endif
      }
   }

	send_string(sock_server, "(whoami)");
	sexp = rec_sexp(sock_server);
	if(!check_sexp(sexp, "who-you-are")) {
		cerr << "Can't figure out who you are..." << endl;
		return 0;
	}

	string my_user_id = sexp->list->next->next->val; // sexp_part(sexp->list, 2); <= this function never really worked...
	return my_user_id;
}

string create_game(int sock_server) {
		send_string(sock_server, "(create-game)");
		sexp_t* sexp = rec_sexp(sock_server);
		if(!check_sexp(sexp, "game-accepted")) {
			cerr << "Error creating game!" << endl;
			return "";
		}

		string game_number = sexp->list->next->val; // sexp_part(sexp->list, 1);
		cout << "Game created: # " << game_number << endl;
		return game_number;
}
