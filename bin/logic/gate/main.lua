local harbor = require "core.harbor"
local co = require "core.co"

harbor.register("add", function(a, b)
	print("add", a, b)
	harbor.ret(a + b)
end)

harbor.add_node_open_listener(function(nodeType, nodeId)
	if nodeType == 3 then
		local c = harbor.call(nodeType, nodeId, "add", true, 1, 2)
		print(c)
	end
end)
