import os
import subprocess

start = "10"
end = "5"

if 'nt' == os.name:
    subprocess.call(f"./DijkstraPathFinder.exe \"{start}\" \"{end}\"")
else:
    subprocess.call(f"./DijkstraPathFinder.bin \"{start}\" \"{end}\"")