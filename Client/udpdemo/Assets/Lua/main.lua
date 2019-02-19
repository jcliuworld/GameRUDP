
local sock = nil
function  main( )
    print("begin");
    local socketevent = require("socketevent")
    sock = socketevent.udp()

    host = "192.168.2.100"
    port = 8000

    
	sock:on("connect", function(event)
		print("connect")
	end)

	sock:on("message", function(event)
		print("data: " .. event.data)
	end)

	sock:on("close", function(event)
		print("close!")
	end)

	sock:on("error", function(event)
		print("error: " .. event.error .. ", " .. event.message)
	end)

	sock:connect(host, port)

end

function sendmsg(  )
	if sock ~= nil then
		print("send data")
		sock:send("hello  upd sock")
	end
end

function sync( )
	if sock ~= nil then
		sock:sync()
	end
end

function close(  )
	if sock ~= nil then
		sock:close()
	end
end

