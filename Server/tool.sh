#!/bin/bash

mycmd=$1
clean()
{
	for file in *
	do
		if [ -d $file ]
		then
			if [ $file == "lua" ]
			then 
				continue
			fi
			if [ $file == "Command" ]
			then
				continue
			fi
			if [ $file == "Language" ]
			then 
				continue
			fi

			if [ $file == "autom4te.cache" ]
			then 
				continue
			fi
			
			if [ $file == "sql" ]
			then
				continue
			fi
			
			rm -rf $file/*.lo
			rm -rf $file/*.o

		fi
	done
}

checkinclude()
{
	for file in sdk/*.h
	do 
		ifd=`cat $file | grep ifndef`
		def=`cat $file | grep define`
		echo $ifd
		echo $def
	done
}
main()
{
	if [[ $mycmd == "clean" ]]
	then
		echo "clean is runing"
		clean
	elif [[ $mycmd == "check" ]]
	then
		echo "chech is runing"
		checkinclude
	else
		echo "no tool"
	fi
}


main

