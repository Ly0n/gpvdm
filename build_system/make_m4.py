#!/usr/bin/python
# 
# General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
# base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
# 
# Copyright (c) 2012-2019, Roderick C. I. MacKenzie
# All rights reserved.
# 
# 	https://www.gpvdm.com
# 	Room B86 Coates, University Park, Nottingham, NG7 2RD, UK
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the GPVDM nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL Roderick C. I. MacKenzie BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 

import os
import sys
import argparse

def make_m4(hpc=False, win=False,usear=False):
	path=os.getcwd()
	make_m4_core(path,hpc=hpc, win=win,usear=usear)
	make_m4_gui(path,hpc=hpc, win=win,usear=usear)
	make_m4_data(path,hpc=hpc, win=win,usear=usear)

def make_m4_core(path,hpc=False, win=False,usear=False):
	path=os.path.join(path,"gpvdm_core")
	config_files=[]
	gpvdm_lib=[]
	link_libs=""

	config_files.append("")

	if os.path.isdir(os.path.join(path,"lang")):
		config_files.append("lang")

	config_files.append("libi")
	gpvdm_lib.append("libi")

	config_files.append("libmath2d")
	gpvdm_lib.append("libmath2d")

	config_files.append("libbasicmath")
	gpvdm_lib.append("libbasicmath")

	if os.path.isdir(os.path.join(path,"libfxdomain")):
		config_files.append("libfxdomain")
		gpvdm_lib.append("libfxdomain")

	config_files.append("librpn")
	gpvdm_lib.append("librpn")

	config_files.append("libshape")
	gpvdm_lib.append("libshape")

	if os.path.isdir(os.path.join(path,"libemission")):
		config_files.append("libemission")
		gpvdm_lib.append("libemission")

	config_files.append("libmemory")
	gpvdm_lib.append("libmemory")

	config_files.append("libdos")
	gpvdm_lib.append("libdos")

	config_files.append("liblight")
	gpvdm_lib.append("liblight")

	if os.path.isdir(os.path.join(path,"libheat")):
		config_files.append("libheat")
		gpvdm_lib.append("libheat")

	if os.path.isdir(os.path.join(path,"libray")):
		config_files.append("libray")
		gpvdm_lib.append("libray")

	config_files.append("libcolor")
	gpvdm_lib.append("libcolor")

	config_files.append("libmeasure")
	gpvdm_lib.append("libmeasure")

	config_files.append("libcontacts")
	gpvdm_lib.append("libcontacts")

	config_files.append("lib")
	gpvdm_lib.append("lib")

	config_files.append("libdump")
	gpvdm_lib.append("libdump")

	config_files.append("libdumpctrl")
	gpvdm_lib.append("libdumpctrl")

	config_files.append("libdevice")
	gpvdm_lib.append("libdevice")

	if os.path.isdir(os.path.join(path,"libserver")):
		config_files.append("libserver")
		gpvdm_lib.append("libserver")

	config_files.append("libmesh")
	gpvdm_lib.append("libmesh")

	if os.path.isdir(os.path.join(path,"libperovskite")):
		config_files.append("libperovskite")
		gpvdm_lib.append("libperovskite")

	config_files.append("libnewtontricks")
	gpvdm_lib.append("libnewtontricks")

	if os.path.isdir(os.path.join(path,"libfit")):
		config_files.append("libfit")
		link_libs=link_libs+" -lgpvdm_fit"

	if os.path.isdir(os.path.join(path,"libsimplex")):
		config_files.append("libsimplex")
		gpvdm_lib.append("libsimplex")

	if os.path.isdir(os.path.join(path,"libfdtd")):
		config_files.append("libfdtd")
		gpvdm_lib.append("libfdtd")

	if os.path.isdir(os.path.join(path,"liblock")):
		config_files.append("liblock")
		gpvdm_lib.append("liblock")

	if os.path.isdir(os.path.join(path,"libcircuit")):
		config_files.append("libcircuit")
		gpvdm_lib.append("libcircuit")

	if win==False:
		if os.path.isdir(os.path.join(path,"mumps")):
			config_files.append("mumps")

	link_libs=link_libs+" -lgpvdm_core"

	config_files.append("src")

	for root, dirs, files in os.walk(os.path.join(path,"plugins")):
		for file in files:
			if file.endswith("Makefile.am"):
				name=os.path.join(root, file)[len(path)+1:-12]
				config_files.append(name)
	if win==True:
		try:
			config_files.remove("plugins/external_solver")
		except:
			pass

		try:
			config_files.remove("plugins/superlu")
		except:
			pass

	if hpc==False:
		config_files.append("cluster_")
		if win==False:
			config_files.append("man")

	config_files.append("inp_template")

	f = open(os.path.join(path,"config_files.m4"), "w")
	f.write( "#This file has been autogenerated\n")
	for i in range(0,len(config_files)):
		f.write( "AC_CONFIG_FILES(["+os.path.join(config_files[i],"Makefile")+"])\n")

	f.close()

	f = open(os.path.join(path,"make_files.m4"), "w")
	f.write( "#This file has been autogenerated\n")
	f.write( "AC_SUBST(BUILD_DIRS,\"")
	for i in range(0,len(config_files)):
		f.write(config_files[i]+" ")

	f.write("\")")

	f.close()



	f = open(os.path.join(path,"local_link.m4"), "w")
	f.write( "AC_SUBST(LOCAL_LINK,\"")
	f.write(link_libs)
	f.write("\")")

	f.close()

	f = open(os.path.join(path,"ar.m4"), "w")

	if usear==True:
		f.write("AM_PROG_AR")
	else:
		f.write("")

	f.close()


	f = open(os.path.join(path,"gpvdm_core_lib.m4"), "w")
	f.write( "AC_SUBST(GPVDM_CORE_LIB_FILES,\"")
	for file_name in gpvdm_lib:
		f.write("../"+file_name+"/*.o ")
	f.write("\")")

	f.close()


	f = open(os.path.join(path,"include","enabled_libs.h"), "w")
	f.write("//This file is auto_generated do not edit.\n")
	for lib_name in gpvdm_lib:
		f.write("#define "+lib_name+"_enabled\n")

	if config_files.count("libfit")!=0:
		f.write("#define libfit_enabled\n")

	f.close()

