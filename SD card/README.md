# SD card contains text to display

The SD card must contain the following directories:
zalmy
kancional

The psalms are placed in the psalms directory so that each psalm has its own number. So text files created using psalmconvert are placed in this directory. In the same way, the output from hymnconvert is placed in the directory cancional (here each directory with a number is one hymn and in it the stanzas are placed).

filenames must start from 001 to 996

the overall picture of the SD katra will then be as follows :

[ROOT]
	[zalmy]
		001.txt
		002.txt
		...
		996.txt	
	[kancional]
		[001]
			1.txt
		...
		...
		[215]
			1.txt
			2.txt
		...
		...
	
