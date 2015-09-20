local pairs = pairs
local ipairs = ipairs
local assert = assert
local system = require "serverd.system"
local reg_loop = reg_loop

_ENV = {}

local ELAPSE = 20
local cycles = {
	{ timers = {}, size = 256, now = 0 },
	{ timers = {}, size = 64, now = 0 },
	{ timers = {}, size = 64, now = 0 },
	{ timers = {}, size = 64, now = 0 },
	{ timers = {}, size = 64, now = 0 },
}
local running = {}
local now = 0
local tick = system.tick()

local findTimerListFromCycle = function(cycle, expire)
	local index = expire % cycle.size
	local list = cycle.timers[index]
	if list == nil then
		list = {}
		cycle.timers[index] = list
	end
	return list
end

local findTimerList = function(expire)
	if expire <= now then
		return running
	end
	
	local life = expire - now
	if life < 256 then
		return findTimerListFromCycle(cycles[1], expire)
	elseif life < 64 * 256 then
		return findTimerListFromCycle(cycles[2], expire)
	elseif life < 64 * 64 * 256 then
		return findTimerListFromCycle(cycles[3], expire)
	elseif life < 64 * 64 * 64 * 256 then
		return findTimerListFromCycle(cycles[4], expire)
	elseif life < 64 * 64 * 64 * 64 * 256 then
		return findTimerListFromCycle(cycles[5], expire)
	end
end

local schedule = function(base)
	local list = findTimerList(base)
	assert(list, "where is timer list")
	list[base] = true
end

local checkHighGear = function(index)
	if index == nil then
		index = 1
	end
	local cycle = cycles[index]
	assert(cycle, "where is cycle")
	
	if cycle.now > cycle.size then
		cycle.now = 0
	end
	
	if index ~= 5 then
		if cycle.now == 0 then
			checkHighGear(index + 1)
		end
	end
	
	if index ~= 1 then
		local list = cycle.timers[cycle.now]
		if list ~= nil then
			cycle.timers[cycle.now] = nil
			for k, v in pairs(list) do
				schedule(k)
			end
		end
		cycle.now = cycle.now + 1
	end
end

local update = function()
	local cycle = cycles[1]
	local list = cycle.timers[cycle.now]
	if list ~= nil then
		cycle.timers[cycle.now] = {}
		for k, v in pairs(list) do
			if k.pause ~= nil then
				cycle.timers[cycle.now][k] = true
			else
				running[k] = true
			end
		end
	end
	cycle.now = cycle.now + 1
end

local endTimer = function(base, nonviolent)
	if base.timer.onEnd ~= nil then
		base.timer.onEnd(nonviolent, system.tick())
	end
	base.valid = false
end

local pool = function(base)
	if not base.started then
		if base.timer.onStart ~= nil then
			base.timer.onStart(system.tick())
		end
		base.started = false
		
		if base.valid then
			schedule(base)
		end
	else
		if base.timer.onTimer ~= nil then
			base.timer.onTimer(system.tick())
		end
		
		if base.valid then
			if base.count > 0 then
				base.count = base.count - 1
				if base.count == 0 then
					endTimer(base, true)
				end
			end
		end
	end
end

function start(timer, delay, count, interval)
	if interval < ELAPSE then
		interval = ELAPSE
	end
	if delay > 0 and delay < ELAPSE then
		delay = ELAPSE
	end
	
	assert(not timer.__TIMER_BASE__, "there is alreay a timer base")
	
	local base = {
		interval = math.ceil(interval / ELAPSE),
		delay = math.ceil(delay / ELAPSE),
		count = count,
		timer = timer,
		started = false,
		valid = true
	}
	base.expire = now + base.delay
	timer.__TIMER_BASE__ = base
	
	schedule(base)
end

function stop(timer)
	local base = timer.__TIMER_BASE__
	assert(base, "where is timer base")
	
	local list = findTimerList(base.expire)
	list[base] = nil
	endTimer(base, false)
end

function pause(timer)
	local base = timer.__TIMER_BASE__
	assert(base, "where is timer base")
	
	base.pause = now
	if base.timer.onPause ~= nil then
		base.timer.onPause(system.tick())
	end
end

function resume(timer)
	local base = timer.__TIMER_BASE__
	assert(base, "where is timer base")
	
	local list = findTimerList(base.expire)
	list[base] = nil
	
	base.expire = now + base.expire - base.pause
	base.pause = false
	schedule(base)
	if base.timer.onResume ~= nil then
		base.timer.onResume(system.tick())
	end
end

reg_loop(function ()
	local now = system.tick()
	local count = math.ceil((now - tick) / ELAPSE)
	for i = 1, count do
		checkHighGear()
		now = now + 1
		update()
	end
	
	while true do
		local list = running
		running = {}
		
		local count = 0
		for base, v in pairs(list) do
			poll(k)
			count = count + 1
		end
		
		if count == 0 then
			break
		end
	end
end)

return _ENV