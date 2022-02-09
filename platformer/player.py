import pygame,time
from pygame.locals import *

"""
this file holds the players attrabutes. It stores all of the players veriables and updates them internally. 
"""
class Player(pygame.sprite.Sprite):
    def __init__(self, pos, size):
        #inital setup
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface((size//2,size//2))
        self.image.fill('blueviolet')
        self.rect = self.image.get_rect(topleft = pos)
        #death variables
        self.spawn = pos
        self.is_dead = False
        #movement variables
        self.direction = pygame.math.Vector2(0,0)
        self.constant_gravity = 1.0
        self.velosity = 1
        self.jump_height = -(size//2.5)
        self.jump = False
        self.double_jump = False
        #player status
        self.wall_sliding = False #used to check if you are activly sliding on the wall
        self.in_water = False
        self.on_ice = False
        self.on_ground = False
        self.on_ceiling = False
        self.on_left = False
        self.on_right = False

    def get_input(self):
        key = pygame.key.get_pressed()

        if (key[pygame.K_LEFT] or key[pygame.K_a]):
            if self.on_ice:
                if self.direction.x >= 0:
                        self.direction.x += -1
                if self.velosity < 4:
                    self.velosity += 0.1
            else:#normal movement
                if self.direction.x >= 0:
                    self.direction.x += -1
                if self.velosity < 3:
                    self.velosity += 0.1
            self.wall_sliding = True
            
        elif (key[pygame.K_RIGHT] or key[pygame.K_d]): 
            if self.on_ice:
                if self.direction.x <= 0:
                        self.direction.x += 1
                if self.velosity < 4:
                    self.velosity += 0.1
            else:   #normal movement
                if self.direction.x <= 0:
                    self.direction.x += 1
                if self.velosity < 3:
                    self.velosity += 0.1
            self.wall_sliding = True

            
        else:
            if self.on_ice:
                if self.velosity > 2:
                    self.velosity -= 0.01
                if self.velosity <= 2:
                    self.velosity = 2 
                    
            else:
                if self.velosity > 2:
                    self.velosity -= 0.4
                if self.velosity <= 2:
                    self.velosity = 2
                    self.direction.x = 0
                    
            self.wall_sliding = False


        if (key[pygame.K_SPACE] or key[pygame.K_UP] or key[pygame.K_w]) and (self.jump):
            if self.on_ground:
                #normal jump
                self.direction.y = self.jump_height
                self.jump = False
                self.double_jump = True
            if not self.on_ground:
                #allows you to wall jump
                if self.on_left: self.direction.xy = 4,self.jump_height // 1.15
                elif self.on_right: self.direction.xy = -4,self.jump_height // 1.15
                #alowes you to double jump
                if self.double_jump: 
                    self.direction.y = self.jump_height//1.1
                    self.double_jump = False
                self.jump = False
                #allows you to "swim" when in water
                if self.in_water: self.direction.y = self.jump_height // 1.25

        #resets ablility to jump when you let go of space
        if not (key[pygame.K_SPACE] or key[pygame.K_UP]): self.jump = True


    def apply_gravity(self):
        
        if (self.on_left or self.on_right) and (self.wall_sliding) and (not self.on_ice):
            if self.direction.y > 2: self.direction.y = 2
            self.velosity = 1
        
        if self.in_water:
            if self.direction.y > 1: self.direction.y = 1
            self.velosity = 1
        
        self.direction.y += self.constant_gravity
        self.rect.y += self.direction.y

    def death_check(self,enemy_group):
        if pygame.sprite.spritecollide(self,enemy_group,False):
            self.rect.topleft = self.spawn
            time.sleep(0.2)
            

    def update(self):
        self.get_input()
        
        
        

