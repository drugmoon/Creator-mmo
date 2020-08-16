import Point from "./Point";
import RoadNode from "./RoadNode";
import { MapType } from "../base/MapType";

/**
 * 地图路点的换算 
 * @author 落日故人 QQ 583051842
 * 
 */
export default class MapRoadUtils  {

    private static _instance:MapRoadUtils;

    public static get instance():MapRoadUtils
    {
        if(this._instance == null)
        {
            this._instance = new MapRoadUtils();
        }
        return this._instance;
    }

    /**
     * 地图宽度
     */		
    private _mapWidth:number; 
    
    /**
     *地图高度 
     */		
    private _mapHeight:number;
    
    /**
     *地图一共分成几行 
     */		
    private _row:number;
    
    /**
     *地图一共分成几列 
     */		
    private _col:number;
    
    /**
     *地图路点单元格宽 
     */		
    private _nodeWidth:number;
    
    /**
     *地图路点单元格高 
     */		
    private _nodeHeight:number;
    
    /**
     *地图路点单元宽的一半 
     */	
    private _halfNodeWidth:number;
    
    /**
     *地图路点单元高的一半 
     */	
    private _halfNodeHeight:number;
    
    private _mapType:number;
    
    private _mapRoad:IMapRoad;

    public updateMapInfo(mapWidth:number, mapHeight:number, nodeWidth:number, nodeHeight:number,mapType:MapType):void
    {
        this._mapWidth = mapWidth;
        this._mapHeight = mapHeight;
        this._nodeWidth = nodeWidth;
        this._nodeHeight = nodeHeight;
        
        this._halfNodeWidth = Math.floor(this._nodeWidth / 2);
        this._halfNodeHeight = Math.floor(this._nodeHeight / 2);
        
        this._col = Math.ceil(mapWidth / this._nodeWidth);
        this._row = Math.ceil(mapHeight / this._nodeHeight);
        
        this._mapType = mapType;
        
        switch(this._mapType)
        {
            case MapType.angle45:this._mapRoad = new MapRoad45Angle(this._row,this._col,this._nodeWidth,this._nodeHeight,this._halfNodeWidth,this._halfNodeHeight);break;
            case MapType.angle90:this._mapRoad = new MapRoad90Angle(this._row,this._col,this._nodeWidth,this._nodeHeight,this._halfNodeWidth,this._halfNodeHeight);break;
            case MapType.honeycomb:
                
                //this._nodeHeight = (this._nodeWidth / 2) * 1.732;
                
                this._col = Math.ceil((this._mapWidth - this._nodeWidth / 4) / (this._nodeWidth / 4 * 6)) * 2;
                this._row = Math.ceil((this._mapHeight - this._nodeHeight / 2) / this._nodeHeight);
                
                this._mapRoad = new MapRoadHoneycomb(this._row,this._col,this._nodeWidth,this._nodeHeight,this._halfNodeWidth,this._halfNodeHeight);break;
        }
    }
    
    /**
     *根据地图平面像素坐标获得路节点  
        * @param x 
        * @param y
        * @return 
        * 
        */		
    public getNodeByPixel(x:number,y:number):RoadNode
    {
        if(this._mapRoad)
        {
            return this._mapRoad.getNodeByPixel(x,y);
        }
        return new RoadNode();
    }
    
    /**
     *根据路点平面坐标点获得路节点  
        * @param px
        * @param py
        * @return 
        * 
        */			
    public getNodeByDerect(dx:number,dy:number):RoadNode
    {
        if(this._mapRoad)
        {
            return this._mapRoad.getNodeByDerect(dx,dy);
        }
        return new RoadNode();
    }
    
    /**
     *根据路点场景世界坐标获得路节点 
        * @param wx
        * @param wy
        * @return 
        * 
        */			
    public getNodeByWorldPoint(wx:number,wy:number):RoadNode
    {
        if(this._mapRoad)
        {
            return this._mapRoad.getNodeByWorldPoint(wx,wy);
        }
        return new RoadNode();
    }
    
    /**
     *根据像素坐标得到场景世界坐标 
        * @param x
        * @param y
        * @return 
        * 
        */		
    public getWorldPointByPixel(x:number,y:number):Point
    {
        if(this._mapRoad)
        {
            return this._mapRoad.getWorldPointByPixel(x,y);
        }
        return new Point();
    }
    
