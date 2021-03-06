#!/usr/bin/env python
# Copyright (C) 2008-2012,2017 Bernd Feige
# This file is part of avg_q and released under the GPL v3 (see avg_q/COPYING).

import sys
import voglps
import SVG

# Derived class with SVG plotting operators
class voglSVG(voglps.voglps):
 def __init__(self,psfile,scene,width,height):
  voglps.voglps.__init__(self,psfile)
  self.scene=scene
  self.width=width
  self.height=height
  self.current_color=(0,0,0)
  self.current_position=(0,0)
  self.coords=None
 def moveto(self,x,y):
  self.current_position=(x,self.height-y)
 def lineto(self,x,y):
  if not self.coords:
   self.coords=[self.current_position]
  newpos=(x,self.height-y)
  self.coords.append(newpos)
  self.current_position=newpos
 def stroke(self):
  if self.coords:
   self.scene.add(SVG.Path(self.coords,self.current_color))
   self.coords=None
 def text(self,text):
  self.scene.add(SVG.Text(self.current_position,text,24,self.current_color))
 def color(self,r,g,b):
  self._stroke()
  self.current_color=(r*255,g*255,b*255)

# These are the postscript coordinates
WIDTH, HEIGHT  = 2700/voglps.scale_factor, 2000/voglps.scale_factor

if __name__ == '__main__':
 for psfile in sys.argv[1:]:
  svgfile=psfile.replace(".ps",".svg")
  print(svgfile)
  scene = SVG.Scene(svgfile.replace(".svg",""), HEIGHT, WIDTH)
  #v=voglps.voglps(psfile)
  v=voglSVG(psfile,scene,WIDTH,HEIGHT)
  v.parse()
  scene.write_svg(svgfile)
