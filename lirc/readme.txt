http://www.pridopia.co.uk/rs-pi-set-ir-remote.html

1. install lirc
sudo apt-get install lirc
2. sudo apt-get install python-lirc    
You can test the remote works with the mode2 program
> mode2 -d /dev/lirc0

3. Add to config.txt to let lirc work on gpio_in_pin=15
dtoverlay=lirc-rpi
dtparam=gpio_in_pin=15


4. Copy remote config to /etc/lirc/lircd.conf.d
  cp lircd.conf /etc/lirc/lircd.conf.d/


2. copy lircrc to /root
  cp lircrc  /root/.lircrc
  
       
4. reboot
