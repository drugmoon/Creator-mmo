 // 定义一个接口，用来对应json配置文件转成对象
interface FishType{
    name: string;
    hp: number;
    gold: number
}

// 鱼的生命状态
enum FishState {
    alive,
    dead
}
let MAX_SIZE = 1024*24;

let TILE_WIDTH = 66;
let TILE_HEIGHT = 44;
interface define {

}


export { FishState, FishType,MAX_SIZE,TILE_WIDTH, TILE_HEIGHT};