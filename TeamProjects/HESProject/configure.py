import os
import sys
import shutil
import subprocess

buildDir  = "./UI/node_modules/solver"
solverDst = "./UI/node_modules/solver/Solver"
nodeWVers = "0.10.5"
shellMode = sys.platform.startswith("win32")

def copySolverLibraryToAddon():
	global solverDst
	if os.path.exists(solverDst):
		shutil.rmtree(solverDst)	
	shutil.copytree("./Solver", solverDst)

def configureGyp():
	nwArgs = ["--target=" + nodeWVers] + sys.argv[1:]
	subprocess.call(["nw-gyp", "configure"] + nwArgs, shell=shellMode)

def runGyp():
	subprocess.call(["nw-gyp", "build"], shell=shellMode)


copySolverLibraryToAddon()
os.chdir(buildDir)
configureGyp()
runGyp()