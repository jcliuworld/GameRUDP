libtoolize --force --automake
rm -f config.cache
rm -f config.log
aclocal
autoheader
autoconf
automake -a
./configure
