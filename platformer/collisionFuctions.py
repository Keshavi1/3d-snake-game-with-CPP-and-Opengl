import pygame

"""
stores some of the fuctions between the player, tiles, and enemies
the objects that kill are in the player class death_check() method
"""
#horizantal collisions
def normal_tile_collision_h(player,sprite):
    if player.rect.colliderect(sprite):
        if player.direction.x > 0:
            player.rect.right = sprite.rect.left
            player.direction.x = 0
            player.velosity = 3
            player.on_right = True
            
        if player.direction.x < 0:
            player.rect.left = sprite.rect.right
            player.direction.x = 0
            player.velosity = 3
            player.on_left = True
        return True

def ice_tile_collision_h(player,sprite, current):
    on_ice = current
    if player.rect.colliderect(sprite):
        if player.direction.x > 0:
            player.rect.right = sprite.rect.left
            player.direction.x = 0
            player.velosity = 3
            player.on_right = True
            on_ice = True
            
        if player.direction.x < 0:
            player.rect.left = sprite.rect.right
            player.direction.x = 0
            player.velosity = 3
            player.on_left = True  
            on_ice = True  
    return on_ice   

def slime_tile_collision_h(player,sprite):
    if player.rect.colliderect(sprite):
        if player.direction.x > 0:
            player.rect.right = sprite.rect.left    
        if player.direction.x < 0:
            player.rect.left = sprite.rect.right
        player.direction.x *= -2
        
            
#vertical collisions

def normal_tile_collision_v(player,sprite):
    if player.rect.colliderect(sprite):
        if player.direction.y > 0:
            player.rect.bottom = sprite.rect.top 
            player.direction.y = 0
            player.on_ground = True
        if player.direction.y < 0:
            player.rect.top = sprite.rect.bottom 
            player.direction.y = 0
            player.velosity = 3
            player.on_celing = True
        return True



def ice_tile_collision_v(player,sprite, current):
    on_ice = current
    if player.rect.colliderect(sprite):
        if player.direction.y > 0:
            player.rect.bottom = sprite.rect.top
            player.direction.y = 0
            player.on_ground = True
            on_ice = True
        if player.direction.y < 0:
            player.rect.top = sprite.rect.bottom
            player.direction.y = 0
            player.velosity = 3
            player.on_celing = True
    return on_ice

def slime_tile_collision_v(player,sprite):
    if player.rect.colliderect(sprite):
        if player.direction.y > 0:
            player.rect.bottom = sprite.rect.top   
        if player.direction.y < 0:
            player.rect.top = sprite.rect.bottom
        player.direction.y *= -0.9
            




#side dependent colision
def water_tile_collision(player,sprite, current):
    water = current
    if player.rect.colliderect(sprite):
        water = True
    return water



