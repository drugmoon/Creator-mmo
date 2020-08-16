import RoadNode from "./RoadNode";
import IRoadSeeker from "./IRoadSeeker";

/**
 * A*寻路算法 
 * @author 落日故人 QQ 583051842
 * 
 */
export default class AStarRoadSeeker implements IRoadSeeker {

    
    /**
     * 横向移动一个格子的代价
     */		
    private COST_STRAIGHT:number = 10; 
    
    /**
     * 斜向移动一个格子的代价
     */		
    private COST_DIAGONAL:number = 14;
    
    /**
     *最大搜寻步骤数，超过这个值时表示找不到目标 
        */		
    private maxStep:number = 1000;
    
    /**
     * 开启列表
     */		
    private _openlist:Array<RoadNode>; 
    
    /**
     *关闭列表 
        */		
    private _closelist:Array<RoadNode>;
    
    /**
     *开始节点 
        */		
    private _startNode:RoadNode;
    
    /**
     *当前检索节点 
        */		
    private _currentNode:RoadNode;
    
    /**
     *目标节点 
        */		
    private _targetNode:RoadNode;
    
    /**
     *地图路点数据 
        */		
    private _roadNodes:{[key:number]:RoadNode};
    
    /**
     *用于检索一个节点周围8个点的向量数组 
        */		
    private _round:number[][] = [[0,-1],[1,-1],[1,0],[1,1],[0,1],[-1,1],[-1,0],[-1,-1]]
    
    private handle:number = -1;

    /**
     * 是否优化路径
     */
    private optimize:boolean = true;
    
    public constructor(roadNodes:{[key:string]:RoadNode})
    {
        this._roadNodes = roadNodes;
    }
    
    
    /**
     *寻路入口方法 
        * @param startNode
        * @param targetNode
        * @return 
        * 
        */		
    public seekPath(startNode:RoadNode,targetNode:RoadNode):Array<RoadNode>
    {
        this._startNode = startNode;
        this._currentNode = startNode;
        this._targetNode = targetNode;
        
        if(!this._startNode || !this._targetNode)
            return [];
        
        if(this._targetNode.value == 1)
        {
            console.log("目标不可达到：");
            return [];
        }
        
        this._openlist = [];
        this._closelist = [];
        
        var step:number = 0;
        
        while(true)
        {
            if(step > this.maxStep)
            {
                console.log("没找到目标计算步骤为：",step);
                return [];
            }
            
            step++;
            
            this.searchRoundNodes(this._currentNode);
            
            if(this._openlist.length == 0)
            {
                console.log("没找到目标计算步骤为：",step);
                return [];
            }
            
            this._openlist.sort(this.sortNode);
            
            this._currentNode = this._openlist.shift();
            
            if(this._currentNode == this._targetNode)
            {
                console.log("找到目标计算步骤为：",step);
                return this.getPath();
            }else
            {
                this._closelist.push(this._currentNode);
            }
            
        }
        
        return [];
    }


    /**
     *寻路入口方法 如果没有寻到目标，则返回离目标最近的路径
    * @param startNode
    * @param targetNode
    * @return 
    * 
    */		
    public seekPath2(startNode:RoadNode,targetNode:RoadNode,maxStep:number = 1000):Array<RoadNode>
    {
        this._startNode = startNode;
        this._currentNode = startNode;
        this._targetNode = targetNode;
        
        if(!this._startNode || !this._targetNode)
            return [];
        
        /*if(this._targetNode.value == 1)
        {
            console.log("目标不可达到：");
            return [];
        }*/
        
        this._openlist = [];
        this._closelist = [];
        
        var step:number = 0;
        
        var closestNode:RoadNode = null; //距离目标最近的路点

        while(true)
        {
            if(step > maxStep)
            {
                console.log("没找到目标计算步骤为：",step);
                return this.seekPath(startNode,closestNode);
            }
            
            step++;
            
            this.searchRoundNodes(this._currentNode);
            
            if(this._openlist.length == 0)
            {
                console.log("没找到目标计算步骤为：",step);
                return this.seekPath(startNode,closestNode);
            }
            
            this._openlist.sort(this.sortNode);
            
            this._currentNode = this._openlist.shift();

            if(closestNode == null)
            {
                closestNode = this._currentNode;
            }else
            {
                if(this._currentNode.h < closestNode.h)
                {
                    closestNode = this._currentNode;
                }
            }
            
            if(this._currentNode == this._targetNode)
            {
                console.log("找到目标计算步骤为：",step);
                return this.getPath();
            }else
            {
                this._closelist.push(this._currentNode);
            }
            
        }
        
        return this.seekPath(startNode,closestNode);
    }
    
