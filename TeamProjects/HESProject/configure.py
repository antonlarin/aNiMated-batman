import os
import sys
import shutil
import urllib
import zipfile
import tarfile
import platform
import subprocess

def isWindows():
    return sys.platform.startswith("win32")
def isLinux():
    return sys.platform.startswith("linux")
def isMacOS():
    return sys.platform.startswith("darwin")

def getArchitectureName():
    arcMap = { 
        "AMD64": "x64", "x86_64": "x64", "i686": "x64", 
        "i386": "ia32", "x86": "ia32" 
    }
    return arcMap[platform.machine()]


shellMode = isWindows()
scriptPath = os.path.dirname(os.path.realpath(sys.argv[0]))
def fullPath(rPath):
    return os.path.join(scriptPath, rPath)

nodeWVers = "0.10.5"
uiDir     = fullPath("UI")
solverSrc = fullPath("Solver")
buildDir  = fullPath("UI/node_modules/solver")
solverDst = fullPath("UI/node_modules/solver/Solver")
nwUrlTmpl = "http://dl.node-webkit.org/v%s/node-webkit-v%s-%s-%s.%s"


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

def getPlatformParams():
    if isLinux():
        return ("linux", getArchitectureName(), "tar.gz")
    if isMacOS():
        return ("osx", getArchitectureName(), "zip")
    if isWindows():
        return ("win", "ia32", "zip")
    raise(Exception("We are not support current platform."))

def getnwBuildUrl():
    urlFormatArgs = [nodeWVers, nodeWVers]
    urlFormatArgs += getPlatformParams()
    nwUrl = nwUrlTmpl % tuple(urlFormatArgs)
    return nwUrl

def copyAllFromDir(src, dst):
    srcFiles = os.listdir(src)
    for fileName in srcFiles:
        srcFileName = os.path.join(src, fileName)
        dstFileName = os.path.join(dst, fileName)
        print("->" + fileName)
        if os.path.isfile(srcFileName):
            shutil.copy(srcFileName, dstFileName)
        else:
            if os.path.exists(dstFileName):
                shutil.rmtree(dstFileName)
            shutil.copytree(srcFileName, dstFileName)

def getFileNameNoExt(fileName):
    res = os.path.splitext(fileName)[0]
    if isLinux():
        res = os.path.splitext(res)[0]
    return res

def unpackArchive(fileName):
    if isLinux():
        archive = tarfile.open(fileName)
    else:
        archive = zipfile.ZipFile(fileName)
    archive.extractall(uiDir)
    archive.close()
    archiveRootDir = getFileNameNoExt(fileName)
    copyAllFromDir(archiveRootDir, uiDir)
    shutil.rmtree(archiveRootDir)


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

def installnwCommand():
    nwUrl = getnwBuildUrl()
    nwArchiveName = nwUrl.split('/')[-1]
    nwArchivePath = os.path.join(uiDir, nwArchiveName)
    if not os.path.exists(nwArchivePath):
        print("Downloading node-webkit..")
        urllib.urlretrieve(nwUrl, nwArchivePath)
    
    print("Unpacking node-webkit..")
    unpackArchive(nwArchivePath)

def installCommand():
    installnwCommand()
    rebuildCommand()

def executeCommand(cmd):
    cmdMap = {
        "build": buildCommand,
        "clear": clearCommand,
        "rebuild": rebuildCommand,
        "installnw": installnwCommand,
        "install": installCommand
    }
    if not cmd in cmdMap:
        print("We are can't execute '%s' command." % cmd)
        return
    cmdMethod = cmdMap[cmd]
    cmdMethod()


def main():
    if len(sys.argv) == 1:
        buildCommand()
    else:
        executeCommand(sys.argv[1])        

if __name__ == "__main__":
    main()
