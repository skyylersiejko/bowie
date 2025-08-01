import cv2
import numpy as np;
from time import sleep
points = []
def onSegment(p, q, r):
    return (q[0] <= max(p[0], r[0]) and q[0] >= min(p[0], r[0]) and
            q[1] <= max(p[1], r[1]) and q[1] >= min(p[1], r[1]))
def orientation(p, q, r):
    val = (q[1] - p[1]) * (r[0] - q[0]) - (q[0] - p[0]) * (r[1] - q[1])
    if val == 0:
        return 0  # collinear
    return 1 if val > 0 else 2  # clockwise or counterclockwise
def doIntersect(p1, q1, p2, q2):
    o1 = orientation(p1, q1, p2)
    o2 = orientation(p1, q1, q2)
    o3 = orientation(p2, q2, p1)
    o4 = orientation(p2, q2, q1)

    # General case
    if o1 != o2 and o3 != o4:
        return True

    # Special cases
    if o1 == 0 and onSegment(p1, p2, q1): return True
    if o2 == 0 and onSegment(p1, q2, q1): return True
    if o3 == 0 and onSegment(p2, p1, q2): return True
    if o4 == 0 and onSegment(p2, q1, q2): return True

    return False
def intersection(p1, q1, p2, q2):
    A1 = q1[1] - p1[1]
    B1 = p1[0] - q1[0]
    C1 = A1 * p1[0] + B1 * p1[1]

    A2 = q2[1] - p2[1]
    B2 = p2[0] - q2[0]
    C2 = A2 * p2[0] + B2 * p2[1]

    determinant = A1 * B2 - A2 * B1

    if determinant == 0:
        return None  # Lines are parallel

    x = (B2 * C1 - B1 * C2) / determinant
    y = (A1 * C2 - A2 * C1) / determinant
    return (x, y)
def readFile(file="pointb.txt"):
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
def writeFile(content, points, file="pointb.txt"):
    with open("pointb.txt", "w") as file:
        if len(content) < 2:
            file.write(str(points))
        else:
            file.write(str(content)+"\n"+str(points))
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



frame = cv2.imread("Screenshot_2025-07-28_221547.png")
image = frame.copy()
lines, content = readFile()
if len(content) > 1:
    for points in lines:
        print(points)
        px, py = -1, -1
        for x, y in points:
            cv2.circle(image, (x, y), 3, (0, 0, 255), -1)
            if px != -1:
                cv2.line(image, (x, y), (px, py), (0, 0, 255), 2)
            px, py = x, y
intersectingPoints = []
for i in range(len(lines)):
    for j in range(i, len(lines)):
        for k in range(len(lines[i]) - 1):
            for l in range(len(lines[j]) - 1):
                if doIntersect(lines[i][k], lines[i][k + 1], lines[j][l], lines[j][l + 1]) and (lines[i][k] != lines[j][l] and lines[i][k + 1] != lines[j][l + 1]) and (lines[i][k+1] != lines[j][l] and lines[i][k] != lines[j][l + 1]):
                    point = intersection(lines[i][k], lines[i][k + 1], lines[j][l], lines[j][l + 1])
                    print(point)
                    if point != None:
                        intersectingPoints.append(point)
                        cv2.circle(image, (round(point[0]), round(point[1])), 3, (0, 255, 255), -1)

savedLines = lines.copy()
points = []
reRun = False
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

if len(points) < 2:
    cv2.destroyAllWindows()
    if reRun:
        import os
        os.system("python dotter/dotter.py")
    exit()

writeFile(content, points)

cv2.destroyAllWindows()
if reRun:
    import os
    os.system("python dotter/dotter.py")
    sleep(1)