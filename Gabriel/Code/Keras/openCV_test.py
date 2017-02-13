import numpy as np
import cv2

img = cv2.imread('iccv09Data/images/0000176.jpg')

cv2.imshow('image',img)

img_copy = img #cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)



with open('iccv09Data/labels/0000176.regions.txt') as f:
    counter = 0
    for line in f:

        for i in range(0,len(line),2):
            if int(line[i]) == 0 :
                img_copy[counter][int(i/2)] = [0,0,255]
            if int(line[i]) == 1 :
                img_copy[counter][int(i/2)] = [0,255,0]
            if int(line[i]) == 2 :
                img_copy[counter][int(i/2)] = [255,0,0]
            if int(line[i]) == 3 :
                img_copy[counter][int(i/2)] = [255,255,0]
            if int(line[i]) == 4 :
                img_copy[counter][int(i/2)] = [255,0,255]
            if int(line[i]) == 5 :
                img_copy[counter][int(i/2)] = [0,255,255]
            if int(line[i]) == 6 :
                img_copy[counter][int(i/2)] = [255,0,0]
            if int(line[i]) == 7 :
                img_copy[counter][int(i/2)] = [255,255,255]
                    
            
            
        counter += 1



cv2.imshow('surface',img_copy)

cv2.waitKey(0)
cv2.destroyAllWindows()
