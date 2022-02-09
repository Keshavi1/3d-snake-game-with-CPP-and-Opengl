import pygame,time



"""
this file hold the classes for building all the tiles/obsticles/traps

"""




class Tile(pygame.sprite.Sprite):
    def __init__(self, pos,size, color):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface((size,size))
        self.image.fill(color)
        self.rect = self.image.get_rect(topleft = pos)



class LiquidTile(pygame.sprite.Sprite):
    def __init__(self, pos, size, surface, color):
        pygame.sprite.Sprite.__init__(self)
        if surface:
            self.image = pygame.Surface((size,size//2))
            self.rect = self.image.get_rect(topleft = pos)
            self.rect.y += size//2+1
        else:
            self.image = pygame.Surface((size,size))
            self.rect = self.image.get_rect(topleft = pos)
        self.image.fill(color)


class SpikeTile(pygame.sprite.Sprite):
    def __init__(self,pos,size, rotations):
        pygame.sprite.Sprite.__init__(self)
        spike_img = pygame.image.load('platformer/img/spike.png').convert_alpha()
        self.image = pygame.transform.scale(spike_img, (size,size))
        self.rect = self.image.get_rect(topleft = pos)
        self.image = pygame.transform.rotate(self.image, 90.0 * rotations)
    

class Laser(pygame.sprite.Sprite):
    def __init__(self,pos,size,rotation, deadly):
        pygame.sprite.Sprite.__init__(self)
        laser_img = None
        if deadly:
            laser_img = pygame.image.load('platformer/img/laser_flashing.png').convert_alpha()
        else: 
            laser_img = pygame.image.load('platformer/img/wall_laser.png').convert_alpha()
        self.image = pygame.transform.scale(laser_img, (size,size))
        self.image = pygame.transform.rotate(self.image, 180.0 * rotation)

        self.rect = self.image.get_rect(topleft = pos)

class Laserbeam(pygame.sprite.Sprite):
    def __init__(self,pos1,pos2, color, surface, on_index):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface((5,pos2[1]-pos1[1]))
        self.image.fill(color)
        self.rect = self.image.get_rect(midtop = pos1)
        if on_index:
            self.on = True
        else:
            self.on = False
    def kill_check(self, player, sprite):
        if pygame.sprite.spritecollide(self, player, False) and self.on:
            time.sleep(0.1)
            sprite.rect.topleft = sprite.spawn
            time.sleep(0.2)

    def update(self, e, timer1):
        if e.type == timer1 and self.on:
            self.on = False
        elif e.type == timer1 and not self.on:
            self.on = True


class ButtonTile(pygame.sprite.Sprite):
    def __init__(self, pos,size):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface((size,size//2))
        self.image.fill('white')
        self.rect = self.image.get_rect(topleft = pos)
        self.rect.y += size//2 +1
        self.x = size
        self.pressed = False
    def check_pressed(self,player):
        if pygame.sprite.spritecollide(self,player,False):
            if player.sprite.direction.y > 1:
                self.pressed = True
    def update(self,player):
        self.check_pressed(player)
        if self.pressed:
            if self.x > 0:
                self.rect.y += 1
                self.x -= 1
            else:
                self.kill()





        