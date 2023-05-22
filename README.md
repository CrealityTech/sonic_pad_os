# sonic_pad_os
It is recommended to use Ubuntu18.04 to compile sonic_pad_os, and install the related Package dependency "sudo apt install git gcc gawk flex libc6:i386 libstdc++6:i386 lib32z1 libncurses5 libncurses5-dev python g++ libz-dev libssl-dev make p7zip-full"
1. Download the code: git clone https://github.com/CrealityTech/sonic_pad_os.git
2. Download the dl package, save the downloaded dl package in the sonic_pad_os/dl directory.
   Download link: https://klipper.cxswyjy.com/download/sonic_dl/
3. Enter the root directory of sonic_pad_os and execute the script ./scripts/prepare.sh
4. Compile steps:
  4.1 source build/envsetup.sh
  4.2 lunch 6
  4.3 make -j2 && pack
  4.4 swupdate_pack_swu -ab
5. U flash drive upgrade method: Copy the config.ini and t800-sonic_lcd-ab_1.0.6.48.57.swu files in the sonic_pad_os/out/r818-sonic_lcd/ directory to the root directory of the U flash drive. The firmware version number must be greater than the current version number of the device, otherwise the upgrade box will not pop up.
6. Computer upgrade method: The image path generated after compilation is sonic_pad_os/out/r818-sonic_lcd/t800-sonic_lcd_uart0.img, please refer to the link for the burning tool and upgrade method: https://github.com/CrealityOfficial/Creality_Sonic_Pad_Firmware
