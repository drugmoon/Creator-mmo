

export class RoleAttributeVo
{
    /** 角色 */
    public _ID:number=0;
    /** 名称 */
    public _Name:string='';
    /** 类型，1.无限制：初始座驾，多地方使用  2.有限制：只能一个挂机点使用*/
    public _Type:number=0;
    /** 技能 1.谷类，2.肉类，3.蔬菜，4.调料 */
    public _Skill:number=0;
    /** 掉落道具的值 */
    public _Value:number=0;
    /** 角色模型 */
    public _PlayerID:number=0;

    public _Hp:number=100;
    public _Mp:number=100;
    public _Exp:number=0;
    /** 来源描述 */
    public _Description:string='';
    public constructor(){

    }
}
