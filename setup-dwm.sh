# mkdir -p ~/.temp/
# git clone https://github.com/noctuid/tdrop.git ~/.temp/tdrop
# cd ~/.temp/tdrop
# sudo make install

sudo zypper in libX11-devel libXft-devel libXinerama-devel
cp -r .dwm ~/
sudo cp dwm.desktop /usr/share/xsessions/
cd slstatus/
sudo make clean install
cd ..
sudo make clean install
