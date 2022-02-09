import pygame
from pygame.locals import *
import sys
import random
import math
# colors
RED = (255, 0, 0)
LIGHT_CRIMSION = (190, 0, 0)
DARK_CRIMSON = (140, 0, 0)
ORANGE = (245, 140, 65)
YELLOW = (245, 245, 65)
GOLD = (140, 140, 14)
GREEN = (0, 255, 0)
BLUE = (50, 100, 255)
LIGHT_BLUE = (50, 200, 255)
PURPLE = (100, 50, 250)
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)

# class for the player and their movement and collision
# includes their stats as well.


class Player(pygame.sprite.Sprite):
    def __init__(self, size):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface(size)
        self.image.fill(GREEN)
        self.rect = self.image.get_rect()
        self.rect.center = [200, 200]
        self.size = size
        # stats the player has that can be upgraded
        self.stats = {
            'speed': 1,
            'HP': 100,
            'maxHP': 100,
            'shield': 1,
            'maxshield': 1,
            'bulletdmg': 1,
            'bulletspeed': 1,
            'firerate': 1,
            '$x': 1,
        }

        self.cool_down = 0  # fire rate of bullets
        self.IV_frames = 0  # invincability frames
        self.IV_time = 3
        self.color_tick = 0
    # moves the player

    def movement(self):
        key = pygame.key.get_pressed()
        if key[pygame.K_UP]:
            if self.rect.y > (game.screen_size[1] / 2):
                self.rect.y -= self.stats['speed']
        if key[pygame.K_DOWN]:
            if self.rect.y < game.screen_size[1] - self.image.get_size()[1]:
                self.rect.y += self.stats['speed']
        if key[pygame.K_LEFT]:
            if self.rect.x > 0:
                self.rect.x -= self.stats['speed']
        if key[pygame.K_RIGHT]:
            if self.rect.x < game.screen_size[1] - self.image.get_size()[0]:
                self.rect.x += self.stats['speed']
    # shoots bullets

    def shoot(self):
        if self.cool_down <= 0:
            key = pygame.key.get_pressed()
            if key[pygame.K_SPACE]:
                x = self.rect.x + self.size[0] / 2
                y = self.rect.y - self.size[1]
                bullet = Bullets(
                    1, x, y, self.stats['bulletspeed'], WHITE, self.stats['bulletdmg'])
                game.player_bullet_group.add(bullet)
                self.cool_down = game.fps
        else:
            self.cool_down -= self.stats['firerate']
    # ticks down the players Invincability frames

    def Invincability_check(self):  # may be change to check if alive
        # need to fix the color flashing
        if self.IV_frames != 0:
            self.IV_frames -= 1
            self.color_tick += 1
            if self.color_tick == game.fps:
                if self.image.get_colorkey == GREEN:
                    self.image.fill(BLACK)
                else:
                    self.image.fill(GREEN)
                self.color_tick = 0


# class for creating player and enemy bullets


class Bullets(pygame.sprite.Sprite):
    def __init__(self, type, x, y, speed, color, dmg):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface([5, 10])
        self.type = type
        self.image.fill(color)
        self.rect = self.image.get_rect()
        self.rect.center = [x, y]
        self.speed = speed
        self.dmg = dmg
    # moves the bullet up if from player and down if from enemy

    def move(self):
        if self.type == 1:
            self.rect.y -= self.speed
        else:
            self.rect.y += self.speed

    def hit_player(self):
        hit_player = pygame.sprite.spritecollide(
            self, game.player_group, False)
        if hit_player:
            self.kill()
            if game.player.IV_frames == 0:
                # damages the player and gives them invicabilaty frames
                if game.player.stats['shield'] == 0:
                    game.player.stats['HP'] -= self.dmg  # change formula later
                else:
                    game.player.stats['shield'] -= 1
                game.player.IV_frames = game.fps * game.player.IV_time

# enemy classes start
# type 1: astroid, moves from the top of the screen to the bottom. 4 types: small, mid, large, and golden


