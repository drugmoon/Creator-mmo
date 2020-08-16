/**
 * 地图路节点 
 * @author 落日故人 QQ 583051842
 * 
 */
export class FormationProperty {

    public self:number[][] = [[100,100],[150,150],[200,200],[250,250],[300,300]];
    public self_monster:number[][] = [[0,-1],[1,-1],[1,0],[1,1],[0,1]];
    public foe:number[][] = [[0,-1],[1,-1],[1,0],[1,1],[0,1]];
    public foe_monster:number[][] = [[0,-1],[1,-1],[1,0],[1,1],[0,1]];
    public constructor()
    {
    
    }
    
    
 
}

export default class Formation {

    public FormationPos:FormationProperty[] = []
    
    private static _instance:Formation = null;

    public static get instance():Formation
    {
        if(!this._instance)
        {
            this._instance = new Formation();
            this._instance.init();
        }

        return this._instance;
    }

    start () {

    }

    private init():void
    {
        let forma = new FormationProperty();
        forma.self         = [[705,135],[700,230],[595,160],[750,270],[530,130]];
        forma.self_monster = [[100,100],[150,150],[200,200],[250,250],[300,300]];
        forma.foe          = [[500,100],[150,150],[200,200],[250,250],[300,300]];
        forma.foe_monster  = [[100,100],[150,150],[200,200],[250,250],[300,300]];
       // for(let i = 0; i < 1; i++)
        {
            this.FormationPos[0] = forma;
        }
    }

}
