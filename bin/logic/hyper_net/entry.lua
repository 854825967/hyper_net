local loops = {}
function reg_loop(func)
	loops[#loops + 1] = func
end

local destroy = {}
function reg_destroy(func)
	destroy[#destroy + 1] = func
end

function loop()
    for i = 1, #loops do
		loops[i]()
	end
end

function release()
    for i = 1, #destroy do
		destroy[i]()
	end
end

collectgarbage("collect")
-- avoid memory leak
collectgarbage("setpause", 100)
collectgarbage("setstepmul", 5000)

math.randomseed(os.time())
require "core.harbor"
--require "core.timer"
require "main"

return true
