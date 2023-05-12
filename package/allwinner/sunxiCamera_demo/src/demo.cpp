#include <iostream>
#include <stdio.h>
#include "convert.h"
#include "sunxi_cam.h"

using namespace std;
using namespace sunxi_cam;

#define REAR_VIDEO_INDEX 0
#define REAR_VIDEO_FORMAT V4L2_PIX_FMT_NV21
#define REAR_VIDEO_WIDTH 2048
#define REAR_VIDEO_HEIGHT 1152
#define REAR_VIDEO_FPS 60

#define BEFORE_VIDEO_INDEX 2
#define BEFORE_VIDEO_FORMAT V4L2_PIX_FMT_NV21
#define BEFORE_VIDEO_WIDTH 1920
#define BEFORE_VIDEO_HEIGHT 1080
#define BEFORE_VIDEO_FPS 60

#define MAX_FILE_NUM 50

void *capture(void *handle)
{
	SunxiCam *camera = (SunxiCam *)handle;
	bool result = false;
	int count = 0;
	ImageFrame image_frame;
	char filename[128] = {0};

	while(1){
		result = camera->GetImageFrame(&image_frame);
		if(false == result){
			//cout << "get image frame error" << endl;
			continue;
		}

		count++;
		memset(filename, 0, sizeof(filename));
		sprintf(filename, "/tmp/%scamera_%d.bmp", (camera->camera->video_index == 0) ? "rear" : "before", count);

		YUVToBMP(filename, image_frame.data, NV21ToRGB24,
					(camera->camera->video_index == 0) ? REAR_VIDEO_WIDTH : BEFORE_VIDEO_WIDTH,
					(camera->camera->video_index == 0) ? REAR_VIDEO_HEIGHT : BEFORE_VIDEO_HEIGHT);

		result = camera->ReturnImageFrame(&image_frame);
		if(false == result){
			cout << "return image frame error" << endl;
			break;
		}

		if(count >= MAX_FILE_NUM)
			break;
	}
}

int main(int argc, char** argv)
{
    SunxiCam camera_rear, camera_before;
	ImageFrame image_rear, image_before;
	pthread_t pthread_rear, pthread_before;
	bool result = false;

	/* must open rear camera first */
	result = camera_rear.OpenCamera(REAR_VIDEO_INDEX,
									REAR_VIDEO_FORMAT,
									REAR_VIDEO_FPS,
									REAR_VIDEO_WIDTH, REAR_VIDEO_HEIGHT);
	if(false == result){
		cout << "open rear camera error" << endl;
		return -1;
	}

	result = camera_before.OpenCamera(BEFORE_VIDEO_INDEX,
										BEFORE_VIDEO_FORMAT,
										BEFORE_VIDEO_FPS,
										BEFORE_VIDEO_WIDTH, BEFORE_VIDEO_HEIGHT);
	if(false == result){
		cout << "open before camera error" << endl;
		camera_rear.CloseCamera();
		return -1;
	}

	pthread_create(&pthread_before, NULL, capture, &camera_before);
	pthread_create(&pthread_rear, NULL, capture, &camera_rear);

	pthread_join(pthread_before, NULL);
	pthread_join(pthread_rear, NULL);

	/* must close before camera first */
	result = camera_before.CloseCamera();
	if(false == result)
		cout << "close before camera error" << endl;

	result = camera_rear.CloseCamera();
	if(false == result)
		cout << "close rear camera error" << endl;

	return 0;
}
