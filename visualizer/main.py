import sys


import display
import load

import time


import copy

MOVE = 0
SPAWN = 1
ATTACK = 2
WALLTEAMID = -1
WALLTEAMNAME = "Natural"
NODETEAMID = -2
NODETEAMNAME = "Natural"

class Unit:
  id = 0
  x = 0.
  y = 0.
  z = 0.
  red = blue = green = yellow = 0
  type = ""
  team = 0
  teamname = ""

  def __init__(self, team, type, id, startx, starty, startz):
    if team is WALLTEAMID:
      self.teamname = WALLTEAMNAME
    elif team is NODETEAMID:
      self.teamname = NODETEAMNAME
    elif team is State.Team1ID:
      self.teamname = State.Team1
    elif team is State.Team2ID:
      self.teamname = State.Team2
    else:
      print "Unrecognized team name:", team, "  For unit id:", id
    self.team = team
    self.type = type
    self.id = id
    self.x=startx
    self.y=starty
    self.z=startz
    
class Effect:
  x = 0.
  y = 0.
  z = 0.
  image = "Sphere"
  color = (255, 255, 255)
  
  def __init__(self, x = 0, y = 0, z = 0, image = "Sphere", color = (255,255,255)):
    self.x = x
    self.y = y
    self.z = z
    self.image = image
    self.color = color
  
class State:
  units = {}
  score0 = 0
  score1 = 0
  turn = 0
  speed = 9
  back = None
  forward = None
  Map = 'Default'
  Team1 = 'Team1'
  Team1ID = 0
  Team2 = 'Team2'
  Team2ID = 0
  scenery = []
  animations = []

  def __init__(self):
    pass

  def GetUnitType(self, unitID):
    return self.units[unitID].type
    

class Action:
  #this is an iterator to allow coroutines to simplify animation
  def run(self, state):
    yield

  def reverse(self):
    return self

class Move(Action):
  unit = 0
  oldX = newX = 0.
  oldY = newY = 0.
  oldZ = newZ = 0.
  def __init__(self, unit, newX, newY, newZ = 0.):
    self.unit = unit
    self.newX = newX
    self.newY = newY
    self.newZ = newZ

  def run(self, s):
    self.oldX = s.units[self.unit].x
    self.oldY = s.units[self.unit].y
    self.oldZ = s.units[self.unit].z
    for i in xrange(s.speed+1):
      s.units[self.unit].x = (self.oldX * (s.speed-i) + self.newX * i) / s.speed
      s.units[self.unit].y = (self.oldY * (s.speed-i) + self.newY * i) / s.speed
      s.units[self.unit].z = (self.oldZ * (s.speed-i) + self.newZ * i) / s.speed
      yield MOVE

  def reverse(self):
    return Move(self.unit, self.oldX, self.oldY, self.oldZ)

class Add(Action):
  unit = None

  def __init__(self, unit):
    self.unit = unit

  def run(self, state):
    yield SPAWN
    state.units[self.unit.id] = copy.deepcopy(self.unit)

  def reverse(self):
    return Remove(self.unit)

class Remove(Action):
  unit = None

  def __init__(self, unit):
    self.unit = unit

  def run(self, state):
    yield
    del state.units[self.unit.id]

  def reverse(self):
    return Add(self.unit)
class SetStats(Action):
  unit = None
  red = blue = green = yellow = 0

  def __init__(self, unit, red, blue, green, yellow):
    self.unit = unit
    self.red = red
    self.blue = blue
    self.green = green
    self.yellow = yellow

  def run(self, state):
    self.backward = SetStats(self.unit, self.unit.red, self.unit.blue,
                            self.unit.green, self.unit.yellow)
    yield
    self.unit.red = self.red
    print self.unit.red
    self.unit.blue = self.blue
    self.unit.green = self.green
    self.unit.yellow = self.yellow
  def reverse(self):
    return self.backward
class Combine(Action):
  unit0 = unit1 = None

  def __init__(self, unit0, unit1):
    self.unit0 = unit0
    self.unit1 = unit1

  def run(self, state):
    if 'forward' not in dir(self):
      move = Move(self.unit0.id, self.unit1.x, self.unit1.y, self.unit1.z)
      remove = Remove(self.unit0)
      stat = SetStats(self.unit1, self.unit0.red + self.unit1.red,
                       self.unit0.blue + self.unit1.blue,
                       self.unit0.green + self.unit1.green,
                       self.unit0.yellow + self.unit1.yellow)
      self.forward = CompositeAction([move, remove, stat])
    for i in self.forward.run(state):
    	yield i

  def reverse(self):
    return self.forward.reverse()
    