    /**
     *根据世界坐标获得像素坐标 
        * @param cx
        * @param cy
        * @return 
        * 
        */		
    public getPixelByWorldPoint(cx:number,cy:number):Point
    {
        if(this._mapRoad)
        {
            return this._mapRoad.getPixelByWorldPoint(cx,cy);
        }
        return new Point();
    }
    
    /**
     *根据像素坐标获得网格平面坐标
        * @param x
        * @param y
        * @return 
        * 
        */		
    public getDerectByPixel(x:number,y:number):Point
    {
        if(this._mapRoad)
        {
            return this._mapRoad.getDerectByPixel(x,y);
        }
        return new Point();
        
    }
    
    /**
     *根据世界坐标获得网格平面坐标 
        * @param cx
        * @param cy
        * @return 
        * 
        */		
    public getDerectByWorldPoint(cx:number,cy:number):Point
    {
        if(this._mapRoad)
        {
            return this._mapRoad.getDerectByWorldPoint(cx,cy);
        }
        return new Point();
    }
    
    /**
     *根据网格平面坐标获得世界坐标 
        * @param dx
        * @param dy
        * @return 
        * 
        */		
/*	public getWorldPointByDerect(dx:number,dy:number):Point
    {
        var cx:number = (dy + dx) / 2;
        var cy:number = (dy - dx) / 2 + col - 1;
        return new Point(cx,cy);
    }*/
    
    public getPixelByDerect(dx:number,dy:number):Point
    {
        if(this._mapRoad)
        {
            return this._mapRoad.getPixelByDerect(dx,dy);
        }
        return new Point();
    }
    
    public get mapWidth():number
    {
        return this._mapWidth;
    }
    
    public get mapHeight():number
    {
        return this._mapHeight;
    }
    
    public get nodeWidth():number
    {
        return this._nodeWidth;
    }
    
    public get nodeHeight():number
    {
        return this._nodeHeight;
    }
    
    public get row():number
    {
        return this._row;
    }
    
    public get col():number
    {
        return this._col;
    }

    public get halfNodeWidth():number
    {
        return this._halfNodeWidth;
    }

    public get halfNodeHeight():number
    {
        return this._halfNodeHeight;
    }

    /**
     *地图类型 0:斜45度等视角地图, 1:90度角平面地图 
        */
    public get mapType():number
    {
        return this._mapType;
    }

}

/**
 *地图路点处理接口 
 * @author Administrator
 * 
 */
interface IMapRoad
{
	/**
	 *根据地图平面像素坐标获得路节点  
	 * @param x 
	 * @param y
	 * @return 
	 * 
	 */
	getNodeByPixel(x:number,y:number):RoadNode;
	
	/**
	 *根据路点平面坐标点获得路节点  
	 * @param px
	 * @param py
	 * @return 
	 * 
	 */			
	getNodeByDerect(dx:number,dy:number):RoadNode;
	
	/**
	 *根据路点场景世界坐标获得路节点 
	 * @param wx
	 * @param wy
	 * @return 
	 * 
	 */			
	getNodeByWorldPoint(wx:number,wy:number):RoadNode;
	
	
	/**
	 *根据像素坐标得到场景世界坐标 
	 * @param x
	 * @param y
	 * @return 
	 * 
	 */		
	getWorldPointByPixel(x:number,y:number):Point;
	
	
	/**
	 *根据世界坐标获得像素坐标 
	 * @param cx
	 * @param cy
	 * @return 
	 * 
	 */		
	getPixelByWorldPoint(cx:number,cy:number):Point;
	
	
	/**
	 *根据像素坐标获得网格平面坐标
	 * @param x
	 * @param y
	 * @return 
	 * 
	 */		
	getDerectByPixel(x:number,y:number):Point;
	
	
	/**
	 *根据世界坐标获得网格平面坐标 
	 * @param cx
	 * @param cy
	 * @return 
	 * 
	 */		
	getDerectByWorldPoint(cx:number,cy:number):Point;
	
	
	/**
	 *根据网格平面坐标获得像素坐标 
	 * @param dx
	 * @param dy
	 * @return 
	 * 
	 */		
	getPixelByDerect(dx:number,dy:number):Point;
}

/**
 *45度等视角地图路点处理接口实现 
 * @author Administrator
 * 
 */
class MapRoad45Angle implements IMapRoad
{
	
	/**
	 *地图一共分成几行 
	 */		
	private _row:number;
	
	/**
	 *地图一共分成几列 
	 */		
	private _col:number;
	
	/**
	 *地图路点单元格宽 
	 */		
	private _nodeWidth:number;
	
