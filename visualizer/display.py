import pygame
import time
import main

from GuiHandler import *
from TextureHandler import *

ZOOM = 1
CENTER = [13, 13]
DIMENSIONS = [27, 27]

END=0
CONTINUE=1
LEFT=2
PAUSE=3
RIGHT=4
TURBO=5

pygame.init()
pygame.display.set_mode((800, 600),pygame.OPENGL|pygame.DOUBLEBUF)#|pygame.FULLSCREEN)

GUI = GuiHandler(26, 27)
textures = TextureHandler('./sprites/')

def loadSprite(self):
  image = self.type + ".png"
  if self.team == main.State.Player1ID:
    color = (0, 119, 255)
  elif self.team == main.State.Player2ID:
    color = (196, 32, 32)
  else:
    color = (0, 0, 255)
  textures.load((image, color))

def drawSprite(self):
  global GUI
  loadSprite(self)
  GUI.draw(self.x, self.x+1, self.y, self.y+1)
  if self.red:
    label = '"' + `self.red` + '"'
    textures.load((label, (255, 64, 0)))
    GUI.draw(self.x, self.x+.5, self.y+.5, self.y+1)
  if self.blue:
    label = '"' + `self.blue` + '"'
    textures.load((label, (0, 0, 255)))
    GUI.draw(self.x+.5, self.x+1, self.y+.5, self.y+1)
  if self.green:
    label = '"' + `self.green` + '"'
    textures.load((label, (0, 255, 0)))
    GUI.draw(self.x+.5, self.x+1, self.y, self.y+.5)
  if self.yellow:
    label = '"' + `self.yellow` + '"'
    textures.load((label, (255, 255, 0)))
    GUI.draw(self.x, self.x+.5, self.y, self.y+.5)

def drawEffect(self):
  global GUI
  textures.load((self.image+".png", self.color))
  GUI.draw(self.x-.25, self.x+.25, self.y-.25, self.y+.25)

def setWindow():
  global GUI
  global CENTER
  global DIMENSIONS
  global ZOOM
  GUI.setDimensions(CENTER[0] - DIMENSIONS[0]/2.**ZOOM,
                    CENTER[0] + DIMENSIONS[0]/2.**ZOOM,
                    CENTER[1] - DIMENSIONS[1]/2.**ZOOM,
                    CENTER[1] + DIMENSIONS[1]/2.**ZOOM)

def draw(state):
    global GUI

    
    textures.load(('background.png', (128, 128, 128)))
    for i in xrange(int(GUI.l-1), int(GUI.r+1)):
        for j in xrange(int(GUI.b-1), int(GUI.t-1)):
            GUI.draw(i, i+1, j, j+1)
    textures.load(('Base.png', (0, 119, 255)))
    GUI.draw(0, 1, 0, 1)
    textures.load(('Base.png', (255, 0, 0)))
    GUI.draw(25, 26, 25, 26)
    
    for i in state.scenery:
        drawSprite(i)

    label1 = '"' + main.State.Player1 + ':' + str(state.score0) + '"'
    textures.load((label1, (0, 0, 180)))
    GUI.draw(0,0 + .2 * len(label1),26,27)
    label2 = '"' + main.State.Player2 + ':' + str(state.score1) + '"'
    textures.load((label2, (180, 0, 0)))
    GUI.draw(24 - .2 * len(label2), 24, 26, 27)
    label3 = '"TURN:' + str(state.turn) + '"'
    textures.load((label3, (180, 0, 180)))
    GUI.draw(12, 12 + .2 * len(label3), 26, 27)
    
    for i in state.units.values():
        drawSprite(i)
    
    for i in state.animations:
      drawEffect(i)
    
    pygame.display.set_caption("Test?")
    
    GUI.clear()

def eventHandler(e,s):
    global ZOOM
    global CENTER
    global DIMENSIONS
    ret = e
    for event in pygame.event.get():
      #print event
      if event.type == pygame.QUIT:
        ret = END
      if event.type == pygame.KEYDOWN:
        if event.key == pygame.K_q:
          ret = END
        elif event.key == pygame.K_BACKSPACE:
          ret = PAUSE
        elif event.key == pygame.K_LEFT:
          ret = LEFT
        elif event.key == pygame.K_RIGHT:
          ret = RIGHT
        elif event.key == pygame.K_t:
          s.speed = 1
          ret = TURBO
        else:
          ret = CONTINUE
      if event.type == pygame.MOUSEBUTTONDOWN:
         if event.button == 4: #scroll up
            if ZOOM < 4:
              #ZOOM += 1
              setWindow()
         if event.button == 5: # scroll down
            if ZOOM > 1:
              #ZOOM -= 1
              setWindow()
         if event.type == pygame.MOUSEMOTION:
           if event.buttons[1]:#the scroll wheel is down
             CENTER[0] -= event.rel[0] / 800. * DIMENSIONS[0]
             CENTER[1] += event.rel[1] / 600. * DIMENSIONS[1]
             setWindow()
      if event.type == pygame.KEYDOWN:
        if event.key == pygame.K_1:
          s.speed = 3
        elif event.key == pygame.K_2:
          s.speed = 5
        elif event.key == pygame.K_3:
          s.speed = 7
        elif event.key == pygame.K_4:
          s.speed = 9
        elif event.key == pygame.K_5:
          s.speed = 11
        elif event.key == pygame.K_6:
          s.speed = 13
        elif event.key == pygame.K_7:
          s.speed = 15
        elif event.key == pygame.K_8:
          s.speed = 17
        elif event.key == pygame.K_9:
          s.speed = 19
        elif event.key == pygame.K_0:
          s.speed = 1
      
    return ret    
      
    """
    while not quit:
        print 'j'
        for event in pygame.event.get():
            print event
            if event.type == pygame.QUIT:
                quit = True
                pygame.quit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_BACKSPACE:
                    running = False
                if event.key == pygame.K_LEFT:
                    running = False
                elif event.key == pygame.K_RIGHT:
                    Running = False
                elif event.key == pygame.K_q:
                    quit = True
                    pygame.quit()
                elif event.key == pygame.K_SPACE:
                    running = not running
                    lastRun = time.time()
            if event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 4: #scroll up
                    if ZOOM < 4:
                        ZOOM += 1
                    setWindow()
                elif event.button == 5: # scroll down
                    if ZOOM > 1:
                        ZOOM -= 1
                    setWindow()
            if event.type == pygame.MOUSEMOTION:
                if event.buttons[1]:#the scroll wheel is down
                    CENTER[0] -= event.rel[0] / 800. * DIMENSIONS[0]
                    CENTER[1] += event.rel[1] / 600. * DIMENSIONS[1]
                    setWindow()
        print 'a'
        pygame.event.pump()
        yield"""

class Display:
  def __init__(self):
    pass
  
  def display(self, state):
    #print state.units
    draw(state)

