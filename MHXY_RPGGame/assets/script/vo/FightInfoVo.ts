
export enum AttackType {
 
    Att_None,		//pt
    Att_Casting,	//施法
    Att_A,	        //合击
}

export class FightInfoVo
{
    /** 角色 */
    public _ID:number=0;
    /** 名称 */
    public _Name:string='';
    /** 攻击类，0.普通攻击  1.施法  2.合击*/
    public _Type:number=0;
    /** 技能 1.攻击技能 */
    public _Skill:number=0;
    /** 掉落道具的值 */
    public _Value:number=0;
    /** 攻击者id */
    public _ID_Att:number=0;
    /** 防御者id */
    public _ID_Def:number=0;
    
    public _Idx_Att:number=0;
    public _Idx_Def:number=0;

    public _Hp_att:number=0;//攻击人 是否掉血
    public _Hp_def:number=0;//防御 是否掉血
    public _Mp_att:number=0;//攻击人 是否掉蓝
    public _Mp_def:number=0;//防御 是否掉蓝
    /*负面效果*/

    /** 来源描述 */
    public _Description:string='';
    public constructor(){

    }
}
