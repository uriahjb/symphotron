'''
Controlling video from midi

At each frame we load an image from the hd, 

video images are created via:
  ffmpeg -i globe_good_ocean.mov v2i_test/frame%04d.png

'''

import pygame
from pygame.locals import*

from time import time as now, sleep
from random import randint

# Load all the images into RAM
imgs = []

num_frames = 400

'''
print "Loading images"
t0 = now()
for i in xrange(1,num_frames+1):
   if (i % 100 == 0):
       print "Loaded img: ", i 
   imgs.append(pygame.image.load('/Users/ujb/Class/IPD516/v2i_test/frame%04d.png' % i ))

dt = now() - t0
print "Load time: ", dt
print "Image per second", num_frames/dt
'''

pygame.init()

frames_str = '/Users/ujb/Class/IPD516/goodstuff/'

imgs.append( pygame.image.load(frames_str + 'frame%04d.png' % 1 ))

white = (255, 64, 64)
w = imgs[0].get_width() 
h = imgs[0].get_height()
print w,h
aspect_ratio = float(h)/w
#screen = pygame.display.set_mode((w, h))
width = 1200
screen_size = (width, int(width*aspect_ratio))
print "screen size: ", screen_size
screen = pygame.display.set_mode(screen_size)
screen.fill((white))
running = 1

direction = 1
speed = 1

frame_num = 0
fps_counter = 0
t0 = now()

ft0 = now()

while running:
    try:
        if now() - ft0 < 1/30.0:
          continue
        ft0 = now()

        if randint(1,20) == 1:
            direction = randint(0,1)
            if direction == 0:
                direction = -1
            speed = randint(2,7)
        

        frame_num += direction*speed
        fps_counter+=1
     
        curtime = now()
        if curtime - t0 > 1.0:
           t0 = curtime 
           print "fps: ", fps_counter
           fps_counter = 0
        
        if frame_num <= 0:
           frame_num = num_frames-1
        if frame_num >= num_frames:
           frame_num = 0

        screen.fill((white))
        img = pygame.image.load(frames_str + 'frame%04d.png' % (frame_num+1) )
        img = pygame.transform.scale(img, screen_size)
        screen.blit(img, (0,0))
        #screen.blit(imgs[frame_num],(0,0))
        pygame.display.flip()
    except KeyboardInterrupt:
        break

pygame.quit()
