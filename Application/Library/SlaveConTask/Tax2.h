#ifndef  _TAX2_h_
#define  _TAX2_h_

#define			NO_TAX_COM_ERR			91				//无TAX2信息
#define			TAX_BEF_REC_ERR			92				//38部分不能正常接收
#define			TAX_BAK_REC_ERR			93				//39部分不能正常接收
#define			TAX_TIME_RULE_ERR		94				//TAX箱时间不合法
#define			TAX_INFO_NO_CHG_ERR		95				//TAX箱内容不更新
#define			TAX_TIME_REASON_ERR		96				//TAX箱时间信息不合理	

#define			TAX_ROAD_NUM_CHG_EVT	91				//TAX箱交路号改变事件	

//----------------------------------------------------------------------------
// 名    称：   stcTAX2Full GetTAX2Info( void )
// 功    能：   获取TAX2信息 供外部访问
// 入口参数：  
// 出口参数：   
//----------------------------------------------------------------------------
extern	uint8 GetTAX2Info(stcTAX2Full * sTAX2Full);

//-------------------------------------------------------------------------------------------------
//函数名称:             void  ShowTAX2Info(stcTAX2Full sTAX) 
//函数功能:             打印TAX2时间,公里标信息
//入口参数:             无
//出口参数:             无
//说明:                 
//--------------------------------------------------------------------------------------------------
extern	void  ShowTAX2Info(stcTAX2Full sTAX) ;

//------------------------------------------------------------------------
// 名    称：   void OpenTax2(void) 
// 功    能：   打开Tax接收功能
// 入口参数：   
// 出口参数：   
//------------------------------------------------------------------------
extern	void OpenTax2(void) ;

//----------------------------------------------------------------------------
// 名    称：   void  RecTAX2Info(void)
// 功    能：   接收TAX2信息。
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
extern	void  RecTAX2Info(void);

//-------------------------------------------------------------------------------------------------
//函数名称:             void  TestTax2(void) 
//函数功能:             TAX2箱通信信息测试
//入口参数:             无
//出口参数:             无
//说明:                 
//--------------------------------------------------------------------------------------------------
extern	uint8  TestTax2(uint8 *ErrFlgTmp);

//-------------------------------------------------------------------------------------------------
//函数名称:             uint8  JudgeTAX2Error(stcTAX2Full sTAX) 
//函数功能:             对TAX2箱信息通信正确性可靠性进行判断
//入口参数:             TAX2信息
//出口参数:             时间合理,返回TRUE
//--------------------------------------------------------------------------------------------------
extern	uint8  JudgeTAX2Error(void) ;

//-------------------------------------------------------------------------------------------------
//函数名称:             void  TestTax2(void) 
//函数功能:             TAX2箱通信信息测试
//入口参数:             无
//出口参数:             无
//说明:                 
//--------------------------------------------------------------------------------------------------
extern	void  Tax2Task(void);

//-------------------------------------------------------------------------------------------------
//函数名称:            	uint8  JudgeTAX2Evt(void)  
//函数功能:             对TAX2箱信息通信中的事件判断
//入口参数:             
//出口参数:             
//--------------------------------------------------------------------------------------------------
extern	uint8  JudgeTAX2Evt(void);

//------------------------------------------------------------------------
// 名    称：   void OpenTax2(void) 
// 功    能：   打开Tax接收功能
// 入口参数：   
// 出口参数：   
//------------------------------------------------------------------------
extern	void CloseTax2(void) ;

//-------------------------------------------------------------------------------------------------
//函数名称:             uint8	GetNoSpeedFlg(void)
//函数功能:             取速度为零的标识
//入口参数:             无
//出口参数:             无
//说明:                 
//--------------------------------------------------------------------------------------------------
extern	uint8	GetNoSpeedFlg(void);

//----------------------------------------------------------------------------
// 名    称：   void  TestTAX2Info(void)
// 功    能：   测试TAX2箱信息。
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
extern	void  TestTAX2Info(void);
#endif
