import { MapType } from "./MapType";
import { MapLoadModel } from "./MapLoadModel";

/**
 * 地图参数
 */
export default class MapParams
{
    /**
     * 地图名称
     */
    public name:string = "";

    /**
     * 底图资源名称
     */
    public bgName:string = "";

    /**
     * 地图类型
     */
    public mapType:MapType = MapType.angle45;

    /**
     * 地图宽
     */
    public mapWidth:number = 750;

    /**
     * 地图高
     */
    public mapHeight:number = 1600;

    /**
     * 地图单元格宽
     */
    public ceilWidth:number = 75;

    /**
     * 地图单元格高
     */
    public ceilHeight:number = 75;

    /**
     * 地图视野宽
     */
    public viewWidth:number = 750;

    /**
     * 地图视野高
     */
    public viewHeight:number = 1334;

    /**
     * 地图切片宽
     */
    public sliceWidth:number = 256;

    /**
     * 地图切片高
     */
    public sliceHeight:number = 256;

    /**
     * 底图加载模式，是单张还是切片加载
     */
    public mapLoadModel:MapLoadModel = MapLoadModel.single;

    /**
     * 地图底图
     */
    public bgTex:cc.Texture2D = null;


}
