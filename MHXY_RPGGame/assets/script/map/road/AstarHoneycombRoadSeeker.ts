import RoadNode from "./RoadNode";
import IRoadSeeker from "./IRoadSeeker";

/**
 * 六边形 A*寻路算法 
 * @author 落日故人 QQ 583051842
 * 
 */
export default class AstarHoneycombRoadSeeker implements IRoadSeeker {

    /**
		 * 横向移动一个格子的代价
		 */		
		private COST_STRAIGHT:number = 10; 
		
		/**
		 * 斜向移动一个格子的代价
		 */		
		private COST_DIAGONAL:number = 10;
		
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
		 *用于检索一个节点周围6个点的向量数组 格子列数为偶数时使用
		 */		
		private _round1:number[][] = [[0,-1],[1,-1],[1,0],[0,1],[-1,0],[-1,-1]];
		/**
		 *用于检索一个节点周围6个点的向量数组 格子列数为奇数时使用
		 */
		private _round2:number[][] = [[0,-1],[1,0],[1,1],[0,1],[-1,1],[-1,0]];
		
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
		/*private getPath():Array<RoadNode>
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

			//把多个节点连在一起的，横向或者斜向的一连串点，除两边的点保留
			
			var preNode:RoadNode;
			var midNode:RoadNode;
			var nextNode:RoadNode;
			
			for(var i:number = 1 ; i < nodeArr.length - 1 ; i++)
			{
				preNode = nodeArr[i - 1] as RoadNode;
				midNode = nodeArr[i] as RoadNode;
				nextNode = nodeArr[i + 1] as RoadNode;
				
				var bool:Boolean = false;
				
				var otherNode:RoadNode = null;
				
				if(Math.abs(nextNode.cx - preNode.cx) == 2 && preNode.cy == nextNode.cy)
				{
					if(midNode.cx % 2 == 0)
					{
						if(midNode.cy == preNode.cy)
						{
							otherNode = this._roadNodes[midNode.cx + "_" + (midNode.cy + 1)] as RoadNode
						}else
						{
							otherNode = this._roadNodes[midNode.cx + "_" + (midNode.cy - 1)] as RoadNode
						}
						
					}else
					{
						if(midNode.cy == preNode.cy)
						{
							otherNode = this._roadNodes[midNode.cx + "_" + (midNode.cy - 1)] as RoadNode
						}else
						{
							otherNode = this._roadNodes[midNode.cx + "_" + (midNode.cy + 1)] as RoadNode
						}
					}
				}
				
				if(otherNode)
					bool = otherNode.value != 1 ? true : false;
				
				if(bool)
				{
					nodeArr.splice(i,1)
					i--;
				}
			}
			
			for(i = 1 ; i < nodeArr.length - 1 ; i++)
			{
				preNode = nodeArr[i - 1] as RoadNode;
				midNode = nodeArr[i] as RoadNode;
				nextNode = nodeArr[i + 1] as RoadNode;
				
				var bool1:Boolean = midNode.cx == preNode.cx && midNode.cx == nextNode.cx;
				
				var bool2:Boolean = (midNode.cy == preNode.cy && midNode.cy == nextNode.cy) && ((preNode.cx % 2 == midNode.cx % 2 && midNode.cx % 2 == nextNode.cx % 2) );
				
				var bool3:Boolean = preNode.cy - Math.floor(preNode.cx / 2) == midNode.cy - Math.floor(midNode.cx / 2) && midNode.cy - Math.floor(midNode.cx / 2) == nextNode.cy - Math.floor(nextNode.cx / 2);
				
				var bool4:Boolean = preNode.cy + Math.ceil(preNode.cx / 2) == midNode.cy + Math.ceil(midNode.cx / 2) && midNode.cy + Math.ceil(midNode.cx / 2) == nextNode.cy + Math.ceil(nextNode.cx / 2);
				
				if(bool1 || bool2 || bool3 || bool4)
				{
					nodeArr.splice(i,1)
					i--;
				}
			}
			
			return nodeArr
		}*/


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

			//把多个节点连在一起的，横向或者斜向的一连串点，除两边的点保留
			
			var preNode:RoadNode;
			var midNode:RoadNode;
			var nextNode:RoadNode;

			var preHpos:HoneyPoint;
			var midHpos:HoneyPoint;
			var nextHpos:HoneyPoint;

			//var hround:number[][] = [[-1,-1],[-1,0],[0,1],[1,1],[1,0],[0,-1]];
			//var hround2:number[][] = [[-2,-1],[-1,1],[1,2],[2,1],[1,-1],[-1,-2]];
			
