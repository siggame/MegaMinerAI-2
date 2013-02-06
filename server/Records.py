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
import math
import os.path

RECORDS_FOLDER = "records/"
LAST_FOLDER = "lastGame/"
ANY_FOLDER = "anyGame/"
ALL_FOLDER = "allGames/"
PLAYERS_FOLDER = "players/"
HIGH_SCORES_FILE = "highScores.dat"
HOLDERS_FILE = "recordHolders.dat"

SCORE_TYPES = []
#ACHIEVEMENTS = []
#Mode 0 : all
#Mode 1 : any
#Mode 2 : latest

#                   Description, Score can be summed across all games?
SCORE_TYPES.append(["Unit level",                          False])
#ACHIEVEMENTS.append([])
SCORE_TYPES.append(["Pure unit level",		           False])
SCORE_TYPES.append(["Build points collected",              True ])
SCORE_TYPES.append(["Units built",                         True ])
SCORE_TYPES.append(["Units combined",                      True ])
SCORE_TYPES.append(["Distance moved by all units",         True ])
SCORE_TYPES.append(["Units killed",                        True ])
SCORE_TYPES.append(["Units killed in a single turn",       False])
SCORE_TYPES.append(["Units killed by a single unit",       False])
SCORE_TYPES.append(["Games won",                           True ])
SCORE_TYPES.append(["Games lost",                           True ])

