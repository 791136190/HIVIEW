#ifndef __onvif_api_h__
#define __onvif_api_h__

#include <stdint.h>

#define MAX_PRESET_NUM  256
#define ONVIF_PRESET_NAME_FMT  "PtzName_%d"

/*
 * ��̨������ϢID����
 */
typedef enum {
    NVC_PTZ_UP_START = 0x001
  , NVC_PTZ_UP_STOP
  , NVC_PTZ_DOWN_START
  , NVC_PTZ_DOWN_STOP
  , NVC_PTZ_LEFT_START
  , NVC_PTZ_LEFT_STOP
  , NVC_PTZ_RIGHT_START    
  , NVC_PTZ_RIGHT_STOP
  , NVC_PTZ_LEFT_UP_START
  , NVC_PTZ_LEFT_UP_STOP
  , NVC_PTZ_RIGHT_UP_START
  , NVC_PTZ_RIGHT_UP_STOP
  , NVC_PTZ_LEFT_DOWN_START
  , NVC_PTZ_LEFT_DOWN_STOP
  , NVC_PTZ_RIGHT_DOWN_START
  , NVC_PTZ_RITHT_DOWN_STOP
  , NVC_PTZ_PRESET_SET
  , NVC_PTZ_PRESET_CALL
  , NVC_PTZ_PRESET_DEL
  , NVC_PTZ_ZOOM_ADD_START
  , NVC_PTZ_ZOOM_ADD_STOP
  , NVC_PTZ_ZOOM_SUB_START
  , NVC_PTZ_ZOOM_SUB_STOP
  , NVC_PTZ_FOCUS_ADD_START
  , NVC_PTZ_FOCUS_ADD_STOP
  , NVC_PTZ_FOCUS_SUB_START
  , NVC_PTZ_FOCUS_SUB_STOP
  , NVC_PTZ_IRIS_ADD_START
  , NVC_PTZ_IRIS_ADD_STOP
  , NVC_PTZ_IRIS_SUB_START
  , NVC_PTZ_IRIS_SUB_STOP
  , NVC_PTZ_GOTO_ZERO_PAN
  , NVC_PTZ_FLIP_180
  , NVC_PTZ_SET_PATTERN_START
  , NVC_PTZ_SET_PATTERN_STOP
  , NVC_PTZ_RUN_PATTERN
  , NVC_PTZ_SET_AUXILIARY
  , NVC_PTZ_CLEAR_AUXILIARY
  , NVC_PTZ_AUTO_SCAN_START
  , NVC_PTZ_AUTO_SCAN_STOP
  , NVC_PTZ_RANDOM_SCAN_START
  , NVC_PTZ_RANDOM_SCAN_STOP
  , NVC_PTZ_LEFT_OFFSET
  , NVC_PTZ_RIGHT_OFFSET
  , NVC_PTZ_DOWN_OFFSET
  , NVC_PTZ_UP_OFFSET
  , NVC_PTZ_ZOOM_MULTIPLE
  , NVC_PTZ_POINT_CENTER
  , NVC_PTZ_VIEW_CENTER
  , NVC_PTZ_BUTT
}NVC_PTZ_ID_E;


/* probe */
typedef struct nvc_probe_item_s {
    uint8_t  dst_id[32];/* Ŀ���豸ID */
    uint8_t  ip[64];
    uint32_t type;	    /**Device_Type_E 0:Device 1:NetworkVideoTransmitter*/
    uint32_t ch_num;
    uint32_t port;
}nvc_probe_item_t;
typedef int (NVC_PROBER_CB)(nvc_probe_item_t *item, void *user_args);

/*------------------------------------------------------*/

int32_t nvc_init(void);
int32_t nvc_uninit(void);

/*------------------------------------------------------*/

int32_t nvc_probe(int type, int timeout, NVC_PROBER_CB *cb, void *user_args);

/*------------------------------------------------------*/

void *nvc_dev_open(char *url, int timeout /*EVENT_CB*/);
int32_t nvc_dev_close(void *dev);


typedef struct {
    uint8_t    trans;       /* 0: udp, 1: tcp, 3: rtsp 4: http*/
    uint8_t    proto;       /* 0: Unicast, 1: multicast */
    int8_t     url[256];    /* url */
}nvc_media_url_t;

int32_t nvc_media_url_get(void *dev, int ch, int st, nvc_media_url_t* media);

/*
 * ��̨��������
 */
typedef struct {
    uint8_t    cmd;     /* NVC_PTZ_ID_E */
    uint8_t    speed;   /* 1 - 100 */
    uint8_t    res;     /* res */
    uint8_t    val;     /* preset, track ...  */
    uint8_t    name[32];/* preset, track name */
}nvc_ptz_ctl_t;

int32_t nvc_ptz_ctl(void *dev, int ch, int st, nvc_ptz_ctl_t *ctl);

/*
 * Ԥ�õ㶨��
 */
typedef struct {
    uint8_t enable;         //�Ƿ�����
    uint8_t no;             //Ԥ�õ��
    uint8_t res[2];         //����
    uint8_t name[32];       //Ԥ�õ�����
}nvc_preset_t;
 
/*
 * Ԥ�õ㼯��(ÿ��ͨ��һ������)
 */
typedef struct {
    uint8_t         num;                       //�����õ�Ԥ�õ����
    uint8_t         res[3];                    //����
    nvc_preset_t    preset_set[MAX_PRESET_NUM];//Ԥ�õ�
}nvc_preset_param_t;

// ��ѯԤ��λ����,�Ա�������Ѳ�� 
int32_t nvc_ptz_presets_get(void *dev, int ch, int st, nvc_preset_param_t *preset);


typedef struct {
    uint8_t brightness;         //����
    uint8_t contrast;           //�Աȶ�
    uint8_t saturation;         //���Ͷ�
    uint8_t hue;                //ɫ��
    uint8_t sharpness;          //���
    uint8_t effect_mode;        //0:ʹ���Զ���ֵ��1��ʹ��Ĭ��ֵ
    uint8_t res[2];
}nvc_image_attr_t;

int32_t nvc_img_attr_get(void *dev, int ch, int st, nvc_image_attr_t *attr);
int32_t nvc_img_attr_set(void *dev, int ch, int st, nvc_image_attr_t *attr);

int32_t nvc_snap_url_get(void *dev, int ch, int st, char *url);

typedef struct {
    uint32_t year;
    uint32_t mon;
    uint32_t day;
    uint32_t hour;
    uint32_t min;
    uint32_t sec;
}nvc_time_t; // == NVC_time_t;

//Time synchronization to the device
int32_t nvc_systime_set(void *dev, int ch, int st, nvc_time_t *time);

/*------------------------------------------------------*/

/*NVT��������*/
typedef struct nvt_parm_s {
  int ch_num;     //nvt�豸ͨ����
	uint16_t port;  //nvt�����˿�
}nvt_parm_t;

typedef struct user_right_s {
    uint8_t user_name[32];
    uint8_t user_pwd[32];
    int32_t local_right;    //����GUIȨ��
    uint32_t remote_right;  //Զ��Ȩ��
}user_right_t;


/* ��ʼ��NVT */
int32_t nvt_init(nvt_parm_t *parm);
int32_t nvt_uninit(void);

/* ����NVT���� */
int32_t nvt_start(void);
int32_t nvt_stop(void);

/*------------------------------------------------------*/


#endif //__onvif_api_h__


