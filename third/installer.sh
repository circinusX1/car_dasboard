#!/bin/sh

do_install() {
    echo " Compilation..."
    if make
    then
    echo ""
    echo " Compilation finished "
    echo ""
    else echo " FAIL: Compilation error"
    exit 1
    echo ""
    fi
    echo " Creating dirs and copying files => if exist replacing them!"
    echo""
    if
    make install
    then
    echo ""
    echo " Installed"
    else echo " Can't install!"
    fi
    if
    chmod 644 -R /usr/local/share/QtRadio && \
    chmod 755  /usr/local/share/QtRadio   && \
    chmod 755  /usr/local/share/QtRadio/Skins && \
    chmod 755  /usr/local/share/QtRadio/Skins/Blue && \
    chmod 755  /usr/local/share/QtRadio/Skins/Green && \
    chmod 755  /usr/local/share/QtRadio/Skins/Red && \
    chmod 755  /usr/local/share/QtRadio/icons      && \
    chmod 755  /usr/local/share/QtRadio/Languages
    then
    echo " Modes changed"
    else echo " Can't change files permissions"
    fi
    echo "************************************************"
    echo " SUCCESS:Installation finished. Type: qtradio"
    echo "************************************************"

}

config() {
    echo " Creating Makefiles [Lirc disabled]"
    if qmake
    then
    do_install
    else
    echo " Error while  configuring : Aborting Installation"
    fi
}

config_yes() {
    echo " Creating Makefiles [Lirc enabled]"
    if qmake -config lirc
    then
    do_install
    else
    echo " Error while  configuring : Aborting Installation"
    fi
}

rm -f Makefile src/Makefile

echo ""
echo ""
echo " Welcome to Installation Script for QtRadio-0.9.0 "
echo " [December 2013 : Oleh Nykyforchyn, oleh.nyk@gmail.com]"
echo ""
echo " Starting installation script..."
echo ""
echo " Do you want to enable LIRC support? "
echo " [Yes] [yes] [Y] [y] ;  [No] [no] [N] [n] "
read lirc
case "$lirc" in
    "Yes" | "yes" | "y" | "Y")
	config_yes
	;;
    "No" | "no" | "n" | "N")
	config
	;;
    *)
	echo "I don't understand this answer, default configuration"
        config
	;;
esac

exit 0
