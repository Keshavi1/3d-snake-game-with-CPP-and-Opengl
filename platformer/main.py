import pygame, sys
from pygame.locals import *
from level import*
from menu import*
from random import randint
from levelEditor import *

"""
initalises the games screen and clock and runs the game in it's loop.

todo: Add fuctionality to floating platforms and fix them in general
Add buttons and turrets 

"""
pygame.init()

screen = pygame.display.set_mode((screen_width, screen_height + 150))
pygame.display.set_caption('platformer')
clock = pygame.time.Clock()
level = Level(screen)

menu_bg1 = pygame.image.load('platformer/img/menu1.png').convert()
menu_bg1 = pygame.transform.scale(menu_bg1,(screen_width,screen_height + 150))
menu_bg2 = pygame.image.load('platformer/img/menu2.png').convert()
menu_bg2 = pygame.transform.scale(menu_bg2,(screen_width,screen_height + 150))
menu_bg3 = pygame.image.load('platformer/img/menu3.png').convert()
menu_bg3 = pygame.transform.scale(menu_bg3,(screen_width,screen_height + 150))
end_bg = pygame.image.load('platformer/img/end.png').convert()
end_bg = pygame.transform.scale(end_bg,(screen_width + 10,screen_height + 150))

end1_text =draw_text(80,"Congratulations!",'black')
end2_text =draw_text(80,"You Escaped The Dungeon",'black')
end_button = Buttons((screen_width//2,350),125,'white','Main Menu','black')




#menu buttons
title1_text = draw_text(60,"Immortal Square",'white')
title2_text = draw_text(60,"in a Brutal Dungeon",'white')
nstart_button = Buttons((screen_width//2,250),125,'white','New game','black')
cstart_button = Buttons((screen_width//2,350),125,'green','continue','white')
select_button = Buttons((screen_width//2,500),125,'white','Select','black')
editor_button = Buttons((screen_width//2,625),125,'white','Editor','black')
quit_button = Buttons((screen_width//2,750),125,'red','Quit','white')


#pause buttons
pause_text = draw_text(200,"Paused",'white')
continue_ = Buttons((screen_width - 250,500),150,'white','Continue','black')
exit_ = Buttons((screen_width//2 - 150,500),150,'white','Exit','black')
bg = randint(1,3)

game_completed = True

#editor buttons
play_button = Buttons((width//2 + 150, height - 60),70, 'white', 'EXIT','black')
execption_text =draw_text(30,"Level Can't Be Made",'black')


laser_timer = pygame.USEREVENT + 1
pygame.time.set_timer(laser_timer, 2000)
laser_timer2 = pygame.USEREVENT + 2
pygame.time.set_timer(laser_timer2, 1500)

in_menu = True
in_select = False
in_level = False
in_pause = False
in_editor = False
in_endscreen = False


while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_p:
                in_level == False
                in_pause = True
        if in_editor:
            editor_events(event)
        elif in_level:
            if level.level < 20:
                level.run_events(event,laser_timer)
            else:
                level.run_events(event,laser_timer2)
        

        
    
    if in_menu:
        #make this random later
        if bg == 1:
            screen.blit(menu_bg1,(0,0))
        elif bg == 2:
            screen.blit(menu_bg2,(0,0))
        elif bg == 3:
            screen.blit(menu_bg3,(0,0))

        title1_text.draw((screen_width//2 - 170,40),screen)
        title2_text.draw((screen_width//2 - 200,100),screen)

        if nstart_button.update(screen):
            in_level = True
            in_menu = False
            level.level = 0
            level.setup_level(level.get_data())
        if level.level > 0 and cstart_button.update(screen):
            in_level = True
            in_menu = False
            level.setup_level(level.get_data())
        if game_completed and select_button.update(screen):
            in_select = True
            in_menu = False
        if game_completed and editor_button.update(screen):
            in_editor = True
            in_menu = False
        if quit_button.update(screen):
            pygame.quit()
            sys.exit()

    elif in_select:
        screen.fill('black')
        ls = level_selet(level.level,game_completed)
        sllevel = ls.draw(screen)
        if sllevel:
            level.level = sllevel - 1
            level.setup_level(level.get_data())

            in_level = True
            in_select = False
              
         
    elif in_editor:
        edit_screen.fill('black')
        if run_editor():
            try:
                level.level = edit_level
                level.setup_level(level.get_data())
                level.run()
                in_editor = False
                in_level = True
            except:
                execption_text.draw((width //2 - 50, height - 60),screen)
            
        if play_button.update(screen):
            in_editor = False
            in_menu = True
            bg = randint(1,3)

    elif in_level:
        current_level_text = draw_text(25,f"level {level.level +1}/30",'white')
        
        if level.level >= 13 and level.level <= 16:
            screen.fill('blue')
        else:
            screen.fill('black')
        if in_pause:
            pause_text.draw((screen_width //2 - 250,250),screen)
            if continue_.update(screen):
                in_pause = False
                in_level = True
            if exit_.update(screen):
                in_pause = False
                in_menu = True
                bg = randint(1,3)

        else:
            if level.level <= 29:
                level.run()
            else:
                in_level = False
                in_endscreen = True
        current_level_text.draw((screen_width - 100,5),screen)
    elif in_endscreen:
        screen.blit(end_bg, (-10,0))
        end1_text.draw((screen_width//2.5 - 200,100),screen)
        end2_text.draw((screen_width//2.5 - 300,200),screen)
        if end_button.update(screen):
            in_endscreen = False
            in_menu = True
            game_completed = True

    pygame.display.update()
    clock.tick(60)