			//第一阶段优化： 对横，竖，正斜进行优化
			for(i = 1 ; i < nodeArr.length - 1 ; i++)
			{
				preNode = nodeArr[i - 1] as RoadNode;
				midNode = nodeArr[i] as RoadNode;
				nextNode = nodeArr[i + 1] as RoadNode;

				preHpos = this.getHoneyPoint(preNode);
				midHpos = this.getHoneyPoint(midNode);
				nextHpos = this.getHoneyPoint(nextNode);
				
				var bool1:Boolean = midNode.cx == preNode.cx && midNode.cx == nextNode.cx;
				
				var bool2:Boolean = (midNode.cy == preNode.cy && midNode.cy == nextNode.cy) && ((preNode.cx % 2 == midNode.cx % 2 && midNode.cx % 2 == nextNode.cx % 2) );
				
				var bool3:Boolean = preHpos.hx == midHpos.hx  && midHpos.hx == nextHpos.hx;
				
				var bool4:Boolean = preHpos.hy == midHpos.hy  && midHpos.hy == nextHpos.hy;
				
				if(bool1 || bool2 || bool3 || bool4)
				{
					nodeArr.splice(i,1)
					i--;
				}
			}
			
			//第二阶段优化：对不在横，竖，正斜的格子进行优化
			for(var i:number = 0 ; i < nodeArr.length - 2 ; i++)
			{
				var startNode:RoadNode = nodeArr[i] as RoadNode;
				var optimizeNode:RoadNode = null;
	
				//优先从尾部对比，如果能直达就把中间多余的路点删掉
				for(var j:number = nodeArr.length - 1 ; j > i + 1 ; j--)
				{
					var targetNode:RoadNode = nodeArr[j] as RoadNode;
	
					if(this.isArriveBetweenTwoNodes(this.getHoneyPoint(startNode),this.getHoneyPoint(targetNode)))
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

			return nodeArr
		}

		/**
		 * 两点之间是否可到达
		 */
		private isArriveBetweenTwoNodes(startPoint:HoneyPoint,targetPoint:HoneyPoint):boolean
		{
			if(startPoint.hx == targetPoint.hx && startPoint.hy == targetPoint.hy)
			{
				return false;
			}

			var disX:number = Math.abs(targetPoint.hx - startPoint.hx);
			var disY:number = Math.abs(targetPoint.hy - startPoint.hy);

			var dirX = 0;

			if(targetPoint.hx > startPoint.hx)
			{
				dirX = 1;
			}else if(targetPoint.hx < startPoint.hx)
			{
				dirX = -1;
			}

			var dirY = 0;

			if(targetPoint.hy > startPoint.hy)
			{
				dirY = 1;
			}else if(targetPoint.hy < startPoint.hy)
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

				for(var i = 0 ; i < disX ; i += 2)
				{
					ry = i * dirY * rate;
					intNum = dirY > 0 ? Math.floor(startPoint.hy + ry) : Math.ceil(startPoint.hy + ry);
					decimal = Math.abs(ry % 1);

                    var hpoint1:HoneyPoint = new HoneyPoint();
                    hpoint1.hx = startPoint.hx + i * dirX;
                    hpoint1.hy = decimal <= 0.5 ? intNum : intNum + 1 * dirY;

                    //cc.log(i + "  :: " ,hpoint1.hx, hpoint1.hy," yy ",startPoint.hy + i * dirY * rate,ry % 1,rate,intNum,decimal,dirY,ry);

					ry = (i + 1) * dirY * rate;
					intNum = dirY > 0 ? Math.floor(startPoint.hy + ry) : Math.ceil(startPoint.hy + ry);
					decimal = Math.abs(ry % 1);

					var hpoint2:HoneyPoint = new HoneyPoint();
					hpoint2.hx = startPoint.hx + (i + 1) * dirX;
                    hpoint2.hy = decimal <= 0.5 ? intNum : intNum + 1 * dirY;
                    
                    ry = (i + 2) * dirY * rate;
					intNum = dirY > 0 ? Math.floor(startPoint.hy + ry) : Math.ceil(startPoint.hy + ry);
					decimal = Math.abs(ry % 1);

					var hpoint3:HoneyPoint = new HoneyPoint();
					hpoint3.hx = startPoint.hx + (i + 2) * dirX;
					hpoint3.hy = decimal <= 0.5 ? intNum : intNum + 1 * dirY;

					if(!this.isCrossAtAdjacentNodes(startPoint,targetPoint,hpoint1,hpoint2,hpoint3))
					{
						return false;
                    }

				}

			}else
			{
				var rate:number = disX / disY;
	
				for(var i = 0 ; i < disY ; i += 2)
				{
					rx = i * dirX * rate;
					intNum = dirX > 0 ? Math.floor(startPoint.hx + rx) : Math.ceil(startPoint.hx + rx);
					decimal = Math.abs(rx % 1);
	
					var hpoint1:HoneyPoint = new HoneyPoint();
					hpoint1.hx = decimal <= 0.5 ? intNum : intNum + 1 * dirX;
					hpoint1.hy = startPoint.hy + i * dirY;

					rx = (i + 1) * dirX * rate;
					intNum = dirX > 0 ? Math.floor(startPoint.hx + rx) : Math.ceil(startPoint.hx + rx);
					decimal = Math.abs(rx % 1);
	
					var hpoint2:HoneyPoint = new HoneyPoint();
					hpoint2.hx = decimal <= 0.5 ? intNum : intNum + 1 * dirX;
                    hpoint2.hy = startPoint.hy + (i + 1) * dirY;
                    
                    rx = (i + 2) * dirX * rate;
					intNum = dirX > 0 ? Math.floor(startPoint.hx + rx) : Math.ceil(startPoint.hx + rx);
					decimal = Math.abs(rx % 1);
	
					var hpoint3:HoneyPoint = new HoneyPoint();
					hpoint3.hx = decimal <= 0.5 ? intNum : intNum + 1 * dirX;
					hpoint3.hy = startPoint.hy + (i + 2) * dirY;

					if(!this.isCrossAtAdjacentNodes(startPoint,targetPoint,hpoint1,hpoint2,hpoint3))
					{
						return false;
                    }
				}
			}

			return true;
		}

