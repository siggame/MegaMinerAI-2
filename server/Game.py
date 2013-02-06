"""

   Copyright (C) 2008 by Steven Wallace, Ben Murrell
   snwallace@gmail.com, ben@benmurrell.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
"""
from Records import Records
import random
import math

MAP_HEIGHT = 26
MAP_WIDTH = 26

MAP_FOLDER = "maps/"
MAP_LIST = "mapList.dat"

#To balance the colors, edit these variables.
#BASE - The amount every unit starts with
#INCR - The amount gained per matching color (or pair of color)
HP_BASE = 50
HP_INCR = 20
RANGE_BASE = 1
RANGE_INCR = 1
ATTACK_BASE = 20
ATTACK_INCR = 8
MOVE_BASE = 2
MOVE_INCR = 1
HEAL_BASE = 0
HEAL_INCR = 8
SPLASH_BASE = 0
SPLASH_INCR = 4
POISON_BASE = 0
POISON_INCR = 6
REGEN_BASE = 0
REGEN_INCR = 3

SPLASH_RADIUS = 1

MAX_HEAL_COEFF = 1.5

class BuildZone:
    id = 0
    x = 0
    y = 0

    def __init__(self, x, y, id):
        self.x = x
        self.y = y
        self.id = id

    def toList(self):
        list = [self.id, self.x, self.y]
        return list

class Wall:
    id = 0
    x = 0
    y = 0
    def __init__(self, x, y, id):
        self.x = x
        self.y = y
        self.id = id

    def toList(self):
        list = [self.id, self.x, self.y]
        return list

class Unit:
    id = 0
    x = 0
    y = 0
    green = 0
    yellow = 0
    red = 0
    blue = 0    
    buildPoints = 0
    hp = 0
    player = 0
    moves = 0
    poisonDmg = 0
    actions = 0
    kills = 0
    lastAttackerID = 0

    def __init__(self, id, player, x, y, red, green, blue, yellow):
        self.x = x
        self.y = y
        self.player = player
        self.buildPoints = 0
        self.poisonDmg = 0

        self.red = red
        self.blue = blue
        self.green = green
        self.yellow = yellow
        
        self.hp = self.maxHP()
        self.moves = 0
        self.actions = 0
        self.id = id
        self.kills = 0
        self.lastAttackerID = 0
        
    def maxHP(self):
        return ((HP_INCR * self.green) + HP_BASE)
        
    def range(self):
        return ((RANGE_INCR * self.yellow) + RANGE_BASE)
        
    def attack(self):
        return ((ATTACK_INCR * self.red) + ATTACK_BASE)
        
    def maxMoves(self):
        return ((MOVE_INCR * self.blue) + MOVE_BASE)
        
    def heal(self):
        return ((HEAL_INCR * min(self.green, self.yellow)) + HEAL_BASE)
        
    def splash(self):
        return ((SPLASH_INCR * min(self.yellow, self.red)) + SPLASH_BASE)
        
    def poisonAtt(self):
        return ((POISON_INCR * min(self.red, self.blue)) + POISON_BASE)

    def regen(self):
        return ((REGEN_INCR * min(self.blue, self.green)) + POISON_BASE)

    def level(self):
        return (self.red + self.blue + self.green + self.yellow)

    def colors(self):
        colors = 0
        if (self.red > 0):
            colors += 1
        if (self.green > 0):
            colors += 1
        if (self.blue > 0):
            colors += 1
        if (self.yellow > 0):
            colors += 1
        return colors

    def maxBuildPoints(self):
        return (self.level() * 3)
        
    def newTurn(self, myTurn):
        if myTurn:
            #poison
            self.hp -= self.poisonDmg
            self.poisonDmg = math.floor(self.poisonDmg / 2)

            #regen
            self.hp = min(self.hp + self.regen(), max(self.hp, self.maxHP()))

            #if self.hp + self.regen() <= self.maxHP():
            #    self.hp += self.regen()
            #else:
            #    self.hp = min(self.maxHP(), self.hp)

            self.moves = self.maxMoves()
            self.actions = 1
        else:
            self.moves = 0
            self.actions = 0
        return True
    
    def toList(self):
        list = [self.id, self.hp, self.x, self.y, self.buildPoints,
          self.poisonDmg, self.red, self.blue, self.green, self.yellow]
        return list
    
    def giveBuildPoint(self):
        if self.buildPoints < self.maxBuildPoints():
            self.buildPoints += 1

    
