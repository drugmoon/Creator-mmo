import Player from "../gamescene/player/Player"

const {ccclass, property} = cc._decorator;

/**
 * 场景管理器
 */
@ccclass
export default class Command {

    public  source:Player;//原始
    public  target:Player;//目标
	private cmd:string;
	
	public constructor(cmd, source:Player, target:Player) {
		this.cmd = cmd;
		this.source = source;
		this.target = target;
	}
	
   // public params:cc.Prefab[] = [];

    public getCmd():string {
		return this.cmd;
	}

	/**
	 * 添加一个参数
	 * 
	 * @param name
	 * @param value
	 */
	public add(name:string, value:string) {
		//this.params.put(name, value);
	}

	// public get(String name) {
	// 	return this.params.get(name);
	// }

	// public int getInt(String name) {
	// 	Integer val = (Integer) this.params.get(name);
	// 	return val != null ? val.intValue() : 0;
	// }

	// public boolean getBool(String name) {
	// 	Boolean val = (Boolean) this.params.get(name);
	// 	return val != null ? val.booleanValue() : false;
	// }

	/**
	 * @return the source 已方
	 */
	public  getSource():Player {
		return this.source;
	}

	/**
	 * @return the target 目标
	 */
	public getTarget():Player {
		return this.target;
	}
}