		/**
     * 判断三个相邻的点是否可通过
     * @param node1 
     * @param node2 
     */
    private isCrossAtAdjacentNodes(startPoint:HoneyPoint,targetPoint:HoneyPoint,hpoint1:HoneyPoint,hpoint2:HoneyPoint,hpoint3:HoneyPoint):boolean
    {
		var node1:RoadNode = this.getNodeByHoneyPoint(hpoint1.hx,hpoint1.hy);
        var node2:RoadNode = this.getNodeByHoneyPoint(hpoint2.hx,hpoint2.hy);
        var node3:RoadNode = this.getNodeByHoneyPoint(hpoint3.hx,hpoint3.hy); //节点3主要用做路径方向的判断

        if(node1 == node2)
        {
            return false;
        }

        //前两个点只要有一个点不能通过就不能通过，节点3只做方向向导，不用考虑是否可通过和是否存在
        if(!this.isPassNode(node1) || !this.isPassNode(node2))
        {
            return false;
        }

        var dirX1:number = hpoint1.hx - hpoint2.hx;
        var dirY1:number = hpoint1.hy - hpoint2.hy;

        var dirX2:number = hpoint3.hx - hpoint2.hx;
		var dirY2:number = hpoint3.hy - hpoint2.hy;
		
		//hround:number[][] = [[-1,-1],[-1,0],[0,1],[1,1],[1,0],[0,-1]]; //相邻点向量 
		//[-1,1] [1,-1] //特殊相邻点向量

        //如果不是相邻的两个点 则不能通过
        if((Math.abs(dirX1) > 1 || Math.abs(dirY1) > 1) || (Math.abs(dirX2) > 1 || Math.abs(dirY2) > 1))
        {
            return false;
        }

        //特殊相邻点 特殊对待
        if(dirX1 == -dirY1) //如果第一个点和第二个点是特殊相邻点
        {
            if(dirX1 == -1)
            {
                if(!this.isPassNode(this.getNodeByHoneyPoint(hpoint2.hx -1,hpoint2.hy)) || !this.isPassNode(this.getNodeByHoneyPoint(hpoint2.hx,hpoint2.hy + 1)))
                {
                    return false;
                }
            }else
            {
                if(!this.isPassNode(this.getNodeByHoneyPoint(hpoint2.hx + 1,hpoint2.hy)) || !this.isPassNode(this.getNodeByHoneyPoint(hpoint2.hx,hpoint2.hy - 1)))
                {
                    return false;
                }
            }
		}
		
		//第一个点和第二个点已经可通过，如果第二个点是终点，那么可直达
		if(hpoint2.hx == targetPoint.hx && hpoint2.hy == targetPoint.hy) 
        {
            return true;
        }

		//特殊相邻点 特殊对待
        if(dirX2 == -dirY2) //如果第二个点和第三个点是特殊相邻点 
        {
            if(dirX2 == -1)
            {
                if(!this.isPassNode(this.getNodeByHoneyPoint(hpoint2.hx -1,hpoint2.hy)) || !this.isPassNode(this.getNodeByHoneyPoint(hpoint2.hx,hpoint2.hy + 1)))
                {
                    return false;
                }
            }else
            {
                if(!this.isPassNode(this.getNodeByHoneyPoint(hpoint2.hx + 1,hpoint2.hy)) || !this.isPassNode(this.getNodeByHoneyPoint(hpoint2.hx,hpoint2.hy - 1)))
                {
                    return false;
                }
            }
        }

        //如果相邻的点和目标点在同一直线
        if(hpoint1.hx == hpoint2.hx  && hpoint2.hx == hpoint3.hx)
        {
            return true;
        }
        
        //var hround2:number[][] = [[-2,-1],[-1,1],[1,2],[2,1],[1,-1],[-1,-2]];

        if(this.isPassNode(this.getNodeByHoneyPoint(hpoint2.hx + (dirX1 + dirX2),hpoint2.hy + (dirY1 + dirY2))))
        {
 
            return true;
        }

        return false;

    }

