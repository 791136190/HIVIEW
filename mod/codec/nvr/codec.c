#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "inc/gsf.h"
#include "fw/comm/inc/proc.h"
#include "mod/bsp/inc/bsp.h"

#include "codec.h"
#include "cfg.h"
#include "msg_func.h"
#include "mpp.h"
#include "live.h"


GSF_LOG_GLOBAL_INIT("CODEC", 8*1024);


static int req_recv(char *in, int isize, char *out, int *osize, int err)
{
    int ret = 0;
    gsf_msg_t *req = (gsf_msg_t*)in;
    gsf_msg_t *rsp = (gsf_msg_t*)out;

    *rsp  = *req;
    rsp->err  = 0;
    rsp->size = 0;

    ret = msg_func_proc(req, isize, rsp, osize);

    rsp->err = (ret == TRUE)?rsp->err:GSF_ERR_MSG;
    *osize = sizeof(gsf_msg_t)+rsp->size;

    return 0;
}


int gsf_venc_recv(VENC_CHN VeChn, PAYLOAD_TYPE_E PT, VENC_STREAM_S* pstStream, void* uargs)
{
  static int cnt[128];
  if(cnt[VeChn]++ % 100 == 0)
    printf(" VeChn:%d, PT:%d, cnt:%d\n", VeChn, PT, cnt[VeChn]);
  return 0;
}

int venc_start(int start)
{
  int ret = 0, i = 0, j = 0, k = 0;
  
  gsf_mpp_recv_t st = {
    .s32Cnt = 0,  
    .VeChn = {0,},
    .uargs = NULL,
    .cb = gsf_venc_recv,
  };
  
  if(!start)
  {
    printf("stop >>> gsf_mpp_venc_dest()\n");
    gsf_mpp_venc_dest();
  }

  for(i = 0; i < 4; i++)
  for(j = 0; j < GSF_CODEC_VENC_NUM; j++)
  {
    if (j == 1 || j == GSF_CODEC_SNAP_IDX ) // SUB;
    {
      continue; 
    }
    
    gsf_mpp_venc_t venc = {
      .VencChn    = i*GSF_CODEC_VENC_NUM+j,
      .srcModId   = HI_ID_VPSS,
      .VpssGrp    = i, // grp;
      .VpssChn    = 0, // ch;
      .enPayLoad  = PT_H264,
      .enSize     = PIC_1080P,
      .enRcMode   = SAMPLE_RC_CBR,
      .u32Profile = 0,
      .bRcnRefShareBuf = HI_TRUE,
      .enGopMode  = VENC_GOPMODE_NORMALP,
      .u32FrameRate = 30,
      .u32Gop       = 30,
      .u32BitRate   = 4000,
      };
    
    if(!start)
    {
      ret = gsf_mpp_venc_stop(&venc);
      printf("stop >>> ch:%d, st:%d, width:%d, ret:%d\n", i, j, 1920, ret);
    }
    else
    {
      ret = gsf_mpp_venc_start(&venc);
      printf("start >>> ch:%d, st:%d, width:%d, ret:%d\n", i, j, 1920, ret);
    }
    
    if(!start)
      continue;  
    
    if(j < 2) // st_num+1(JPEG);
    {
      st.VeChn[st.s32Cnt] = venc.VencChn;
      st.s32Cnt++;
    }
  }
  
  if(!start)
    return 0;

  // recv start;
  printf("start >>> gsf_mpp_venc_recv s32Cnt:%d, cb:%p\n", st.s32Cnt, st.cb);
  gsf_mpp_venc_recv(&st);
  return 0;
}





int main(int argc, char *argv[])
{
    gsf_bsp_def_t bsp_def;  
    
    #if 1
      
    if(argc < 2)
    {
      printf("pls input: %s codec_parm.json\n", argv[0]);
      return -1;
    }
    if(json_parm_load(argv[1], &codec_nvr) < 0)
    {
      json_parm_save(argv[1], &codec_nvr);
      json_parm_load(argv[1], &codec_nvr);
    }
    
    while(1)
    {
      //register To;
      GSF_MSG_DEF(gsf_mod_reg_t, reg, 8*1024);
      reg->mid = GSF_MOD_ID_CODEC;
      strcpy(reg->uri, GSF_IPC_CODEC);
      int ret = GSF_MSG_SENDTO(GSF_ID_MOD_CLI, 0, SET, GSF_CLI_REGISTER, sizeof(gsf_mod_reg_t), GSF_IPC_BSP, 2000);
      printf("GSF_CLI_REGISTER To:%s, ret:%d, size:%d\n", GSF_IPC_BSP, ret, __rsize);
      
      static int cnt = 3;
      if(ret == 0)
        break;
      if(cnt-- < 0)
        return -1;
      sleep(1);
    }
    if(1)
    {
      //get bsp_def;
      GSF_MSG_DEF(gsf_msg_t, msg, 4*1024);
      int ret = GSF_MSG_SENDTO(GSF_ID_BSP_DEF, 0, GET, 0, 0, GSF_IPC_BSP, 2000);
      gsf_bsp_def_t *cfg = (gsf_bsp_def_t*)__pmsg->data;
      printf("GET GSF_ID_BSP_DEF To:%s, ret:%d, model:%s\n", GSF_IPC_BSP, ret, cfg->board.model);
    
      if(ret < 0)
        return 0;
        
      bsp_def = *cfg;
    }
    
    #endif
    
    char home_path[256] = {0};
    proc_absolute_path(home_path);
    sprintf(home_path, "%s/../", home_path);
    printf("home_path:[%s]\n", home_path);
    gsf_mpp_cfg(home_path, NULL);
    
    //gsf_mpp_vo_start(VODEV_HD0, VO_INTF_VGA|VO_INTF_HDMI, VO_OUTPUT_1280x1024_60, 0);
    gsf_mpp_vo_start(VODEV_HD0, VO_INTF_HDMI, VO_OUTPUT_1080P60, 0);
    gsf_mpp_fb_start(VOFB_GUI, VO_OUTPUT_1080P60, 0);
    
    live_mon();
    
    //test vdec => vpss => venc;
    venc_start(1);
    
    
    GSF_LOG_CONN(0, 100);
    void* rep = nm_rep_listen(GSF_IPC_CODEC, NM_REP_MAX_WORKERS, NM_REP_OSIZE_MAX, req_recv);
    
    while(1)
    {
      sleep(1);
    }
    
    gsf_mpp_fb_hide(VOFB_GUI, 1);
    gsf_mpp_vo_layout(VOLAYER_HD0, VO_LAYOUT_NONE, NULL);
    gsf_mpp_vo_stop(VODEV_HD0);
    
    GSF_LOG_DISCONN();
    return 0;
}