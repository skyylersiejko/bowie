import json
import cv2
import numpy as np;
from time import sleep

class Point:
    def __init__(self, x, y, color):
        self.x = x
        self.y = y
        self.color = color
setofPoints = []

def mouse_callback(event, x, y, flags, params):
    if event == 1:
        print(f"coords {x, y}")
        points.append((x, y))
        cv2.circle(image, (x, y), 5, (255, 0, 255), -1)
        if len(points) > 1:
            cv2.line(image, points[-2], points[-1], (255, 0, 255), 2)
    elif event == 2:
        if points:
            points.pop()
            image[:] = frame.copy()
            for i, point in enumerate(points):
                cv2.circle(image, point, 5, (255, 0, 255), -1)
                if i > 0:
                    cv2.line(image, points[i - 1], point, (255, 0, 255), 2)

try:
    frame = cv2.imread("Stanford_map.png")
    image = frame.copy()

    while True:
        cv2.namedWindow("image")
        cv2.setMouseCallback("image", mouse_callback)
        cv2.imshow("image", image)
        key = cv2.waitKey(1)
        if key == ord('q'):
            break 
        if key == ord('r') or key == ord('n'):
            reRun = True
            break
        cv2.circle(image, (710, 760), 3, (255, 0, 255), -1)

#with open("bestPath.json", 'r') as file:
 #       points = json.load(file)
  #      for data in points.values():
     

    
    
    
except FileNotFoundError:
    print(f"Error: The file was not found.")



file.close()