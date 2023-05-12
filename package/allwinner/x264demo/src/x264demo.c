#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"
#include "x264.h"



static void show_help(void)
{
	printf("******************** ffmepgdemo help *********************\n");
        printf("-This program is a h264 encode test .\n");
        printf("-Example:x264demo 640 480 input.yuv output.h264 100\n");
        printf("-argv[1]--width,argv[2]--height, argv[3]--Inputfile, argv[4]--Outputfile, argv[5]--encode_frame_number\n ");
        printf("-Inputfile format must is YUV420 or YUV444\n");
	printf("-Outputfile format is h264\n");
	printf("-There are two ways to run:\n");
        printf("    1.x264demo -help   --show help information.\n");
        printf("    2.x264demo 640 480 input.yuv output.h264 100.\n");
        printf("**********************************************************\n");
}


int main(int argc, char** argv)
{
	int ret;
        int y_size;
        int i,j;
	if ((argc == 2) && (strcmp(argv[1], "-help") == 0)) {
		show_help();
                return 0;
	}else if (argc != 6){
	 	printf("Please enter corret: x264demo -help or Example:x264demo 640 480 input.yuv output.yuv 100\n");
		return -1;
	}
        int csp=X264_CSP_I420;
        int width=atoi(argv[1]),height=atoi(argv[2]);
	FILE* fp_src  = fopen(argv[3], "rb");
        FILE* fp_dst = fopen(argv[4], "wb");
	//if set 0, encode all frame
	int frame_num=atoi(argv[5]);

        int iNal   = 0;
        x264_nal_t* pNals = NULL;
        x264_t* pHandle   = NULL;
        x264_picture_t* pPic_in = (x264_picture_t*)malloc(sizeof(x264_picture_t));
        x264_picture_t* pPic_out = (x264_picture_t*)malloc(sizeof(x264_picture_t));
        x264_param_t* pParam = (x264_param_t*)malloc(sizeof(x264_param_t));
         //Check
        if(fp_src==NULL||fp_dst==NULL){
		printf("Error open files.\n");
                goto error;
        }

        x264_param_default(pParam);
        pParam->i_width   = width;
        pParam->i_height  = height;
        //Param
        pParam->i_log_level  = X264_LOG_DEBUG;
        pParam->i_threads  = X264_SYNC_LOOKAHEAD_AUTO;
        pParam->i_frame_total = 0;
        pParam->i_keyint_max = 10;//IDR frame interval
        pParam->i_bframe  = 0;//Number of B frames between two I frames
        pParam->b_open_gop  = 0;
        pParam->i_bframe_pyramid = 0;
        pParam->rc.i_qp_constant=0;/* 0 to (51 + 6*(x264_bit_depth-8)). 0=lossless */
        pParam->rc.i_qp_max=0;
        pParam->rc.i_qp_min=0;
        pParam->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
        pParam->i_fps_den  = 1;
        pParam->i_fps_num  = 25;//frame rate
        pParam->i_timebase_den = pParam->i_fps_num;
        pParam->i_timebase_num = pParam->i_fps_den;
        pParam->i_csp=csp;

        x264_param_apply_profile(pParam, x264_profile_names[5]);
        pHandle = x264_encoder_open(pParam);
        x264_picture_init(pPic_out);
        x264_picture_alloc(pPic_in, csp, pParam->i_width, pParam->i_height);

        y_size = pParam->i_width * pParam->i_height;
        //detect frame number
        if(frame_num==0){
                  fseek(fp_src,0,SEEK_END);
                  switch(csp){
                  case X264_CSP_I444:frame_num=ftell(fp_src)/(y_size*3);break;
                  case X264_CSP_I420:frame_num=ftell(fp_src)/(y_size*3/2);break;
                  default:printf("Colorspace Not Support.\n");return -1;
                  }
                  fseek(fp_src,0,SEEK_SET);
        }
        //Loop to Encode
        for( i=0;i<frame_num;i++){
                  switch(csp){
                  case X264_CSP_I444:{
                           fread(pPic_in->img.plane[0],y_size,1,fp_src);//Y
                           fread(pPic_in->img.plane[1],y_size,1,fp_src);//U
                           fread(pPic_in->img.plane[2],y_size,1,fp_src);//V
                           break;}
                  case X264_CSP_I420:{
                           fread(pPic_in->img.plane[0],y_size,1,fp_src);//Y
                           fread(pPic_in->img.plane[1],y_size/4,1,fp_src);//U
                           fread(pPic_in->img.plane[2],y_size/4,1,fp_src);//V
                           break;}
                  default:{
                           printf("Colorspace Not Support.\n");
                           goto error;}
                  }
                  pPic_in->i_pts = i;

                  ret = x264_encoder_encode(pHandle, &pNals, &iNal, pPic_in, pPic_out);
                  if (ret< 0){
                           printf("Error.\n");
                           goto error;
                  }
                  printf("Succeed encode frame: %5d\n",i);

                  for ( j = 0; j < iNal; ++j){
                            fwrite(pNals[j].p_payload, 1, pNals[j].i_payload, fp_dst);
                  }
        }

        i=0;
        //flush encoder
        while(1){
                  ret = x264_encoder_encode(pHandle, &pNals, &iNal, NULL, pPic_out);
                  if(ret==0){
                           break;
                  }
                  printf("Flush 1 frame.\n");
                  for (j = 0; j < iNal; ++j){
                           fwrite(pNals[j].p_payload, 1, pNals[j].i_payload, fp_dst);
                  }
                  i++;
        }

        x264_picture_clean(pPic_in);
        x264_encoder_close(pHandle);
        pHandle = NULL;

        free(pPic_in);
        free(pPic_out);
        free(pParam);

        fclose(fp_src);
        fclose(fp_dst);

        return 0;
error:
        free(pPic_in);
        free(pPic_out);
        free(pParam);

        fclose(fp_src);
        fclose(fp_dst);

	return -1;
}
