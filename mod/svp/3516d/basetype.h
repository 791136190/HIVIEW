
#ifdef __cplusplus       // �¼��ϵ�
extern "C" {
#endif


typedef struct RESULT_INFO
{
	int width;
	int heigth;
	int centerx;
	int centery;
	int ID;
}RESULT_INFO;

typedef struct RESULT_BAG
{
	unsigned int obj_num;
	RESULT_INFO  obj[24];
}RESULT_BAG;




#ifdef __cplusplus     // �¼��ϵ�
}
#endif