	/**
	 *地图路点单元格高 
	 */		
	private _nodeHeight:number;
	
	/**
	 *地图路点单元宽的一半 
	 */	
	private _halfNodeWidth:number;
	
	/**
	 *地图路点单元高的一半 
	 */	
	private _halfNodeHeight:number;
	
	public constructor(row:number,col:number,nodeWidth:number,nodeHeight:number,halfNodeWidth:number,halfNodeHeight:number)
	{
		this._row = row;
		this._col = col;
		this._nodeWidth = nodeWidth;
		this._nodeHeight = nodeHeight;
		this._halfNodeWidth = halfNodeWidth;
		this._halfNodeHeight = halfNodeHeight;
	}
	
	/**
	 *根据地图平面像素坐标获得路节点  
	 * @param x 
	 * @param y
	 * @return 
	 * 
	 */		
	public getNodeByPixel(x:number,y:number):RoadNode
	{
		var wPoint:Point = this.getWorldPointByPixel(x,y);
		var fPoint:Point = this.getPixelByWorldPoint(wPoint.x,wPoint.y);
		var dPoint:Point = this.getDerectByPixel(x,y);
		
		var node:RoadNode = new RoadNode();
		
		node.cx = wPoint.x;
		node.cy = wPoint.y;
		
		node.px = fPoint.x;
		node.py = fPoint.y;
		
		node.dx = dPoint.x;
		node.dy = dPoint.y;
		
		return node;
	}
	
	/**
	 *根据路点平面坐标点获得路节点  
	 * @param px
	 * @param py
	 * @return 
	 * 
	 */			
	public getNodeByDerect(dx:number,dy:number):RoadNode
	{
		
		var fPoint:Point = this.getPixelByDerect(dx,dy);
		var wPoint:Point = this.getWorldPointByPixel(fPoint.x,fPoint.y);
		
		var node:RoadNode = new RoadNode();
		
		node.cx = wPoint.x;
		node.cy = wPoint.y;
		
		node.px = fPoint.x;
		node.py = fPoint.y;
		
		node.dx = dx;
		node.dy = dy;
		
		return node;
	}
	
	/**
	 *根据路点场景世界坐标获得路节点 
	 * @param wx
	 * @param wy
	 * @return 
	 * 
	 */			
	public getNodeByWorldPoint(wx:number,wy:number):RoadNode
	{
		var point:Point = this.getPixelByWorldPoint(wx,wy)
		return this.getNodeByPixel(point.x,point.y);
	}
	
	/**
	 *根据像素坐标得到场景世界坐标 
	 * @param x
	 * @param y
	 * @return 
	 * 
	 */		
	public getWorldPointByPixel(x:number,y:number):Point
	{
		var cx:number = Math.ceil(x/this._nodeWidth - 0.5 + y/this._nodeHeight) - 1;
		var cy:number = (this._col - 1) - Math.ceil(x/this._nodeWidth - 0.5 - y/this._nodeHeight);
		
		return new Point(cx,cy);
	}
	
	/**
	 *根据世界坐标获得像素坐标 
	 * @param cx
	 * @param cy
	 * @return 
	 * 
	 */		
	public getPixelByWorldPoint(cx:number,cy:number):Point
	{
		var x:number = Math.floor((cx + 1 - (cy  - (this._col - 1))) * this._halfNodeWidth);
		var y:number = Math.floor((cx + 1 + (cy  - (this._col - 1))) * this._halfNodeHeight);
		return new Point(x,y);
	}
	
	/**
	 *根据像素坐标获得网格平面坐标
	 * @param x
	 * @param y
	 * @return 
	 * 
	 */		
	public getDerectByPixel(x:number,y:number):Point
	{
		var worldPoint:Point = this.getWorldPointByPixel(x,y);
		var pixelPoint:Point = this.getPixelByWorldPoint(worldPoint.x,worldPoint.y);
		var dx:number = Math.floor( pixelPoint.x / this._nodeWidth ) - ( pixelPoint.x  % this._nodeWidth == 0 ? 1 : 0 );
		var dy:number =  Math.floor( pixelPoint.y / this._halfNodeHeight ) - 1;
		return new Point(dx,dy);
	}
	
	/**
	 *根据世界坐标获得网格平面坐标 
	 * @param cx
	 * @param cy
	 * @return 
	 * 
	 */		
	public getDerectByWorldPoint(cx:number,cy:number):Point
	{
		var dx:number = Math.floor((cx - (cy - (this._col -1)))/2);
		var dy:number = cx  + (cy  - (this._col - 1));
		return new Point(dx,dy);
	}
	
