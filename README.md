# car_dasboard
Linux Desktop and WIndow Manager For a Card Dashboard written in QT.

... under recovery.. This is a 7 years old project just ran into. I am making it runable on latest QT. Does not run properly yet.

   * If you run it start the start-xephir.sh and run the appon display 1, otherwis would iterfere with curent desktop.
   
   
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
sudo apt-get  install gnuradio
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



#### Bluetooth


  * WIP

#### Camera's

  * ESP32 cam & ethernet cam

#### For Media player

  * mplayer


#### For OBD2

   * pyobd

#### For temperature / Humidity

   * builtin

### For acceleration, orientation compass

  * builtin





