import cv2
from math import sqrt, pow
import numpy as np
import json
import callDijkstra as cdij
from time import sleep

class Point:
    def __init__(self, x, y, name):
        self.x = x
        self.y = y
        self.name = name
    def __str__(self):
        return f"({self.x}, {self.y})"
setofPoints = []

def drawPath(path="path/conner.json"):
    global image
    setofPointsN = []
    image = frame.copy()
    try:
        with open(path, 'r') as file:
            points = json.load(file)
        for other_i in list(points.keys()):
            point_data = Point(points[other_i]["x"], points[other_i]["y"], other_i)
            setofPointsN.append(point_data)

            
        file.close()
        pass
    except FileNotFoundError:
        print(f"Error: The file was not found.")
    c = 0
    for i in setofPointsN:
        c+=1
        if c == 1 or c == len(setofPointsN):
            if c == 1:
                cv2.circle(image, (i.x, i.y), 10, (0, 0, 255), -1)
            else:
                drawSprite([i.y-sprite.shape[0]//2, i.x-sprite.shape[1]//2])

        lookFor = points[i.name]["connections"]
        for j in setofPointsN:
            if j.name in lookFor:
                cv2.line(image, (j.x, j.y), (i.x, i.y), (255, 0, 255), 2)
    

global currentPoint_
global pointNames_
global clickedPoints_
currentPoint_ = 0
pointNames_ = [[]]
clickedPoints_ = [[]]

pointsMain = []

for i in ["conner"]:
    try:
        with open(f"graphs/{i}.json", 'r') as file:
            points = json.load(file)

        for i in range(len(points)):
            other_i = list(points.keys())[i]
            point_data = Point(points[other_i]["x"], points[other_i]["y"], other_i)
            setofPoints.append(point_data)
            
        file.close()
        pointsMain.append(setofPoints)
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
    global pointNames_
    global image
    global clickedPoints_
    global currentPoint_
    if event == 1 and currentPoint_ < 2:
        currentPoint_ += 1
        bestDist = [1000, 1000]
        bestPoint = [None, None]
        for j in range(1):
            for i in range(len(allPoints[j])):
                if sqrt(pow(abs(allPoints[j][i][0]-x), 2) + pow(abs(allPoints[j][i][1]-y), 2)) < bestDist[j]:
                    bestDist[j] = sqrt(pow(abs(allPoints[j][i][0]-x), 2) + pow(abs(allPoints[j][i][1]-y), 2))
                    bestPoint[j] = allPoints[j][i]
            clickedPoints_[j].append((bestPoint[j][0], bestPoint[j][1]))
            pointNames_[j].append(bestPoint[j][2])
        if currentPoint_ == 1:
            drawSprite([bestPoint[0][1]-sprite.shape[0]//2, bestPoint[0][0]-sprite.shape[1]//2])
        elif currentPoint_ == 2:
            cv2.circle(image, (bestPoint[0][0], bestPoint[0][1]), 7, (0, 0, 255), -1)
            distances = []
            k = 0
            """for i, j in zip(['graphs/conner.json', 'graphs/Mav.json'], ['path/conner.json', 'path/Mav.json']):
                distances.append(cdij.run(pointNames_[k][-2], pointNames_[k][-1], i, j))
                k+=1
            sleep(0.2)
            drawPath(['graphs/Mav.json', 'graphs/conner.json'][distances.index(min(distances))])"""
            cdij.run(pointNames_[k][-2], pointNames_[k][-1])
            sleep(0.2)
            drawPath()
            currentPoint_ = 0
            pointNames_ = [[], []]
            clickedPoints_ = [[], []]

        #cv2.circle(image, (bestPoint[0], bestPoint[1]), 5, (255, 0, 0), -1)

        

frame = cv2.imread("dotter/Screenshot 2025-07-28 221547.png")
sprite = cv2.imread("treeWithBackground.png")
sprite = cv2.resize(sprite, (sprite.shape[0]//2, sprite.shape[1]//2))

for setofPoints in pointsMain:
    allPoints.append([])
    for i in setofPoints:
        #cv2.circle(frame, (i.x, i.y), 5, (50, 50, 50), -1)
        allPoints[-1].append((i.x, i.y, i.name))
        lookFor = points[i.name]["connections"]
        for j in setofPoints:
            if j.name in lookFor:
                pass
                #cv2.line(frame, (j.x, j.y), (i.x, i.y), (50, 50, 50), 2)

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