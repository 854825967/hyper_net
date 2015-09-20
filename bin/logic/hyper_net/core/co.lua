local table = table
local coroutine = coroutine
local string = string
local assert = assert
local unpack = table.unpack

_ENV = {}
------------------------------------------- coroutine -------------------------------------------
local coroutine_pool = {}
local create_co = function (func)
	local co = table.remove(coroutine_pool)
	if co == nil then
		co = coroutine.create(function()
			func()
			while true do
				func = nil
				coroutine_pool[#coroutine_pool+1] = co
				func = coroutine.yield("EXIT")
				func(coroutine.yield())
			end
		end)
	else
		coroutine.resume(co, func)
	end
	return co
end

local next_sequence_id = 1
function create_sequence()
	next_sequence_id = next_sequence_id + 1
	if next_sequence_id < 1 then 
		next_sequence_id = 1 
	end
	
	return next_sequence_id
end

local wait_sequence = {}
local co_queue = {}
local co_exit_listener = {}

local co_exit = function(co, excption)
	for i = 1, #co_exit_listener do
		co_exit_listener[i](co)
	end
end

local co_call = function(co, seq)
	wait_sequence[seq] = co
end

local co_suspend = function(co, result, command, ...)
	if not result then
		co_exit(co, true)
		error("coroutine stop exception:"..command)
	end
	
	if command == "EXIT" then
		co_exit(co, false)
	elseif command == "CALL" then
		co_call(co, ...)
	else
		error("Unknown command : " .. command)
	end
end

--------------------------------------------- api ----------------------------------------------
function fork(f, onCoCreated, ...)
	local args = {...}
	local co = create_co(function() f(unpack(args)) end)
	if co then
		if onCoCreated ~= nil then
			onCoCreated(co)
		end
		co_suspend(co, coroutine.resume(co))
	end	
end

function respone(seq, ...)
	if wait_sequence[seq] ~= nil then
		local co = wait_sequence[seq]
		if co then
			wait_sequence[seq] = nil
			co_suspend(co, coroutine.resume(co, ...))
		end
	end
end

function wait(func)
	local sequence_id = create_sequence()
	func(sequence_id)
	return coroutine.yield("CALL", sequence_id)
end

function reg_co_exit_listener(func)
	co_exit_listener[#co_exit_listener + 1] = func
end

return _ENV