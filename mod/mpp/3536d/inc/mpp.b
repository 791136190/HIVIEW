#ifndef __mpp_h__
#define __mpp_h__

#include "sample_comm.h"

/*
 * 1, ���벻����֧�ֶ���оƬϵ��, ����3536Xϵ�в�����3531X; 
 * 2, Ϊ�������SDK,��������sample_comm�����еĴ���;(���޸�,������ע��);
 */

//vodev;
enum {
  VODEV_HD0 = 0,  // ��Ƶ�豸
};

//������Ƶ����豸
int gsf_mpp_vo_start(int vodev, VO_INTF_TYPE_E type, VO_INTF_SYNC_E sync);
//ֹͣ��Ƶ����豸
int gsf_mpp_vo_stop(int vodev);
//�޸���Ƶ������÷ֱ���
int gsf_mpp_vo_resolu(int vodev, VO_INTF_SYNC_E sync);
//WBCʹ��
int gsf_mpp_vo_wbc(int vodev, int en);

//vofb;
enum {
  VOFB_GUI   = 0, // GUI Layer
  VOFB_MOUSE = 1, // MOUSE Layer
};
// FB�����
int gsf_mpp_fb_start(int vofb, VO_INTF_SYNC_E sync, int hide);
int gsf_mpp_fb_stop(int vofb);
int gsf_mpp_fb_hide(int vofd);
int gsf_mpp_fb_move(int vofb, int xpos, int ypos);
int gsf_mpp_fb_draw(int vofb, void *data /* ARGB1555 */, int w, int h);


//volayer;
enum {
  VOLAYER_HD0 = 0,  // ��Ƶ��
  VOLAYER_PIP = 1,  // PIP��
};

// ��ʾͨ������
typedef enum {
  VO_SPLIT_NONE  = 0, VO_SPLIT_10MUX = 10,
  VO_SPLIT_1MUX  = 1, VO_SPLIT_12MUX = 12,
  VO_SPLIT_4MUX  = 4, VO_SPLIT_16MUX = 16,
  VO_SPLIT_6MUX  = 6, VO_SPLIT_25MUX = 25,
  VO_SPLIT_8MUX  = 8, VO_SPLIT_36MUX = 36,
  VO_SPLIT_9MUX  = 9, VO_SPLIT_64MUX = 64,
  VO_SPLIT_BUTT
}VO_SPLIT_E;

//����ͼ�����ʾͨ��
//����VPSS/VOͨ��, split=VO_SPLIT_NONEʱ,����������ʾͨ��;
//��vdch!=NULLʱ,��ָ������ͨ����(���ڶ��VPSS��һ��VDEC�ĳ���,�续�л���ͬһ����ƵԴ)
int gsf_mpp_vo_split(int volayer, VO_SPLIT_E split, RECT_S *rect, int vdch[VO_SPLIT_BUTT]);
//�ƶ�����ͼ�����ʾ����(λ��,��С)
int gsf_mpp_vo_move(int volayer, RECT_S *rect);
//����ͨ��Դͼ��ü�����(���ھֲ��Ŵ�)
int gsf_mpp_vo_crop(int volayer, int ch, RECT_S *rect);

//vdec;
//��������֡����
typedef struct {
    int size;     // data size;
    int ftype;    // frame type;
    int etype;    // encode type;
    int width;    // width;
    int height;   // height;
    int au_chs;   // audio channels;
    unsigned long long pts; // timestamp;
}gsf_mpp_frm_attr_t;

//������Ƶ���ݵ���ʾͨ��(����VDECͨ��)
int gsf_mpp_vo_vsend(int volayer, int ch, char *data, gsf_mpp_frm_attr_t *attr);

//output format: YUV420
int gsf_mpp_vo_getyuv(int volayer, int ch, char *buf, int size, int *w, int *h);

//����״̬;
typedef struct {
    int left_byte;    // vdec
    int left_frame;   // vdec
    int left_pics;    // vdec
    int bufused;      // vo
}gsf_mpp_vo_stat_t;

//��ȡ������ʾ״̬
int gsf_mpp_vo_stat(int volayer, int ch, gsf_mpp_vo_stat_t *stat);
//���ý�����ʾFPS
int gsf_mpp_vo_setfps(int volayer, int ch, int fps);
//���������ʾBUFF
int gsf_mpp_vo_clear(int volayer, int ch);

//adec;
//���õ�ǰ��Ƶ����ͨ����(!=ch�����ݲ��ͽ���)
int gsf_mpp_vo_afilter(int vodev, int ch);
//������Ƶ���ݽ���(ch��ʾ��Ƶ���ݵ�ͨ����, ch!=gsf_mpp_vo_afilter(ch)ʱ,ֱ�ӷ���)
int gsf_mpp_vo_asend(int vodev, int ch, char *data, gsf_mpp_frm_attr_t *attr);

//venc;
//�������
typedef struct {
  VENC_CHN        VencChn;  // vech;
  VPSS_GRP        VpssGrp;  // -1: VO(WBC), >= 0: VPGRP;
  VPSS_CHN        VpssChn;  
  PAYLOAD_TYPE_E  enPayLoad;
  PIC_SIZE_E      enSize;
  HI_S32          enRcMode;
  HI_U32          u32Profile;
  HI_BOOL         bRcnRefShareBuf;
  VENC_GOP_MODE_E enGopMode;
}gsf_mpp_venc_t;

//��������ͨ��
int gsf_mpp_venc_start(gsf_mpp_venc_t *venc);
//ֹͣ����ͨ��
int gsf_mpp_venc_stop(gsf_mpp_venc_t *venc);

enum {
  GSF_MPP_VENC_CTL_IDR = 0,
};
int gsf_mpp_venc_ctl(int VencChn, int id, void *args);

typedef struct {
  HI_S32 s32Cnt;                    //��ӵ������̵߳�ͨ���Ÿ���;
  VENC_CHN VeChn[VENC_MAX_CHN_NUM]; //��ӵ������̵߳�ͨ���ż���;
  void *uargs;
  int (*cb)(VENC_CHN VeChn, PAYLOAD_TYPE_E PT, VENC_STREAM_S* pstStream, void* uargs);
}gsf_mpp_recv_t;
//���������߳�
int gsf_mpp_venc_recv(gsf_mpp_recv_t *recv);
//ֹͣ�����߳�
int gsf_mpp_venc_dest();


//private for mpp;

/////////////////;


#endif //__mpp_h__
