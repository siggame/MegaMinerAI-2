import main
import sexpr
import sys

class Loader:
  def __init__(self):
    pass

  
  def preload(self):
    if len(sys.argv) < 2:
      f = open('input.viz')
    else:
      f = open(sys.argv[1])
    text = sexpr.str2sexpr(f.read())
    initialload = 0
    actionlist = []
    for item in text:
      #print item[0]
      if item[0] == 'ident':
        main.State.Player1ID=int(item[1][0][0])
        main.State.Player1=item[1][0][1]
        main.State.Player2ID=int(item[1][1][0])
        main.State.Player2=item[1][1][1]
      elif item[0] == 'walls':
        for w in item[1:]:
          #print "Constructing a wall at (",w[1],",",w[2],")"
          initialload += -1
          actionlist.append(main.Place(main.Unit(main.WALLTEAMID, "Tree", initialload, int(w[1]), int(w[2]), 0)))
      elif item[0] == 'buildZones':
        for n in item[1:]:
          #print "Constructing a BuildZone at (",n[1],",",n[2],")"
          initialload += -1
          actionlist.append(main.Place(main.Unit(main.NODETEAMID, "BuildZone", initialload, int(n[1]), int(n[2]), 0)))
    return main.CompositeAction(actionlist)
    

  def load(self, s):
    
    #LOAD FROM FILE
    #"""
    yield self.preload()
    if len(sys.argv) < 2:
      f = open('input.viz')
    else:
      f = open(sys.argv[1])
    text = sexpr.str2sexpr(f.read())
    actions = []
    initialload = 0
    for item in text:
      if item[0] == 'animations':
        for i in item[1]:
          if i[0] == 'appear':
            #print "Appear hit!",i[1],i[3],i[4],"0"
            id = int(i[1][0])
            unit = main.Unit(int(i[2]), "Sphere", id, float(i[1][2]), float(i[1][3]), 0.)
            unit.red = int(i[1][6])
            unit.blue = int(i[1][7])
            unit.green = int(i[1][8])
            unit.yellow = int(i[1][9])
            actions.append(main.Add(unit))
          elif i[0] == 'move':
            #print "Move hit!"
            #print i[1:]
            id = int(i[1])
            actions.append(main.Move(id,float(i[2]), float(i[3])))
          elif i[0] == 'attack':
            print "Attack hit!"
            #print i[1:]
          elif i[0] == 'combine':
            print "Combine hit!"
            id0 = int(i[1])
            id1 = int(i[2])
            actions.append(main.Combine(s.units[id0], s.units[id1]))
            
          elif i[0] == 'build':
            print "Build hit!"
            unit = int(i[1])
            x = int(i[2])
            y = int(i[3])
            actions.append(main.Build(unit, x, y))
          elif i[0] == 'die':
            #print "Die hit!"
            id = int(i[1])
            actions.append(main.Remove(s.units[id]))
          elif i[0] == 'achievement':
            print "Achievement hit!"
          elif i[0] == 'game-move-denied':
            print "ERROR: Move Denied!"
            #print i[1:]
          elif i[0] == 'game-attack-denied':
            print "ERROR: Attack Denied!"
            #print i[1:]
          elif i[0] == 'game-build-denied':
            print "ERROR: Build Denied!"
            #print i[1:]
          elif i[0] == 'game-combine-denied':
            print "ERROR: Combine Denied!"
            #print i[1:]
        yield main.CompositeAction(actions)
        actions = []
      elif item[0] == 'status':
        #main.State.Turn=item[1]
        #main.State.score0=item[1][1][1]
        #main.State.score0=item[1][2][1]
        actions.append(main.UpdateStatus(s.score0, item[1][1][1], s.score1, item[1][2][1], s.turn, item[1][0]))
        #print "score0 = ",item[2][1],"score1 = ", item[3][1]
        #print "turn[1:] == ", item[1:]
    #"""
    
    """
    #ORIGINAL TEST
    yield main.Add(main.Unit("Omicron", "Fight-man", 0, 15, 15, 0))
    yield main.Add(main.Unit("Uniicron", "Flight-man", 1, 5, 5, 0))
    i = 15.
    while True:
      yield main.Move(0, i, i+1, i, i+1)
      yield main.Move(1, 20-i, 20-i-1, 20-i, 20-i-1)
      i += 1
    """


      
    """
    def animate(animation):
      for i in animation[1]:
        if i[0] == 'appear':
          print "Appearing hit!"
          print i[1:]
          yield main.Move("UnitTeamName", "UnitName", int(i[1]), int(i[3]), int(i[4]), 0)
        elif i[0] == 'move':
          print "Move hit!"
          print i[1:]
          #main.Move(0, i, i+1, i, i+1)
        elif i[0] == 'attack':
          print "Attack hit!"
        elif i[0] == 'combine':
          print "Combine hit!"
        elif i[0] == 'build':
          print "Build hit!"
        elif i[0] == 'disappear':
          print "Disappear hit!"
        elif i[0] == 'achievement':
          print "Achievement hit!"
    """
    
    """
    if animation[0] == 'appear':
      main.Add(main.Unit("Omicron", "Fight-man", 0)
    elif animation[0] == 'move':
      main.Move(0, i, i+1, i, i+1)
    elif animation[0] == 'attack':
      main.Attack(0, i, i+1, i, i+1)
    elif animation[0] == 'combine':
      main.Combine(0, i, i+1, i, i+1)
    elif animation[0] == 'build':
      main.Build(0, i, i+1, i, i+1)
    elif animation[0] == 'disappear':
      main.Disappear(0, i, i+1, i, i+1)
    elif animation[0] == 'achievement':
      main.Achievement(0, i, i+1, i, i+1)
    """
            
    #animations = ['appear', 'move', 'attack', 'combine', 'build', 'disappear', 'achievement']
    #f = open('current.viz')
    #text = sexpr.str2sexpr(f.read())
    #  for item in text
    #    for anim in animations
    #      if item == anim
    #        process(item,anim)

      
    
    #PORT STANDARD WRITE TEST
    #sys.stdout.write(sys.stdin.read())