class Asteroid(pygame.sprite.Sprite):
    # initalising astroid object
    def __init__(self, size, base_HP, color, speed, money, power):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface(size)
        pygame.draw.ellipse(self.image, color, self.image.get_rect())
        self.rect = self.image.get_rect()
        max_x = game.screen_size[0] - size[0]
        spawn_x = random.randint(size[0], int(max_x))
        self.rect.center = [spawn_x, -50]
        self.pause = 1
        # stats for the astroid, some may need to be changed
        self.stats = {
            'HP': base_HP * asteroid_stat[0] * (int(0.5 * game.level) + 1),
            'speed': speed + int(0.03 * game.level),
            '$': money * game.player.stats['$x'] * asteroid_stat[1] * (int(0.4 * game.level) + 1),
            'dmg_multi': power * asteroid_stat[2] + int(0.3 * game.level)
        }
    # moves the astroid and also checks if the astroid should be killed

    def move(self):
        if self.pause == 1:
            self.rect.y += self.stats['speed']
            self.pause += 2 - asteroid_stat[3]
        else:
            self.pause -= 1
        if self.rect.y > game.screen_size[1] + 100:
            self.kill()
        if self.stats['HP'] <= 0:  # add colision here
            self.kill()
            game.money += self.stats['$']

    # checks for collision and damages itself and the player by the damage variables stored in player classes.
    # also give invincability frames

    def colision(self):
        hit_player = pygame.sprite.spritecollide(
            self, game.player_group, False)
        hit_bullet = pygame.sprite.spritecollide(
            self, game.player_bullet_group, True)
        if hit_bullet:
            self.stats['HP'] -= game.player.stats['bulletdmg']
        if hit_player:
            self.kill()
            game.money += self.stats['$']
            if game.player.IV_frames == 0:
                # damages the player and gives them invicabilaty frames
                if game.player.stats['shield'] == 0:
                    # change formula later
                    game.player.stats['HP'] -= 5 * self.stats['dmg_multi']
                else:
                    game.player.stats['shield'] -= 1
                game.player.IV_frames = game.fps * game.player.IV_time

# sets peramiters for the astroid based on it's type


def create_astroid(num):
    type = 1
    if num > 1 and num <= 5:
        type = 4
    if num > 5 and num <= 20:
        type = 3
    if num > 20 and num <= 55:
        type = 2
    if type == 1:  # small
        asteroid = Asteroid([25, 25], 1, RED, 1, 1, 1)
    elif type == 2:  # mid
        asteroid = Asteroid([35, 35], 2, LIGHT_CRIMSION, 1, 2, 2)
    elif type == 3:  # large
        asteroid = Asteroid([50, 50], 3, DARK_CRIMSON, 1, 3, 3)
    elif type == 4:  # special
        asteroid = Asteroid([20, 20], 1, GOLD, 2, 10, 1)
    game.enemy_group_astroid.add(asteroid)
# other enemy ideas: one that shoots bullets forword, one that chases the player/suicide bombs, shotgun enemy,
# May be have special events so enemys spawn at different rates


class Shooter(pygame.sprite.Sprite):
    # initalising astroid object
    def __init__(self, size, base_HP, color, speed, money, fr, c_dmg, b_dmg, b_spd):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface(size)
        self.image.fill(color)
        self.color = color
        self.rect = self.image.get_rect()
        max_x = game.screen_size[0] - size[0]
        spawn_x = random.randint(size[0], int(max_x))
        self.rect.center = [spawn_x, -50]
        self.size = size
        self.cool_down = 100
        self.pause = 1
        # stats for the astroid, some may need to be changed
        self.stats = {
            'HP': (base_HP * shooter_stat[0]) * (int(0.2 * game.level) + 1),
            'speed': speed + int(game.level * 0.05),
            '$': (money * game.player.stats['$x'] * shooter_stat[4]) * (int(0.4 * game.level) + 1),
            'firerate': (fr * shooter_stat[1]),
            'contactdmg': (c_dmg * shooter_stat[2]) + (int(0.2 * game.level) + 1),
            'bulletdmg': (b_dmg * shooter_stat[3]) + (int(0.1 * game.level) + 1),
            'bulletspeed': b_spd + int(game.level * 0.05)
        }
    # moves the astroid and also checks if the astroid should be killed

    def move(self):
        if self.pause == 1:
            self.rect.y += self.stats['speed']
            self.pause += 3

        else:
            self.pause -= 1
        if self.rect.y > game.screen_size[1] + 100:
            self.kill()
        if self.stats['HP'] <= 0:  # add colision here
            self.kill()
            game.money += self.stats['$']
    # checks for collision and damages itself and the player by the damage variables stored in player classes.
    # also give invincability frames

    def colision(self):
        hit_player = pygame.sprite.spritecollide(
            self, game.player_group, False)
        hit_bullet = pygame.sprite.spritecollide(
            self, game.player_bullet_group, True)
        if hit_bullet:
            self.stats['HP'] -= game.player.stats['bulletdmg']
        if hit_player:
            self.kill()
            game.money += self.stats['$']
            if game.player.IV_frames == 0:
                # damages the player and gives them invicabilaty frames
                if game.player.stats['shield'] == 0:
                    # change formula later
                    game.player.stats['HP'] -= self.stats['contactdmg']
                else:
                    game.player.stats['shield'] -= 1
                game.player.IV_frames = game.fps * game.player.IV_time

    def shoot(self):
        if self.cool_down <= 0:
            x = self.rect.x + self.size[0] / 2
            y = self.rect.y + self.size[1]
            bullet = Bullets(
                2, x, y, self.stats['bulletspeed'], self.color, self.stats['bulletdmg'])
            game.ememy_bullets.add(bullet)
            self.cool_down = game.fps * 3
        else:
            self.cool_down -= self.stats['firerate']

# create a random type of shooter