class Place(Action):
  unit = None

  def __init__(self, unit):
    self.unit = unit

  def run(self, state):
    yield SPAWN
    state.scenery.append(self.unit)

  def reverse(self):
    return Unplace(self.unit)

class Unplace(Action):
  unit = None

  def __init__(self, unit):
    self.unit = unit

  def run(self, state):
    yield SPAWN
    state.scenery.remove(self.unit)

  def reverse(self):
    return Place(self.unit)


class Attack(Action):
  unit = None
  oldX = newX = 0
  oldY = newY = 0

  def __init__(self, unit, x, y):
    self.unit = unit
    self.oldX = oldX
    self.newX = newX
    self.oldY = oldY
    self.newY = newY

  def run(self, state):
    pass
    

class CompositeAction(Action):
  actions = []
  def __init__(self, actions):
    self.actions = actions

  def run(self, state):
    for i in self.actions:
      for j in i.run(state):
        yield j

  def reverse(self):
    reversed = [i.reverse() for i in self.actions]
    reversed.reverse() #run them in reverse order, this may be unnecessary but it's aesthetic
    return CompositeAction(reversed)

class Draw(Action):
  effect = None
  
  def __init__(self, effect):
    self.effect = effect
  
  def run(self, state):
    state.animations.append(self.effect)
    yield
    for i in xrange(state.speed):
      yield
    state.animations.remove(self.effect)
  def reverse(self):
    return Draw(self.effect)

class Hit(Action):
  unit0 = unit1 = None
  
  def __init__(self, unit0, unit1):
    self.effect = effect
  
  def run(self, state):
    state.animations.append(self.effect)
    yield
    for i in xrange(state.speed):
      yield
    state.animations.remove(self.effect)
  def reverse(self):
    return Draw(self.effect)

class Build(Action):
  unit = 0
  x = y = 0
  
  def __init__(self, unit, x, y):
    self.unit = unit
    self.x = x
    self.y = y
  
  def run(self, state):
    if 'forward' not in dir(self):
      x = (state.units[self.unit].x + self.x) / 2. + .5
      y = (state.units[self.unit].y + self.y) / 2. + .5
      self.forward = Draw(Effect(x, y, 0, "Heart"))
    for i in self.forward.run(state):
      yield i
  
  def reverse(self):
    return self.forward.reverse()

class UpdateStatus(Action):
  oldScore0 = 0
  newScore0 = 0
  oldScore1 = 0
  newScore1 = 0
  oldTurn = 0
  newTurn = 0
  
  def __init__(self, oldScore0, newScore0, oldScore1, newScore1, oldTurn, newTurn):
    self.oldScore0 = oldScore0
    self.newScore0 = newScore0
    self.oldScore1 = oldScore1
    self.newScore1 = newScore1
    self.oldTurn = oldTurn
    self.newTurn = newTurn

  def run(self, state):
    yield
    state.score0 = self.newScore0
    state.score1 = self.newScore1
    state.turn = self.newTurn

  def reverse(self):
    return UpdateStatus(self.newScore0, self.oldScore0, self.newScore1, self.oldScore1, self.newTurn, self.oldTurn)

if __name__ == '__main__':
  s = State()
  d = display.Display()
  l = load.Loader()
  loader = l.load(s)
  e = display.CONTINUE
  #e = display.PAUSE
  r = False #going in reverse?
  done = False #eof?
  i = 0
  timer=0
  a =[]
  for j in loader.next().run(s):
    pass
  while True:
    #time.sleep(.25)
    #time.sleep(.1)
    #print r
    if e is display.PAUSE:
      pass
    else:
      if not r:
        if i >= len(a) and not done:
          try:
            a.append(loader.next())
            continue
          except StopIteration:
            done = True
        elif i >= len(a):
          timer += 1
          if timer > 100:
            sys.exit(0)
      else:
        if i <= 0:
          i = 0
          r = False
          e = display.PAUSE
          continue
      if not r and i != len(a):
        for j in a[i].run(s):
          if e is not display.TURBO:
            d.display(s)
            #time.sleep(.1)
        i += 1
      elif r:
        timer=0
        for j in a[i].reverse().run(s):
          if e is not display.TURBO:
            d.display(s)
            #time.sleep(.01)
        i -= 1
    e = display.eventHandler(e,s)
    d.display(s)
    
    if e is display.END:
      sys.exit()
    elif e is display.LEFT:
      if r is False:
        i -= 1
        r=True
    elif e is display.RIGHT:
      if r is True:
        r=False
