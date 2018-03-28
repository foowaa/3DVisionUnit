# -*- coding: utf-8 -*-
"""
Author: cltian
Date: 2018/3/28
"""
import sys
import subprocess
import os
def func_build():
	if !os.path.exists("./build"):
		os.mkdir("./build")
	else:
		subprocess.call("cd ./build", shell=True)
		subprocess.call("rm -rf *", shell=True)
def linux_call(string):
	subprocess.call(string, shell=True)

if len(sys.argv)!=1:
	raise Exception, "Just 1 argument!"
else:
	arguments = str(sys.argv)
	subprocess.call("source /etc/profile", shell=True)
	if arguments=="all":
		linux_call("cp ./cmake/library/SSCA/CMakeLists.txt ./SSCA")
		linux_call("cp ./cmake/library/grid/CMakeLists.txt ./grid")
		linux_call("cp ./cmake/library/utils/CMakeLists.txt ./utils")
		func_build()
		print "start cmake..."
		linux_call("cmake ..")
		print "start make..."
		linux_call("make")
	elif arguments=="SSCA":
		linux_call("cp ./cmake/execution/SSCA/CMakeLists.txt ./SSCA")
		linux_call("cd ./SSCA")
		func_build()
		print "start cmake..."
		linux_call("cmake ..")
		print "start make..."
		linux_call("make")
	elif arguments=="grid":
		linux_call("cp ./cmake/execution/grid/CMakeLists.txt ./grid")
		linux_call("cd ./grid")
		func_build()
		print "start cmake..."
		linux_call("cmake ..")
		print "start make..."
		linux_call("make")
	else:
		raise Exception "Invalid arguments!"



