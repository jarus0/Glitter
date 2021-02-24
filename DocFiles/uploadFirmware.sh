which esptool
whereis esptool
echo "Press Ctrl+T then Q to close Serial monitor"
echo "Please enter esptool.py Location"


read jesptoolLocation

$jesptoolLocation erase_flash

mkdir binaryFiles
tar -xvf binary.tar.gz -C ./binaryFiles

cd binaryFiles/
jbootloader=jarusBin/bootloader.bin
jenterpriseBin=jarusBin/internal_communication.bin
jpartitions=jarusBin/partitions_two_ota.bin


$jesptoolLocation --chip esp32 --port /dev/ttyUSB0 --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 0x1000 $jbootloader 0x10000 $jenterpriseBin 0x8000 $jpartitions

cd ..
rm -rf ./binaryFiles
tio /dev/ttyUSB0 -b115200
