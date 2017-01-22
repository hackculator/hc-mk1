#!/bin/python3

#this script will rotate your display and touch panel
#tested on Raspbian Jessie with Pixel on 10/23/2016 on a Raspberry Pi 3
#with the Raspberry Pi Touch Display and python 3.4.2

from subprocess import STDOUT, check_call
import os

def make_executable(path): #This function adds execution permissions to a file
    mode = os.stat(path).st_mode
    mode |= (mode & 0o444) >> 2
    os.chmod(path, mode)

def delete_words(words, path): #This functions deletes all file lines containig a word from the list words
    document = open(path, "r")     
    lines = document.readlines()
    document.close()
    document = open(path, "w")
    flag=False
    for line in lines:
        if line[0] != "#":
            line_copy=line
            splitline = line.split()
            for word in splitline:
                if word not in words:
                    flag=True
                else:
                    flag=False
                    break
            if flag:
                document.write("\n" + line_copy + "\n")
                flag=False
        else:
            document.write(line)
            
    document.close()

def rotate_touch(position): #This function rotates the touch panel coordinates
    xinput_script = open("/usr/local/bin/touch-rotate.sh", "w") #first it writes an script with the commands
    xinput_script.write("#!/bin/bash\n")
    xinput_script.write("xinput set-prop 'FT5406 memory based driver' 'Evdev Axes Swap' 1\n")
    xinput_script.write("xinput --set-prop 'FT5406 memory based driver' 'Evdev Axis Inversion' " + position + "\n")
    xinput_script.close()
    delete_words(["@/usr/local/bin/touch-rotate.sh"],"/home/pi/.config/lxsession/LXDE-pi/autostart" ) #it deletes a possible old code line with same information
    autostart = open("/home/pi/.config/lxsession/LXDE-pi/autostart", "a")
    autostart.write("@/usr/local/bin/touch-rotate.sh") #add the route to the script
    autostart.close()
    make_executable("/usr/local/bin/touch-rotate.sh") #make it executable


print("You are going to rotate the screen and the touch panel 0º, 90º, 180º or 270º\nWe are preparing your Raspberry, please wait")

check_call(['apt-get', 'install', '-y', 'xinput'],stdout=open(os.devnull,'wb'),stderr=STDOUT)

str = "" 
words = ["lcd_rotate=2","display_rotate=0","display_rotate=1","display_rotate=2","display_rotate=3","display_rotate=0x10000","display_rotate=0x20000"]

while str not in ["0","90","180","270"]: #asks the user to select the angle of rotation
    str = input("Please Write 0, 90, 180 or 270: ")

delete_words(words,"/boot/config.txt") #deletes old code lines to avoid colisons
document = open("/boot/config.txt", "a")

if str == "0":
    document.write("\nlcd_rotate=0")
    delete_words(["@/usr/local/bin/touch-rotate.sh"],"/home/pi/.config/lxsession/LXDE-pi/autostart" )

elif str == "90":
    document.write("\ndisplay_rotate=1")
    rotate_touch("0 1")
    
elif str == "180":
    document.write("\nlcd_rotate=2")
    delete_words(["@/usr/local/bin/touch-rotate.sh"],"/home/pi/.config/lxsession/LXDE-pi/autostart" )
    
elif str == "270":
    document.write("\ndisplay_rotate=3")
    rotate_touch("1 0")

document.close()

while str not in ["yes", "y", "no", "n"]: #asks the user to reboot the pi
    str = input("The changes you have made requiere the Raspberry Pi to be rebooted to take effect.\nWould you like to reboot now? [y/n]")
    if str == "n" or str == "no":
        pass
    elif str == "yes" or str == "y":
        os.system('sudo shutdown -r now')
        

