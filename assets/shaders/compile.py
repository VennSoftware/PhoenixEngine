import glob
import os
from os.path import basename, splitext 

fragmentShaders = glob.glob("**.frag")
vertexShaders = glob.glob("**.vert")

for frag in fragmentShaders:
    os.system(f"glslc.exe {frag} -o cache\\{basename(frag)}.spv")
    print(f"{frag} -> cache\\{basename(frag)}.spv")


for vert in vertexShaders:
    os.system(f"glslc.exe {vert} -o cache\\{basename(vert)}.spv")
    print(f"{vert} -> cache\\{basename(vert)}.spv")


input("Press Enter To Close...")