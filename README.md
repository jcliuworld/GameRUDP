# GameRUDP
可靠UDP框架

分为Server 和 Client 两部分

Server端目前只支持Linux 

编译方式 执行 init.sh 

make proto

make 

执行DemoServer/DemoServer


客户端是用纯c实现的 你可以用cocoslua 或者 uinty + xlua（slua）的形式实现

这里借鉴了 luasocketvent的代码，感谢作者dotcoo

将源文件编译进对应的lua库 就可以使用 具体例子 见demo
