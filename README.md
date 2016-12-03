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
