'''
Controlling video from midi
'''

import pygame
from pygame.locals import*

from time import time as now, sleep
from random import randint

# Load all the images into RAM
imgs = []

num_frames = 9999

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

imgs.append( pygame.image.load('/Users/ujb/Class/IPD516/v2i_test/frame%04d.png' % 1 ))

white = (255, 64, 64)
w = imgs[0].get_width() 
h = imgs[0].get_height()
screen = pygame.display.set_mode((w, h))
screen.fill((white))
running = 1

direction = 1
speed = 1

frame_num = 0
fps_counter = 0
t0 = now()
while running:
    try:

        if randint(1,20) == 1:
            direction = randint(0,1)
            if direction == 0:
                direction = -1
            speed = randint(4,20)
        

        frame_num += direction*speed
        fps_counter+=1
     
        curtime = now()
        if curtime - t0 > 1.0:
           t0 = curtime 
           print "fps: ", fps_counter
           fps_counter = 0
        
        if frame_num <= 0:
           frame_num = num_frames
        if frame_num >= num_frames:
           frame_num = 0

        screen.fill((white))
        img = pygame.image.load('/Users/ujb/Class/IPD516/v2i_test/frame%04d.png' % (frame_num+1) )
        screen.blit(img, (0,0))
        #screen.blit(imgs[frame_num],(0,0))
        pygame.display.flip()
    except KeyboardInterrupt:
        break

pygame.quit()