def create_shooter(num):
    type = 1
    if num > 1 and num <= 3:
        type = 4
    if num > 3 and num <= 25:
        type = 3
    if num > 25 and num <= 50:
        type = 2
    if type == 1:  # normal
        shooter = Shooter([25, 25], 2, BLUE, 1, 2, 1, 3, 5, 1)
    elif type == 2:  # fast fire rate
        shooter = Shooter([25, 25], 1, LIGHT_BLUE, 1, 1.3, 3, 5, 2, 1)
    elif type == 3:  # strong bullets
        shooter = Shooter([30, 30], 3, PURPLE, 1, 3, 1, 3, 10, 1)
    elif type == 4:  # special
        shooter = Shooter([30, 30], 1, GOLD, 1, 20, 1.15, 5, 7, 2)
    game.enemy_group_shooter.add(shooter)
# enemy classes end

# spawns the enemies

# balance the spawn rates some how


def base_enemy_spawn(e):  # add more enemies when they come
    shooter_chance = random.randint(1, 3)
    if e == pygame.USEREVENT:
        create_astroid(random.randint(1, 100))
        if shooter_chance == 1:
            create_shooter(random.randint(1, 100))
# class to set up the interface and create the text to be displayed


class Interface():
    def __init__(self):
        pygame.font.init()
        self.font = pygame.font.SysFont("Times new Roman", 20)
        self.tick = 0
        self.timer = [0, 0]  # timer[0] = seconds, timer[1] = minutes
    # menu text

    def menu_text(self):
        self.font = pygame.font.SysFont("Times new Roman", 30)
        self.title = self.font.render("Chaotic Space Travel", True, WHITE)
        self.font = pygame.font.SysFont("Times new Roman", 15)
        self.start = self.font.render("NEW GAME", True, BLACK)
        self.quit = self.font.render("QUIT", True, BLACK)
    # difficulty screen text

    def difficulty_text(self):
        self.font = pygame.font.SysFont("Times new Roman", 25)
        self.select = self.font.render("SELECT", True, WHITE)
        self.dif = self.font.render("DIFFICULTY", True, WHITE)
        self.font = pygame.font.SysFont("Times new Roman", 15)
        self.easy = self.font.render("EASY", True, BLACK)
        self.normal = self.font.render("NORMAL", True, BLACK)
        self.hard = self.font.render("HARD", True, BLACK)
    # resets font and timer when the game starts

    def set_game_text(self):
        self.font = pygame.font.SysFont("Times new Roman", 20)
        self.tick = 0
        self.timer = [0, 0]
    # information shown during the game

    def update_text(self):
        self.font = pygame.font.SysFont("Times new Roman", 20)
        # add color changing in the future
        # showes health and shield
        self.health_text = self.font.render(
            "HP: {}/{}".format(game.player.stats['HP'], game.player.stats['maxHP']), True, GREEN)
        self.shield_text = self.font.render(
            "Shields: {}".format(game.player.stats['shield']), True, BLUE)
        # showes the timer
        if self.timer[0] < 10:
            self.time_text = self.font.render("{}:0{}".format(
                self.timer[1], self.timer[0]), True, WHITE)
        else:
            self.time_text = self.font.render("{}:{}".format(
                self.timer[1], self.timer[0]), True, WHITE)
        # showes the state of the laser
        if game.player.cool_down != 0:
            self.laserstate_text = self.font.render(
                "Cooling Down", True, RED)
        else:
            self.laserstate_text = self.font.render("Laser ready", True, GREEN)
        self.money_text = self.font.render(
            "${}".format(game.money), True, WHITE)
        self.difficulty_text = self.font.render(
            "{}".format(game.difficulty), True, WHITE)
        self.lv = self.font.render(
            "Level: {}".format(game.level), True, WHITE)

    # timer that the player sees and also spawns the enemy_group_astroid
    # will also end the current level

    def timer_update(self):
        self.tick += 1
        if self.tick == game.fps:
            self.timer[0] += 1
            self.tick = 0
            if self.timer[0] == 59:
                self.timer[1] += 1
                self.timer[0] = 0
    # The text that will be showen on the shop button and screen

    def shop_text(self):
        self.font = pygame.font.SysFont("Times new Roman", 25)
        self.upgrade_text = self.font.render("UPGRADES", True, WHITE)
        self.font = pygame.font.SysFont("Times new Roman", 20)
        self.cmoney_text = self.font.render(
            "${}".format(game.money), True, WHITE)
        self.next_text = self.font.render("Next Level", True, BLACK)
        self.font = pygame.font.SysFont("Times new Roman", 15)
        # health button
        self.HP_text = self.font.render("Increase Max HP", True, BLACK)
        self.HPc = self.font.render(
            "cost: ${}".format(game.cost['HP']), True, BLACK)
        self.HPcr = self.font.render("current: {}".format(
            game.player.stats['maxHP']), True, BLACK)
        # shield button
        self.shield_text = self.font.render("Increase number", True, BLACK)
        self.shield_text2 = self.font.render("of Shields", True, BLACK)
        self.shieldc = self.font.render(
            "cost: ${}".format(game.cost['sld']), True, BLACK)
        if game.player.stats['maxshield'] <= 5:
            self.shieldcr = self.font.render("current: {}".format(
                game.player.stats['maxshield']), True, BLACK)
        else:
            self.shieldcr = self.font.render("Maxed: {}".format(
                game.player.stats['maxshield']), True, BLACK)
        # money button
        self.money_text = self.font.render("Increase money", True, BLACK)
        self.money_text2 = self.font.render("gained", True, BLACK)
        self.moneyc = self.font.render(
            "cost: ${}".format(game.cost['$']), True, BLACK)
        self.moneycr = self.font.render(
            "current: {}x".format(game.player.stats['$x']), True, BLACK)
        # firerate button
        self.fr_text = self.font.render("Increase firerate", True, BLACK)
        self.frc = self.font.render(
            "cost: ${}".format(game.cost['fr']), True, BLACK)
        if game.player.stats['firerate'] <= 4:
            self.frcr = self.font.render(
                "current: {}/s".format(game.player.stats['firerate']), True, BLACK)
        else:
            self.frcr = self.font.render(
                "Maxed: {}/s".format(game.player.stats['firerate']), True, BLACK)
        # dmg button
        self.dmga_text = self.font.render("Increase DMG", True, BLACK)
        self.dmgac = self.font.render(
            "cost: ${}".format(game.cost['dmg+']), True, BLACK)
        self.dmgacr = self.font.render("current: {}".format(
            game.player.stats['bulletdmg']), True, BLACK)
        # dmg multi button
        self.dmgm_text = self.font.render("Multiply DMG", True, BLACK)
        self.dmgmc = self.font.render(
            "cost: ${}".format(game.cost['dmgx']), True, BLACK)
        self.dmgmcr = self.font.render("multi: {}".format(
            math.pow(1.5, game.x['dmgx'])), True, BLACK)
        self.speed_text = self.font.render("Increase movement", True, BLACK)
        self.speed_text2 = self.font.render("speed", True, BLACK)
        self.speedc = self.font.render(
            "cost: ${}".format(game.cost['speed']), True, BLACK)
        if game.player.stats['speed'] <= 4:
            self.speedcr = self.font.render(
                "current: {}/s".format(game.player.stats['speed']), True, BLACK)
        else:
            self.speedcr = self.font.render(
                "Maxed: {}/s".format(game.player.stats['speed']), True, BLACK)

    def over_text(self):
        self.font = pygame.font.SysFont("Times new Roman", 50)
        self.gameover = self.font.render("GAME OVER", True, RED)
        self.font = pygame.font.SysFont("Times new Roman", 20)
        self.restart = self.font.render("Restart", True, BLACK)
        self.exit = self.font.render("Main Menu", True, BLACK)
        self.endlv = self.font.render(
            "Reached Level: {}".format(game.level), True, WHITE)
