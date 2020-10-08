# car_dasboard


This is a 7 years old project just ran into. Work in progress...

   * If you run it start the start-xephir.sh and run the app on display 1, otherwise would iterfere with curent desktop.
   
   
   carutza -display ":1"
   
   
   #R-PI install
   
   * Have a non UI image installed. Like: Raspbian GNU/Linux 8 \n \l

   ## carutza (qt4)
   
```   
   sudo apt-get update && sudo apt-get install libtool libx11-dev xinput autoconf libx11-dev libxi-dev x11proto-input-dev xorg -y
   sudo apt-get install x11-xserver-utils xinit
   sudo apt-get install -y qt4-dev-tools
   sudo apt-get install rtl-sdr
   
##########################################   
   sudo nano /etc/X11/Xwrapper.config
   
#   dpkg-reconfigure xserver-xorg-legacy
allowed_users=anybody # make this change

##########################################
   
   cd carutza
   make clean
   rm moc_*
   qmake .
   make
xset s off
xset -dpms  
xset s noblank

   cp ./caruthza ../bin
   # for arm R-PI etc...
   cd ../bin
   ./start-arm.sh  
```

On a PC where already there is a desktop runnning

```
   cd car_dasboard
   ./start<tab> CR   start xephir
   ./carutza -display :1

```

#### For RTL radio  (qt5)

```
work in progress bottom notes

```

#### Navigaton

 * using navit
 
NEO-6M GPS WITH R-PI: follow: http://comarius.blogspot.com/2020/09/neo-6m-gps-with-raspberry-pi.html



```

sudo apt-get install gpsd
stty -F /dev/serial1 9600
 
sudo nano /etc/default/gpsd
# add this content
START_DAEMON="true"
USBAUTO="true"
DEVICES="/dev/serial0"
GPSD_OPTIONS=""
GPSD_SOCKET="/var/run/gpsd.sock"
 
 ```


### Configure desktops and panels

todo

#### Bluetooth


todo

#### Camera's

  * ESP32 cam & ethernet cam

#### For Media player

  * mplayer


#### For OBD2

   * pyobd

#### For temperature / Humidity

   *todo

### For acceleration, orientation compass

  todo


![](https://raw.githubusercontent.com/circinusX1/car_dasboard/master/docs/car1.png)


![](https://raw.githubusercontent.com/circinusX1/car_dasboard/master/docs/car2.png)


![](https://raw.githubusercontent.com/circinusX1/car_dasboard/master/docs/car3.png)


![](https://raw.githubusercontent.com/circinusX1/car_dasboard/master/docs/car_d.png)




######## NOTES:

```
#trying
git clone https://github.com/ElektorLabs/160520-FM-Radio-Receiver-with-RDS-for-Raspberry-Pi

sudo apt-get install qtmultimedia5-dev
sudo apt-get install libqt5websockets5-dev


export QT_SELECT=qt5
qtchooser -print-env


```



### P.S.
   * Contact me on the git email or at: https://www.meeiot.org/?p=contact.php   or https://mylinuz.com/?p=_contact.php
     * Custom private Server for remote shell.
     * Professional Embedded Linux support.
     * C /C ++ Coding
     * Amazon, Google, Azure server configuration (Linux, Web Servers, Databases).
     * Backup sollution servers on site for small companies up to 30 computers (GTA Only).
     * Antivirus installation (GTA Only)
                                                     @ CoMarius 1994-2020


