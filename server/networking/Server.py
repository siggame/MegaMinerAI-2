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
import socket
import select
import threading

import Filter

class Server:
    def __init__(self):
        self.__stop = False
        self.server = False
        self.lock = threading.Lock()
        
    def run(self):
        self.__stop = False
        while not self.__stop:
            self.poll()

    def stop(self):
        self.__stop = True

    def poll(self):
        pass

class MultiServer(Server):
    """ Server to control several other servers at once"""
    def __init__(self, *servers):
        Server.__init__(self)
        self.__servers = list(servers)

    def addServer(self, server):
        if not server in self.servers:
            self.servers.append(server)

    def removeServer(self, server):
        self.servers.remove(server)

    def getServers(self):
        return list(servers)

    def poll(self):
        for i in self.servers:
            i.poll()


class TCPServer(Server):
    def __init__(self, port, *filters):
        Server.__init__(self)
        self.filters = filters
        self.serverSocket = None
        self.sockets = []
        if port:
            self.serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.serverSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.serverSocket.bind(('0.0.0.0', port))
            self.serverSocket.listen(5)

            self.sockets = [self.serverSocket]
        self.connections = {}

    def poll(self):
        self.lock.acquire()
        try:
            inputReady = select.select(self.sockets, [], [])[0]
        

            for s in inputReady:
                if s == self.serverSocket:
                    socket, address = s.accept()
                    self.openSocket(socket, True)
    
                else:
                    try:
                        self.connections[s].poll()
                    except Exception, e:
                        raise
        except Exception, e:
            raise
        self.lock.release()


    def openConnection(self, host, port):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((host, port))
        return self.openSocket(s)

    def openSocket(self, socket, server=False):
        self.sockets.append(socket)
        filters = [i.duplicate() for i in self.filters]
        filters = [Filter.TCPFilter(socket)] + filters
        for i, j in enumerate(filters):
            if i > 0:
                j.setOut(filters[i-1])
            if i + 1 < len(filters):
                j.setIn(filters[i + 1])
            j.server = server
            j.master = self

        self.connections[socket] = filters[0]
        filters[-1].connect()
        return filters[-1]

    def remove(self, key):
        if key in self.connections:
            del self.connections[key]
            self.sockets.remove(key)
            return True
        return False
