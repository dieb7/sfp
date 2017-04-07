#!/usr/bin/env python
# common CPU ID generator  Rob Chapman  Feb 1, 2011

# read in a text file and generate a C header file and a Python file so a
# single list can be used to keep embedded and host software in sync

pydests = ['../../../TimbreTalk/'] # place a copy here for Timbre Talk
hdest = '../'

cpuidlist = []

''' Text file source looks like:
MAIN_CPU		0x1
'''

def readCpuids(file):
	global cpuidlist
	cpuidlast = 0
	lines = open(file, 'r').readlines()
	for line in lines:
		if line.strip():
			l = line.split()
			if l[0] != '//':
				cpuid = l[1]
				if cpuid[0] == '(':
					cpuidlast = cpuid
				else:
					if cpuid == '++': # increment from last id
						cpuidlast += 1
						cpuid = hex(cpuidlast)
					else:
						cpuidlast = int(cpuid, 0)
				cpuidlist.append([l[0],cpuid,' '.join(w for w in l[2:])])


''' C header looks like:
#define HOST 0x00	// blah
'''
def generateC(file):
	file = open(file, 'w')
	file.write('// CPU ID declarations  %s'%genby())
	file.write("// don't modify this file. Make changes to cpuids.txt and regenerate\n")
	file.write("#ifndef _WHO_H_\n#define _WHO_H_\n")
	for cpuid in cpuidlist:
		if cpuid is cpuidlist[-1]:
			comma = ''
		else:
			comma = ','
		file.write('#define %s %s \t// %s\n'%(cpuid[0], cpuid[1], cpuid[2]))
	file.write('#endif\n\n')
	file.write('unsigned char whoami(void);')
	file.close()


''' Python file looks like:
# CPU names
HOST=0x00	#  blah
whoDict = {
	'Main':MAIN_CPU,
	'Main Host':MAIN_HOST,
	'Direct':0,
	'Host':0,
	'':0
}

'''
def generatePython(filename):
	import shutil
	file = open(filename, 'w')
	file.write('# CPU ID declarations  %s'%genby())
	file.write("# don't modify this file. Make changes to cpuids.txt and regenerate\n")
	file.write('# CPU names')
	for cpuid in cpuidlist:
		file.write('\n%s=%s\t# %s'%(cpuid[0], cpuid[1], cpuid[2]))
	file.write('\n\n# who dictionary\nwhoDict = {')
	for cpuid in cpuidlist:
		file.write("\n    '%s': %s,"%(cpuid[0].replace('_',' ').title(), cpuid[0]))
	file.write("\n	'':0\n}")
	file.close()
	for pydest in pydests:
		shutil.copy(filename, pydest+filename)

def genby(): # string for heading
	import time
	t = time.strftime('%b %d, %Y  %H:%M:%S',time.localtime())
	return 'generated by parsecpuids.py  %s\n\n'%t

if __name__ == '__main__':
	readCpuids('cpuids.txt')
	generateC(hdest+'who.h')
	generatePython('cpuids.py')
