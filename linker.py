from collections import OrderedDict
import sys
import fileinput;

dict = OrderedDict()
input = fileinput.input()
fileNum = 0
for i in (raw.strip().split() for raw in input ):
	if len(i) == 1:
		fileNum += 1
	elif len(i) == 2:
		#Undefined stuff
		dict[i[1]] = i[0]
	elif len(i) > 2:
		#global variables
		if i[3] in dict:
			#check against u's
			if i[2] == "C":
				i[2] = "B"
				del dict[i[3]]
			if i[2] == "T":
				i[2] == "T"
				del dict[i[3]]
			if i[2] == "D":
				i[2] == "D"
				del dict[i[3]]
		key = i[3] + " " + i[2] + " " + str(fileNum)
		if i[2].isupper():
			key = i[3] + " " + i[2]
		#key is name-letter-filenum
		value = [i[1],i[2]]
		#value is <offset-t>


		if key not in dict:
			dict[key] = value
		else:
			print "Symbol '" + i[3] + "' muliply defined"
			sys.exit()
textAddr = 0x400680
dataAddr = 0x601044
i = 0;
us = list()
for key in dict:
	parsedKey = key.strip().split()
	if len(parsedKey) == 1:
		us.append(key)
		i += 1
us.sort()
for key in us:
	parsedKey = key.strip().split()
	if len(parsedKey) == 1:
		print "                 ",
		print dict[key] + " ", 
		print key
for key in dict:
	parsedKey = key.strip().split()
	if len(parsedKey) > 1:
		if parsedKey[1] == "t" or parsedKey[1] == "T":
			hexStr = "0000000000" + hex(textAddr)[2:8] + " "
			print hexStr,
			textAddr += int(dict[key][0], 16)
			print dict[key][0],
			print dict[key][1],
			print parsedKey[0]
for key in dict:
	parsedKey = key.strip().split()
	if len(parsedKey) > 1:
		if parsedKey[1] == "d" or parsedKey[1] == "D":
			hexStr = "0000000000" + hex(dataAddr)[2:8] + " "
			print hexStr,
			dataAddr += int(dict[key][0], 16)
			print dict[key][0],
			print dict[key][1],
			print parsedKey[0]
for key in dict:
	parsedKey = key.strip().split()
	if len(parsedKey) > 1:
		if parsedKey[1] == "b" or parsedKey[1] == "B":
			hexStr = "0000000000" + hex(dataAddr)[2:8] + " "
			print hexStr,
			dataAddr += int(dict[key][0], 16)
			print dict[key][0],
			print dict[key][1],
			print parsedKey[0]