class Game:
    def __init__(self, id):
        self.players = [] #list of player objects
        self.units = []
        self.buildZones = []
        self.walls = []
        self.turn = None #the player whose turn it is; None before and after the game.
        self.turnNum = 0
        self.maxTurns = 400
        self.id = id
        self.winner = None #the player who won, set after the game  is ended
        self.unitID = 0
        self.zoneID = 0
        self.wallID = 0
        self.animations = []
        random.seed()
    
    #Gameplay functions
    def createBuildZone(self, x, y):
        self.zoneID += 1
        self.buildZones.append(BuildZone(x, y, self.zoneID))
        return True

    def createWall(self, x, y):
        self.wallID += 1
        self.walls.append(Wall(x, y, self.wallID))
        return True

    def addUnit(self, player, x, y, red, green, blue, yellow):
        if self.unitAt(x, y):
             return False
        self.unitID += 1
        self.units.append(Unit(self.unitID, player, x, y, red, green, blue, yellow))
        self.units[-1].moves = 0
        self.animations.append(["appear", self.units[-1].toList(), player.ID])
        return True        

    def buildUnit(self, builderID, x, y):
        if not self.getUnit(builderID): #check whether the unit exists
            self.animations.append(["game-build-denied", str(builderID)+" does not exist"])
            return (str(builderID)+" does not exist")
        unit = self.getUnit(builderID)

        if (max(abs(unit.x - x) , abs(unit.y - y))) > 1:
            self.animations.append(["game-build-denied", str(builderID)+" cannot build to non-adjaced square", builderID, x , y])
            return (str(builderID)+" cannot build to non-adjaced square") #bad move
        if self.unitAt(x, y):
            self.animations.append(["game-build-denied", str(builderID)+" cannot build onto other units", builderID, x , y])
            return (str(builderID)+" cannot build onto other units") #occupied
        if self.wallAt(x, y):
            self.animations.append(["game-build-denied", str(builderID)+" cannot build onto walls", builderID, x , y])
            return (str(builderID)+" cannot build onto walls") #walls
        if unit.actions < 1:
            self.animations.append(["game-build-denied", str(builderID)+" is out of actions for this turn", builderID, x , y])
            return (str(builderID)+" is out of actions for this turn") #no more actions
        if unit.player != self.turn:
            self.animations.append(["game-build-denied", str(builderID)+" is not your unit", builderID, x , y])
            return (str(builderID)+" is not your unit")#not your unit!
        if x > 25 or x < 0 or y > 25 or y < 0:
            self.animations.append(["game-build-denied", str(builderID)+" cannot build out of bounds", builderID, x , y])
            return (str(builderID)+" cannot build out of bounds") #out of ranges
        if unit.buildPoints < unit.maxBuildPoints():
            self.animations.append(["game-build-denied", str(builderID)+" is not at max build points", builderID, x , y])
            return (str(builderID)+" is not at max build points") #not enough money
        unit.actions -= 1
        unit.buildPoints = 0
        self.addUnit(unit.player, x, y, unit.yellow, unit.blue, unit.red, unit.green)
        self.animations.append(["build", builderID, x, y])
        return True

    def combineUnits(self, id, targetId):
        if not self.getUnit(targetId): #check whether the unit exists
            self.animations.append(["game-combine-denied", str(targetId)+" does not exist, cannot be targeted for combine", id, targetId])
            return (str(targetId)+" does not exist, cannot be targeted for combine")
        target = self.getUnit(targetId)
        return self.combineToLocation(id, target.x, target.y)


    def combineToLocation(self, id, x, y):
        if not self.getUnit(id): #check whether the unit exists
            self.animations.append(["game-combine-denied", str(id)+" does not exist", id, x, y])
            return (str(id)+" does not exist")
        unit = self.getUnit(id)

        if (unit.x == x and unit.y == y):
            self.animations.append(["game-combine-denied", str(id)+" cannot combine with yourself", id, x, y])
	    return (str(id)+" cannot combine with yourself") #self combine
        if (max(abs(unit.x - x) , abs(unit.y - y))) > 1:
            self.animations.append(["game-combine-denied", str(id)+" cannot combine to non-adjaced square", id, x, y])
            return (str(id)+" cannot combine to non-adjaced square") #bad move
        if x > 25 or x < 0 or y > 25 or y < 0:
            self.animations.append(["game-combine-denied", str(id)+" cannot combine out of bounds", id, x, y])
            return (str(id)+" cannot combine out of bounds") #out of ranges
        if not self.squareContainsAllies(x, y):
            self.animations.append(["game-combine-denied", str(id)+" must combine with allied units", id, x, y])
            return (str(id)+" must combine with allied units") #occupied
        if self.wallAt(x, y):
            self.animations.append(["game-combine-denied", str(id)+" cannot combine with a wall", id, x, y])
            return (str(id)+" cannot combine with a wall") #walls
        if unit.actions < 1:
            self.animations.append(["game-combine-denied", str(id)+" is out of actions for this turn. (attmepted to combine)", id, x, y])
            return (str(id)+" is out of actions for this turn. (attmepted to combine)") #no more actions
        if unit.player != self.turn:
            self.animations.append(["game-combine-denied", str(id)+" is not your unit. (attempted to combine)", id, x, y])
            return (str(id)+" is not your unit. (attempted to combine)")#not your unit!
        
        target = self.unitAt(x, y)
        if target.actions < 1:
            self.animations.append(["game-combine-denied", str(target.id)+" is out of actions for this turn and cannot be targeted for combine", id, x, y])
            return (str(target.id)+" is out of actions for this turn and cannot be targeted for combine") #no more actions

        target.red += unit.red
        target.green += unit.green
        target.blue += unit.blue
        target.yellow += unit.yellow
        target.actions -= 1

        self.animations.append(["combine", id, target.id])

        #TODO: Figure out how much health should transfer to the target
        target.hp += min(HP_INCR * unit.green, unit.hp)
        self.checkHP(target)

        #Kill the unit that used the combine ability.
        #unit.hp = -999
        #self.checkHP(unit)
        self.units.remove(unit)

        for i in self.players:
            if (i == self.turn):
                i.r_level = max(target.level(), i.r_level)
                if (target.colors() == 1):
                    i.r_pureLevel = max(target.level(), i.r_pureLevel)
                i.r_combined += 1
                

        return True

    def moveUnitToPoint(self, id, x, y):
        if not self.getUnit(id): #check whether the unit exists
            self.animations.append(["game-move-denied", str(id)+" does not exist, cannot move", id, x, y])
            return (str(id)+" does not exist, cannot move")
        unit = self.getUnit(id)
        if self.disUnitToSquare(unit, x, y) > 1:
            self.animations.append(["game-move-denied", str(id)+" cannot move to non-adjaced square", id, x, y])
            return (str(id)+" cannot move to non-adjaced square") #bad move
        if self.unitAt(x, y):
            self.animations.append(["game-move-denied", str(id)+" cannot move onto other units", id, x, y])
            return (str(id)+" cannot move onto other units") #occupied
        if self.wallAt(x, y):
            self.animations.append(["game-move-denied", str(id)+" ran head first into a wall", id, x, y])
            return (str(id)+" ran head first into a wall") #walls
        if unit.moves < 1:
            self.animations.append(["game-move-denied", str(id)+" is out of moves for this turn, cannot move", id, x, y])
            return (str(id)+" is out of moves for this turn, cannot move") #cannot move
        if unit.player != self.turn:
            self.animations.append(["game-move-denied", str(id)+" is not your unit, cannot be moved", id, x, y])
            return (str(id)+" is not your unit, cannot be moved")#not your unit!
        if x > 25 or x < 0 or y > 25 or y < 0:
            self.animations.append(["game-move-denied", str(id)+" cannot move out of bounds", id, x, y])
            return (str(id)+" cannot move out of bounds") #out of ranges
        unit.moves -= 1
        unit.x = x
        unit.y = y
        self.animations.append(["move", id, x, y])

        for i in self.players:
            if (i == self.turn):
                i.r_moved += 1

        return True
    
    def moveUnit(self, id, direction):
        if not self.getUnit(id): #check whether the unit exists
            return False
        unit = self.getUnit(id)
        x = unit.x
        y = unit.y
        if direction & 1:
            if direction & 2:
                y -= 1
            else:
                y += 1
        else:
            if direction & 2:
                x -= 1
            else:
                x += 1
        return self.moveUnitToPoint(unit, id, x, y)
    
    def attack(self, id, x, y):
        if not self.getUnit(id): #check whether the unit exists
            self.animations.append(["game-attack-denied", str(id)+" does not exist, cannot attack", id, x, y])
            return (str(id)+" does not exist, cannot attack")
        attacker = self.getUnit(id)
        
        #Check attack range
        if (self.disUnitToSquare(attacker, x, y) > attacker.range):
            self.animations.append(["game-attack-denied", str(id)+"", id, x, y])
            return (str(id)+" tried to attack out of range")

        #Units can not use a ranged attack and move in the same turn
        if (self.disUnitToSquare(attacker, x, y) > 1 and attacker.moves < attacker.maxMoves()):
            self.animations.append(["game-attack-denied", str(id)+" can not use a ranged attack because it has moved this turn", id, x, y])
            return (str(id)+" can not use a ranged attack because it has moved this turn")

        if attacker.actions < 1:
            self.animations.append(["game-attack-denied", str(id)+" is out of actions, cannot attack", id, x, y])
            return (str(id)+" is out of actions, cannot attack") #cannot move

        if attacker.player != self.turn:
            self.animations.append(["game-attack-denied", str(id)+" is not your unit, cannot attack", id, x, y])
            return (str(id)+" is not your unit, cannot attack") #not your unit!

        attacker.actions -= 1
        attacker.moves = 0

        healMode = self.squareContainsAllies(x, y)

        for target in self.unitsAt(x, y):
            #target.poisonDmg += attacker.poisonAtt()
            #target.hp -= attacker.damage()
            if healMode:
                target.hp += attacker.heal()
                self.animations.append(["healed", id, target.hp])
            else:
                target.poisonDmg += attacker.poisonAtt()
                target.hp -= attacker.attack()
                self.animations.append(["hurt", id, target.hp])


            self.checkHP(target)
        
        #Splash
        if not healMode:
            for enemy in self.enemiesInSplash(x, y, SPLASH_RADIUS):
                enemy.hp -= attacker.splash()
                self.animations.append(["hurt", id, enemy.hp])
                self.checkHP(enemy)
        
        self.animations.append(["attack", id, x, y])
        return True

    def checkHP(self, unit):
        maxOverHeal = int(math.ceil(unit.maxHP()*MAX_HEAL_COEFF))
        if unit.hp <= 0:

            #update unit kill counts
            attacker = self.getUnit(unit.lastAttackerID)
            if attacker:
                attacker.kills += 1
                for i in self.players:
                    if (i == attacker.owner):
                        i.r_singleUnitKills = max(i.r_singleUnitKills, attacker.kills)

            self.units.remove(unit)
            self.animations.append(["die", unit.id])
            self.killsThisTurn += 1
        if unit.hp > maxOverHeal:
            unit.hp = maxOverHeal

    def squareContainsAllies(self, x, y):
        for u in self.unitsAt(x, y):
            if u.player == self.turn:
                return True
            return False

