import os
import shutil
import subprocess

path  = os.getcwd() 
url = "https://github.com/marconaveni/ClassicLauncherWinDependencies" 
tempDir = os.path.join(path, "ClassicLauncherWinDependencies")
sourceDir =  os.path.join(tempDir, "libvlc")
destinationDir = os.path.join(path, "extern", "libvlc")

os.chdir(path) # Specifying the path where the cloned project needs to be copied

def removeReadonly(func, path, excInfo): #callback function
    os.chmod(path, 0o777)
    func(path)

if not os.path.isdir(destinationDir):
    if not os.path.isdir(sourceDir):
        print("Downloading windows dependencies...")
        try:
            subprocess.run(["git", "clone", url], check=True)
        except subprocess.CalledProcessError as e:
            print(f"Error cloning repository \"ClassicLauncherWinDependencies\": {e}")
            print("Please check your internet connection or Git installation.")
            exit(1)
        
    shutil.move(sourceDir, destinationDir)
    print(f"move files from {sourceDir} to {destinationDir}")
    shutil.rmtree(tempDir, onexc=removeReadonly) 
    print(f"Temporary directory {tempDir} removed.")

else:
    print("Dependencies are already configured.")




