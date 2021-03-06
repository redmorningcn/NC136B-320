/*******************************************************************************
*   Filename:       app_comm_otr.c
*   Revised:        All copyrights reserved to redmorningcn.
*   Revision:       v1.0
*   Writer:	     redmorningcn.
*
*   Description:    双击选中 otr 按 Ctrl + H, 钩选 Match the case, Replace with
*                   输入您要的名字，点击 Replace All
*                   双击选中 Otr 按 Ctrl + H, 钩选 Match the case, Replace with
*                   输入您要的名字，点击 Replace All
*                   双击选中 OTR 按 Ctrl + H, 钩选 Match the case, Replace with
*                   输入您要的名字，点击 Replace All
*                   在 app_cfg.h 中指定本任务的 优先级  （ APP_TASK_COMM_PRIO     ）
*                                            和 任务堆栈（ APP_TASK_COMM_STK_SIZE ）大小
*
*   Notes:
*     				E-mail: redmorningcn@qq.com
*
*******************************************************************************/

/*******************************************************************************
* INCLUDES
*/
#define  SNL_APP_SOURCE
#include <includes.h>
#include <app_comm_protocol.h>
#include <app_ctrl.h>
#include <app.h>
#include <os_cfg_app.h>
#include <RecDataTypeDef.h>


#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *app_comm_otr__c = "$Id: $";
#endif

#define APP_TASK_COMM_EN     DEF_ENABLED
#if APP_TASK_COMM_EN == DEF_ENABLED
/*******************************************************************************
* CONSTANTS
*/

/*******************************************************************************
* MACROS
*/
#define CYCLE_TIME_TICKS            (OS_TICKS_PER_SEC * 1)

/*******************************************************************************
* TYPEDEFS
*/
#define		DATA_CARD           0x00000001			//数据卡------暂时未用
#define		CALC_MODEL_CARD     0x00000002			//计算模型卡--暂时未用
#define		IAP_CARD            0x00000004			//IAP升级-----暂时未用

#define		DENSITY_CARD		0x00000020			//密度卡
#define		MODEL_CARD			0x00000040			//模型卡-----暂时未用
#define		TIME_CARD           0x00000100			//时间卡-----
#define		COM_MODEL_CARD	    0x00000100		    //串口模型卡-暂时未用
#define		HIGHT_CARD			0x00000200			//高度卡-----
#define		COPY_CARD           0x00000400			//复制卡-----暂时未用
#define		MODEL_SELECT_CARD	0x00001000			//模型选择卡-
#define		FIX_CARD            0x00002000			//装车卡-----
#define		COPY_FRAM_CARD	    0x00004000			//铁电复制卡-暂时未用
#define		SET_DATA_CARD		0x00000080			//置数卡-----按时未用
#define		DEBUG_CARD			0x00000800			//调试卡-----暂时未用

#define		EXPAND_CARD			0x00008000			//扩展卡-----暂时未用

#define		DATA_CARD_ERR		0x00008000			//IC卡错误

#define		DATA_CARD_DIS		0x00010000			//数据卡指示
#define		DATA_CARD_FIN		0x00020000			//数据卡结束
#define		SYS_RUN_PARA		0x00040000			//系统参数卡--暂未使用
#define		CLEAR_ALL			0x00080000			//清零卡，数据记录清零

#define		RST_SYS				0x00100000		    //复位指令
#define		CALC_PARA           0x00200000		    //计算斜率

/*******************************************************************************
* LOCAL VARIABLES
*/

/*******************************************************************************
* GLOBAL VARIABLES
*/

/***********************************************
* 描述： 软定时器声明
*/

/***********************************************
* 描述： 软定时器回调函数
*/


/*******************************************************************************
* LOCAL FUNCTIONS
*/


/*******************************************************************************
* GLOBAL FUNCTIONS
*/

/*******************************************************************************
* EXTERN VARIABLES
*/

