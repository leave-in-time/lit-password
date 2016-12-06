## Install openframeworks

In the pi home : `cd ~`

[http://openframeworks.cc/setup/raspberrypi/](http://openframeworks.cc/setup/raspberrypi/)

## Install wiringPi

```
cd ~
git clone git://git.drogon.net/wiringPi
cd wiringPi
./build
```

## Clone the project

```
cd ~
git clone https://github.com/xseignard/lit-password.git
```

## Overclock RPi

Copy `misc/config.txt` to `/boot/config.txt`

```
sudo cp misc/config.txt /boot/config.txt
```

## Compile and run it

Add the env variables to the `/etc/environment` file (once)

```
MAKEFLAGS=-j4
```

Compile and run (for developement)

```
make && make runRelease
```

## `init.d` script

Copy the following

```
sudo cp misc/lit-password /etc/init.d/
sudo chmod +x /etc/init.d/lit-password
sudo update-rc.d lit-password defaults
```

This will make the app run at startup

## Disable shortcuts (ctrl-alt-del, tty)

### ctrl-alt-del

```
sudo rm /lib/systemd/system/ctrl-alt-del.target
sudo ln -s /dev/null /lib/systemd/system/ctrl-alt-del.target
sudo systemctl daemon-reload
```

### tty

```
sudo nano /etc/systemd/logind.conf
```

Then use the following:

```
[Login]
NAutoVTs=1
ReserveVT=0
```

### alt-f4

TODO?
