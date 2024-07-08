from pathlib import Path

path = "../res/01 Towns"

with open(path, "rb") as f:
    while byte := f.read(1):
        print("0x" + byte.hex() + ",")
