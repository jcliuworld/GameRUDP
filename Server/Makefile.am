SUBDIRS =  sdk protocol DemoServer
.PHONY: proto dist_sdk
proto:
	@python genmsg.py protocol/ cfg/Xml/ 
	@protoc -I=./protocol --cpp_out=./protocol ./protocol/*.proto
dist_sdk:
	@ echo "make pack sdk ..."
	@ rm -rf release_sdk
	@ mkdir release_sdk
	@ mkdir release_sdk/sdk
	@ mkdir release_sdk/sdk/Encrypt
	@ cp -avf sdk/Encrypt/*.h release_sdk/sdk/Encrypt
	@ cp -avf sdk/Encrypt/lib*.* release_sdk/sdk/Encrypt/
	@ cp -avf sdk/Encrypt/.libs/libencdec.a release_sdk/sdk/Encrypt/
	@ cp -avf sdk/*.h release_sdk/sdk/
	@ cp -avf sdk/lib*.* release_sdk/sdk/
	@ cp -avf sdk/.libs/libage.a release_sdk/sdk/


