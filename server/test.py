"""

   Copyright (C) 2008 by Steven Wallace
   snwallace@gmail.com

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
from networking.Server import *
from networking.Filter import *

import sexpr

ID = 0

REGISTERED_USERS = []
REGISTERED_USERS.append(["Shell AI", "password"])
REGISTERED_USERS.append(["Mues", "6E72091D6DDD974"])
REGISTERED_USERS.append(["elements1", "nspi5hLpC"])
REGISTERED_USERS.append(["elements2", "L0cvuS0DS"])
REGISTERED_USERS.append(["elements3", "u0V9F4K6un"])
REGISTERED_USERS.append(["elements4", "He2UgVK"])
REGISTERED_USERS.append(["elements5", "JcnKLKMime"])
REGISTERED_USERS.append(["elements6", "xN77J4Ssaa"])
REGISTERED_USERS.append(["elements7", "m5asHmjzSp"])
REGISTERED_USERS.append(["elements8", "xtNYuzH"])
REGISTERED_USERS.append(["elements9", "nhxYbFcEc"])
REGISTERED_USERS.append(["elements10", "mCXaduB6"])
REGISTERED_USERS.append(["elements11", "1zVihYP0"])
REGISTERED_USERS.append(["elements12", "tg2b3HTWE"])
REGISTERED_USERS.append(["elements13", "iAUXAGR"])
REGISTERED_USERS.append(["elements14", "g7G2XvrB"])
REGISTERED_USERS.append(["elements15", "vRCKytsrpN"])
#REGISTERED_USERS.append(["Team 16", "8CA419F10FB09FA"])


REGISTERED_USERS.append(["The Rain", "nspi5hLpC"])
REGISTERED_USERS.append(["The Cake is a Lie", "L0cvuS0DS"])
REGISTERED_USERS.append(["Shroom", "u0V9F4K6un"])
REGISTERED_USERS.append(["Twos Complement", "He2UgVK"])
REGISTERED_USERS.append(["Going Solo", "JcnKLKMime"])
REGISTERED_USERS.append(["Going Solo", "xN77J4Ssaa"])
REGISTERED_USERS.append(["More of a Crucible", "m5asHmjzSp"])
REGISTERED_USERS.append(["Maelstrom", "xtNYuzH"])


class GameServerFilter(Filter):
    def initialize(self, *args):
        self.user = None
        self.password = None
        self.hash = None
        self.game = None
        global ID
        ID += 1
        self.ID = ID
    
    def writeSExpr(self, list):
        self.writeOut(sexpr.sexpr2str(list))
    
    def readOut(self, data):
        #print "Receiving message from ", self.ID, ": ", data
        try:
            self.readSExpr(sexpr.str2sexpr(data))
        except ValueError:
            self.writeSExpr(['malformed-message', data])

    def writeOut(self, data):
        #print "Sending message to ", self.ID, ": ", data
        Filter.writeOut(self, data)        

    def disconnect(self):
        print self.ID, "disconnect"
        if self.game:
            pass
            #self.readSExpr("(leave-game)")

    def readSExpr(self, expression):
        for i in expression:
            self.evalStatement(i)
    def evalStatement(self, expression):
        if type(expression) != list:
            self.writeSExpr(['invalid-expression', expression])
            return False
        try:
            statements[expression[0]](self, expression)
        except (KeyError, IndexError):
            self.writeSExpr(['malformed-statement', expression])
    
    
    def login(self, user, password):
        registered = False
        output = ""
        for r in REGISTERED_USERS:
            if r[0] == user and r[1] == password:
                registered = True
                self.user = user
                self.password = password

        if not registered:
            output = "user    :" + user + '\n'
            output += "password:" + password + '\n\n'
            outFile = open("badLogin.dat", "a")
            outFile.write(output)
            outFile.close()

        return registered
   
    def logout(self):
        self.user = None
        self.password = None
        self.hash = None
        return True
        

from Statements import statements

server = TCPServer(19000, PacketizerFilter(), CompressionFilter(), GameServerFilter())
server.run()
