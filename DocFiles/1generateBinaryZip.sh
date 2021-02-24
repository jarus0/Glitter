mkdir jarusBin
cp build/*.bin jarusBin
cp build/bootloader/*.bin jarusBin

tar -czvf binary.tar.gz jarusBin
rm -rf jarusBin
