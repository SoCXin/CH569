


#ifndef __FAT_PROCESS_H__
#define __FAT_PROCESS_H__


#define		MAX_LEN_MCU			32

typedef struct				//����λ����
{
	UINT8 	Name[ 13 ];				// ���ļ��� 8+3��ʽ(�Ѿ�ת����)
	UINT8	attrib;					// ���ļ������ļ���0--�ļ���1--�ļ���
	UINT32	FileLen;				// �ļ����ȣ��ļ���Ϊ0
	UINT16	UpdateDate;				// �޸�����
	UINT16	UpdateTime;				// �޸�ʱ��
	UINT16	CreateDate;				// ��������
	UINT16  CreateTime;				// ����ʱ��
	UINT16  LongNameLen;			// ��ų��ļ��� ��Ч���ȵ���: RecCmd.len - 25
} disk_2;


//#pragma pack(1)
typedef  union
	{
  	unsigned char	buf[ MAX_LEN_MCU];	// �������
	disk_2  FileDef;
} DiskData_1;
DiskData_1  DiskData;


extern  UINT8  Fat_Init( void );
extern  unsigned char Enum_Dir( void );
extern  unsigned char Manual_Send_Dir( void );
extern  UINT8 path_name[255],back_path_name[255];


#endif
