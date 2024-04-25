#!/usr/bin/python3

from PIL import Image
#import numpy

#im = Image.open('/mnt/ramdisk/FireIndex_NEW.png') # Can be many different formats.
im = Image.open('/mnt/ramdisk/NDDESFireIndex.png')

pix = im.load()
#print(im.size)  # Get the width and hight of the image for iterating over
#print(pix[550,620])  # Get the RGBA Value of the a pixel of an image
#pix[x,y] = value  # Set the RGBA Value of the image (tuple)
#im.save('alive_parrot.png')  # Save the modified pixels as .png

green=pix[40,230]
blue=pix[40,260]
yellow=pix[40,285]
orange=pix[40,310]
red=pix[40,336]

color=pix[550,620];

riskscore=4

if color==green:
        print("LOW-green")
        riskscore=0
if color==blue:
        print("MODERATE-blue")
        riskscore=1
if color==yellow:
        print("HIGH-yellow")
        riskscore=2
if color==orange:
        print("VERY HIGH-orange")
        riskscore=3
if color==red:
        print("EXTREME-red")
        riskscore=4




if 'riskscore' in locals():
        f = open('/home/weewx/archive/fire.txt','w')
        f.write(str(riskscore))
        f.close()