    /**
     * 对路节点进行排序
     * @param node1 
     * @param node2 
     */
    private sortNode(node1:RoadNode,node2:RoadNode)
    {
        if(node1.f < node2.f)
        {
            return -1;
        }else if(node1.f > node2.f)
        {
            return 1;
        }

        return 0;
    }

    /**
     *获得最终寻路到的所有路点 
        * @return 
        * 
        */		
    private getPath():Array<RoadNode>
    {
        var nodeArr:Array<RoadNode> = [];
        
        var node:RoadNode = this._targetNode;
        
        while(node != this._startNode)
        {
            nodeArr.unshift(node);
            node = node.parent;
        }
        
        nodeArr.unshift(this._startNode);

        if(!this.optimize)
        {
            return nodeArr;
        }
        
        //第一阶段优化： 对横，竖，正斜进行优化
        //把多个节点连在一起的，横向或者斜向的一连串点，除两边的点保留
        for(var i:number = 1 ; i < nodeArr.length - 1 ; i++)
        {
            var preNode:RoadNode = nodeArr[i - 1] as RoadNode;
            var midNode:RoadNode = nodeArr[i] as RoadNode;
            var nextNode:RoadNode = nodeArr[i + 1] as RoadNode;
        
            var bool1:Boolean = midNode.cx == preNode.cx && midNode.cx == nextNode.cx;
            var bool2:Boolean = midNode.cy == preNode.cy && midNode.cy == nextNode.cy;
            var bool3:Boolean = ((midNode.cx - preNode.cx) / (midNode.cy - preNode.cy)) * ((nextNode.cx - midNode.cx) / (nextNode.cy - midNode.cy)) == 1
        
            if(bool1 || bool2 || bool3)
            {
                nodeArr.splice(i,1)
                i--;
            }
        }
        
        //return nodeArr;

        //第二阶段优化：对不在横，竖，正斜的格子进行优化
        for(var i:number = 0 ; i < nodeArr.length - 2 ; i++)
        {
            var startNode:RoadNode = nodeArr[i] as RoadNode;
            var optimizeNode:RoadNode = null;

            //优先从尾部对比，如果能直达就把中间多余的路点删掉
            for(var j:number = nodeArr.length - 1 ; j > i + 1 ; j--)
            {
                var targetNode:RoadNode = nodeArr[j] as RoadNode;

                //在第一阶段优已经优化过横，竖，正斜了，所以再出现是肯定不能优化的，可以忽略
                if(startNode.cx == targetNode.cx || startNode.cy == targetNode.cy || Math.abs(targetNode.cx - startNode.cx) == Math.abs(targetNode.cy - startNode.cy))
                {
                    continue;
                }

                if(this.isArriveBetweenTwoNodes(startNode,targetNode))
                {
                    optimizeNode = targetNode;
                    break;
                }

            }

            if(optimizeNode)
            {
                var optimizeLen:number = j - i - 1;
                nodeArr.splice(i + 1,optimizeLen);
            }
        
        }

        return nodeArr;
    }
    
