import os
import subprocess
import sys

start = "10"
end = "5"
def LastNlines(fname, N):
    with open(fname) as file:
        for line in file.readlines()[-N:]:
            return line

def run(start=start, end=end, ReadFile="graphs/conner.json", OutFile="path/conner.json"):
    # Create a pipe to capture output
    read_fd, write_fd = os.pipe()
    # Redirect stdout to the write end of the pipe
    os.dup2(write_fd, sys.stdout.fileno())
    if 'nt' == os.name:
        subprocess.call(f"./DijkstraPathFinder.exe \"{start}\" \"{end}\"")# \"{ReadFile}\" \"{OutFile}\"")
    else:
        subprocess.call(f"./DijkstraPathFinder \"{start}\" \"{end}\"")# \"{ReadFile}\" \"{OutFile}\"")s
    # Close the write end of the pipe
    os.close(write_fd)

    # Read the output from the read end of the pipe
    output = os.read(read_fd, 1024).decode('utf-8')

    # Close the read end of the pipe
    os.close(read_fd)

    # Remove trailing newlines and get the last character
    output = output.rstrip('\n')
    return output[-1] if output else None
if __name__ == "__main__":
    run(start, end)