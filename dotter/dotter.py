import cv2
import numpy as np;
points = []
frame = cv2.imread("C:/Users/jimjt/Pictures/Screenshots/Screenshot 2025-07-28 221547.png")
image = frame.copy()
with open("dotter/points.txt", "r") as file:
    content = file.read().replace("\\n", "\n")
    lines = content.split("\n")
    lines = [str(x).replace("[", "").replace("]", "").replace("(", "").replace(")", "").split(",") for x in lines]
    if len(content) > 1:
        for i in range(len(lines)):
            if len(lines[i]) > 1:
                lines[i] = [[lines[i][x], lines[i][x+1]] for x in range(0, len(lines[i]), 2)]
                lines[i] = [[int(y) for y in x] for x in lines[i]]
if len(content) > 1:
    for points in lines:
        print(points)
        px, py = -1, -1
        for x, y in points:
            cv2.circle(image, (x, y), 5, (0, 0, 255), -1)
            if px != -1:
                cv2.line(image, (x, y), (px, py), (0, 0, 255), 2)
            px, py = x, y
savedLines = lines.copy()
points = []
def mouse_callback(event, x, y, flags, params):
    if event == 1:
        print(f"coords {x, y}")
        points.append((x, y))
        cv2.circle(image, (x, y), 5, (255, 0, 0), -1)
        if len(points) > 1:
            cv2.line(image, points[-2], points[-1], (255, 0, 0), 2)
    elif event == 2:
        if points:
            points.pop()
            image[:] = frame.copy()
            for i, point in enumerate(points):
                cv2.circle(image, point, 5, (0, 0, 255), -1)
                if i > 0:
                    cv2.line(image, points[i - 1], point, (255, 0, 0), 2)

while True:
    cv2.namedWindow("image")
    cv2.setMouseCallback("image", mouse_callback)
    cv2.imshow("image", image)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
if len(points) < 2:
    cv2.destroyAllWindows()
    exit()
with open("dotter/points.txt", "w") as file:
    if len(content) < 2:
        file.write(str(points))
    else:
        file.write(str(content)+"\n"+str(points))
cv2.destroyAllWindows()