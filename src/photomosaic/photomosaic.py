import pygame.camera
import pygame.image
from termcolor import colored

class Camera:
    def __init__(self):
        pygame.camera.init()
        self.camera = pygame.camera.Camera(
            pygame.camera.list_cameras()[1], (1024, 768))
        self.camera.start()

    def capture_image(self, image_num):
        image = self.camera.get_image()
        pygame.image.save(image, "image{}.jpg".format(image_num))

def main():
    print(colored("STEVE's Photomosaic Generator", "blue"))
    camera = Camera()

    for i in range(8):
        input("")
        camera.capture_image(i)



if __name__ == "__main__":
    main()