/*******************************************************************************
* EXTERN FUNCTIONS
*/
uint8   FRAM_StoreRecNumMgr(StrRecNumMgr  *SRecNumMgr);
uint8   FRAM_StoreOilPara(StrOilPara  *SOilPara);
void    BSP_DispClrAll(void);
uint8   FRAM_StoreProductInfo(StrProductInfo  *sProductInfo);
void    comm_para_flow(StrDevOtr * sDtu,uint8 addrnum);
uint8 	ReadFM24CL64(uint16  Addr, uint8 *DataBuf, uint32 DataBufLen);            
uint32  GetRecNumAddr(uint32 FlshRecNum);
INT8U	WriteFlsh(INT32U Dst, uint8 * sFlshRec, INT32U NByte);


/*******************************************************************************/
stcModelCard            m_sModelCard;

/*******************************************************************************
* 名    称： DataComReadAsk
* 功    能： IC卡请求数据记录。
			 开始数据记录号和结束数据记录号。
* 入口参数： 
* 出口参数： 无
* 作　  者： redmorningcn.
* 创建日期： 2017-05-31
* 修    改：
* 修改日期：
* 备    注： 任务创建函数需要在app.h文件中声明
*******************************************************************************/
void DataComReadAsk(unsigned int startnum,unsigned int endnum)
{
	Ctrl.Otr.Wr.Code = DATA_CARD;						//数据类型
	Ctrl.Otr.Wr.Info.sReqRec.StartNum 	= startnum;		//开始记录
	Ctrl.Otr.Wr.Info.sReqRec.EndNum	= endnum;		//结束记录
	
	CSNC_SendData(	Ctrl.Otr.pch,						//通讯控制块
                  SLAVE_ADDR_OTR,						//源地址
                  MASTE_ADDR_HOST,					    //目标地址
                  Ctrl.Otr.ConnCtrl[0].SendFramNum,	//发送帧号
                  0,									//帧类型，默认为0，在数据区内明确		
                  (uint8 *)&Ctrl.Otr.Wr,				//发送缓冲区
                  //发送数据区长度
                  sizeof(Ctrl.Otr.Wr.Code)+sizeof(Ctrl.Otr.Wr.Info.sReqRec));
}


/*******************************************************************************
* 名    称： 取机车信息
* 功    能： 
* 入口参数： 
* 出口参数： 无
* 作　  者： redmorningcn.
* 创建日期： 2017-12-06
* 修    改：
* 修改日期：
* 备    注： 任务创建函数需要在app.h文件中声明
*******************************************************************************/
uint8  	JudegLocoInfo(uint8   buf[])
{
    uint16  crc16;
    stcFlshRec	*psRec;						    //数据记录     128 	
    psRec       = (stcFlshRec	*)buf;
    
    crc16 = GetCrc16Check(buf,sizeof(stcFlshRec)-2);
    
    if(crc16 == psRec->CrcCheck){               //校验正确
        //机车信息错误，
        if(     psRec->JcRealType   != 0xffff 
           &&   psRec->JcRealNo     != 0xffff
               &&    psRec->JcRealType  != 0 
                   &&   psRec->JcRealNo     != 0    ){
                       
                       if(psRec->JcRealType   == Ctrl.sProductInfo.sLocoId.Type
                           &&psRec->JcRealNo     == Ctrl.sProductInfo.sLocoId.Num){
                               return 1;                       //机车信息正确
                           }
                       
                       Ctrl.sProductInfo.sLocoId.Type = psRec->JcRealType ; 
                       Ctrl.sProductInfo.sLocoId.Num  = psRec->JcRealNo   ;
                       
                       FRAM_StoreProductInfo((StrProductInfo  *)&Ctrl.sProductInfo);//存机车号
                       return 0;
                   }
    }
    
    return 1;
}

//stcFlshRec  sFlshRecTmp;