#    def unitHitsEnemySquare(self, attacker, x, y):
#        for u in self.unitsAt(x, y):
#            u.poisonDmg += attacker.poison()
#            u.hp -= attacker.damage()

    def disUnitToUnit(self, unit1, unit2):
        return self.distance(unit1.x, unit1.y, unit2.x, unit2.y)

    def disUnitToSquare(self, unit, x, y):
        return self.distance(unit.x, unit.y, x, y)

    def distance(self, x1, y1, x2, y2):
        dis = abs(x1 - x2) + abs(y1 - y2)
        return dis

    def enemiesInSplash(self, x, y, radius):     
        targets = []
        for u in self.units:
            if (u.player != self.turn) and (self.disUnitToSquare(u, x, y) <= radius) and (self.disUnitToSquare(u, x, y) > 0 ):
                targets.append(u)
        return targets
   
    
    #administrative functions
    def addPlayer(self, player):
        registered = False
        allowed = False

        if len(self.players) < 2:
            self.players.append(player)
            player.resources = 0
            allowed = True
            
        return allowed
    
    def removePlayer(self, player):
        try:
            self.players.remove(player)
            if self.turn:
                self.turn = None
                self.winner = self.players[0] #the remaining player
                for i in self.players:
                    i.writeSExpr(['game-over', self.id])
                print "Game " + `self.id` + " ended! Winner:" + self.winner.user
            return True
        except:
            return False
    
    def unitsForPlayer(self, player):
        list = [i for i in self.units if i.player == player]
        return list
    
    def unitListsForPlayer(self, player):
        list = self.unitsForPlayer(player)
        return [i.toList() for i in list]

    def initializeUnits(self):
        #                             x  y  r  g  b  yel
        self.addUnit(self.players[0], 0, 0, 1, 0, 0, 0)
        self.addUnit(self.players[0], 1, 0, 0, 1, 0, 0)

        self.addUnit(self.players[1], 24, 25, 0, 1, 0, 0)
        self.addUnit(self.players[1], 25, 25, 1, 0, 0, 0)
        self.unitAt(25, 25).giveBuildPoint()

        #self.addUnit(self.players[1], 0, 1, 0, 1, 0, 0)
        #self.addUnit(self.players[1], 1, 1, 0, 1, 0, 0)
        #self.unitAt(1, 1).giveBuildPoint()
        #self.unitAt(1, 1).giveBuildPoint()
        #self.unitAt(1, 1).giveBuildPoint()
    
    def start(self):
        if len(self.players) != 2 or self.turn or self.winner:
            return False
        for i in self.players:
            i.score = 0

        self.killsThisTurn = 0

        self.loadMap()        
        self.initializeUnits()
        self.initializeRecords()
        
        self.turn = self.players[1]
        self.turnNum = 0
        self.sendIdent(self.players)
        self.sendConstants()
        self.sendFormulas()
        self.sendWalls()
        self.sendZones()
        self.nextTurn()
        return True

    def initializeRecords(self):
        for i in self.players:
            i.r_level = 1
            i.r_pureLevel = 1
            i.r_buildPoints = 0
            i.r_built = 0
            i.r_combined = 0
            i.r_moved = 0
            i.r_kills = 0
            i.r_turnKills = 0
            i.r_singleUnitKills = 0
            i.r_gamesWon = 0
            i.r_gamesLost = 0

    def sendWalls(self):
        wallList = []
        wallList.append('walls')
        for i in self.walls:
            wallList.append(i.toList())
        for i in self.players:
            i.writeSExpr(wallList)

    def sendZones(self):
        zoneList = []
        zoneList.append('buildZones')
        for i in self.buildZones:
            zoneList.append(i.toList())
        for i in self.players:
            i.writeSExpr(zoneList)
    
    def sendStatus(self, players):
        if self.turn == None:
            list = [None]
        else:
            list = [self.turnNum]
           
            wallList = []
            zoneList = []
            for i in self.players:
                list += [[i.ID, i.score, self.unitListsForPlayer(i)]]
            #for i in self.buildZones:
            #    zoneList += [i.toList()]
            #list += [zoneList]
            #for i in self.walls:
            #    wallList += [i.toList()]
            #list += [wallList]
        for i in players:
            i.writeSExpr(['status', list])
                
    def sendIdent(self, players):
        if len(self.players) != 2:
            return False #bad bad bad
        list = []
        for i in self.players:
            list += [[i.ID, i.user]]
            
        for i in players:
            i.writeSExpr(['ident', list])

    def listPlayers(self):
        list = []
        for i in self.players:
            list.append(i.user)
        return list

    def sendConstants(self):
        list = []
        list += [HP_BASE, HP_INCR, RANGE_BASE,
               RANGE_INCR, ATTACK_BASE, ATTACK_INCR, MOVE_BASE, MOVE_INCR,
               HEAL_BASE, HEAL_INCR, SPLASH_BASE, SPLASH_INCR,
               POISON_BASE, POISON_INCR, REGEN_BASE, REGEN_INCR,
               SPLASH_RADIUS, MAX_HEAL_COEFF]
        for i in self.players:
            i.writeSExpr(['constants', list])
        return True
                        
    def sendFormulas(self):
        list = []
        list.append(['maxHP', '(' + str(HP_INCR) + '*G)+' + str(HP_BASE) ])
        list.append(['range', '(' + str(RANGE_INCR) + '*Y)+' + str(RANGE_BASE) ])
        list.append(['attack', '(' + str(ATTACK_INCR) + '*R)+' + str(ATTACK_BASE) ])
        list.append(['move', '(' + str(MOVE_INCR) + '*B)+' + str(MOVE_BASE) ])
        list.append(['heal', '(' + str(HEAL_INCR) + '*min(G, Y))+' + str(HEAL_BASE) ])
        list.append(['splash', '(' + str(SPLASH_INCR) + '*min(Y, R))+' + str(SPLASH_BASE) ])
        list.append(['poison', '(' + str(POISON_INCR) + '*min(R, B))+' + str(POISON_BASE) ])
        list.append(['regen', '(' + str(REGEN_INCR) + '*min(B, G))+' + str(REGEN_BASE) ])
        list.append(['level', 'R+B+G+Y'])

        for i in self.players:
            i.writeSExpr(['formulas', list])
        return True
  
    #utility functions
    def chat(self, player, message):
        for i in self.players:
            i.writeSExpr(['says', player.user, message])
        return True

    def giveBuildPoints(self):
        for z in self.buildZones:
            for u in self.unitsAt(z.x, z.y):
                if u in self.unitsForPlayer(self.turn):
                    u.giveBuildPoint()
                    u.player.score += 1

    
    def nextTurn(self):
        if (not self.turn) or self.winner:
            return False

        for i in self.players:
            if (i == self.turn):
                i.r_turnKills = max(i.r_turnKills, self.killsThisTurn)
                i.r_kills += self.killsThisTurn
        self.killsThisTurn = 0

        self.turn = [i for i in self.players if i != self.turn][0]
        for i in self.units:
            i.newTurn(self.turn == i.player)
            self.checkHP(i) #must check hp because unit.newTurn() causes poison and regen to occur.
        self.turnNum += 1
        
        self.giveBuildPoints()

        unitsFor0 = len(self.unitsForPlayer(self.players[0]))
        unitsFor1 = len(self.unitsForPlayer(self.players[1]))

        if (self.turnNum >= self.maxTurns) or (unitsFor0 == 0)  or (unitsFor1 == 0):
            
            if (unitsFor0 == 0):
                self.players[0].score = 0
            if (unitsFor1 == 0):
                self.players[1].score = 0

            if self.players[0].score > self.players[1].score:
                self.winner = self.players[0]
                self.players[0].r_gamesWon = 1
                self.players[1].r_gamesLost = 1
            else:
                self.winner = self.players[1]
                self.players[0].r_gamesLost = 1
                self.players[1].r_gamesWon = 1

            #Records
	    practice = (self.players[0].user == self.players[1].user)

            if not practice:
                myRecords = Records()
                myRecords.addGameData(self)

                for i in self.players:
                    for j in self.players:
                        i.writeSExpr(['records', myRecords.showScores(2, j.user)])
                    i.writeSExpr(['records', myRecords.showScores(1, i.user)])
                    i.writeSExpr(['records', myRecords.showScores(0, i.user)])
                    i.writeSExpr(['records', myRecords.showScores(0)])

            #self.sendStatus(self.players)

            for i in self.players:
                i.writeSExpr(['game-over', self.winner.ID])

            for i in self.players:
                print "Game " + `self.id` + " ended! Winner:" + self.winner.user
                
            self.sendStatus(self.players)

            for i in self.players:
                i.writeSExpr(['animations', self.animations])

            self.turn = None
            return True


        for i in self.players:
            i.writeSExpr(['new-turn', self.turn.ID])
        self.sendStatus(self.players)

        for i in self.players:
            i.writeSExpr(['animations', self.animations])
        self.animations = []
        return True
      
    
    def unitAt(self, x, y):
        unit = [i for i in self.units if i.x == x and i.y == y]
        if not unit:
            return False
        return unit[0]

    def wallAt(self, x, y):
        wall = [i for i in self.walls if i.x == x and i.y == y]
        if not wall:
            return False
        return wall
    
    def unitsAt(self, x, y):
        unit = [i for i in self.units if i.x == x and i.y == y]
        #if not unit:
        #   return False
        return unit
    
    def zoneAt(self, x, y):
        zone = [i for i in self.buildZones if i.x == x and i.y == y]
        if not zone:
            return False
        return node[0]
        
    def getZone(self, id):
        zone = [i for i in self.buildZones if i.id == id]
        if not zone:
            return False
        return node[0]
    
    def getUnit(self, id):
        unit = [i for i in self.units if i.id == id]
        if not unit:
            return False
        return unit[0]


    def loadMap(self):
        mapList = []
        infile = open(MAP_FOLDER + MAP_LIST)
        mapList = infile.readlines()

        for map in mapList:
            if (len(map) == 1):
                mapList.remove(map)

        for i in range(0, len(mapList)):
            mapList[i] = mapList[i].strip()
            #print ("Mapname:'" + mapList[i] + "'")

        if (len(mapList) == 0):
            return "Map list is empty."

        mapNumber = random.randint(0, len(mapList) - 1)
        #mapNumber = 3 #money map
        #mapNumber = 4 # maze map
        for i in self.players:
            i.writeSExpr(['map', mapNumber])

        #print (mapNumber)
        print ("'" + mapList[mapNumber] + "'")
        self.readMapFile(MAP_FOLDER + mapList[mapNumber])

    def readMapFile(self, mapFile):
        infile = open(mapFile)
        lineList = infile.readlines()
    
        for y in range(0, len(lineList)):
            print(lineList[y].strip())
            if (len(lineList[y]) > 2):
                for x in range(0, MAP_WIDTH):
                    cell = lineList[y][2*x]
                    if (cell == 'O'):
                        self.createBuildZone(x,25-y)
                    elif (cell == '#'):
                        self.createWall(x,25-y)

