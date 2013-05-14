#!/usr/bin/python
import sys

def convertHeader(header):
	headerParts = header.split(' ');
	if (headerParts[0] != "x" or headerParts[1] != "=" or
		headerParts[3] != "y" or headerParts[4] != "="):
		raise Exception("Invalid header");
	strX = headerParts[2][:-1];
	strY = headerParts[5][:-1];
	return (int(strX), int(strY));

def writeFile(fileName, pattern):
	out = open(fileName, 'w');
	out.write("void pattern(uint32_t x, uint32_t y)\n{\n");
	for coord in pattern:
		out.write("\tset(x + %u, y + %u, true);\n" % (coord['x'], coord['y']));
	out.write("}\n");
	out.close();

def convertRLE(fileName, outFile):
	rle = open(fileName, 'r');
	header = False;
	x = 0;
	y = 0;
	xMax = 0;
	yMax = 0;
	partialNum = '';
	pattern = []

	while (True):
		c = rle.read(1);
		if (c is None):
			break;
		elif (c == '#'):
			rle.readline();
		elif (not header):
			(xMax, yMax) = convertHeader(c + rle.readline());
			header = True;
		else:
			if (c >= '0' and c <= '9'):
				partialNum += c;
			elif (c == 'b'):
				if (partialNum == ''):
					partialNum = '1';
				x += int(partialNum);
				partialNum = '';
			elif (c == 'o'):
				if (partialNum == ''):
					partialNum = '1';
				for i in xrange(0, int(partialNum)):
					pattern.append({'x': x, 'y': y});
					x += 1;
				partialNum = '';
			elif (c == '$'):
				if (partialNum == ''):
					partialNum = '1';
				y += int(partialNum);
				partialNum = '';
				x = 0;
			elif (c == '!'):
				break;

	writeFile(outFile, pattern);
	rle.close();

if __name__ == '__main__':
	if (len(sys.argv) < 2):
		print "Error, GOL converter must be called with the name of a valid RLE file!";
	elif (len(sys.argv) < 3):
		print "ERror, GOL converter must be called with the name of an output file!";
	else:
		convertRLE(sys.argv[1], sys.argv[2]);