		/**
		 * 获得六边形格子坐标（以正斜角和反斜角为标准的坐标）
		 * @param node 
		 */
		public getHoneyPoint(node:RoadNode):HoneyPoint
		{
			var hx:number = node.cy + Math.ceil(node.cx / 2); //设置反斜角为x坐标
			var hy:number = node.cy - Math.floor(node.cx / 2); //设置正斜角为y坐标
			
			return new HoneyPoint(hx,hy);
		}

		public getNodeByHoneyPoint(hx:number,hy:number):RoadNode
		{
			var cx:number = hx - hy; //研究出来的
			var cy:number = Math.floor((hx - hy) / 2) + hy; //研究出来的

			return this._roadNodes[cx + "_" + cy] as RoadNode;
		}

		/**
		 * 获得一个点周围指定方向相邻的一个点
		 * @param node 制定的点
		 * @param roundIndex 0是下，然后顺时针，5右下
		 */
		public getRoundNodeByIndex(node:RoadNode,roundIndex:number):RoadNode
		{
			if(!node)
			{
				return null;
			}

			roundIndex = roundIndex % 6;

			var round:number[][];
			
			node.cx % 2 == 0 ? round = this._round1 : round = this._round2;
			var cx:number = node.cx + round[roundIndex][0];
			var cy:number = node.cy + round[roundIndex][1];

			return this._roadNodes[cx + "_" + cy] as RoadNode;
			
		}
		
		/**
		 * 获得一个点周围所有的相邻点
		 * @param node 
		 */
		public getRoundNodes(node:RoadNode):RoadNode[]
		{
			var round:number[][];
			
			node.cx % 2 == 0 ? round = this._round1 : round = this._round2;
			
			var nodeArr:RoadNode[] = [];

			for(var i:number = 0 ; i < round.length ; i++)
			{
				var cx:number = node.cx + round[i][0];
				var cy:number = node.cy + round[i][1];

				var node2:RoadNode = this._roadNodes[cx + "_" + cy] as RoadNode;

				nodeArr.push(node2);
			}

			return nodeArr;
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
			this.handle = setInterval(()=>{

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
			var round:number[][];
			
			node.cx % 2 == 0 ? round = this._round1 : round = this._round2;
			
			for(var i:number = 0 ; i < round.length ; i++)
			{
				var cx:number = node.cx + round[i][0];
				var cy:number = node.cy + round[i][1];
				var node2:RoadNode = this._roadNodes[cx + "_" + cy] as RoadNode;
				
				if(node2 != null && node2 != this._startNode && node2.value != 1 && !this.isInCloseList(node2))
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
		private isInOpenList(node:RoadNode):Boolean
		{
			return this._openlist.indexOf(node) != -1 ? true : false;
		}
		
		/**
		 *节点是否在关闭列表 
		 * 
		 */		
		private isInCloseList(node:RoadNode):Boolean
		{
			return this._closelist.indexOf(node) != -1 ? true : false;
		}
		
		public dispose():void
		{
			this._roadNodes = null;
			this._round1 = null;
			this._round2 = null;
		}
}

/**
 * 六边形格子坐标（以正斜角和反斜角为标准的坐标）
 */
class HoneyPoint
{
	public hx:number = 0;
	public hy:number = 0;

	constructor(x:number = 0,y:number = 0)
	{
		this.hx = x;
		this.hy = y;
	}
}