import pygame

pygame.font.init()


class Buttons():
    def __init__(self, pos, size, color, text, tcolor):
        self.image = pygame.Surface((size *1.25,size//1.5))
        self.image.fill(color)
        self.rect = self.image.get_rect(center = pos)
        self.font = pygame.font.SysFont('Futura',size//3)
        self.text = text
        self.text_surf = self.font.render(text,False,tcolor)
        self.text_rect = self.text_surf.get_rect(center = self.rect.center)
        self.clicked = False
    def update(self, screen):
        action = False
    

        pos = pygame.mouse.get_pos()
        if self.rect.collidepoint(pos):
            if pygame.mouse.get_pressed()[0] == 1 and not self.clicked:
                self.clicked = True
                action = True
            if pygame.mouse.get_pressed()[0] == 0:
                self.clicked = False
            
        
        screen.blit(self.image, self.rect)
        screen.blit(self.text_surf, self.text_rect)
        return action

class draw_text():
    def __init__(self,size,text,color):
        self.text = text
        self.font = pygame.font.SysFont('Futura',size)

        self.text_surf = self.font.render(text,True,color)
    def draw(self,pos, screen):
        screen.blit(self.text_surf,pos)

class level_selet():
    def __init__(self,level, complete):
        self.buttons = []
        self.text = draw_text(60,"Select Level", 'white')
        if complete: level = 30
        for i in range(level):
            if i < 6:
                b = Buttons((130*i + 50,150) ,90,'white',str(i +1),'black')
            elif i < 12:
                b = Buttons((130*(i -6) + 50,300),90,'white',str(i+1),'black')
            elif i < 18:
                b = Buttons((130*(i - 12)+ 50,450),90,'white',str(i+1),'black')
            elif i < 24:
                b = Buttons((130*(i - 18)+ 50,600),90,'white',str(i+1),'black')
            elif i < 30:
                b = Buttons((130*(i - 24)+ 50,750),90,'white',str(i+1),'black')
            self.buttons.append(b)
    def draw(self, screen):
        self.text.draw((250,50),screen)
        for b in self.buttons:

            if b.update(screen):
                return int(b.text)

        