def make_m4_gui(path,hpc=False, win=False,usear=False):
	path=os.path.join(path,"gpvdm_gui")
	config_files=[]
	link_libs=""

	config_files.append("")
	config_files.append("gui")
	config_files.append("lang")

	if hpc==False:
		config_files.append("images/16x16")
		config_files.append("images/32x32")
		config_files.append("images/48x32")
		config_files.append("images/64x64")

		config_files.append("images/icons/16x16")
		config_files.append("images/icons/32x32")
		config_files.append("images/icons/48x48")
		config_files.append("images/icons/64x64")
		config_files.append("images/icons/128x128")
		config_files.append("images/icons/256x256")
		config_files.append("images/icons/512x512")

		config_files.append("css")
		config_files.append("scripts")
		config_files.append("html")
		config_files.append("bib")
		config_files.append("video")

		if os.path.isdir(os.path.join(path,"desktop")):
			config_files.append("desktop")

		if win==False:
			config_files.append("man")

	f = open(os.path.join(path,"config_files.m4"), "w")
	f.write( "#This file has been autogenerated\n")
	for i in range(0,len(config_files)):
		f.write( "AC_CONFIG_FILES(["+os.path.join(config_files[i],"Makefile")+"])\n")

	f.close()

	f = open(os.path.join(path,"make_files.m4"), "w")
	f.write( "#This file has been autogenerated\n")
	f.write( "AC_SUBST(BUILD_DIRS,\"")
	for i in range(0,len(config_files)):
		f.write(config_files[i]+" ")

	f.write("\")")

	f.close()


	f = open(os.path.join(path,"local_link.m4"), "w")
	f.write( "AC_SUBST(LOCAL_LINK,\"")
	f.write(link_libs)
	f.write("\")")

	f.close()

	f = open(os.path.join(path,"ar.m4"), "w")

	if usear==True:
		f.write("AM_PROG_AR")
	else:
		f.write("")

	f.close()

def make_m4_data(path,hpc=False, win=False,usear=False):
	path=os.path.join(path,"gpvdm_data")
	config_files=[]
	link_libs=""

	config_files.append("")
	config_files.append("cie_color")
	#config_files.append("docs/man")

	f = open(os.path.join(path,"config_files.m4"), "w")
	f.write( "#This file has been autogenerated\n")
	for i in range(0,len(config_files)):
		f.write( "AC_CONFIG_FILES(["+os.path.join(config_files[i],"Makefile")+"])\n")

	f.close()

	f = open(os.path.join(path,"make_files.m4"), "w")
	f.write( "#This file has been autogenerated\n")
	f.write( "AC_SUBST(BUILD_DIRS,\"")
	for i in range(0,len(config_files)):
		f.write(config_files[i]+" ")

	f.write("\")")

	f.close()


	f = open(os.path.join(path,"local_link.m4"), "w")
	f.write( "AC_SUBST(LOCAL_LINK,\"")
	f.write(link_libs)
	f.write("\")")

	f.close()

	f = open(os.path.join(path,"ar.m4"), "w")

	if usear==True:
		f.write("AM_PROG_AR")
	else:
		f.write("")

	f.close()



