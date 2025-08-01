import cv2
from math import sqrt, pow
import numpy as np

def drawSprite(loc):
    for i in range(sprite.shape[0]):
        for j in range(sprite.shape[1]):
            if list(sprite[i][j]) != [232, 162, 0]:
                image[i+loc[0]][j+loc[1]] = sprite[i][j]

def readFile(file="dotter/points.txt"):
    with open(file, "r") as f:
        content = f.read().replace("\\n", "\n")
        lines = content.split("\n")
        lines = [str(x).replace("[", "").replace("]", "").replace("(", "").replace(")", "").split(",") for x in lines]
        if len(content) > 1:
            for i in range(len(lines)):
                if len(lines[i]) > 1:
                    lines[i] = [[lines[i][x], lines[i][x+1]] for x in range(0, len(lines[i]), 2)]
                    lines[i] = [[int(y) for y in x] for x in lines[i]]
    return lines, content

allPoints = []

def mouse_callback(event, x, y, flags, params, points=allPoints):
    if event == 1:
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
frame.copy()
lines, content = readFile()
if len(content) > 1:
    for points in lines:
        print(points)
        allPoints += points
        px, py = -1, -1
        for x, y in points:
            cv2.circle(frame, (x, y), 3, (0, 0, 255), -1)
            if px != -1:
                cv2.line(frame, (x, y), (px, py), (0, 0, 255), 2)
            px, py = x, y

"""indecesToRemove = []
for p1ind in range(len(allPoints)):
    for p2ind in range(len(allPoints)-p1ind-1):
        if allPoints[p1ind] == allPoints[p2ind]:
            indecesToRemove.append(p2ind)
indecesToRemove.sort(reverse=True)
for index in indecesToRemove:
    allPoints.pop(index)"""

loc = [0, 0]
image = frame.copy()
#image[0:sprite.shape[0],0:sprite.shape[1],:] = sprite[0:sprite.shape[0],0:sprite.shape[1],:] #Background = 0, 162, 232
while True:
    cv2.namedWindow("Image")
    cv2.setMouseCallback("Image", mouse_callback)
    cv2.imshow("Image", image)
    key = cv2.waitKey(1) & 0xFF
    if key == ord("q"):
        break
cv2.destroyAllWindows()