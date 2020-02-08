module(..., package.seeall)
require "task.util"
require "task.task1000"
require "task.task1100"
require "task.task2000"
require "task.simulateTask"

function on_login(player)
	-- player:set_task_state(const.TASK_ID_FUBEN_SEXP,0);
	-- player:push_task_data(const.TASK_ID_FUBEN_SEXP,0);
	-- player:set_task_state(const.TASK_ID_ZMT,0);
	-- player:push_task_data(const.TASK_ID_ZMT,0);
	-- player:set_task_state(const.TASK_ID_FUBEN_GUILD,0);
	-- player:push_task_data(const.TASK_ID_FUBEN_GUILD,0);
end
login.add_login_listener(on_login);