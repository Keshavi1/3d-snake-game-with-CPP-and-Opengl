import pygame, sys
from pygame.locals import *
from player import Player
from collisionFuctions import *
from tiles import *
import pickle
from os import path

"""
This file creates the level and updates everything in it.
It takes the variables from the other files and updates the screen by it.
"""

#screen settings
tile_size = 40
screen_width = tile_size * 20
screen_height = (tile_size) * 20



class Level():
    def __init__(self, screen):
        self.display_surface = screen
        self.level = 0
        

        #player veriables
        self.currentx = None
        self.c_ice_h = True
        self.c_ice_v = False

    #gets the data for the level
    def get_data(self):
        #lodes the world from the files
        if path.exists(f'platformer/levels/level{self.level}_data'):
            pickle_in = open(f'platformer/levels/level{self.level}_data', 'rb')
            world_data = pickle.load(pickle_in)
            return world_data
    #sets up the current level
    def setup_level(self, data):
        #signle tile objects groups
        #solid tiles
        self.normal_tiles = pygame.sprite.Group()      
        self.ice_tiles = pygame.sprite.Group()
        self.slime_tiles = pygame.sprite.Group()
        #liquid tiles
        self.water_tiles = pygame.sprite.Group()
        self.lava_tiles = pygame.sprite.Group()
        #partial tiles
        self.spike_tiles = pygame.sprite.Group()
        
        #laser creators
        self.death_lasers_top = pygame.sprite.Group()
        self.death_lasers_bottom = pygame.sprite.Group()
        self.wall_lasers_top = pygame.sprite.Group()
        self.wall_lasers_bottom = pygame.sprite.Group()
        self.all_laser_creators = pygame.sprite.Group()
        #multi-tile object groups
        self.death_laserbeams = pygame.sprite.Group()
        self.wall_laserbeams = pygame.sprite.Group()
        #non visable objects
        
        self.buttons = pygame.sprite.Group()
        self.goal_tiles = pygame.sprite.Group()      

        self.player = pygame.sprite.GroupSingle()
        for row_index, row in enumerate(data):
            for col_index, ceil in enumerate(row):
                x = col_index * tile_size
                y = row_index * tile_size
                if ceil == 1: #player
                    self.player.add(Player((x,y),tile_size))
                if ceil == 2: #normal tiles
                    self.normal_tiles.add(Tile((x,y), tile_size, 'slategrey')) 
                if ceil == 3: #ice tiles
                    self.ice_tiles.add(Tile((x,y), tile_size, 'aqua'))    
                if ceil == 4: #water full tiles
                    self.water_tiles.add(LiquidTile((x,y), tile_size,False, 'blue'))    
                if ceil == 5: #water surface tiles
                    self.water_tiles.add(LiquidTile((x,y), tile_size,True,'blue'))
                if ceil == 6: #lava full tiles
                    self.lava_tiles.add(LiquidTile((x,y), tile_size,False,'crimson'))
                if ceil == 7: #lava surface tiles
                    self.lava_tiles.add(LiquidTile((x,y), tile_size,True,'crimson'))
                if ceil == 8: #slime tiles
                    self.slime_tiles.add(Tile((x,y), tile_size, 'green'))  
                if ceil == 9:
                    self.spike_tiles.add(SpikeTile((x,y), tile_size, 0))
                if ceil == 10:
                    self.spike_tiles.add(SpikeTile((x,y), tile_size, 1))
                if ceil == 11:
                    self.spike_tiles.add(SpikeTile((x,y), tile_size, 2))
                if ceil == 12:
                    self.spike_tiles.add(SpikeTile((x,y), tile_size, 3))
                if ceil == 13:
                    self.death_lasers_top.add(Laser((x,y),tile_size,0,True))
                if ceil == 14:
                    self.death_lasers_bottom.add(Laser((x,y),tile_size,1,True))
                if ceil == 15:
                    self.wall_lasers_top.add(Laser((x,y),tile_size,0,False))
                if ceil == 16:
                    self.wall_lasers_bottom.add(Laser((x,y),tile_size,1,False))
                if ceil == 17:
                    self.buttons.add(ButtonTile((x,y),tile_size))
                if ceil == 18: #normal tiles
                    self.goal_tiles.add(Tile((x,y), tile_size, 'orange')) 
                
        
        self.set_laserbeam()
        
        
    #srts the laserbeams positions and adds them to a group

    def set_laserbeam(self):
        xy1 = []
        xy2 = []
        #checks if the top laser has a bottom counter part
        for laser1 in self.death_lasers_top.sprites():
            for laser2 in self.death_lasers_bottom.sprites():
                if laser1.rect.x == laser2.rect.x: #sets the xy's to the center of the two lasers
                    xy1.append(laser1.rect.center)
                    xy2.append(laser2.rect.center)
        #draws the lasers
        for l2,l1 in enumerate(xy1):
            self.death_laserbeams.add(Laserbeam(l1,xy2[l2],'red', self.display_surface, l2%2))
        
        xy1 = []
        xy2 = []
        for laser1 in self.wall_lasers_top.sprites():
            for laser2 in self.wall_lasers_bottom.sprites():
                if laser1.rect.x == laser2.rect.x:
                    xy1.append(laser1.rect.center)
                    xy2.append(laser2.rect.center)
                    break
        for l2,l1 in enumerate(xy1):
            self.wall_laserbeams.add(Laserbeam(l1,xy2[l2],'white', self.display_surface, 1))
        
        self.all_laser_creators.add(self.death_lasers_top.sprites())
        self.all_laser_creators.add( self.death_lasers_bottom.sprites())
        self.all_laser_creators.add(self.wall_lasers_top.sprites())
        self.all_laser_creators.add(self.wall_lasers_bottom.sprites())
    #draws the laser beam if they are on
    def draw_laserbeams(self):
        for laser in self.death_laserbeams.sprites():
            if laser.on:
                self.display_surface.blit(laser.image, laser.rect)
        for laser in self.wall_laserbeams.sprites():
            if laser.on:
                self.display_surface.blit(laser.image, laser.rect)
    def run_events(self, e, timer1):
        self.death_laserbeams.update(e,timer1)

            

    def horizontal_collision(self):
        player = self.player.sprite
        player.rect.x += int(player.direction.x * player.velosity)
        #checks the colision for normal tiles
        for sprite in self.normal_tiles.sprites():
            normal_tile_collision_h(player,sprite)
        #checks colision with the laser creator tiles
        for sprite in self.all_laser_creators.sprites():
            normal_tile_collision_h(player, sprite)
        
        for sprite in self.wall_laserbeams.sprites():
            if sprite.on:
                normal_tile_collision_h(player,sprite)
        #checks the colision for ice tiles
        self.c_ice_h = False
        for sprite in self.ice_tiles.sprites():
            self.c_ice_h = ice_tile_collision_h(player,sprite,self.c_ice_h)
            if self.c_ice_h: break
        #slime block collision
        for sprite in self.slime_tiles.sprites():
            normal_tile_collision_h(player,sprite)
        for sprite in self.buttons.sprites():
            normal_tile_collision_h(player,sprite)
        

        #gets the curent position of the side that is touching the wall
        if player.on_right and self.currentx == None: self.currentx = player.rect.right 
        if player.on_left and self.currentx == None: self.currentx = player.rect.left
        #create if statement to check if you are not on the walls
        #if the side that was touching the wall is > or < then your not
        if (player.on_left and self.currentx != None) and (player.rect.left < self.currentx or player.direction.x > 0):
            player.on_left = False
            self.currentx = None
            
            
        if (player.on_right and self.currentx != None) and (player.rect.right > self.currentx or player.direction.x < 0):
            player.on_right = False
            self.currentx = None
            
        

    def vertical_collision(self):
        player = self.player.sprite
        player.apply_gravity()
        #normal tile collisions
        for sprite in self.normal_tiles.sprites():
            normal_tile_collision_v(player,sprite)
        #checks collision with the laser creator tiles
        for sprite in self.all_laser_creators.sprites():
            normal_tile_collision_v(player, sprite)
        
        #ice tile colision
        self.c_ice_v = False
        for sprite in self.ice_tiles.sprites():
            self.c_ice_v = ice_tile_collision_v(player,sprite,self.c_ice_v)
            if self.c_ice_v: break
        if self.c_ice_h or self.c_ice_v: player.on_ice = True
        else: player.on_ice = False

        #slime block collision
        for sprite in self.slime_tiles.sprites():
            slime_tile_collision_v(player,sprite)
        
        
        #checks if the player is touching the ground
        if (player.direction.y < 0 or player.direction.y > 1) and player.on_ground: player.on_ground = False
        elif player.direction.y > 1 and player.on_ceiling: player.on_ceiling = False
    #collisions that arn't side specific
    def other_collision(self):
        player = self.player.sprite
        #checks if the player is in the water
        
        currently_in_water = False
        for sprite in self.water_tiles.sprites():
            currently_in_water = water_tile_collision(player,sprite, currently_in_water)
            if currently_in_water: break 
        if not currently_in_water: player.in_water = False 
        else: player.in_water = True

        player.death_check(self.lava_tiles)
        player.death_check(self.spike_tiles)
        for laser in self.death_laserbeams.sprites():
            laser.kill_check(self.player,player)
        
        
        

    def level_cleared(self):
        if pygame.sprite.groupcollide(self.player,self.goal_tiles,False, False):
            self.level += 1
            try:
                self.setup_level(self.get_data())
            except:
                pass
    
    
    def run(self):
        pygame.draw.rect(self.display_surface,'slategrey',(0,screen_height,screen_height,screen_height + 150))
        #draws all the tiles
        self.buttons.draw(self.display_surface)
        self.buttons.update(self.player)
        #turns of the lasers when the button is pressed
        for b in self.buttons.sprites():
            if b.pressed == True:
                for laser in self.wall_laserbeams.sprites():
                    laser.on = False

        self.normal_tiles.draw(self.display_surface)
        self.ice_tiles.draw(self.display_surface)
        self.water_tiles.draw(self.display_surface)
        self.lava_tiles.draw(self.display_surface)
        self.slime_tiles.draw(self.display_surface)
        self.spike_tiles.draw(self.display_surface)
        #laser tiles and laser beam. Also checks their own colision
        self.death_lasers_top.draw(self.display_surface)
        self.death_lasers_bottom.draw(self.display_surface)
        self.wall_lasers_top.draw(self.display_surface)
        self.wall_lasers_bottom.draw(self.display_surface)
        self.draw_laserbeams()
        
        

        #draws the player and checks colision
        self.horizontal_collision()
        self.vertical_collision()
        self.other_collision()
        self.player.update()
        self.player.draw(self.display_surface)
        self.goal_tiles.draw(self.display_surface)
        self.level_cleared()


