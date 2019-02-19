#coding=utf-8
import os

#检查编译程序
def checkCCFile():
	ccFileName = 'protoc.exe'
	if ccFileName not in os.listdir(os.getcwd()):
		print u'没有 ' + ccFileName
		return False
	return True

#检查文件
def checkFile():
	print u'输入要编译的文件名'
	s = raw_input()
	if s not in os.listdir(os.getcwd()):
		print(u"文件[" + s + u"] 没有找到.")
		return

	os.system("start protoc.exe --as3_out=. " + s)
	print u'生成完成'

if __name__ == '__main__':
	if checkCCFile():
		checkFile()		
	
	print u"任意键退出"
	raw_input()
