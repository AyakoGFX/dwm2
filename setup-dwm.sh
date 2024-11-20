# arch 
sudo pacman -S --needed base-devel extra/git extra/libx11 extra/libxcb extra/libxinerama extra/libxft extra/imlib2

# debian & ubuntu
sudo apt install build-essential git libx11-dev libx11-xcb-dev libxcb-res0-dev libxinerama-dev libxft-dev libimlib2-dev

# OpenSuse
sudo zypper in libX11-devel libXft-devel libXinerama-devel
sudo zypper install --type pattern devel_basis

# void linux
sudo xbps-install -Su base-devel libX11-devel libXft-devel libXinerama-devel freetype-devel fontconfig-devel libxcb-devel imlib2-devel

cp -r .dwm ~/
sudo cp dwm.desktop /usr/share/xsessions/

cd keybind-demon/
./stow.sh
cd ..

cd slstatus/
sudo make clean install
cd ..

sudo make clean install