	/**
	 *根据网格平面坐标获得像素坐标 
	 * @param dx
	 * @param dy
	 * @return 
	 * 
	 */		
	public getPixelByDerect(dx:number,dy:number):Point
	{
		var x:number = Math.floor((dx + dy % 2) * this._nodeWidth + (1 - dy % 2) * this._halfNodeWidth);
		var y:number = Math.floor((dy + 1) * this._halfNodeHeight);
		return new Point(x,y);
	}
}

/**
 *90度平面地图路点处理接口实现 
 * @author Administrator
 * 
 */
class MapRoad90Angle implements IMapRoad
{
	/**
	 *地图一共分成几行 
	 */		
	private _row:number;
	
	/**
	 *地图一共分成几列 
	 */		
	private _col:number;
	
	/**
	 *地图路点单元格宽 
	 */		
	private _nodeWidth:number;
	
	/**
	 *地图路点单元格高 
	 */		
	private  _nodeHeight:number;
	
	/**
	 *地图路点单元宽的一半 
	 */	
	private _halfNodeWidth:number;
	
	/**
	 *地图路点单元高的一半 
	 */	
	private _halfNodeHeight:number;
	
	public constructor(row:number,col:number,nodeWidth:number,nodeHeight:number,halfNodeWidth:number,halfNodeHeight:number)
	{
		this._row = row;
		this._col = col;
		this._nodeWidth = nodeWidth;
		this._nodeHeight = nodeHeight;
		this._halfNodeWidth = halfNodeWidth;
		this._halfNodeHeight = halfNodeHeight;
	}
	
	/**
	 *根据地图平面像素坐标获得路节点  
	 * @param x 
	 * @param y
	 * @return 
	 * 
	 */		
	public getNodeByPixel(x:number,y:number):RoadNode
	{
		var wPoint:Point = this.getWorldPointByPixel(x,y);
		var fPoint:Point = this.getPixelByWorldPoint(wPoint.x,wPoint.y);
		var dPoint:Point = this.getDerectByPixel(x,y);
		
		var node:RoadNode = new RoadNode();
		
		node.cx = wPoint.x;
		node.cy = wPoint.y;
		
		node.px = fPoint.x;
		node.py = fPoint.y;
		
		node.dx = dPoint.x;
		node.dy = dPoint.y;
		
		return node;
	}
	
	/**
	 *根据路点平面坐标点获得路节点  
	 * @param px
	 * @param py
	 * @return 
	 * 
	 */			
	public getNodeByDerect(dx:number,dy:number):RoadNode
	{
		
		var fPoint:Point = this.getPixelByDerect(dx,dy);
		var wPoint:Point = this.getWorldPointByPixel(fPoint.x,fPoint.y);
		
		var node:RoadNode = new RoadNode();
		
		node.cx = wPoint.x;
		node.cy = wPoint.y;
		
		node.px = fPoint.x;
		node.py = fPoint.y;
		
		node.dx = dx;
		node.dy = dy;
		
		return node;
	}
	
	/**
	 *根据路点场景世界坐标获得路节点 
	 * @param wx
	 * @param wy
	 * @return 
	 * 
	 */			
	public getNodeByWorldPoint(wx:number,wy:number):RoadNode
	{
		var point:Point = this.getPixelByWorldPoint(wx,wy)
		return this.getNodeByPixel(point.x,point.y);
	}
	
	/**
	 *根据像素坐标得到场景世界坐标 
	 * @param x
	 * @param y
	 * @return 
	 * 
	 */		
	public getWorldPointByPixel(x:number,y:number):Point
	{
		var cx:number = Math.floor(x / this._nodeWidth);
		var cy:number = Math.floor(y / this._nodeHeight);
		
		return new Point(cx,cy);
	}
	
	/**
	 *根据世界坐标获得像素坐标 
	 * @param cx
	 * @param cy
	 * @return 
	 * 
	 */		
	public getPixelByWorldPoint(cx:number,cy:number):Point
	{
		var x:number = Math.floor((cx + 1) * this._nodeWidth - this._halfNodeWidth);
		var y:number = Math.floor((cy + 1) * this._nodeHeight - this._halfNodeHeight);
		return new Point(x,y);
	}
	
	/**
	 *根据像素坐标获得网格平面坐标
	 * @param x
	 * @param y
	 * @return 
	 * 
	 */		
	public getDerectByPixel(x:number,y:number):Point
	{
		var dx:number = Math.floor(x / this._nodeWidth);
		var dy:number = Math.floor(y / this._nodeHeight);
		return new Point(dx,dy);
	}
	
