# linux-kernel-module

## Setup

Use Debian Buster V10.3.0 and install additional packages:

```bash
# module-assistant
sudo apt-get install module-assistant
# make
sudo apt-get install make
# look up your linux version 
uname -a
# and install matching linux source and header (in my case)
sudo apt-get install linux-source-4.19
sudo apt-get install linux-headers-4.19.0-8-amd64
# fakeroot
sudo apt-get install fakeroot
# libncurses5-dev
sudo apt-get install libncurses5-dev
# bin86
sudo apt-get install bin86
```
Source: Prof. Dr. Stefan Böhmer



