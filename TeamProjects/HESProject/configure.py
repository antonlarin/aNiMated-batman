import os
import sys
import shutil
import subprocess

scriptPath = os.path.dirname(os.path.realpath(sys.argv[0]))
def fullPath(rPath):
    return os.path.join(scriptPath, rPath)


buildDir  = fullPath("./UI/node_modules/solver")
solverDst = fullPath("./UI/node_modules/solver/Solver")
solverSrc = fullPath("./Solver")
shellMode = sys.platform.startswith("win32")
nodeWVers = "0.10.5"


def moveToBuildDir():
    if os.getcwd() != buildDir:
        os.chdir(buildDir)

def removeSolverDir():
    if os.path.exists(solverDst):
        shutil.rmtree(solverDst)

def copySolverDir():
    removeSolverDir()
    shutil.copytree(solverSrc, solverDst)

def gypConfigure():
    moveToBuildDir()
    nwArgs = ["--target=" + nodeWVers]
    subprocess.call(["nw-gyp", "configure"] + nwArgs, shell=shellMode)

def gypBuild():
    moveToBuildDir()
    subprocess.call(["nw-gyp", "build"], shell=shellMode)

def gypClean():
    moveToBuildDir()
    subprocess.call(["nw-gyp", "clean"], shell=shellMode)

def clearCommand():
    gypClean()
    removeSolverDir()

def buildCommand():
    copySolverDir()
    gypConfigure()
    gypBuild()

def rebuildCommand():
    clearCommand()
    buildCommand()

def executeCommand(cmd):
    cmdMap = {
        "build": buildCommand,
        "clear": clearCommand,
        "rebuild": rebuildCommand
    }
    if not cmd in cmdMap:
        print("We are can't execute '%s' command." % cmd)
        return
    cmdMethod = cmdMap[cmd]
    cmdMethod()


def main():
    print __file__
    if len(sys.argv) == 1:
        buildCommand()
    else:
        executeCommand(sys.argv[1])        

if __name__ == "__main__":
    main()
