import RoadNode from "./RoadNode";

/**
 * 寻路接口
 * @author 落日故人 QQ 583051842
 * 
 */
export default interface IRoadSeeker {

    /**
     *寻路入口方法 
        * @param startNode
        * @param targetNode
        * @return 
        * 
        */	
    seekPath(startNode:RoadNode,targetNode:RoadNode):Array<RoadNode>;

    /**
     *寻路入口方法 如果没有寻到目标，则返回离目标最近的路径
    * @param startNode
    * @param targetNode
    * @return 
    * 
    */
    seekPath2(startNode:RoadNode,targetNode:RoadNode,maxStep:number):Array<RoadNode>;

    /**
     *测试寻路步骤 
        * @param startNode
        * @param targetNode
        * @return 
        * 
        */
    testSeekPathStep(startNode:RoadNode,targetNode:RoadNode,callback:Function,target:any,time:number):void;
}