# class that makes the game run and sets up main variables.


class Buttons(pygame.sprite.Sprite):
    def __init__(self, size, pos, color):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface(size)
        self.image.fill(color)
        self.rect = self.image.get_rect()
        self.rect.center = pos
        self.pos = pos

    def clicked(self, mouse_pos):
        if self.rect.collidepoint(mouse_pos):
            return True

# maybe turn into a class and have a dictionary store the variable for the enemy stats and give the variable to the enemy at creation
# possible variables: enemy base dmg and base health, upgrade cost, score multi


def difficulty_variables(mode):
    global shooter_stat
    global asteroid_stat
    if mode == 'easy':
        pygame.time.set_timer(pygame.USEREVENT, 5000)
        game.cost = {
            'HP': 50, 'sld': 80, '$': 77,
            'fr': 100, 'dmg+': 100, 'dmgx': 200,
            'speed': 55.55
        }
        game.formula = {
            'HP': lambda a: a * 1.5,
            'sld': lambda a: a * 2,
            '$': lambda a: a * 4,
            'fr': lambda a: a * 2,
            'dmg+': lambda a: a * 1.3,
            'dmgx': lambda a: a * 2,
            'speed': lambda a: a * 2.5
        }
        shooter_stat = [1, 1, 1, 1, 1]
        asteroid_stat = [1, 1, 1, 0]
    elif mode == 'normal':
        pygame.time.set_timer(pygame.USEREVENT, 4000)
        game.cost = {
            'HP': 70, 'sld': 100, '$': 77.7,
            'fr': 150, 'dmg+': 150, 'dmgx': 300,
            'speed': 75.22
        }
        game.formula = {
            'HP': lambda a: a * 1.7,
            'sld': lambda a: a * 2,
            '$': lambda a: a * 5,
            'fr': lambda a: a * 2.5,
            'dmg+': lambda a: a * 1.5,
            'dmgx': lambda a: a * 3,
            'speed': lambda a: a * 2.6
        }
        shooter_stat = [1.5, 1.25, 2, 1.5, 2]
        asteroid_stat = [1.25, 2, 1.5, 0]
    elif mode == 'hard':
        pygame.time.set_timer(pygame.USEREVENT, 3000)
        game.cost = {
            'HP': 100, 'sld': 150, '$': 77.77,
            'fr': 200, 'dmg+': 200, 'dmgx': 500,
            'speed': 122.33
        }
        game.formula = {
            'HP': lambda a: a * 2,
            'sld': lambda a: a * 2,
            '$': lambda a: a * 5,
            'fr': lambda a: a * 3,
            'dmg+': lambda a: a * 1.5,
            'dmgx': lambda a: a * 3.5,
            'speed': lambda a: a * 2.6
        }
        shooter_stat = [2, 1.5, 3, 2, 3]
        asteroid_stat = [1.5, 3, 2, 1]