	/**
	 *根据世界坐标获得网格平面坐标 90度地图的世界坐标和网格坐标相同
	 * @param cx
	 * @param cy
	 * @return 
	 * 
	 */		
	public getDerectByWorldPoint(cx:number,cy:number):Point
	{
		return new Point(cx,cy);
	}
	
	/**
	 *根据网格平面坐标获得像素坐标 
	 * @param dx
	 * @param dy
	 * @return 
	 * 
	 */	
	public getPixelByDerect(dx:number,dy:number):Point
	{
		var x:number = Math.floor((dx + 1) * this._nodeWidth - this._halfNodeWidth);
		var y:number = Math.floor((dy + 1) * this._nodeHeight - this._halfNodeHeight);
		return new Point(x,y);
	}
}

/**
 *蜂巢式（即正六边形）地图路点处理接口实现 
 * @author Administrator
 * 
 */
class MapRoadHoneycomb implements IMapRoad
{
	/**
	 *地图一共分成几行 
	 */		
	private _row:number;
	
	/**
	 *地图一共分成几列 
	 */		
	private _col:number;
	
	/**
	 *地图路点单元格宽 
	 */		
	private _nodeWidth:number;
	
	/**
	 *地图路点单元格高 
	 */		
	private _nodeHeight:number;
	
	/**
	 *地图路点单元宽的一半 
	 */	
	private _halfNodeWidth:number;
	
	/**
	 *地图路点单元高的一半 
	 */	
	private _halfNodeHeight:number;
	
	/**
	 * 六边形直径的4分之一
	 */	
	private _nwDiv4:number;
	
	/**
	 * 六边形直径的半径
	 */	
	private _radius:number;

	/**
	 * 六边形宽高的tan值，正六边形为1.732
	 */
	private  _proportion = 1.732;
	
	/**
	 *蜂巢式（即正六边形）地图路点处理 
	 * @param row
	 * @param col
	 * @param nodeWidth
	 * @param nodeHeight
	 * @param halfNodeWidth
	 * @param halfNodeHeight
	 * 
	 */	
	public constructor(row:number,col:number,nodeWidth:number,nodeHeight:number,halfNodeWidth:number,halfNodeHeight:number)
	{
		this._row = row;
		this._col = col;
		this._nodeWidth = nodeWidth;
		//this._nodeHeight = (this._nodeWidth / 2) * 1.732;
		this._nodeHeight = nodeHeight;
		this._halfNodeWidth = halfNodeWidth;
		this._halfNodeHeight = halfNodeHeight;
		
		this._nwDiv4 = this._nodeWidth / 4;
		this._radius = this._nwDiv4 * 4;

		this._proportion = this._nodeHeight * 2 / this._nodeWidth;

	}
	
	/**
	 *根据地图平面像素坐标获得路节点  
	 * @param x 
	 * @param y
	 * @return 
	 * 
	 */		
	public getNodeByPixel(x:number,y:number):RoadNode
	{
		var wPoint:Point = this.getWorldPointByPixel(x,y);
		var fPoint:Point = this.getPixelByWorldPoint(wPoint.x,wPoint.y);
		//var dPoint:Point = getDerectByPixel(x,y);
		
		var node:RoadNode = new RoadNode();
		
		node.cx = wPoint.x;
		node.cy = wPoint.y;
		
		node.px = fPoint.x;
		node.py = fPoint.y;
		
		node.dx = wPoint.x;
		node.dy = wPoint.y;
		
		return node;
	}
	
	/**
	 *根据路点平面坐标点获得路节点  
	 * @param px
	 * @param py
	 * @return 
	 * 
	 */			
	public getNodeByDerect(dx:number,dy:number):RoadNode
	{
		
		var fPoint:Point = this.getPixelByDerect(dx,dy);
		//var wPoint:Point = getWorldPointByPixel(fPoint.x,fPoint.y);
		
		var node:RoadNode = new RoadNode();
		
		node.cx = dx;
		node.cy = dy;
		
		node.px = fPoint.x;
		node.py = fPoint.y;
		
		node.dx = dx;
		node.dy = dy;
		
		return node;
	}
	
	/**
	 *根据路点场景世界坐标获得路节点 
	 * @param wx
	 * @param wy
	 * @return 
	 * 
	 */			
	public getNodeByWorldPoint(wx:number,wy:number):RoadNode
	{
		var point:Point = this.getPixelByWorldPoint(wx,wy)
		return this.getNodeByPixel(point.x,point.y);
	}
	
