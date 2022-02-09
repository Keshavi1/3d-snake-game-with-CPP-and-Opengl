"""this is a edit_level editor for future games
update with more blocks and enemies with time
"""


import pygame
from pygame.locals import*
from menu import *
import pickle
from os import path

#intialize variables
pygame.init()
clock = pygame.time.Clock()
fps = 60
tile_size = 25
col = 20
height = col *tile_size + 150
width = col* tile_size
edit_screen = pygame.display.set_mode((width, height))
#add sprites here if nessecery

spike_img = pygame.image.load('platformer/img/spike.png').convert_alpha()
flashing_laser_img = pygame.image.load('platformer/img/laser_flashing.png').convert_alpha()
wall_laser_img = pygame.image.load('platformer/img/wall_laser.png').convert_alpha()






#variables
clicked = False
keypressed = False
edit_level =-1
click_value = 0
pygame.font.init()
font = pygame.font.SysFont('Futura', 24)


#data for the world
world_data = []
for row in range(col):
    r = [0] * col
    world_data.append(r)

menu1 = [0] * col
for cell in range(col):
    menu1[cell] = cell
menu2 = [0] * col
for cell in range(col):
    menu2[cell] = cell

for tile in range(col):
    world_data[0][tile] = 2
    world_data[col - 1][tile] = 2
    world_data[tile][0] = 2
    world_data[tile][col-1] = 2

def draw_grid():
    for row in range(col):
        pygame.draw.line(edit_screen,(255,255,255),(0,row*tile_size), (width, row * tile_size))
        pygame.draw.line(edit_screen,(255,255,255),(row*tile_size,0), (row* tile_size, height - 150))

