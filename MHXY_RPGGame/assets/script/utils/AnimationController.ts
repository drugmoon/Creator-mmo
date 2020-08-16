// Learn TypeScript:
//  - [Chinese] http://docs.cocos.com/creator/manual/zh/scripting/typescript.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/typescript.html
// Learn Attribute:
//  - [Chinese] http://docs.cocos.com/creator/manual/zh/scripting/reference/attributes.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/reference/attributes.html
// Learn life-cycle callbacks:
//  - [Chinese] http://docs.cocos.com/creator/manual/zh/scripting/life-cycle-callbacks.html
//  - [English] http://www.cocos2d-x.org/docs/creator/manual/en/scripting/life-cycle-callbacks.html

const {ccclass, property} = cc._decorator;

@ccclass
export default class AnimationController extends cc.Component {

    @property(cc.SpriteFrame)
    public images:cc.SpriteFrame[] = [];

    @property()
    public frameTime:number = 0.1;

    /// <summary>
    /// 播放次数
    /// </summary>
    @property()
    public playTimes:number = 0;

    /**
     * 是否倒播
     */
    @property({displayName:"是否倒播",tooltip:"勾选，倒着播放，不钩，顺序播放"})
    public reverse:boolean = false;

    @property(cc.Boolean)
    public autoPlayOnLoad:boolean = true;

    /// <summary>
    /// 播放完自动销毁
    /// </summary>
    @property({tooltip:"勾选，播放完自动销毁"})
    public autoDestroy:boolean = false;

    public frameNum:number = 0;

    public frameIndex:number = 0;

    public nextFrameIndex:number = 0;


    public running:boolean = true;

    private m_render:cc.Sprite;

    private time:number = 0;

    public completeTimesCallback:Function;

    public completeCallback:Function;

    public frameCallback:Function;

    private currentTimes:number = 0;

    onLoad()
    {
        this.m_render = this.getComponent(cc.Sprite);
    }

	// Use this for initialization
	start () {
        if (this.images.length != 0)
        {
            this.frameNum = this.images.length; 
        }

        //this.time = this.frameTime;
        this.running = this.autoPlayOnLoad;

        if(this.reverse)
        {
            this.frameIndex = this.frameNum - 1;
            this.nextFrameIndex = this.frameNum - 1;
        }

	}
	
	// Update is called once per frame
    update (dt) {
      
        if (!this.running)
            return;
       
        if (this.images.length == 0)
            return;

        this.time -= dt;

        if (this.playTimes != 0 && this.currentTimes == this.playTimes)
        {
            this.running = false;
            return;
        }
            

        if (this.time <= 0)
        {
            this.time = this.frameTime;

            if(!this.reverse)
            {
                this.frameIndex = this.nextFrameIndex % this.frameNum;
                this.nextFrameIndex = this.frameIndex + 1;

                this.m_render.spriteFrame = this.images[this.frameIndex];
                
                if(this.m_render.spriteFrame)
                {
                    var rect:cc.Rect = this.m_render.spriteFrame.getRect();
                    this.node.width = rect.width;
                    this.node.height = rect.height;
                }
                
                if (this.frameCallback != null)
                {
                    this.frameCallback(this.frameIndex);
                }

                if (this.frameIndex == this.frameNum - 1)
                {
                    this.currentTimes++;

                    this.node.emit("completeTimes",this.currentTimes);

                    if(this.completeTimesCallback != null)
                    {
                        this.completeTimesCallback(this.currentTimes);
                    }

                    if (this.playTimes != 0 && this.currentTimes == this.playTimes)
                    {

                        this.node.emit("complete");

                        if (this.completeCallback != null)
                        {
                            this.completeCallback();
                        }

                        if (this.autoDestroy)
                        {
                            this.node.destroy();
                        }
                    }
                }
            }else
            {
                this.frameIndex = (this.nextFrameIndex + this.frameNum) % this.frameNum;
                this.nextFrameIndex = this.frameIndex - 1;

                this.m_render.spriteFrame = this.images[this.frameIndex];
                
                if(this.m_render.spriteFrame)
                {
                    var rect:cc.Rect = this.m_render.spriteFrame.getRect();
                    this.node.width = rect.width;
                    this.node.height = rect.height;
                }
                

                if (this.frameCallback != null)
                {
                    this.frameCallback(this.frameIndex);
                }

                if (this.frameIndex == 0)
                {
                    this.currentTimes++;

                    this.node.emit("completeTimes",this.currentTimes);

                    if(this.completeTimesCallback != null)
                    {
                        this.completeTimesCallback(this.currentTimes);
                    }

                    if (this.playTimes != 0 && this.currentTimes == this.playTimes)
                    {

                        this.node.emit("complete");

                        if (this.completeCallback != null)
                        {
                            this.completeCallback();
                        }

                        if (this.autoDestroy)
                        {
                            this.node.destroy();
                        }
                    }
                }

            }
        }
	}

    /// <summary>
    /// 播放
    /// </summary>
    public play(frameCallback:Function = null,completeCallback:Function = null)
    {
        
        this.frameCallback = frameCallback;
        this.completeCallback = completeCallback;

        this.running = true;
        this.frameIndex = 0;
        this.currentTimes = 0;
        
        this.time = this.frameTime;

        if (this.images.length != 0)
        {
            this.frameNum = this.images.length;

            if(this.reverse)
            {
                this.frameIndex = this.frameNum - 1;
                this.nextFrameIndex = this.frameNum - 1;
            }

        }

        if(!this.m_render)
        {
            this.m_render = this.getComponent(cc.Sprite);
        }

        if (this.m_render)
            this.m_render.spriteFrame = this.images[0];

        
        if(this.m_render.spriteFrame)
        {
            var rect:cc.Rect = this.m_render.spriteFrame.getRect();
            this.node.width = rect.width;
            this.node.height = rect.height;
        }

    }

    public gotoAndPlay(frameIndex:number)
    {
        if(!this.m_render)
        {
            this.m_render = this.getComponent(cc.Sprite);
        }

        this.running = true;
        this.frameIndex = frameIndex;
        this.nextFrameIndex = frameIndex;
        this.currentTimes = 0;
        //this.time = 0;
    }

    /// <summary>
    /// 停止
    /// </summary>
    public stop()
    {
        this.running = false;
    }

    public gotoAndStop(frameIndex:number)
    {
        this.frameIndex = frameIndex;

        if (this.frameIndex < 0)
            this.frameIndex = 0;

        if (this.frameIndex > this.images.length - 1)
            this.frameIndex = this.images.length - 1;

        if(!this.m_render)
        {
            this.m_render = this.getComponent(cc.Sprite);
        }

        this.m_render.spriteFrame = this.images[this.frameIndex];

        if(this.m_render.spriteFrame)
        {
            var rect:cc.Rect = this.m_render.spriteFrame.getRect();
            this.node.width = rect.width;
            this.node.height = rect.height;
        }

        this.running = false;
    }

    public isPlayEnd():boolean
    {
        return this.frameIndex == this.frameNum;
    }


}