    /**
     * 两点之间是否可到达
     */
    private isArriveBetweenTwoNodes(startNode:RoadNode,targetNode:RoadNode):boolean
    {
        if(startNode == targetNode)
        {
            return false;
        }

        var disX:number = Math.abs(targetNode.cx - startNode.cx);
        var disY:number = Math.abs(targetNode.cy - startNode.cy);

        var dirX = 0;

        if(targetNode.cx > startNode.cx)
        {
            dirX = 1;
        }else if(targetNode.cx < startNode.cx)
        {
            dirX = -1;
        }

        var dirY = 0;

        if(targetNode.cy > startNode.cy)
        {
            dirY = 1;
        }else if(targetNode.cy < startNode.cy)
        {
            dirY = -1;
        }

        var rx:number = 0;
        var ry:number = 0;
        var intNum:number = 0;
        var decimal:number = 0;

        if(disX > disY)
        {
            var rate:number = disY / disX;

            for(var i = 0 ; i < disX ; i++)
            {
                ry = startNode.cy + i * dirY * rate;
                intNum = Math.floor(ry);
                decimal = ry % 1;

                var cx1:number = startNode.cx + i * dirX;
                var cy1:number = decimal <= 0.5 ? intNum : intNum + 1;


                ry = startNode.cy + (i + 1) * dirY * rate;
                intNum = Math.floor(ry);
                decimal = ry % 1;

                var cx2:number = startNode.cx + (i + 1) * dirX;
                var cy2:number = decimal <= 0.5 ? intNum : intNum + 1;

                var node1:RoadNode = this._roadNodes[cx1 + "_" + cy1] as RoadNode;
                var node2:RoadNode = this._roadNodes[cx2 + "_" + cy2] as RoadNode;

                //cc.log(i + "  :: " + node1.cy," yy ",startNode.cy + i * rate,ry % 1);

                if(!this.isCrossAtAdjacentNodes(node1,node2))
                {
                    return false;
                }
            }

        }else
        {
            var rate:number = disX / disY;

            for(var i = 0 ; i < disY ; i++)
            {
                rx = i * dirX * rate;
                intNum = dirX > 0 ? Math.floor(startNode.cx + rx) : Math.ceil(startNode.cx + rx);
                decimal = Math.abs(rx % 1);

                var cx1:number = decimal <= 0.5 ? intNum : intNum + 1 * dirX;
                var cy1:number = startNode.cy + i * dirY;
                

                rx = (i + 1) * dirX * rate;
                intNum = dirX > 0 ? Math.floor(startNode.cx + rx) : Math.ceil(startNode.cx + rx);
                decimal = Math.abs(rx % 1);

                var cx2:number = decimal <= 0.5 ? intNum : intNum + 1 * dirX;
                var cy2:number = startNode.cy + (i + 1) * dirY;

                var node1:RoadNode = this._roadNodes[cx1 + "_" + cy1] as RoadNode;
                var node2:RoadNode = this._roadNodes[cx2 + "_" + cy2] as RoadNode;

                if(!this.isCrossAtAdjacentNodes(node1,node2))
                {
                    return false;
                }
            }
        }
        

        return true;
    }

    /**
     * 判断两个相邻的点是否可通过
     * @param node1 
     * @param node2 
     */
    private isCrossAtAdjacentNodes(node1:RoadNode,node2:RoadNode):boolean
    {
        if(node1 == node2)
        {
            return false;
        }

        //两个点只要有一个点不能通过就不能通过
        if(!this.isPassNode(node1) || !this.isPassNode(node2))
        {
            return false;
        }

        var dirX = node2.cx - node1.cx;
        var dirY = node2.cy - node1.cy

        //如果不是相邻的两个点 则不能通过
        if(Math.abs(dirX) > 1 || Math.abs(dirY) > 1)
        {
            return false;
        }

        //如果相邻的点是在同一行，或者同一列，则判定为可通过
        if((node1.cx == node2.cx) || (node1.cy == node2.cy))
        {
            return true;
        }

        //只剩对角情况了
        if(
            this.isPassNode(this._roadNodes[node1.cx + "_" + (node1.cy + dirY)]) &&
            this.isPassNode(this._roadNodes[(node1.cx + dirX) + "_" + node1.cy])
        )
        {
            return true;
        }

        return false;
    }

    /**
     * 是否是可通过的点 
     * @param node 
     */
    private isPassNode(node:RoadNode):boolean
    {
        if(!node || node.value == 1)
        {
            return false;
        }

        return true;
    }

