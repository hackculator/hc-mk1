#!/bin/sh
echo "this may take a while"
mv display-rotate.py /bin/display-rotate
chmod +x /bin/display-rotate
cd ~/ 
apt-get update
apt-get install -y florence emacs octave xrdp tightvncserver arduino