/*******************************************************************************
* 名    称： comm_rec_read
* 功    能： 读取数据记录
* 入口参数： 
* 出口参数： 无
* 作　  者： redmorningcn.
* 创建日期： 2017-05-26
* 修    改：
* 修改日期：
* 备    注： 任务创建函数需要在app.h文件中声明
*******************************************************************************/
void	comm_rec_read(StrDevOtr * sDtu,uint8 addrnum)
{
    /***********************************************
    * 描述： 更具数据长度，判读接收的是否为数据记录。
    * 		 接收长度为128，数据记录；长度小于128,接收
    * 		 普通数据。
    */ 
	static	    uint8	    errtimes = 0;
    static     uint8       recsmalltimes = 0;//接收到的记录号小
    static     uint8       recbigtimes = 0; //接收到的记录号大
    static     uint8       rectimes = 0;
	
	//数据记录长度128字节
	if(Ctrl.Otr.RxCtrl.Len == sizeof(Ctrl.Otr.Rd.sRec)){
        
        //if ( Ctrl.sRunPara.DealFlag ) {
        //    return;
        //}
		//存数据记录//如果接收到的数据记录和想要接收的相同，则保存数据。		
		if(Ctrl.Otr.Rd.sRec.StoreCnt == Ctrl.sRecNumMgr.IcRead){
            extern uint8 GetCardStatus(void);
//            
//            if ( 1 == GetCardStatus() ) {
//                BSP_OS_SemPost(&Bsp_Card_Sem);
//                return;
//            } else 
//            if ( 1== Ctrl.sRunPara.plugcard ) {
//                BSP_OS_TimeDly(50);
//            }
            //保存数据记录
            recsmalltimes   = 0;
            recbigtimes     = 0;    
                           
			Ctrl.sRecNumMgr.IcRead++;
            //先应答再存储
		  	uint32	FlshAddr = GetRecNumAddr(Ctrl.Otr.Rd.sRec.StoreCnt); 
            //BSP_LED_Toggle(8);

			WriteFlsh(FlshAddr, (uint8 *)&Ctrl.Otr.Rd.sRec, sizeof(stcFlshRec));
            BSP_LED_Toggle(8);
            osal_start_timerEx( OS_TASK_ID_LED,
                               OS_EVT_LED_TICKS,
                               200);
            //存储器判断
            ReadFlsh(FlshAddr, (uint8 *)&Ctrl.sRec, sizeof(stcFlshRec)); 
            
			//如果还有数据未取完，继续请求数据。
			if(Ctrl.sRecNumMgr.IcRead < Ctrl.sRecNumMgr.Current){	
				errtimes = 0;
				DataComReadAsk(Ctrl.sRecNumMgr.IcRead,Ctrl.sRecNumMgr.IcRead +1);
				
			}else if(Ctrl.sRecNumMgr.IcRead > Ctrl.sRecNumMgr.Current){
				errtimes++;
				if(errtimes > 2)									//流水号异常。
					Ctrl.sRecNumMgr.IcRead = Ctrl.sRecNumMgr.Current;
			}
           
            
//            uint8 storetimes = 3;
//            while((Ctrl.sRec.StoreCnt +1) != Ctrl.sRecNumMgr.IcRead && storetimes-- ) //如果数据存储异常，再次存储
//            {
//                Ctrl.sRec.StoreCnt = Ctrl.sRecNumMgr.IcRead -1;
//                WriteFlsh(FlshAddr, (uint8 *)&Ctrl.Otr.Rd.sRec, sizeof(stcFlshRec));
//            
//                ReadFlsh(FlshAddr, (uint8 *)&Ctrl.sRec, sizeof(stcFlshRec));       //存储器判断
//            }
                
//           //调整未读IC卡值
            if(Ctrl.sRecNumMgr.IcRead != 0xffffffff && Ctrl.sRecNumMgr.CardRead > Ctrl.sRecNumMgr.IcRead){
                Ctrl.sRecNumMgr.CardRead = Ctrl.sRecNumMgr.IcRead;
            }
			FRAM_StoreRecNumMgr((StrRecNumMgr *)&Ctrl.sRecNumMgr);	//存流水号
            
		} else  if(Ctrl.Otr.Rd.sRec.StoreCnt +1 == Ctrl.sRecNumMgr.Current) {//无最新记录，发前条数据，不处理
        
        } else {
        //if(Ctrl.Otr.Rd.sRec.StoreCnt < Ctrl.sRecNumMgr.IcRead ) //流水号比已存的小，认为是新换盒体，重新更改流水号
            recsmalltimes++;
            if(recsmalltimes > 3){                                  //数据异常处理
                recsmalltimes = 0;
                
                if(Ctrl.Otr.Rd.sRec.StoreCnt != 0xffffffff && Ctrl.Otr.Rd.sRec.StoreCnt != 0)
                {     //扇区正常     
                    if(Ctrl.Otr.Rd.sRec.StoreCnt < Ctrl.sRecNumMgr.Current) //数据记录在已存的记录范围内
                    {
                        Ctrl.sRecNumMgr.IcRead     = Ctrl.Otr.Rd.sRec.StoreCnt;               //
                        Ctrl.sRecNumMgr.CardRead   = Ctrl.sRecNumMgr.IcRead ;                 //重新讀卡
                    }else{
                        Ctrl.sRecNumMgr.IcRead++;                              //如果无效，则记录号++，继续执行
                    }
                }             
                else
                {
                    Ctrl.sRecNumMgr.IcRead++;                              //如果无效，则记录号++，继续执行
                }
                
                uint32	FlshAddr = GetRecNumAddr(Ctrl.sRecNumMgr.IcRead);       //先应答再存储
                
                WriteFlsh(FlshAddr, (uint8 *)&Ctrl.Otr.Rd.sRec, sizeof(stcFlshRec));    
                
                DataComReadAsk(Ctrl.sRecNumMgr.IcRead,Ctrl.sRecNumMgr.IcRead +1);
            }
        }
//        else
//        {
//            recbigtimes++;
//            if(Ctrl.sRecNumMgr.IcRead  > 5){       //该数据记录异常，跳过该记录   
//                recbigtimes = 0;
//                Ctrl.sRecNumMgr.IcRead++;
//
//            }
//        }
        
	}
	else if(    Ctrl.Otr.RxCtrl.Len == sizeof(Ctrl.Otr.Rd.sTinyRec)       //V.2.0
            
             ||  Ctrl.Otr.RxCtrl.Len ==  0x13                                //V.1.0
             )	//统计模块发起查询
	{
		if(Ctrl.Otr.Rd.sTinyRec.CurRecNum < Ctrl.sRecNumMgr.IcRead)
		{
			errtimes++;
			if( errtimes > 2 ){										//流水号异常。
				Ctrl.sRecNumMgr.Current = Ctrl.Otr.Rd.sTinyRec.CurRecNum;
				Ctrl.sRecNumMgr.IcRead = Ctrl.sRecNumMgr.Current;
                
				FRAM_StoreRecNumMgr((StrRecNumMgr *)&Ctrl.sRecNumMgr);	//存流水号
			}
		}
		else
		{
			Ctrl.sRecNumMgr.Current = Ctrl.Otr.Rd.sTinyRec.CurRecNum;
			DataComReadAsk(Ctrl.sRecNumMgr.IcRead,Ctrl.sRecNumMgr.IcRead +1);
            
			FRAM_StoreRecNumMgr((StrRecNumMgr *)&Ctrl.sRecNumMgr);	//存流水号
		}
		
	}else{			
        //如果其他数据，应答接受请求
		DataComReadAsk(Ctrl.sRecNumMgr.IcRead,Ctrl.sRecNumMgr.IcRead +1);
	}
}


