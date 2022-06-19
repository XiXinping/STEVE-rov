import io
import PIL
import pygame.camera
import pygame.image

def main():
    pygame.camera.init()
    camera_name = pygame.camera.list_cameras()[1]
    camera = pygame.camera.Camera(camera_name, 1024, 768)
    camera.start()

    image = camera.get_image()
    image_string = pygame.image.tostring(image, "RGB", False)
    pil_image = PIL.Image.frombytes("RGB", image.get_size(), image_string)
    pil_image.save("image.jpg", "JPEG", quality=95, optimize=True)


if __name__ == "__main__":

