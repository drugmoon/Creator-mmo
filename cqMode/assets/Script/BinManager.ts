import ByteArray  from './ByteArray'
import {FrameData, FrameInfo}  from './FrameData'

const {ccclass, property} = cc._decorator;

@ccclass
export default class BinManager extends cc.Component {
    private static instance: BinManager = null;
    public static getInstance(): BinManager {
        this.instance = this.instance || new BinManager()
        return this.instance
    }
    mFrameMapList = []

    loadBiz(group, path){
        var abc:any[][] = [];
        abc[1] = [1,2,3]
        abc[2] = [4,6,7]
        abc[1][2]
        var ba = new ByteArray()
        ba.loadData("resources/biz/cloth.biz", () => {

            var file_num = ba.readInt();
            var frame_id=0,file_id=0;
            var i,j,k,frame_num=0;
            var file_width,file_height,dir_count,frame_count;
            var fx,fy,fw,fh,ox,oy,sw,sh,ro;
            var change:cc.Vec2;

            cc.log("file_num  " + file_num)
            for(i=0;i<file_num;i++)
            {
                file_id = ba.readInt();
                
                if(file_id <= 0)continue;
                
                file_width = ba.readShort();
                file_height = ba.readShort();
                frame_num = ba.readShort();	//��֡��
                dir_count = ba.readShort();	//��������
                frame_count = ba.readShort(); //ÿ������֡��
                    
                var wfd = new FrameData();
                wfd.mFileID = file_id;
                wfd.mDirCount = dir_count;
                wfd.mFrameCount = frame_count;
        
                //wfd.mFrameData// = new FrameInfo();//wfd.mDirCount)
                for(k=0;k<wfd.mDirCount;k++)
                {
                    //wfd.mFrameData[k] = new FrameInfo()[];//wfd.mFrameCount
                    var list = []
                    for(j=0;j<wfd.mFrameCount;j++)
                    {
                        //wfd.mFrameData[k] = [];
                        list[j] =  new FrameInfo()
                    }

                    wfd.mFrameData[k] = list
                }
                //change = getOffVec(group,file_id);
                for(j = 0;j < frame_num; j++)
                {
                    frame_id=ba.readInt();
                    fx=ba.readShort();//Сͼ�ڴ�ͼ��x����
                    fy=ba.readShort();//Сͼ�ڴ�ͼ��y����
                    fw=ba.readShort();//Сͼ�ڴ�ͼ��ռ�п���
                    fh=ba.readShort();//Сͼ�ڴ�ͼ��ռ�и߶�
                    ox=ba.readShort();// x����ƫ����
                    oy=ba.readShort();// y����ƫ����
                    sw=ba.readShort();// ԭͼwidth��С
                    sh=ba.readShort();//ԭͼheight��С
                    ro=ba.readShort();
        
                    var frame = new FrameInfo();
                    frame.id=frame_id;
                    // var difx:number = ox - 24 + 0.5 * fw + change.x;
                    // var dify:number = -(oy - 16) - 0.5*fh - change.y;
                    frame.frame = cc.rect(fx,fy,fw,fh);
                    //frame.offset = new cc.Vec2(difx, dify);
                    frame.sourceSize = cc.size(sw,sh);
                    frame.rotated = ro > 0 ? true : false;
        
                    wfd.mFrameData[0][1]=frame;

                    var a = parseInt( (frame_id/10000).toString())
                    var b = frame_id%10000;
                    cc.log(a + " - " + b)
                    wfd.mFrameData[a][b]=frame;
                    //wfd.mFrameData[frame_id/10000][frame_id%10000]=frame;
                }

                if(this.mFrameMapList[group].find(file_id) == this.mFrameMapList[group].end())
                {
                    this.mFrameMapList[group][file_id] = wfd;
                }
                else
                {
                   // delete wfd;
                }
            } 
        })

 
    
        // ByteArray* ba=ByteArray::buildC((char*)data.getBytes(),data.getSize());
    

    
        // delete ba;
    
        // if(group==AVATAR_CLOTH)
        // {
        //     loadNoDir();
        //     loadSimpleFile();
        //     loadActionInfo();
        // }
        // else if(group==AVATAR_WEAPON)
        // {
        //     loadDoubleIdle();
        //     loadDoublePrepare();
        // }
        // else if(group==AVATAR_EFFECT)
        // {
        //     loadEffectInfo();
        //     loadSoundInfo();
        // }
    }

    start () {

    }

    // update (dt) {}
}
