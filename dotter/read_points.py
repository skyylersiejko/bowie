import json

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
setofPoints = []

try:
    with open("graph.json", 'r') as file:
        points = json.load(file)

    for i in range(len(points)):
        other_i = str(i)
        point_data = Point(points[other_i]["x"], points[other_i]["y"])
        setofPoints.append(point_data)
        globals()[f"p{i}"] = point_data

    print(setofPoints)
    print(p1.x*p5.y)
        
    pass
except FileNotFoundError:
    print(f"Error: The file was not found.")

file.close()