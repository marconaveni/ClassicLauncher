
import os
import subprocess
import sys

if os.name == 'nt':
    import configureWin

build = "build"
path_workspace = os.getcwd() + "\\" + build  
build_type = "Release"
fsanitize = "-DCLASSIC_LAUNCHER_FSANITIZE=FALSE"
raspberrypi = "-DCLASSIC_RASPBERRY_PI=FALSE"
flags = "-DCLASSIC_LAUNCHER_FLAGS=FALSE"
theme = "-DCLASSIC_FORCE_THEME_1X=FALSE"

 
for arg in sys.argv:
    argl = arg.lower()
    if argl == "--debug" or argl == "-d":
        build_type = "Debug" 
    elif argl == "--release" or argl == "-r":
        build_type = "Release" 
    elif argl == "--fsanitize" or argl == "-s":
        fsanitize = "-DCLASSIC_LAUNCHER_FSANITIZE=TRUE"
    elif argl == "--raspberrypi" or argl == "-pi":
        raspberrypi = "-DCLASSIC_RASPBERRY_PI=TRUE"
    elif argl == "--flags" or argl == "-f":
        flags = "-DCLASSIC_LAUNCHER_FLAGS=TRUE"
    elif argl == "--theme1x" or argl == "-t":
        theme = "-DCLASSIC_FORCE_THEME_1X=TRUE"

        


try:
    subprocess.run(["git", "submodule", "update", "--init", "--recursive"], check=True)
except subprocess.CalledProcessError as e:
    print(f"Error init submodules\": {e}")
    print("Please check your internet connection or Git installation.")
    exit(1)


if not os.path.isdir(path_workspace):
    os.mkdir(path_workspace)


try:
    subprocess.run(["cmake", "-B", path_workspace, f"-DCMAKE_BUILD_TYPE={build_type}", fsanitize , raspberrypi , flags, theme], check=True) #cmake -B path/build -DCMAKE_BUILD_TYPE=build_type
except subprocess.CalledProcessError as e:
    print(f"Error cmake \": {e}")
    print("Please check your CMake installation.")
    exit(1)



# git submodule update --init --recursive

# mkdir build
# cd build
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake --build .

# mkdir build
# cd build
# cmake -DCMAKE_BUILD_TYPE=Release ..
# cmake --build .

