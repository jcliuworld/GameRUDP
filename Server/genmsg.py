import sys
import os
from  xml.dom import  minidom

reload(sys)
sys.setdefaultencoding('utf-8')


def genFile(proto,out):
	outPutList = list()
	outPutList.append('<?xml version="1.0" encoding="utf-8"?>\n<Age>');
	for file in os.listdir(proto):
		if not file.endswith('.proto'):
			continue
		print file,
		f = open(proto+file)
		packName = ''
		for line in f:
			if line.find('package') != -1:
				packName = line.strip('package').strip().strip(';')

			if line.find('message') != -1:
				messageName = line.strip('message').strip().strip('{}').strip()
				outPutList.append('\t<msg name="%s.%s"/>'%(packName, messageName))

		f.close()

	outPutList.append('</Age>');	


	outFile = open(out+'MsgList.xml', 'w'); 

	outFile.write('\n'.join(outPutList))
	
	outFile.close()


def main(argv):

	if len(argv) < 3 :
		print "param error" 
		return False
	
	proto_path = argv[1]

	if os.path.exists(proto_path) == False:
		print "out path is not exist" 
		return False

	if os.path.isfile( proto_path ) == True : 
		print"out path must bu a floder"
		return False



	outfile = argv[2]

	if os.path.exists(outfile ) == False:
		print "out path is not exist"
		return False

	if os.path.isfile( outfile ) == True : 
		print"out path must bu a floder"
		return False


	genFile(proto_path,outfile)

if __name__ == "__main__":
	main(sys.argv)		