static     	uint32      modelsendnum = 0;	//模型发送序号。
/*******************************************************************************
* 名    称： ClearModelSendNum
* 功    能： 清模型发送序号。
* 入口参数： 
* 出口参数： 无
* 作　  者： redmorningcn.
* 创建日期： 2017-05-31
* 修    改：
* 修改日期：
* 备    注： 任务创建函数需要在app.h文件中声明
*******************************************************************************/
void	ClearModelSendNum(void)
{
	modelsendnum = 0;
}

/*******************************************************************************
* 名    称： comm_para_flow
* 功    能： 参数流。根据IC卡的类型进行操作
* 入口参数： 
* 出口参数： 无
* 作　  者： redmorningcn.
* 创建日期： 2017-05-26
* 修    改：
* 修改日期：
* 备    注： 任务创建函数需要在app.h文件中声明
*******************************************************************************/
void    comm_para_flow(StrDevOtr * sDtu,uint8 addrnum)
{
    OS_ERR          err;
    uint8       retrytimes = 0;
	uint8		Infolen = 0;
	uint8		*p = (uint8 *)&m_sModelCard;
    
    /***********************************************
    * 描述： 根据IC卡类型进行相应操作。卡类型在插入卡后，赋值
    * 	     如果未插卡，则认为是普通数据卡。
    */ 
    sDtu->Wr.Code	=	Ctrl.sRunPara.CardType;
	
    switch (Ctrl.sRunPara.CardType)  	
    {
        //设置密度
    case    DENSITY_CARD: 
        sDtu->Wr.Info.Density	=	Ctrl.SOilPara.Density;
        Infolen	=	sizeof(Ctrl.SOilPara.Density);
        break;
        
    case    HIGHT_CARD:  
        sDtu->Wr.Info.Hig	=	Ctrl.SOilPara.Hig;
        Infolen	=	sizeof(Ctrl.SOilPara.Hig);            
        break;
        
        //模型选择卡
    case    MODEL_SELECT_CARD:   
        sDtu->Wr.Info.ModelNum	=	Ctrl.SOilPara.ModelNum;
        Infolen	=	sizeof(Ctrl.SOilPara.ModelNum);
        break;
        
        //车型车号
    case    FIX_CARD: 
        sDtu->Wr.Info.sLocoId.Num	=	Ctrl.sProductInfo.sLocoId.Num;
        sDtu->Wr.Info.sLocoId.Type	=	Ctrl.sProductInfo.sLocoId.Type;
        
        Infolen	=	sizeof(	Ctrl.sProductInfo.sLocoId.Num) +
            sizeof( Ctrl.sProductInfo.sLocoId.Type	);
        break;
        
        //读数据指示
    case    DATA_CARD_DIS: 
        
        sDtu->Wr.Info.CardRecNum	=	Ctrl.sRec.StoreCnt;
        
        Infolen	=	sizeof(Ctrl.sRec.StoreCnt);
        
        sDtu->ConnCtrl[0].SendFramNum++;
        
        break;
        
    case    DATA_CARD_ERR: 
        
        sDtu->Wr.Info.Buf[0]	=	Ctrl.sRunPara.CardErrData;
        
        Infolen	=	sizeof(Ctrl.sRunPara.CardErrData);
        
        sDtu->ConnCtrl[0].SendFramNum++;
        
        break;
                
        //读数据指示结束
    case    DATA_CARD_FIN: 
        
        Infolen = 0;
        break; 
        
        /***********************************************
        * 描述： 设置油箱模型。
        *   IC 卡模块插入IC卡后，IC卡主动应答模型。  
        *   后续每200ms发送数据，序号从0开始累加。
        数据发送完成后，序号为0xffffffff。
        */    
    case    MODEL_CARD: {
        modelsendnum         = 0;
        p = (uint8 *)&m_sModelCard;		
        int     sendmodellen  = 0;  //已发送数据长度
        uint16   sendtimes = 0;
        
        OSFlagPendAbort(( OS_FLAG_GRP *)&Ctrl.Os.CommEvtFlagGrp,
                        ( OS_OPT       ) OS_OPT_PEND_ABORT_ALL,
                        ( OS_ERR      *)&err );
        
        while(sizeof(stcModelCard ) > 128 * modelsendnum ) {
            
            if((sizeof(stcModelCard ) - 128 * modelsendnum) > 128 ) {
                if(modelsendnum == 0)
                    p = (uint8 *)&m_sModelCard;		
                //数据序号
                memcpy((uint8 *)&sDtu->Wr.Info.Buf[0],(uint8 *)&modelsendnum,sizeof(modelsendnum));	
                //数据内容
                memcpy((uint8 *)&sDtu->Wr.Info.Buf[4],&p[sendmodellen],128);
                
                
                Infolen = sizeof(modelsendnum)+128;
            } else {		
                //发送结束信号
                modelsendnum = 0xffffffff;
                //数据序号
                memcpy((uint8 *)&sDtu->Wr.Info.Buf[0],(uint8 *)&modelsendnum,sizeof(modelsendnum));	
                //数据内容
                memcpy((uint8 *)&sDtu->Wr.Info.Buf[4],&p[sendmodellen],(sizeof(stcModelCard ) - sendmodellen));
                
                Infolen = sizeof(modelsendnum) + (sizeof(stcModelCard ) - sendmodellen);
                
                sendmodellen = sizeof(stcModelCard ) ;
                //更改卡类型。退出模型卡
                //modelsendnum = 0;
                Ctrl.sRunPara.CardType = DATA_CARD;
            }
            
            
            CSNC_SendData(sDtu->pch,                        //通讯控制块
                          SLAVE_ADDR_OTR,					//源地址
                          MASTE_ADDR_HOST,				    //目标地址
                          sDtu->ConnCtrl[0].SendFramNum,	//发送帧号
                          0,								//帧类型，默认为0，在数据区内明确		
                          (uint8 *)&sDtu->Wr,				//发送缓冲区
                          Infolen+sizeof(sDtu->Wr.Code)	//发送数据区长度
                              );
            
            //数据完成
            if(modelsendnum == 0xffffffff)
                return;            

            OS_FLAGS    flags = 
            OSFlagPend( ( OS_FLAG_GRP *)&Ctrl.Os.CommEvtFlagGrp,
                       ( OS_FLAGS     ) COMM_EVT_FLAG_OTR_RX,
                       ( OS_TICK      ) 250,
                       ( OS_OPT       ) OS_OPT_PEND_FLAG_SET_ANY,
                       ( CPU_TS      *) NULL,
                       ( OS_ERR      *)&err);
            
            
            /***********************************************
            * 描述： 如果串口有应答，则继续发送下一帧
            */
            if ( err == OS_ERR_NONE ) {                
                /***********************************************
                * 描述： 清除标志位
                */
                OSFlagPost( ( OS_FLAG_GRP  *)&Ctrl.Os.CommEvtFlagGrp,
                            ( OS_FLAGS      )COMM_EVT_FLAG_OTR_RX,
                            ( OS_OPT        )OS_OPT_POST_FLAG_CLR,
                            ( OS_ERR       *)&err);
               
                if( Ctrl.Otr.RxCtrl.FramNum == sDtu->ConnCtrl[0].SendFramNum  && 
                    Ctrl.Otr.Rd.modelcontrl.code == MODEL_CARD )  {
                    retrytimes      = 0;
                    modelsendnum++;
                    sendmodellen   += 128;
                } else {
                    retrytimes++;
                    if(retrytimes > 2) {
                        retrytimes      = 0;
                        modelsendnum++;
                        sendmodellen   += 128; 
                   } 
                }
            /***********************************************
            * 描述： 如果串口没有应答，则超时后发送下一帧
            */
            } else  {
                retrytimes++;
                if(retrytimes > 2) {
                    retrytimes      = 0;
                    modelsendnum++;
                    sendmodellen   += 128;

                }
            }
        }
        
        return;
        
    } break;
        /***********************************************
        * 描述： 数据卡(默认状态)。
        *   对
        *   后续每200ms发送数据，序号从0开始累加。
        数据发送完成后，序号为0xffffffff。
        */    		
        //数据卡
    case    DATA_CARD: 
        return;
        break;
        
        //其他类型数据，直接退出
    default:
        return;					   
        
    }
    
	CSNC_SendData(	sDtu->pch,						//通讯控制块
                  SLAVE_ADDR_OTR,					//源地址
                  MASTE_ADDR_HOST,				    //目标地址
                  sDtu->ConnCtrl[0].SendFramNum,	//发送帧号
                  0,								//帧类型，默认为0，在数据区内明确		
                  (uint8 *)&sDtu->Wr,				//发送缓冲区
                  Infolen+sizeof(sDtu->Wr.Code)	//发送数据区长度
                      );
}


/*******************************************************************************
* 名    称： 		GetRecNumAddr
* 功    能：      取数据记录地址。
flash地址;更新记录号等信息。
* 入口参数： 	无
* 出口参数： 	无
* 作　 　者： 	redmornigcn
* 创建日期： 	2017-05-15
* 修    改：
* 修改日期：
*******************************************************************************/
uint32  GetRecNumAddr(uint32 FlshRecNum)
{				
    return  (uint32)(((FlshRecNum * sizeof(stcFlshRec)) % FLSH_MAX_SIZE)); 			
}
/*******************************************************************************
* 				end of file
*******************************************************************************/
#endif


