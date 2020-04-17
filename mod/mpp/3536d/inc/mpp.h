#ifndef __mpp_h__
#define __mpp_h__

#include "sample_comm.h"

//sys;
typedef struct {
  int max_vdcnt;
}gsf_mpp_cfg_t;

int gsf_mpp_cfg(char *path, gsf_mpp_cfg_t *cfg);


//vodev;
enum {
  VODEV_HD0 = 0,  // ��Ƶ�豸
};

//������Ƶ����豸
int gsf_mpp_vo_start(int vodev, VO_INTF_TYPE_E type, VO_INTF_SYNC_E sync, int wbc);

//ֹͣ��Ƶ����豸
int gsf_mpp_vo_stop(int vodev);

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
  VOLAYER_BUTT
};

// ��ʾͨ������
typedef enum {
  VO_LAYOUT_NONE  = 0, VO_LAYOUT_10MUX = 10,
  VO_LAYOUT_1MUX  = 1, VO_LAYOUT_12MUX = 12,
  VO_LAYOUT_4MUX  = 4, VO_LAYOUT_16MUX = 16,
  VO_LAYOUT_6MUX  = 6, VO_LAYOUT_25MUX = 25,
  VO_LAYOUT_8MUX  = 8, VO_LAYOUT_36MUX = 36,
  VO_LAYOUT_9MUX  = 9, VO_LAYOUT_64MUX = 64,
  VO_LAYOUT_BUTT
}VO_LAYOUT_E;

//����ͼ�����ʾͨ��;
int gsf_mpp_vo_layout(int volayer, VO_LAYOUT_E layout, RECT_S *rect);
//�ƶ�����ͼ�����ʾ����(λ��,��С)
int gsf_mpp_vo_move(int volayer, RECT_S *rect);
//����ͨ��Դͼ��ü�����(���ھֲ��Ŵ�)
int gsf_mpp_vo_crop(int volayer, int ch, RECT_S *rect);

//vdec;
//��������֡����
typedef struct {
    int size;     // data size;
    int ftype;    // frame type;
    int etype;    // PAYLOAD_TYPE_E;
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
  HI_U32          u32FrameRate;
  HI_U32          u32Gop;
  HI_U32          u32BitRate;
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
