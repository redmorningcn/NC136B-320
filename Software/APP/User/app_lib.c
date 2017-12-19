/*******************************************************************************
 *   Filename:       app_lib.c
 *   Revised:        All copyrights reserved to Wuming Shen.
 *   Date:           2014-07-05
 *   Revision:       v1.0
 *   Writer:	     Wuming Shen.
 *
 *   Description:    双击选中 wdt 按 Ctrl + H, 钩选 Match the case, Replace with
 *                   输入您要的名字，点击 Replace All
 *                   双击选中 Wdt 按 Ctrl + H, 钩选 Match the case, Replace with
 *                   输入您要的名字，点击 Replace All
 *                   双击选中 WDT 按 Ctrl + H, 钩选 Match the case, Replace with
 *                   输入您要的名字，点击 Replace All
 *                   在 app_cfg.h 中指定任务的优先级 和 任务堆栈大小
 *
 *   Notes:
 *					QQ:276193028
 *     				E-mail:shenchangwei945@163.com
 *
 *******************************************************************************/

/*******************************************************************************
 * INCLUDES
 */
#define  SNL_APP_SOURCE
#include <app.h>
#include <app_lib.h>
#include <app_ctrl.h>
#include <os_cfg.h>

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *app_lib__c = "$Id: $";
#endif

#define APP_TEMP_EN     DEF_ENABLED
#if APP_TEMP_EN == DEF_ENABLED
/*******************************************************************************
 * CONSTANTS
 */
#define CYCLE_TIME_TICKS            (OS_TICKS_PER_SEC * 1)

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * TYPEDEFS
 */
     
/*******************************************************************************
 * LOCAL VARIABLES
 */
/***********************************************
* 描述： NTC线性表
*/

/*******************************************************************************
 * GLOBAL VARIABLES
 */
//extern StrCtrlDCB              Ctrl;
//UnionCtrlPara   EepWrBuf,EepRdBuf;

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


/*******************************************************************************
 * 				                    延时函数                                   *
 *******************************************************************************/
/*******************************************************************************
* 名    称： Delay_Nus()
* 功    能： 延时大概1us
* 入口参数： dly		延时参数，值越大，延时越久
* 出口参数： 无
* 作　 　者： 无名沈
* 创建日期： 2009-01-03
* 修    改：
* 修改日期：
*******************************************************************************/
OPTIMIZE_NONE void  Delay_Nus( INT32U  dly )
{
    /***********************************************
    * 描述：定时器初始化
    *       72000000次循环  = 6.9999999583333333333333333333333秒
    *       一次循环 = 6.9999999583333333333333333333333 / 72000000
    *       = 97.222221643518518518518518518519 ns
    *       ~= 0.1 us
    *       延时1us  dly  = 9
    *       dly = 0时，循环一次
    *
    while(dly--);
    *//***********************************************
    * 描述：定时器初始化
    *       1000000次循环  = 1027.7778611111111111111111111111ms
    *       平均一次循环 = 1.02777786111111111111111111111us
    *       dly = 1时，= 1.1111111111111111111111111111111us
    *       dly = 2时  = 2.1388888888888888888888888888889us
    *       dly 每增加1 增加-1.0277777777777777777777777777778us
    */
    INT32U  i;

    while(dly--) {
        for(i=0; i<10; i++);             // clk = 72000000 1027.7778611111111111111111111111
    }
}

/*******************************************************************************
* 名    称： Delay_Nms()
* 功    能： 延时大概1ms
* 入口参数： dly		延时参数，值越大，延时越久
* 出口参数： 无
* 作　 　者： 无名沈
* 创建日期： 2009-01-03
* 修    改：
* 修改日期：
*******************************************************************************/
OPTIMIZE_NONE void  Delay_Nms( INT32U  dly )
{
#if UCOS_II_EN > 0
    OSTimeDly(dly);
#else
    CPU_INT32U  hclk_freq;
    CPU_INT32U  cnts;
    INT32U      i;

    
    //BSP_OS_TimeDlyMs(dly);
    //return;
    hclk_freq   = CSP_PM_CPU_ClkFreqGet();            // hclk_freq = 72000000时dly 1000 接近1秒
    cnts        = hclk_freq / 9000;
    //WdtReset();

    for( ; dly > 0; dly-- ) {
        for(i=0; i<cnts; i++);
    }
#endif
}

/*******************************************************************************
* 名    称：Delay_Ns()
* 功    能：延时大概1s
* 入口参数：dly		延时参数，值越大，延时越久
* 出口参数：无
* 作　 　者：无名沈
* 创建日期：2009-01-03
* 修    改：
* 修改日期：
*******************************************************************************/
OPTIMIZE_NONE void  Delay_Ns( INT32U  dly )
{
#if UCOS_II_EN > 0
    OSTimeDlyHMSM(dly/3600,dly/60%60,dly%3600,0);
#else
    for( ; dly > 0; dly--) {
        //WdtReset();
        Delay_Nms( 950 );
    }
#endif
}

/*******************************************************************************
 * 				                    end of file                                *
 *******************************************************************************/
#endif