class Game():
    def __init__(self):
        pygame.init()
        self.clock = pygame.time.Clock()
        self.fps = 60
        self.bg = BLACK  # background
        self.screen_size = [400, 400]
        self.screen = pygame.display.set_mode(self.screen_size)
        self.i_f = Interface()
        self.score = 0  # will be calculated at the end. maybe 10x money * level
        self.money = 0
        self.level = 1  # used for scaling the enemies and dificulty
        self.player = Player([20, 20])
        self.player_group = pygame.sprite.Group()
        self.player_group.add(self.player)
        self.enemy_group_astroid = pygame.sprite.Group()
        self.player_bullet_group = pygame.sprite.Group()
        self.enemy_group_shooter = pygame.sprite.Group()
        self.ememy_bullets = pygame.sprite.Group()
        # checks if you are in a specific screen
        self.state = {
            'running': True, 'menu': True,
            'diffscreen': False, 'game': False,
            'shop': False, 'over': False
        }
        # creates all the buttons for the menu
        # buttons and group for the main menu
        self.start_button = Buttons(
            [100, 50], [self.screen_size[0]/2, self.screen_size[1]/2], WHITE)
        self.quit_button = Buttons(
            [100, 50], [self.screen_size[0]/2, self.screen_size[1]/2 + 70], RED)
        self.menu_buttons = pygame.sprite.Group()
        self.menu_buttons.add(self.start_button, self.quit_button)
        # buttons and group for the difficulty screen
        self.easy_button = Buttons(
            [100, 50], [self.screen_size[0]/2, self.screen_size[1]/2 - 70], GREEN)
        self.normal_button = Buttons(
            [100, 50], [self.screen_size[0]/2, self.screen_size[1]/2], YELLOW)
        self.hard_button = Buttons(
            [100, 50], [self.screen_size[0]/2, self.screen_size[1]/2 + 70], ORANGE)
        self.difficulty_buttons = pygame.sprite.Group()
        self.difficulty_buttons.add(
            self.easy_button, self.normal_button, self.hard_button)
        # buttons and group for the shop
        self.HPup_button = Buttons(
            [110, 70], [self.screen_size[0]/5, self.screen_size[1]/3], RED)
        self.shieldup_button = Buttons(
            [110, 70], [self.screen_size[0]/2, self.screen_size[1]/3], BLUE)
        self.moneyup_button = Buttons(
            [110, 70], [self.screen_size[0]/1.25, self.screen_size[1]/3], GREEN)
        self.firerateup_button = Buttons(
            [110, 70], [self.screen_size[0]/1.25, self.screen_size[1]/3 + 80], BLUE)
        self.dmgup_button = Buttons(
            [110, 70], [self.screen_size[0]/5, self.screen_size[1]/3 + 80], RED)
        self.speedup_button = Buttons(
            [110, 70], [self.screen_size[0]/2, self.screen_size[1]/1.38], WHITE)
        self.dmgmultiup_button = Buttons(
            [110, 70], [self.screen_size[0]/2, self.screen_size[1]/3 + 80], RED)
        self.next_button = Buttons(
            [100, 50], [self.screen_size[0] - 70, self.screen_size[1] - 60], WHITE)
        self.shop_buttons = pygame.sprite.Group()
        self.shop_buttons.add(self.HPup_button, self.shieldup_button, self.moneyup_button, self.firerateup_button,
                              self.dmgup_button, self.dmgmultiup_button, self.speedup_button, self.next_button)
        # calculate the cost of the upgrades
        self.cost = {}
        self.formula = {}
        self.x = {'HP': 0, 'sld': 0, '$': 0, 'fr': 0,
                  'dmg+': 0, 'dmgx': 0, 'speed': 0}
        # buttons and group for the game over screen
        self.restart_button = Buttons(
            [100, 50], [self.screen_size[0]/2 - 60, self.screen_size[1]/2 + 70], WHITE)
        self.remenu_button = Buttons(
            [100, 50], [self.screen_size[0]/2 + 60, self.screen_size[1]/2 + 70], RED)
        self.over_buttons = pygame.sprite.Group()
        self.over_buttons.add(self.restart_button, self.remenu_button)
        # move this to menu
        self.difficulty = None
        # timer for spaning enemies. move to menu
        pygame.time.set_timer(pygame.USEREVENT, 5000)
    # showes the menu screen

    def main_Menu(self):
        while self.state['menu']:
            self.screen.fill(self.bg)
            for e in pygame.event.get():
                if e.type == pygame.QUIT:
                    self.state['menu'] = False
                    self.state['running'] = False
                if e.type == pygame.KEYDOWN:
                    key = pygame.key.get_pressed()
                    if key == pygame.K_ESCAPE:
                        self.state['menu'] = False
                        self.state['running'] = False
                # checks which button is pressed
                if e.type == pygame.MOUSEBUTTONDOWN:
                    if self.start_button.clicked(e.pos):
                        self.state['menu'] = False
                        self.state['diffscreen'] = True
                    if self.quit_button.clicked(e.pos):
                        self.state['menu'] = False
                        self.state['running'] = False
            self.menu_buttons.draw(self.screen)
            blit_text()
            pygame.display.update()
            self.clock.tick(self.fps)
    # The difficulty select screen

    def difficulty_screen(self):
        while self.state['diffscreen']:
            self.screen.fill(self.bg)
            for e in pygame.event.get():
                if e.type == pygame.QUIT:
                    self.state['diffscreen'] = False
                    self.state['running'] = False
                if e.type == pygame.KEYDOWN:
                    key = pygame.key.get_pressed()
                    if key == pygame.K_ESCAPE:
                        self.state['diffscreen'] = False
                        self.state['running'] = False
                # checks which button is pressed and sets difficulty
                if e.type == pygame.MOUSEBUTTONDOWN:
                    if self.easy_button.clicked(e.pos):
                        self.difficulty = 'easy'
                        self.state['diffscreen'] = False
                        self.state['game'] = True
                    if self.normal_button.clicked(e.pos):
                        self.difficulty = 'normal'
                        self.state['diffscreen'] = False
                        self.state['game'] = True
                    if self.hard_button.clicked(e.pos):
                        self.difficulty = 'hard'
                        self.state['diffscreen'] = False
                        self.state['game'] = True
                    difficulty_variables(self.difficulty)
            self.difficulty_buttons.draw(self.screen)
            blit_text()
            pygame.display.update()
            self.clock.tick(self.fps)
    # runs the main game

    def run_Game(self):
        self.i_f.set_game_text()
        self.player.stats['HP'] = self.player.stats['maxHP']
        self.player.stats['shield'] = self.player.stats['shield']
        while self.state['game']:
            self.money = math.ceil(self.money)
            self.screen.fill(self.bg)
            for e in pygame.event.get():
                if e.type == pygame.QUIT:
                    self.state['game'] = False
                    self.state['running'] = False
                if e.type == pygame.KEYDOWN:
                    key = pygame.key.get_pressed()
                    if key == pygame.K_ESCAPE:
                        self.state['game'] = False
                        self.state['running'] = False
                if self.i_f.timer[1] < 3:
                    base_enemy_spawn(e.type)
            if self.player.stats['HP'] <= 0:
                self.state['game'] = False
                self.state['over'] = True
            # go to the shop screen and increase level of enemies
            if self.i_f.timer[1] == 3 and self.i_f.timer[0] == 20:
                self.state['game'] = False
                self.state['shop'] = True
                self.level += 1

            # draws all players and enemys to the screen
            draw()
            # creates the text for the Interface and displayes it.#move to a fuction later
            blit_text()
            # players actions
            self.player.movement()
            self.player.Invincability_check()
            self.player.shoot()
            # fuction to call all methods of the groups
            group_methods()

            pygame.display.update()
            self.clock.tick(self.fps)

    def upgrade_Menu(self):
        while self.state['shop']:
            self.screen.fill(self.bg)
            for e in pygame.event.get():
                if e.type == pygame.QUIT:
                    self.state['shop'] = False
                    self.state['running'] = False
                if e.type == pygame.KEYDOWN:
                    key = pygame.key.get_pressed()
                    if key == pygame.K_ESCAPE:
                        self.state['shop'] = False
                        self.state['running'] = False
                # checks which button is pressed
                if e.type == pygame.MOUSEBUTTONDOWN:
                    if game.next_button.clicked(e.pos):
                        self.state['shop'] = False
                        self.state['game'] = True
                    upgrade_check(e.pos)
            self.shop_buttons.draw(self.screen)
            blit_text()
            pygame.display.update()
            self.clock.tick(self.fps)

    def game_over_screen(self):
        while self.state['over']:
            self.screen.fill(self.bg)
            for e in pygame.event.get():
                if e.type == pygame.QUIT:
                    self.state['over'] = False
                    self.state['running'] = False
                if e.type == pygame.KEYDOWN:
                    key = pygame.key.get_pressed()
                    if key == pygame.K_ESCAPE:
                        self.state['over'] = False
                        self.state['running'] = False
                # checks which button is pressed
                if e.type == pygame.MOUSEBUTTONDOWN:
                    if self.restart_button.clicked(e.pos):
                        self.state['over'] = False
                        self.state['game'] = True
                    if self.remenu_button.clicked(e.pos):
                        self.state['over'] = False
                        self.state['menu'] = True
            self.over_buttons.draw(self.screen)
            blit_text()
            pygame.display.update()
            self.clock.tick(self.fps)
