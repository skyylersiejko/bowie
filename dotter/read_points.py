class Point:
    def __init__(self):
        self.x = None
        self.y = None 
setofPoints = []
tuplesofPoints = ""


points = "points.txt" 
try:
    with open(points, 'r') as file:
        for line in file:
            text = line
            for i in range(len(text)):
                if text[i] != "[" and text[i] != "]" and text[i] != ",":
                    tuplesofPoints += text[i]
                

    print(tuplesofPoints)
    pass
except FileNotFoundError:
    print(f"Error: The file '{points}' was not found.")
except Exception as e:
    print(f"An error occurred: {e}")

n = 0 
m = 0

'''
dictofPoints{

}
count = 0
while True:
'''
    
    
   



        
file.close()