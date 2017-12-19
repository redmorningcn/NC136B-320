#ifndef  _Display_h_
#define  _Display_h_

//-------------------------------------------------------------------
//��������:     InitDisplay ()
//��    ��:     7219��ʼ��
//��ڲ���:     ��
//���ڲ���:     ��
//-------------------------------------------------------------------
extern void InitDisplay (void);

//-----------------------------------------------------------------------------------
//��������:               DisplayNum ()
//��    ��:               ������ܽ�����������ʾ����
//��ڲ���:               number
//���ڲ���:               ��
//------------------------------------------------------------------------------------
extern void    DisplayNum (uint32 Unmber);

//-----------------------------------------------------------------------------
//��������:             MovDisplayChar ()
//��    ��:             ���������ʾ�ַ����Ѷ��ַ�
//��ڲ���:             character: �ַ�����
//���ڲ���:             ��
//��ʾ����:             -   E   H   L   P   Black
//���ݴ���:     	    A	B	C	D	E	F	  
//-----------------------------------------------------------------------------
extern void    MovDisplayChar (uint8 Character);

//--------------------------------------------------------------------
//void          DisplayNumWithDoit (uint32 Unmber)
//��    ��:     ������ܽ�����������ʾ����
//��ڲ���:     number
//���ڲ���:     ��
//-------------------------------------------------------------------
extern void    DisplayNumWithDot (uint32 Unmber);

//------------------------------------------------------
//��������:     DisplayString ()
//��    ��:     ���������ʾ�ַ�������
//��ڲ���:     String: �ַ�����
//���ڲ���:     ��
//��ʾ����:     String
//���ݴ���:	    ���ø�ʽ  DisplayString("ABCD");
//-------------------------------------------------------
extern void     DisplayString(int8 *String);

//----------------------------------------------------------------------------
// ��    �ƣ�   uint32 Displayf(char *fmt, va_list param) 
// ��    �ܣ�   
// ��ڲ�����  

// ���ڲ�����   
//----------------------------------------------------------------------------  
extern uint32 Displayf(char *fmt, ...) ;

//----------------------------------------------------------------------------
// ��    �ƣ�   uint32 DisplaySet(char *fmt, va_list param) 
// ��    �ܣ�   
// ��ڲ�����  

// ���ڲ�����   
//----------------------------------------------------------------------------  
extern	uint32 DisplaySet(uint32 Time,uint32 Degree,char *fmt, ... ) ;

//----------------------------------------------------------------------------
// ��    �ƣ�   void HoldDisplay(void) 
// ��    �ܣ�   ��ʾά��	
// ��ڲ�����  

// ���ڲ�����   
//----------------------------------------------------------------------------  
extern	void HoldDisplay(void) ;

//----------------------------------------------------------------------------
// ��    �ƣ�   void TestDisplay(void) 
// ��    �ܣ�   
// ��ڲ�����  

// ���ڲ�����   
//----------------------------------------------------------------------------  
extern	void TestDisplay(void) ;

//----------------------------------------------------------------------------
// ��    �ƣ�   void ClaerDisplaySet(void)
// ��    �ܣ�   
// ��ڲ�����  

// ���ڲ�����   
//----------------------------------------------------------------------------  
extern	void ClaerDisplaySet(void);

//----------------------------------------------------------------------------
// ��    �ƣ�   void ResumeDisplaySet(void)
// ��    �ܣ�   
// ��ڲ�����  

// ���ڲ�����   
//----------------------------------------------------------------------------  
extern	void ResumeDisplaySet(void);

#endif