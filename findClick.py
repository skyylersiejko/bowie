import cv2
from math import sqrt, pow
import numpy as np
import json

class Point:
    def __init__(self, x, y, name):
        self.x = x
        self.y = y
        self.name = name
setofPoints = []

try:
    with open("dotter/graph.json", 'r') as file:
        points = json.load(file)

    for i in range(len(points)):
        other_i = list(points.keys())[i]
        point_data = Point(points[other_i]["x"], points[other_i]["y"], other_i)
        setofPoints.append(point_data)
        globals()[f"p{i}"] = point_data

    print(setofPoints)
        
    file.close()
    pass
except FileNotFoundError:
    print(f"Error: The file was not found.")

def drawSprite(loc):
    for i in range(sprite.shape[0]):
        for j in range(sprite.shape[1]):
            if list(sprite[i][j]) != [232, 162, 0]:
                image[i+loc[0]][j+loc[1]] = sprite[i][j]

allPoints = []

def mouse_callback(event, x, y, flags, params, points=allPoints):
    if event == 1:
        image = frame.copy()
        bestDist = 1000
        bestPoint = None
        if len(allPoints) == 0:
            return None
        for i in range(len(allPoints)):
            if sqrt(pow(abs(allPoints[i][0]-x), 2) + pow(abs(allPoints[i][1]-y), 2)) < bestDist:
                bestDist = sqrt(pow(abs(allPoints[i][0]-x), 2) + pow(abs(allPoints[i][1]-y), 2))
                bestPoint = allPoints[i]
        drawSprite([bestPoint[1]-sprite.shape[0]//2, bestPoint[0]-sprite.shape[1]//2])
        #cv2.circle(image, (bestPoint[0], bestPoint[1]), 5, (255, 0, 0), -1)

        

frame = cv2.imread("dotter/Screenshot 2025-07-28 221547.png")
sprite = cv2.imread("treeWithBackground.png")
sprite = cv2.resize(sprite, (sprite.shape[0]//2, sprite.shape[1]//2))
for i in setofPoints:
    cv2.circle(frame, (i.x, i.y), 5, (255, 0, 0), -1)
    allPoints.append((i.x, i.y))
    lookFor = points[i.name]["connections"]
    for j in setofPoints:
        if j.name in lookFor:
            print(j.name, lookFor)
            cv2.line(frame, (j.x, j.y), (i.x, i.y), (255, 0, 0), 2)

loc = [0, 0]
image = frame.copy()
#image = frame.copy()
while True:
    cv2.namedWindow("Image")
    cv2.setMouseCallback("Image", mouse_callback)
    cv2.imshow("Image", image)
    key = cv2.waitKey(1) & 0xFF
    if key == ord("q"):
        break
cv2.destroyAllWindows()