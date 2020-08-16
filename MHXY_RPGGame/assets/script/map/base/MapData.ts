import { MapType } from "./MapType";

export default class MapData {

    public name:string = "";
    public bgName:string = "";
    public type:MapType = MapType.angle45;
    public mapWidth:number = 0;
    public mapHeight:number = 0;
    public nodeWidth:number = 0; 
    public nodeHeight:number = 0;
    public roadDataArr:number[][] = [];
    //public row:number = 0;
    //public col:number = 0;

    public mapItem:object[] = [];


}
