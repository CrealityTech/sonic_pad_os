#include <wifi_intf.h>
#include <wmg_debug.h>
#include <wifi_udhcpc.h>
#include "wifi.h"

static struct wifi_info *wifi_info_list;

static void wifi_state_handle(struct Manager *w, int event_label)
{
    wmg_printf(MSG_DEBUG,"event_label 0x%x\n", event_label);

    switch(w->StaEvt.state)
    {
                 case CONNECTING:
                 {
                         wmg_printf(MSG_INFO,"Connecting to the network......\n");
                         break;
                 }
                 case CONNECTED:
                 {
                         wmg_printf(MSG_INFO,"Connected to the AP\n");
                         start_udhcpc();
                         break;
                 }

                 case OBTAINING_IP:
                 {
                         wmg_printf(MSG_INFO,"Getting ip address......\n");
                         break;
                 }

                 case NETWORK_CONNECTED:
                 {
                         wmg_printf(MSG_DEBUG,"Successful network connection\n");
                         break;
                 }
                case DISCONNECTED:
                {
                    wmg_printf(MSG_ERROR,"Disconnected,the reason:%s\n",wmg_event_txt(w->StaEvt.event));
                    break;
                }
    }
}

const aw_wifi_interface_t *p_wifi_interface = NULL;
int event_label = 0;

void aw_wifi_open(void)
{
    event_label = rand();
    p_wifi_interface = aw_wifi_on(wifi_state_handle, event_label);
    if(p_wifi_interface == NULL){
        wmg_printf(MSG_ERROR,"wifi on failed\n");
        return -1;
    }
}
static int WifiScan(char *scan_results)
{
    int ret = 0, len = 0, i = 0;
	int num = 0;

	if(p_wifi_interface == NULL){
        wmg_printf(MSG_ERROR,"wifi don't open\n");
        return -1;
	}

	event_label++;

    wmg_printf(MSG_INFO,"\n*********************************\n");
    wmg_printf(MSG_INFO,"***Start scan!***\n");
    wmg_printf(MSG_INFO,"*********************************\n");

	len = 4096;
	ret = p_wifi_interface->get_scan_results(scan_results, &len);
	wmg_printf(MSG_DEBUG,"ret of get_scan_results is %d\n", ret);

	if(ret == 0)
	{
	   // wmg_printf(MSG_INFO,"%s\n",scan_results);
	   // wmg_printf(MSG_INFO,"******************************\n");
	   // wmg_printf(MSG_INFO,"Wifi get_scan_results: Success!\n");
	   // wmg_printf(MSG_INFO,"******************************\n");

		for(int i = 0; i < 4096; i++){
			if(scan_results[i] == '\n'){
				num++;
			}
		}
	    return num - 1;
	}else{
	    wmg_printf(MSG_ERROR,"Get_scan_results failed!\n");
	    return -1; //flag for tinatest
	}

}


struct wifi_info *GetWifiInfo(int *total_num)
{
	char result[256] = {'\0'};
	char scan_results[4096] = {0};
	int count = 0;
	int wifiNum = 0;
	int count1 = 0;
	int num = 0;

	wifiNum = WifiScan(scan_results);

	if(wifiNum < 1){
	    return NULL;
	}

	if(wifi_info_list != NULL){
		free(wifi_info_list);
	}

	*total_num = wifiNum;
	wifi_info_list = (struct wifi_info *)malloc(sizeof(struct wifi_info) * wifiNum);
	memset(wifi_info_list, 0, sizeof(struct wifi_info) * wifiNum);
	wifiNum = 0;

	for(int i = 0; i < sizeof(scan_results); i++){
		if(scan_results[i] == 0 && count == 0){
			break;
		}
		if(scan_results[i] != '\n'){
			result[count] = scan_results[i];
			count++;
			continue;
		}

		count = 0;

		if(wifiNum >= 1){
			for(int i = 0; i < strlen(result); i++){

				if(result[i] == '\0'){
					break;
				}

				if(result[i] == 0x09 ){
					num++;
					count1 = 0;
				}

				switch(num){
					case 0:
						wifi_info_list[wifiNum - 1].bssid[count1] = result[i];
						break;
					case 1:
						break;
					case 2:
						wifi_info_list[wifiNum - 1].signal_level[count1] = result[i];
						break;
					case 3:
						wifi_info_list[wifiNum - 1].flags[count1] = result[i];
						break;
					case 4:

						wifi_info_list[wifiNum - 1].ssid[count1] = result[i];
						break;
					default:
						break;
				}
				count1++;
			}
		}

		wifiNum++;
		num = 0;
		memset(result, '\0', sizeof(result));
	}
	return wifi_info_list;
}