# drawses everything to the screen


def draw():
    game.player_group.draw(game.screen)
    game.enemy_group_astroid.draw(game.screen)
    game.player_bullet_group.draw(game.screen)
    game.enemy_group_shooter.draw(game.screen)
    game.ememy_bullets.draw(game.screen)


def blit_text():
    # text for the menu screen
    if game.state['menu']:
        game.i_f.menu_text()
        game.screen.blit(game.i_f.title, [
                         game.screen_size[0]/2 - 120, game.screen_size[1]/2 - 100])
        game.screen.blit(game.i_f.start, [
                         game.start_button.rect.x + 10, game.start_button.rect.y + 20])
        game.screen.blit(
            game.i_f.quit, [game.quit_button.rect.x + 30, game.quit_button.rect.y + 20])
    # text for the difficulty screen
    if game.state['diffscreen']:
        game.i_f.difficulty_text()
        game.screen.blit(game.i_f.select, [game.screen_size[0]/2 - 50, 10])
        game.screen.blit(game.i_f.dif, [game.screen_size[0]/2 - 70, 40])
        game.screen.blit(
            game.i_f.easy, [game.easy_button.rect.x + 30, game.easy_button.rect.y + 20])
        game.screen.blit(game.i_f.normal, [
                         game.normal_button.rect.x + 20, game.normal_button.rect.y + 20])
        game.screen.blit(
            game.i_f.hard, [game.hard_button.rect.x + 30, game.hard_button.rect.y + 20])
    # display text for the game.
    if game.state['game']:
        game.i_f.timer_update()
        game.i_f.update_text()
        game.screen.blit(game.i_f.health_text, [
            20, game.screen_size[1] - 70])
        game.screen.blit(game.i_f.shield_text, [
            20, game.screen_size[1] - 50])
        game.screen.blit(game.i_f.lv, [20, 20])
        game.screen.blit(game.i_f.time_text, [40, 40])
        game.screen.blit(game.i_f.laserstate_text, [
            game.screen_size[0] - 130, game.screen_size[1] - 70])
        game.screen.blit(game.i_f.money_text, [
            game.screen_size[0] / 2 - 30, 15])
        game.screen.blit(game.i_f.difficulty_text, [
            game.screen_size[0] - 70, 15])
    if game.state['shop']:
        # displays text for the upgrade buttons
        game.i_f.shop_text()
        game.screen.blit(game.i_f.upgrade_text, [
                         game.screen_size[0]/2 - 70, 20])
        game.screen.blit(game.i_f.cmoney_text, [
                         game.screen_size[0]/2 - 20, 50])
        game.screen.blit(game.i_f.next_text, [
                         game.next_button.rect.x, game.next_button.rect.y + 10])
        # hpbutton
        game.screen.blit(game.i_f.HP_text, [
                         game.HPup_button.rect.x, game.HPup_button.rect.y])
        game.screen.blit(
            game.i_f.HPc, [game.HPup_button.rect.x, game.HPup_button.rect.y + 20])
        game.screen.blit(
            game.i_f.HPcr, [game.HPup_button.rect.x, game.HPup_button.rect.y + 40])
        # shieldbutton
        game.screen.blit(game.i_f.shield_text, [
                         game.shieldup_button.rect.x, game.shieldup_button.rect.y])
        game.screen.blit(game.i_f.shield_text2, [
                         game.shieldup_button.rect.x, game.shieldup_button.rect.y + 15])
        game.screen.blit(game.i_f.shieldc, [
                         game.shieldup_button.rect.x, game.shieldup_button.rect.y + 30])
        game.screen.blit(game.i_f.shieldcr, [
                         game.shieldup_button.rect.x, game.shieldup_button.rect.y + 45])
        # money Buttons
        game.screen.blit(game.i_f.money_text, [
                         game.moneyup_button.rect.x, game.moneyup_button.rect.y])
        game.screen.blit(game.i_f.money_text2, [
                         game.moneyup_button.rect.x, game.moneyup_button.rect.y + 15])
        game.screen.blit(game.i_f.moneyc, [
                         game.moneyup_button.rect.x, game.moneyup_button.rect.y + 30])
        game.screen.blit(game.i_f.moneycr, [
                         game.moneyup_button.rect.x, game.moneyup_button.rect.y + 45])
        # firerate button
        game.screen.blit(game.i_f.fr_text, [
                         game.firerateup_button.rect.x, game.firerateup_button.rect.y])
        game.screen.blit(game.i_f.frc, [
                         game.firerateup_button.rect.x, game.firerateup_button.rect.y + 20])
        game.screen.blit(game.i_f.frcr, [
                         game.firerateup_button.rect.x, game.firerateup_button.rect.y + 40])
        # dmg buttons
        game.screen.blit(game.i_f.dmga_text, [
                         game.dmgup_button.rect.x, game.dmgup_button.rect.y])
        game.screen.blit(game.i_f.dmgac, [
                         game.dmgup_button.rect.x, game.dmgup_button.rect.y + 20])
        game.screen.blit(game.i_f.dmgacr, [
                         game.dmgup_button.rect.x, game.dmgup_button.rect.y + 40])
        # dmg multi buttons
        game.screen.blit(game.i_f.dmgm_text, [
                         game.dmgmultiup_button.rect.x, game.dmgmultiup_button.rect.y])
        game.screen.blit(game.i_f.dmgmc, [
                         game.dmgmultiup_button.rect.x, game.dmgmultiup_button.rect.y + 20])
        game.screen.blit(game.i_f.dmgmcr, [
                         game.dmgmultiup_button.rect.x, game.dmgmultiup_button.rect.y + 40])
        # speed button
        game.screen.blit(game.i_f.speed_text, [
                         game.speedup_button.rect.x, game.speedup_button.rect.y])
        game.screen.blit(game.i_f.speed_text2, [
                         game.speedup_button.rect.x, game.speedup_button.rect.y + 15])
        game.screen.blit(game.i_f.speedc, [
                         game.speedup_button.rect.x, game.speedup_button.rect.y + 30])
        game.screen.blit(game.i_f.speedcr, [
                         game.speedup_button.rect.x, game.speedup_button.rect.y + 45])
    if game.state['over']:
        game.i_f.over_text()
        game.screen.blit(game.i_f.gameover, [
                         game.screen_size[0]/2 - 150, game.screen_size[1]/2 - 100])
        game.screen.blit(game.i_f.endlv, [
                         game.screen_size[0]/2 - 60, game.screen_size[1]/2 - 20])
        game.screen.blit(game.i_f.restart, [
                         game.restart_button.rect.x + 20, game.restart_button.rect.y + 15])
        game.screen.blit(
            game.i_f.exit, [game.remenu_button.rect.x + 3, game.remenu_button.rect.y + 15])