    /**
     *测试寻路步骤 
        * @param startNode
        * @param targetNode
        * @return 
        * 
        */		
    public testSeekPathStep(startNode:RoadNode,targetNode:RoadNode,callback:Function,target:any,time:number = 100):void
    {
        this._startNode = startNode;
        this._currentNode = startNode;
        this._targetNode = targetNode;
        
        if(this._targetNode.value == 1)
            return;
        
        this._openlist = [];
        this._closelist = [];
        
        var step:number = 0;
        
        clearInterval(this.handle);
        this.handle = setInterval(()=>
        {
            if(step > this.maxStep)
            {
                console.log("没找到目标计算步骤为：",step);
                clearInterval(this.handle);
                return;
            }
            
            step++;
            
            this.searchRoundNodes(this._currentNode);
            
            if(this._openlist.length == 0)
            {
                console.log("没找到目标计算步骤为：",step);
                clearInterval(this.handle);
                return;
            }
            
            this._openlist.sort(this.sortNode);
            
            this._currentNode = this._openlist.shift();
            
            if(this._currentNode == this._targetNode)
            {
                console.log("找到目标计算步骤为：",step);
                clearInterval(this.handle);
                callback.apply(target,[this._startNode,this._targetNode,this._currentNode,this._openlist,this._closelist,this.getPath()]);
            }else
            {
                this._closelist.push(this._currentNode);
                callback.apply(target,[this._startNode,this._targetNode,this._currentNode,this._openlist,this._closelist,null]);
            }

        },time);
        
    }
    
    /**
     *查找一个节点周围可通过的点 
        * @param node
        * @return 
        * 
        */		
    private searchRoundNodes(node:RoadNode):void
    {
        for(var i:number = 0 ; i < this._round.length ; i++)
        {
            var cx:number = node.cx + this._round[i][0];
            var cy:number = node.cy + this._round[i][1];
            var node2:RoadNode = this._roadNodes[cx + "_" + cy] as RoadNode
            
            if(node2 != null && node2 != this._startNode && node2.value != 1 && !this.isInCloseList(node2) && !this.inInCorner(node2))
            {
                this.setNodeF(node2);
            }
        }
    }
    
    /**
     *设置节点的F值 
        * @param node
        * 
        */		
    public setNodeF(node:RoadNode):void
    {	
        var g:number;
        
        if(node.cx == this._currentNode.cx || node.cy == this._currentNode.cy)
        {
            g = this._currentNode.g + this.COST_STRAIGHT;
        }else
        {
            g = this._currentNode.g + this.COST_DIAGONAL;
        }
        
        if(this.isInOpenList(node))
        {
            if(g < node.g)
            {
                node.g = g;
                
            }else
            {
                return;
            }
        }else
        {
            node.g = g;
            this._openlist.push(node);
        }
        
        node.parent = this._currentNode;
        node.h = (Math.abs(this._targetNode.cx - node.cx) + Math.abs(this._targetNode.cy - node.cy)) * this.COST_STRAIGHT;
        node.f = node.g + node.h;
    }
    
    /**
     *节点是否在开启列表 
        * @param node
        * @return 
        * 
        */		
    private  isInOpenList(node:RoadNode):Boolean
    {
        return this._openlist.indexOf(node) != -1 ? true : false;
    }
    
    /**
     *节点是否在关闭列表 
        * 
        */		
    private  isInCloseList(node:RoadNode):Boolean
    {
        return this._closelist.indexOf(node) != -1 ? true : false;
    }
    
    /**
     *节点是否在拐角处 
        * @return 
        * 
        */		
    private  inInCorner(node:RoadNode):Boolean
    {
        if(node.cx == this._currentNode.cx || node.cy == this._currentNode.cy)
        {
            return false;
        }
        
        var node1:RoadNode = this._roadNodes[this._currentNode.cx + "_" + node.cy] as RoadNode;
        var node2:RoadNode = this._roadNodes[node.cx + "_" + this._currentNode.cy] as RoadNode;
        
        if(this.isPassNode(node1) && this.isPassNode(node2))
        {
            return false;
        }
        
        return true;
    }
    
    public  dispose():void
    {
        this._roadNodes = null;
        this._round = null;
    }
}
