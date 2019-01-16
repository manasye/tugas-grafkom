import pyTGA
'''
For information about the library, consult :
https://github.com/MircoT/pyTGA 
'''

# Load image
# The image MUST ONLY BE BLACK AND WHITE, 
# where white is the background, and black is the actual "object" to be mapped to pixels
image = pyTGA.Image()
image.load("test.tga")

# List of pixels to be put into the text file
pixels_to_render = []

# Get information about the image
pixels = image.get_pixels()

i = 0
j = 0
for row in pixels:
    i = 0
    for col in row:
        # If it's not white, put into the list
        if (col[0] != 255) and (col[1] != 255) and (col[2] != 255):
            pixels_to_render.append((i,j))
        i += 1
    j += 1

# "Dump" list to file
with open("test.txt","w") as f:
    for pixel in pixels_to_render:
        f.write(str(pixel[0]) + "," + str(pixel[1]) + "\n")