# checks which button is clicked and adusts the variables nessecery


def upgrade_check(e):
    if game.HPup_button.clicked(e) and game.cost['HP'] <= game.money:
        game.player.stats['maxHP'] += 10
        game.x['HP'] += 1
        game.money -= game.cost['HP']
        game.cost['HP'] = math.floor(game.formula['HP'](game.cost['HP']))
    if game.shieldup_button.clicked(e) and game.cost['sld'] <= game.money and game.player.stats['maxshield'] <= 5:
        game.player.stats['maxshield'] += 1
        game.x['sld'] += 1
        game.money -= game.cost['sld']
        game.cost['sld'] = math.floor(game.formula['sld'](game.cost['sld']))
    if game.moneyup_button.clicked(e) and game.cost['$'] <= game.money:
        game.player.stats['$x'] *= 1.5
        game.x['$'] += 1
        game.money -= game.cost['$']
        game.cost['$'] = math.floor(game.formula['$'](game.cost['$']))
    if game.firerateup_button.clicked(e) and game.cost['fr'] <= game.money and game.player.stats['firerate'] <= 4:
        game.player.stats['firerate'] += 0.2
        game.x['fr'] += 1
        game.money -= game.cost['fr']
        game.cost['fr'] = math.floor(game.formula['fr'](game.cost['fr']))
    if game.dmgup_button.clicked(e) and game.cost['dmg+'] <= game.money:
        game.player.stats['bulletdmg'] += 2
        game.x['dmg+'] += 1
        game.money -= game.cost['dmg+']
        game.cost['dmg+'] = math.floor(game.formula['dmg+'](game.cost['dmg+']))
    if game.dmgmultiup_button.clicked(e) and game.cost['dmgx'] <= game.money:
        game.player.stats['bulletdmg'] *= 2
        game.x['dmgx'] += 1
        game.money -= game.cost['dmgx']
        game.cost['dmgx'] = math.floor(game.formula['dmgx'](game.cost['dmgx']))
    if game.speedup_button.clicked(e) and game.cost['speed'] <= game.money and game.player.stats['speed'] <= 4:
        game.player.stats['speed'] += 1
        game.x['speed'] += 1
        game.money -= game.cost['speed']
        game.cost['speed'] = math.floor(
            game.formula['speed'](game.cost['speed']))


def group_methods():
    # moves astroids and checks colision
    for i in game.enemy_group_astroid:
        i.move()
        i.colision()
    # moves players bullets
    for i in game.player_bullet_group:
        i.move()
    # moves the shooters, checks there colision, and makes them shoot
    for i in game.enemy_group_shooter:
        i.move()
        i.colision()
        i.shoot()
    for i in game.ememy_bullets:
        i.move()
        i.hit_player()


game = Game()
while game.state['running']:
    if game.state['menu']:
        game.main_Menu()
    elif game.state['diffscreen']:
        game.difficulty_screen()
    elif game.state['game']:
        game.run_Game()
    elif game.state['shop']:
        game.upgrade_Menu()
    elif game.state['over']:
        game.game_over_screen()
pygame.quit()
sys.exit()