class Records:
    def __init__(self):
        folderList = []
        folderList.append(RECORDS_FOLDER)
        folderList.append(RECORDS_FOLDER + LAST_FOLDER)
        folderList.append(RECORDS_FOLDER + LAST_FOLDER + PLAYERS_FOLDER)
        folderList.append(RECORDS_FOLDER + ANY_FOLDER)
        folderList.append(RECORDS_FOLDER + ANY_FOLDER + PLAYERS_FOLDER)
        folderList.append(RECORDS_FOLDER + ALL_FOLDER)
        folderList.append(RECORDS_FOLDER + ALL_FOLDER + PLAYERS_FOLDER)

        for f in folderList:
            if not(os.path.exists(f)):
                os.mkdir(f)


    def addGameData(self, game):
        for i in game.players:
            self.addPlayerData(i)

    def addPlayerData(self, player):
        newScores = self.createListForPlayer(player)

        self.updateLast(player.user, newScores)
        self.updateAny(player.user, newScores)
        self.updateAll(player.user, newScores)

    #Update the records pertaining to the most recently played games
    def updateLast(self, username, newScores):
        self.writeListFile(RECORDS_FOLDER + LAST_FOLDER + PLAYERS_FOLDER + username + ".dat", newScores)

    #Updates the records pertaining to the best individual games
    def updateAny(self, username, newScores):
       
        maxScores = []
        oldScores = []
        baseFolder = RECORDS_FOLDER + ANY_FOLDER
        playerFile = baseFolder + PLAYERS_FOLDER + username + ".dat"
      
        oldScores = self.readScoresFile(playerFile)
     

        for i in range(0, len(oldScores)):
            maxScores.append(max(oldScores[i], int(newScores[i].strip())))

 
        self.writeListFile(playerFile, maxScores)

        self.updateHighScores(baseFolder, username, maxScores)


    #Updates the records pertaining to all games ever played
    def updateAll(self, username, newScores):
        maxScores = []
        oldScores = []
        baseFolder = RECORDS_FOLDER + ALL_FOLDER
        playerFile = baseFolder + PLAYERS_FOLDER + username + ".dat"
        oldScores = self.readScoresFile(playerFile)

        for i in range(0, len(oldScores)):
            if (SCORE_TYPES[i][1]):
                #Score represents a running total over all
                #games ever played
                maxScores.append(int(oldScores[i]) + int(newScores[i]))
            else:
                #Score represents the best done in a single game
                #and should be the same as in the 'any' folder
                maxScores.append(max(oldScores[i], newScores[i]))
        self.writeListFile(playerFile, maxScores)

        self.updateHighScores(baseFolder, username, maxScores)


    def updateHighScores(self, baseFolder, username, newScores):
        highScoresFile = baseFolder + HIGH_SCORES_FILE
        holdersFile = baseFolder + HOLDERS_FILE
        holders = self.readHoldersFile(holdersFile)
        highScores = self.readScoresFile(highScoresFile)      

        for i in range(0, len(highScores)):

            if (newScores[i] > highScores[i]):
                highScores[i] = newScores[i]
                holders[i] = username
        
        self.writeListFile(highScoresFile, highScores)
        self.writeListFile(holdersFile, holders)

    def createListForPlayer(self, player):
        myList = [] 

        myList.append(player.r_level)
        myList.append(player.r_pureLevel)
        myList.append(player.r_buildPoints)
        myList.append(player.r_built)
        myList.append(player.r_combined)
        myList.append(player.r_moved)
        myList.append(player.r_kills)
        myList.append(player.r_turnKills)
        myList.append(player.r_singleUnitKills)
        myList.append(player.r_gamesWon)
        myList.append(player.r_gamesLost)

        return myList

    def writeListFile(self, file, mylist):
        outFile = open(file, "w")

        for i in range(0, len(mylist)):

            mylist[i] = str(mylist[i]).strip() + '\n'



        outFile.writelines(mylist)
        outFile.close()

    def readScoresFile(self, file):
        myScores = []

        if os.path.isfile(file):

            infile = open(file, "r")
            myScores = infile.readlines()
            for i in range(0, len(myScores)):

                myScores[i] = int(myScores[i].strip())
            infile.close()
        else:

            for i in range(0, len(SCORE_TYPES)):

                myScores.append(0)

        return myScores

    def readHoldersFile(self, file):

        holders = []
        if os.path.isfile(file):
            infile = open(file, "r")
            holders = infile.readlines()
            for i in range(0, len(holders)):
                holders[i] = holders[i].strip()
            infile.close()
        else:
            for i in range(0, len(SCORE_TYPES)):
                holders.append("")    

        return holders

    def menu(self):
        #Mode 0 : all
        #Mode 1 : any
        #Mode 2 : latest
        choice = -1
        while not choice == 0:
            print "=== Records - Main Menu ==="
            print "1. See leader board for all games"
            print "2. See leader board for individual games"
            print "3. See records for a particular player for all games"
            print "4. See records for a particular player for individual games"
            print "5. See records for a particular player for their latest game"
            print "6591. Clear all records"
            print "---------------------------"
            print "0. Exit"
            choice = input()

            if (choice >= 3 and choice <= 5):
                username = raw_input("User name:")

            if (choice == 1):
                print self.showScores(0)
            elif (choice == 2):
                print self.showScores(1)
            elif (choice == 3):
                print self.showScores(0, username)
            elif (choice == 4):
                print self.showScores(1, username)
            elif (choice == 5):
                print self.showScores(2, username)
            elif (choice == 6591):
                self.clearScores()

    def showScores(self, mode = 0, username = ""):
        scoreList = []
        holders = []
        scoreFile = RECORDS_FOLDER
        header = ""
        line = ""
        output = ""

        if (mode == 0):
            scoreFile += ALL_FOLDER
            header += "Lifetime Scores"
        elif(mode == 1):
            header += "Single Game Scores"
            scoreFile += ANY_FOLDER
        elif(mode == 2):
            header += "Most recent game results"
            scoreFile += LAST_FOLDER

        if (username == ""):
            header += " - Leader board"
            holders = self.readHoldersFile(scoreFile + HOLDERS_FILE)
            scoreFile += HIGH_SCORES_FILE
        else:
            header += " - " + username
            scoreFile += PLAYERS_FOLDER + username + ".dat"

        scoreList = self.readScoresFile(scoreFile)
        output += '\n\n' + "=====================" + '\n'
        output += header + '\n'
        output += "---------------------" + '\n'
        #print
        #print
        #print "====================="
        #print header
        #print "---------------------"
        for i in range(0, len(SCORE_TYPES)):
            line = ""

            if (SCORE_TYPES[i][1] and mode == 0):
                line += "Total "
            else:
                line += "Max "

            line += SCORE_TYPES[i][0]
            line = line.ljust(50)
            line += str(scoreList[i])
            line = line.ljust(60)

            if i < len(holders):
                line += holders[i]

            output += line + '\n'
            #print line

        output += '\n\n'
        #print
        #print
        return output

    def clearScores(self):
        print "=========================================================="
        print "|                  Woah Woah Woah!                       |"
        print "|                                                        |"
        print "| YOU ARE ABOUT TO DELETE ALL THE SCORES ON THIS SERVER! |"
        print "=========================================================="
        print "Are you absolutely sure you want to do this?"
        confirm = raw_input("(y or n):")
        if (confirm == "y"):
            os.system("rm -rf " + RECORDS_FOLDER)
            print "Fine.  It is done."
        else:
            print "Canceled.  Good Choice."
       
#r = Records()
#r.menu()
