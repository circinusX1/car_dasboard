# car_dasboard
Linux Desktop and WIndow Manager For a Card Dashboard written in QT.

... under recovery.. This is a 7 years old project just ran into. I am making it runable on latest QT. Does not run properly yet.

   * If you run it start the start-xephir.sh and run the appon display 1, otherwis would iterfere with curent desktop.
   
   
   carutza -display ":1"
   
   
   #R-PI install
   
   * Have a non UI image installed. Like: Raspbian GNU/Linux 8 \n \l

```   
   sudo apt-get update && sudo apt-get install libtool libx11-dev xinput autoconf libx11-dev libxi-dev x11proto-input-dev xorg -y
   sudo apt-get install x11-xserver-utils xinit
   sudo apt-get install -y qt4-dev-tools
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
   cd ./bin
   ./carutza -display :1

```
