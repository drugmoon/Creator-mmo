
/**
 * 地图路节点 
 * @author 落日故人 QQ 583051842
 * 
 */
export default class RoadNode {

    private _px:number;//像素坐标x轴
    private _py:number;//像素坐标y轴
    private _cx:number;//世界坐标x轴
    private _cy:number;//世界坐标y轴
    private _dx:number;//直角坐标x轴
    private _dy:number;//直角坐标y轴
    private _value:number = 0;//节点的值
    private _f:number = 0; //路点的f值
    private _g:number = 0; //路点的g值	
    private _h:number = 0; //路点的h值
    private _parent:RoadNode = null; //路点的父节点
    
    public constructor()
    {
    }
    
    public toString():String
    {
        return "路点像素坐标：（" + this._px + "," + this._py +"),  " +
            "路点世界坐标：（" + this._cx + "," + this._cy +"),  " + 
            "路点平面直角坐标：（" + this._dx + "," + this._dy +")";
    }
    
    public get px():number
    {
        return this._px;
    }

    public set px(value:number)
    {
        this._px = value;
    }

    public get py():number
    {
        return this._py;
    }

    public set py(value:number)
    {
        this._py = value;
    }

    public get cx():number
    {
        return this._cx;
    }

    public set cx(value:number)
    {
        this._cx = value;
    }

    public get cy():number
    {
        return this._cy;
    }

    public set cy(value:number)
    {
        this._cy = value;
    }

    public get dx():number
    {
        return this._dx;
    }

    public set dx(value:number)
    {
        this._dx = value;
    }

    public get dy():number
    {
        return this._dy;
    }

    public set dy(value:number)
    {
        this._dy = value;
    }

    public get value():number
    {
        return this._value;
    }

    public set value(val:number)
    {
        this._value = val;
    }

    public get f():number
    {
        return this._f;
    }

    public set f(value:number)
    {
        this._f = value;
    }

    public get g():number
    {
        return this._g;
    }

    public set g(value:number)
    {
        this._g = value;
    }

    public get h():number
    {
        return this._h;
    }

    public set h(value:number)
    {
        this._h = value;
    }

    public get parent():RoadNode
    {
        return this._parent;
    }

    public set parent(value:RoadNode)
    {
        this._parent = value;
    }
}
