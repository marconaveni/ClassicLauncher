#!/usr/bin/env python3

import subprocess
import os
import sys

help = '''
OPTIONS:
  -d, --debug               Compile developer mode Debug  
  -r, --release             Compile Release mode Release  
'''

build = "build"
path_workspace = os.getcwd() + "/" + build  
build_type = "Debug"

for arg in sys.argv:
    argl = arg.lower()
    if argl == "--debug" or argl == "-d":
        build_type = "Debug" 
    elif argl == "--release" or argl == "-r":
        build_type = "Release" 
    elif argl == "--help" or argl == "-h":
        print(help)
        exit(0)

try:
    subprocess.run(["cmake", "--build", path_workspace, "--config", build_type], check=True) # cmake --build path/build --config type
except subprocess.CalledProcessError as e:
    print(f"Error cmake \": {e}")
    print("Please check your CMake installation.")
    exit(1)


