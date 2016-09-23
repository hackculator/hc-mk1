#!/bin/sh
echo "this may take a while"

mv display-rotate.py /bin/display-rotate
chmod +x /bin/display-rotate

mv bat-gpio.py /bin/bat-gpio
chmod +x /bin/bat-gpio

match='fi'
insert='bat-gpio'
file='/etc/rc.local'
sed -i "s/$match/$match\n$insert/" $file

cd ~/ 
apt-get update
apt-get install -y florence emacs octave xrdp tightvncserver arduino

