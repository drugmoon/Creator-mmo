// Learn TypeScript:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/typescript.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/reference/attributes.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - [Chinese] https://docs.cocos.com/creator/manual/zh/scripting/life-cycle-callbacks.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/life-cycle-callbacks.html

import MapData from "./map/base/MapData";
import { MapLoadModel } from "./map/base/MapLoadModel";

const {ccclass, property} = cc._decorator;

/**
 * 场景管理器
 */
@ccclass
export default class SceneManager extends cc.Component {

    private static _instance:SceneManager = null;

    public static get instance():SceneManager
    {
        if(!this._instance)
        {
            this._instance = new SceneManager();
            this._instance.init();
        }

        return this._instance;
    }


    /**
     * 当前地图id
     */
    public currentMapId:number = 0;

    /**
     * 进入地图后的出生点id
     */
    public enterSpawnId:number = 0;

    /**
     * 地图加载模式
     */
    public mapLoadModel:MapLoadModel = MapLoadModel.single;

    /**
     * 地图数据
     */
    public mapData:MapData = null;

    /**
     * 加载到的地图底图（可能是整张，也可能是缩小后的马赛克底图）
     */
    public bgTex:cc.Texture2D = null;

    // LIFE-CYCLE CALLBACKS:

    // onLoad () {}

    start () {

    }

    private init():void
    {

    }

    /**
     * 加载地图
     * @param mapId 地图id
     * @param enterSpawnId 进入地图的出生点id
     * @param mapLoadModel 加载模式，单张加载，还是切片加载
     */
    public loadMap(mapId:number,enterSpawnId:number,mapLoadModel:MapLoadModel = MapLoadModel.single)
    {
        this.currentMapId = mapId;
        this.enterSpawnId = enterSpawnId;
        this.mapLoadModel = mapLoadModel;

        if(mapLoadModel == MapLoadModel.single)
        {
            this.loadSingleMap();
        }else
        {
            this.loadSlicesMap();
        }

    }

    /**
     * 加载单张地图（地图比较小的情况用）
     */
    protected loadSingleMap()
    {

        var mapName:string = "" + this.currentMapId;

        cc.loader.loadRes("map/data/" + mapName,cc.JsonAsset,(error:Error,res:cc.JsonAsset)=>
        {
            this.mapData = res.json;

            cc.loader.loadRes("map/bg/" + this.mapData.bgName,cc.Texture2D,(error:Error,tex:cc.Texture2D)=>
            {
                this.bgTex = tex;
                cc.director.loadScene("map_" + mapName);
            });
        });
    }


    /**
     * 加载分切片地图（地图比较大，加载时间过程。或者超过了 2048 * 2048，微信小游戏不能加载的情况使用）
     */
    protected loadSlicesMap()
    {
        var mapName:string = "" + this.currentMapId;

        cc.loader.loadRes("map/data/" + mapName,cc.JsonAsset,(error:Error,res:cc.JsonAsset)=>
        {
            this.mapData = res.json;

            cc.loader.loadRes("map/bg/" + this.mapData.bgName + "/miniMap",cc.Texture2D,(error:Error,tex:cc.Texture2D)=>
            {
                this.bgTex = tex;
                cc.director.loadScene("map_" + mapName);
            });

        });
    }
}
