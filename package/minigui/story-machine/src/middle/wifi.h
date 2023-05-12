#ifndef __WIFI_H_
#define __WIFI_H_


struct wifi_info{
	char bssid[64];
	unsigned int frequency;
	char signal_level[3];
	char flags[128];
	char ssid[64];
};
void aw_wifi_open(void);
struct wifi_info *GetWifiInfo(int *total_num);

#endif
