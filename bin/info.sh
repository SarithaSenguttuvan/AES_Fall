#!/bin/bash

echo " Operating system type: "
uname -s
echo $'\n'	
echo " OS Distribution: "		
lsb_release -d | cut -d':' -f2
echo $'\n'
echo " Kernel Version: "			
uname -or
echo $'\n'
echo " Information on the kernel build: "			
cat /proc/version
echo $'\n'
echo " Kernel gcc version build: "			
cat /proc/version | cut -d'(' -f3
echo $'\n'
echo " Kernel build time: "	
uname -v	
echo $'\n'
echo " System Architecture information: "
uname -p
echo $'\n'