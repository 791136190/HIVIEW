#ifndef __onvif_api_h__
#define __onvif_api_h__

#include <stdint.h>


/* probe */
typedef struct nvc_probe_item_s {
    uint8_t  dst_id[32];/* Ŀ���豸ID */
    uint8_t  ip[64];
    uint32_t type;	    /**Device_Type_E 0:Device 1:NetworkVideoTransmitter*/
    uint32_t ch_num;
	uint32_t port;
    /* ... */
}nvc_probe_item_t;
typedef int (NVC_PROBER_CB)(nvc_probe_item_t *item, void *user_args);

/* nvc msg hdr */
typedef struct nvc_msg_s {
    uint32_t ch;        /* ͨ����   */
    uint32_t st_ch;     /* ��ͨ���� */
    uint32_t size;      /* �������� */
    uint8_t  data[0];   /* ����     */
}nvc_msg_t;


/*NVT��������*/
typedef struct nvt_parm_s {
	uint16_t port;  //nvt�����˿�
	int (*msg_cb)(void *msg);//�����ص�
}nvt_parm_t;


#define MAX_USER_NAME_LEN       32      //�û�������
#define MAX_PASSWD_LEN          32      //���볤��

typedef struct user_right_s {
    uint8_t user_name[MAX_USER_NAME_LEN];
    uint8_t user_pwd[MAX_PASSWD_LEN];
    int32_t local_right;           //����GUIȨ��
    uint32_t remote_right;          //Զ��Ȩ��
}user_right_t;


#if 0  // for nvr;

#include "sdk_global.h"
#include "sdk_struct.h"
#include "ptzlib.h"


#define ONVIF_PRESET_NAME_FMT  "PtzName_%d"
#define MAX_URL_LEN 256

/* video enc */

typedef sdk_encode_t nvc_enc_parm_t;

/* profile */
typedef struct nvc_profile_enc_s {
    uint8_t resolution; //�ֱ���       SDK_VIDEO_RESOLUTION_E
    uint8_t frame_rate; //����֡��     1-25(PAL)/30
    uint8_t video_enc;  //��Ƶ�����ʽ SDK_VIDEO_ENCODE_FORMAT_E
    uint8_t audio_enc;  //���ӱ����ʽ SDK_AUDIO_CODEC_FORMAT_E
}nvc_profile_enc_t;

/* net parm */
typedef sdk_remote_net_t nvc_net_parm_t;

/* image attr */
typedef sdk_image_attr_t nvc_image_attr_t;

/*------------------------------------------------------*/

int32_t nvc_init(void);
int32_t nvc_uninit(void);

/*------------------------------------------------------*/

int32_t nvc_probe(int type, int timeout, NVC_PROBER_CB *cb, void *user_args);

int32_t nvc_net_parm_set(nvc_net_parm_t *net_parm, char *name, char *passd);    //nvc_net_parm_t
int32_t nvc_net_parm_get(nvc_net_parm_t *net_parm, char *name, char *passwd);    //nvc_net_parm_t

/*------------------------------------------------------*/

void *nvc_dev_open(char *ip, uint16_t port, char *user, char *pass, int timeout /*EVENT_CB*/);
int32_t nvc_dev_close(void *dev);

int nvc_dev_profile_get(void *dev, nvc_msg_t *pmsg);    //nvc_profile_enc_t

int32_t nvc_media_url_get(void *dev, nvc_msg_t *pmsg);  //sdk_media_url_t
int32_t nvc_enc_parm_get(void *dev, nvc_msg_t *pmsg);   //nvc_enc_parm_t
int32_t nvc_enc_parm_set(void *dev, nvc_msg_t *pmsg);   //nvc_enc_parm_t
int32_t nvc_ptz_ctl(void *dev, nvc_msg_t *pmsg);        //sdk_ptz_ctl_t
int32_t nvc_ptz_presets_get(void *dev, nvc_msg_t *pmsg);//sdk_preset_param_t ��ѯԤ��λ����,�Ա�������Ѳ�� 

int32_t nvc_img_attr_get(void *dev, nvc_msg_t *pmsg);  //nvc_image_attr_t
int32_t nvc_img_attr_set(void *dev, nvc_msg_t *pmsg);  //nvc_image_attr_t

int32_t nvc_snap_url_get(void *dev, nvc_msg_t *pmsg);
int32_t nvc_systime_set(void *dev, nvc_msg_t *pmsg, sdk_time_t *time_cfg); //Time synchronization to the device

/*------------------------------------------------------*/



/* ��ʼ��NVT */
int32_t nvt_init(nvt_parm_t *parm);
int32_t nvt_uninit(void);

/* ����NVT���� */
int32_t nvt_start(void);
int32_t nvt_stop(void);

/*------------------------------------------------------*/

#endif

#endif //__onvif_api_h__