def draw_world():
    global col
    #creates the tiles and stes them in a list
    for row in range(col):
        for colm in range(col):
            if world_data[row][colm] > 0:
                x = colm * tile_size
                y = row * tile_size
                if world_data[row][colm] == 1: #player tile
                    img = pygame.Surface((tile_size //2, tile_size//2))
                    img.fill('blueviolet')
                    edit_screen.blit(img, (x, y))

                if world_data[row][colm] == 2: #normal tile
                    img = pygame.Surface((tile_size, tile_size))
                    img.fill('slategray')
                    edit_screen.blit(img, (x, y))

                if world_data[row][colm] == 3: #ice tile
                    img = pygame.Surface((tile_size, tile_size))
                    img.fill('aqua')
                    edit_screen.blit(img, (x, y))

                if world_data[row][colm] == 4: #water full tile
                    img = pygame.Surface((tile_size, tile_size))
                    img.fill('blue')
                    edit_screen.blit(img, (x, y))

                if world_data[row][colm] == 5: #water surface tile
                    img = pygame.Surface((tile_size, tile_size //2))
                    img.fill('blue')
                    edit_screen.blit(img, (x, y + tile_size//2 + 1))   

                if world_data[row][colm] == 6: #lava full tile
                    img = pygame.Surface((tile_size, tile_size))
                    img.fill('crimson')
                    edit_screen.blit(img, (x, y))

                if world_data[row][colm] == 7: #lava surface tile
                    img = pygame.Surface((tile_size, tile_size //2))
                    img.fill('crimson')
                    edit_screen.blit(img, (x, y + tile_size//2 + 1))

                if world_data[row][colm] == 8: #slime tile
                    img = pygame.Surface((tile_size, tile_size))
                    img.fill('green')
                    edit_screen.blit(img, (x, y))

                if world_data[row][colm] == 9: #spike tile
                    img = pygame.transform.scale(spike_img, (tile_size,tile_size))
                    edit_screen.blit(img, (x, y))
                if world_data[row][colm] == 10: 
                    img = pygame.transform.scale(spike_img, (tile_size,tile_size))
                    img = pygame.transform.rotate(img,90.0)
                    edit_screen.blit(img, (x, y))
                if world_data[row][colm] == 11: 
                    img = pygame.transform.scale(spike_img, (tile_size,tile_size))
                    img = pygame.transform.rotate(img,180.0)
                    edit_screen.blit(img, (x, y))
                if world_data[row][colm] == 12: 
                    img = pygame.transform.scale(spike_img, (tile_size,tile_size))
                    img = pygame.transform.rotate(img,270.0)
                    edit_screen.blit(img, (x, y))
                if world_data[row][colm] == 13:#deadly laser
                    img = pygame.transform.scale(flashing_laser_img, (tile_size,tile_size))
                    edit_screen.blit(img,(x,y))
                if world_data[row][colm] == 14:
                    img = pygame.transform.scale(flashing_laser_img, (tile_size,tile_size))
                    img = pygame.transform.rotate(img,180.0)
                    edit_screen.blit(img,(x,y))
                if world_data[row][colm] == 15:#non deadly laser
                    img = pygame.transform.scale(wall_laser_img, (tile_size,tile_size))
                    edit_screen.blit(img,(x,y))
                if world_data[row][colm] == 16:
                    img = pygame.transform.scale(wall_laser_img, (tile_size,tile_size))
                    img = pygame.transform.rotate(img,180.0)
                    edit_screen.blit(img,(x,y))
                if world_data[row][colm] == 17: #button 
                    img = pygame.Surface((tile_size//1.25, tile_size//2))
                    img.fill('white')
                    edit_screen.blit(img, (x, y + tile_size//2))
                if world_data[row][colm] == 18: #goal tile
                    img = pygame.Surface((tile_size, tile_size))
                    img.fill('orange')
                    edit_screen.blit(img, (x, y))
                

        
              #add more blocks here



def draw_menu1():
    for i, ceil in enumerate(menu1):
        x = i*tile_size
        y = height - 130
        if ceil == 1:
            img = pygame.Surface((tile_size //2, tile_size//2))
            img.fill('blueviolet')
            edit_screen.blit(img, (x, y))
        if ceil == 2:
            img = pygame.Surface((tile_size, tile_size))
            img.fill('slategray')
            edit_screen.blit(img, (x, y))
        if ceil == 3:
            img = pygame.Surface((tile_size, tile_size))
            img.fill('aqua')
            edit_screen.blit(img, (x, y))
        if ceil == 4:
            img = pygame.Surface((tile_size, tile_size))
            img.fill('blue')
            edit_screen.blit(img, (x, y))
        if ceil == 5:
            img = pygame.Surface((tile_size, tile_size //2))
            img.fill('blue')
            edit_screen.blit(img, (x, y + tile_size//2))
        if ceil == 6:
            img = pygame.Surface((tile_size, tile_size))
            img.fill('crimson')
            edit_screen.blit(img, (x, y))
        if ceil == 7:
            img = pygame.Surface((tile_size, tile_size //2))
            img.fill('crimson')
            edit_screen.blit(img, (x, y + tile_size//2))
        if ceil == 8:
            img = pygame.Surface((tile_size, tile_size))
            img.fill('green')
            edit_screen.blit(img, (x, y))
        if ceil >=9 and ceil <= 12:
            img = pygame.transform.scale(spike_img, (tile_size,tile_size))
            if ceil == 10: img = pygame.transform.rotate(img,90.0)
            if ceil == 11: img = pygame.transform.rotate(img,180.0)
            if ceil == 12: img = pygame.transform.rotate(img,270.0)
            edit_screen.blit(img, (x, y))
        if ceil == 13 or ceil == 14:
            img = pygame.transform.scale(flashing_laser_img, (tile_size,tile_size))
            if ceil == 14: img = pygame.transform.rotate(img,180.0)
            edit_screen.blit(img,(x,y))
        if ceil == 15 or ceil == 16:
            img = pygame.transform.scale(wall_laser_img, (tile_size,tile_size))
            if ceil == 16: img = pygame.transform.rotate(img,180.0)
            edit_screen.blit(img,(x,y))
        if ceil == 17: #button 
            img = pygame.Surface((tile_size//1.25, tile_size//2))
            img.fill('white')
            edit_screen.blit(img, (x, y + tile_size//2))
        if ceil == 18: #goal tile
            img = pygame.Surface((tile_size, tile_size))
            img.fill('orange')
            edit_screen.blit(img, (x, y))
        
def draw_menu2():
    for i, ceil in enumerate(menu2):
        x = i *tile_size
        y = height - 110  
    if ceil == 19:
        img = pygame.transform.scale(flashing_laser_img, (tile_size,tile_size))
        img = pygame.transform.rotate(img,90.0)
        edit_screen.blit(img,(x,y))
    if ceil == 20:
        img = pygame.transform.scale(flashing_laser_img, (tile_size,tile_size))
        img = pygame.transform.rotate(img,270.0)
        edit_screen.blit(img,(x,y))








save_button = Buttons((width //2 - 100, height - 60),70,'red', 'SAVE','white')

load_button = Buttons((width//2 + 50, height - 60),70, 'red', 'LOAD','white')


def run_editor():
    global world_data
    global edit_level
    if save_button.update(edit_screen):
        pickle_out = open(f'platformer/levels/level{edit_level}_data','wb')
        pickle.dump(world_data,pickle_out)
        pickle_out.close()
    if load_button.update(edit_screen):
        if path.exists(f'platformer/levels/level{edit_level}_data'):
            pickle_in = open(f'platformer/levels/level{edit_level}_data', 'rb')
            world_data = pickle.load(pickle_in)
    level_text = draw_text(25,f'level{edit_level}','white')
    level_text.draw((tile_size,width + 60),edit_screen)
    draw_world()
    draw_menu1()
    draw_grid()
    

def editor_events(e):
    global click_value
    global clicked
    global keypressed
    global edit_level
    if e.type == pygame.MOUSEBUTTONDOWN and not clicked:
            clicked = True
            pos = pygame.mouse.get_pos()
            x = pos[0] // tile_size
            y = pos[1] // tile_size
            if x < col and y < col:
                if pygame.mouse.get_pressed()[0] == 1 and clicked:
                    world_data[y][x] = click_value
            if x < col and y > col and y < col + tile_size:
                if pygame.mouse.get_pressed()[0] == 1 and clicked:
                    click_value = menu1[x]
            if x < col and y > col + tile_size:
                if pygame.mouse.get_pressed()[0] == 1 and clicked:
                    click_value = menu2[x]
                    

    if e.type == pygame.MOUSEBUTTONUP:
        clicked = False
    
    if e.type == pygame.KEYDOWN and not keypressed:
        keypressed = True
        if e.key == pygame.K_UP:
            edit_level += 1
        if e.key == pygame.K_DOWN:
            edit_level -= 1
            if edit_level < -1:
                edit_level = -1
    if e.type == pygame.KEYUP:
        keypressed = False
        