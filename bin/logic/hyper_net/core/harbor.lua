local harbor = require "serverd.harbor"
local co = require "core.co"
local table = table
local coroutine = coroutine
local string = string
local assert = assert
local print = print
local pairs = pairs
local unpack = table.unpack

_ENV = {}

local sequence = {}
local co_sequence = {}
local co_call_node = {}
local proto = {}
local node_open_listener = {}
local node_close_listener = {}

co.reg_co_exit_listener(function(running)
	assert(co_sequence[running] == nil, "rpc but no return any result")
	
	local seq = co_sequence[running]
	if seq ~= nil then
		sequence[seq] = nil
		
		local node = co_call_node[running]
		if node ~= nil then
			harbor.respone(node.nodeType, node.nodeId, string.serialize(seq, false))
		end
	end
	co_sequence[running] = nil
	co_call_node[running] = nil
end)

--------------------------------------------- listener -----------------------------------------------
harbor.onOpen = function(nodeType, nodeId)
	for i = 1, #node_open_listener do
		co.fork(node_open_listener[i], nil, nodeType, nodeId)
	end
end

harbor.onClose = function(nodeType, nodeId)
	for i = 1, #node_close_listener do
		co.fork(node_close_listener[i], nil, nodeType, nodeId)
	end
end

-------------------------------------------- rpc --------------------------------------------------------
local rpc = function(nodeType, nodeId, seq, func, ...)
	assert(seq ~= nil, "where is seq")
	assert((func ~= nil) and (proto[func] ~= nil), "where is rpc func")
	
	co.fork(proto[func], function(running)
		sequence[seq] = running
		co_sequence[running] = seq
		co_call_node[running] = { nodeType = nodeType, nodeId = nodeId }
	end, ...)
end

local rpcnr = function(nodeType, nodeId, func, ...)
	assert((func ~= nil) and (proto[func] ~= nil), "where is rpc func")
	
	co.fork(proto[func], function(running)
		co_call_node[running] = { nodeType = nodeType, nodeId = nodeId }
	end, ...)
end

harbor.onCall = function(nodeType, nodeId, message)
	rpc(nodeType, nodeId, string.unserialize(message))
end

harbor.onCallNoRet = function(nodeType, nodeId, message)
	rpcnr(nodeType, nodeId, string.unserialize(message))
end

harbor.onRespone = function(nodeType, nodeId, message)
	co.respone(string.unserialize(message))
end

--------------------------------------------- api ---------------------------------------------
function add_node_open_listener(listener)
	node_open_listener[#node_open_listener + 1] = listener
end

function add_node_close_listener(listener)
	node_close_listener[#node_close_listener + 1] = listener
end

function register(name, func)
	proto[name] = func
end

function node()
	return co_call_node[coroutine.running()]
end

local callback = function(succ, ...)
	assert(succ, "call failed")
	return ...
end

function call(nodeType, nodeId, func, wait, ...)
	if wait then
		local args = {...}
		return callback(co.wait(function(seq)
			harbor.call(nodeType, nodeId, string.serialize(seq, func, unpack(args)))
		end))
	else
		harbor.callNoRet(nodeType, nodeId, string.serialize(func, ...))
	end
end

function typeCall(nodeType, func, ...)
	harbor.typeCall(nodeType, string.serialize(func, ...))
end

function ret(...)
	local running = coroutine.running()
	
	local seq = co_sequence[running]
	assert(seq ~= nil, "where is seq")
	co_sequence[running] = nil
	sequence[seq] = nil
	
	local node = co_call_node[running]
	assert(node ~= nil, "where is call node")
	
	harbor.respone(node.nodeType, node.nodeId, string.serialize(seq, true, ...))
end

return _ENV