	/**
	 *根据像素坐标得到场景世界坐标 
	 * @param x
	 * @param y
	 * @return 
	 * 
	 */		
	public getWorldPointByPixel(x:number,y:number):Point
	{
		var nwDiv4Index:number = Math.floor(x / this._nwDiv4);    //六边形的外切矩形竖方向均等分成4分，所有的六边形外切矩形连在一起形成一个个4分之一矩形宽的区域，nwDiv4Index就是该区域的索引
		
		var col:number = Math.floor(nwDiv4Index / 3);    //取得临时六边形横轴的索引,根据不同的情况可能会变
		
		var row:number;              //六边形纵轴的索引
		
		var cx:number;
		
		var cy:number;
		
		if((nwDiv4Index - 1) % 6 == 0 || (nwDiv4Index - 2) % 6 == 0)
		{
			row = Math.floor(y / this._nodeHeight);
			cx = col;
			cy = row;
		}else if((nwDiv4Index - 4) % 6 == 0 || (nwDiv4Index - 5) % 6 == 0)
		{
			if(y < this._nodeHeight / 2)
			{
				row = -1;
			}else
			{
				row = Math.floor((y - this._nodeHeight / 2)  / this._nodeHeight);
			}
			cx = col;
			cy = row
		}else{
			
			
			
			if(col % 2 == 0)
			{
				//(x - 1,y - 1)  (x - 1,y)
				row = Math.floor(y / this._nodeHeight);
				
				if(this.testPointInHoneycomb(col,row,x,y))
				{
					cx = col;
					cy = row;
				}else if(this.testPointInHoneycomb(col - 1,row - 1,x,y))
				{
					cx = col - 1;
					cy = row - 1;
				}else
				{
					cx = col - 1;
					cy = row;
				}
				
			}else
			{
				//(x - 1,y)  (x - 1,y + 1)
				
				if(y < this._nodeHeight / 2)
				{
					row = -1;
				}else
				{
					row = Math.floor((y - this._nodeHeight / 2)  / this._nodeHeight);
				}
				
				if(this.testPointInHoneycomb(col,row,x,y))
				{
					cx = col;
					cy = row;
				}else if(this.testPointInHoneycomb(col - 1,row,x,y))
				{
					cx = col - 1;
					cy = row;
				}else
				{
					cx = col - 1;
					cy = row + 1;
				}
				
			}
			
			
		}
		
		return new Point(cx,cy);
	}
	
	private testPointInHoneycomb(col:number,row:number,px:number,py:number):Boolean
	{
		
		var a:number = this._nwDiv4 * 2;
		
		var point:Point = this.getPixelByWorldPoint(col,row);
		
		var absX:number = Math.abs(px - point.x);
		var absY:number = Math.abs(py - point.y);
		
		//return a-absX >= absY/(1.732);

		return a - absX >= absY / this._proportion;
	}
	
	/**
	 *根据世界坐标获得像素坐标 
	 * @param cx
	 * @param cy
	 * @return 
	 * 
	 */		
	public getPixelByWorldPoint(cx:number,cy:number):Point
	{
		var x:number = Math.floor((2 + 3 * cx)/4 * this._nodeWidth);
		var y:number = Math.floor((cy + 1/2 *( 1 + (cx % 2))) * this._nodeHeight);

		return new Point(x,y);
	}
	
	/**
	 *根据像素坐标获得网格平面坐标
	 * @param x
	 * @param y
	 * @return 
	 * 
	 */		
	public getDerectByPixel(x:number,y:number):Point
	{
		
		return this.getWorldPointByPixel(x,y);
	}
	
	/**
	 *根据世界坐标获得网格平面坐标 90度地图的世界坐标和网格坐标相同
	 * @param cx
	 * @param cy
	 * @return 
	 * 
	 */		
	public getDerectByWorldPoint(cx:number,cy:number):Point
	{
		return new Point(cx,cy);
	}
	
	/**
	 *根据网格平面坐标获得像素坐标 
	 * @param dx
	 * @param dy
	 * @return 
	 * 
	 */	
	public getPixelByDerect(dx:number,dy:number):Point
	{
		var x:number = (2 + 3 * dx)/4 * this._nodeWidth;
		var y:number = (dy + 1/2 *( 1 + (dx % 2))) * this._nodeHeight;
		return new Point(x,y);
	}
}
