# VM Setup

This document describes how to set up the workshop VM.  
The VM image is available here (root / couchbase):
* https://drive.google.com/file/d/0B2k_ASGIBGIiN1VJbUUzUnFtUTg/view?usp=sharing

## VirtualBox Settings

> We recommend to install the latest version of VirtualBox. (currently 5.1.30)

VirtualBox allows to configure various virtual network cards. (https://www.virtualbox.org/manual/ch06.html)

We will detail here 2 different interfaces: NAT & Host-only.

### NAT

> _The NAT interface should be there by default._

If you just want to access the outside world from within the VM then a NAT interface is enough.  
The NAT IP is automatically assigned and is usually something like '10.0.2.15'.

In order to enable access from the outside world via NAT, 2 options:
  1. NAT Port forwarding on the same interface.
  2. Create a new interface Host-only as described below => [Host only](#host-only)

To enable NAT Port Forwarding, under the network settings of the VM's NAT network define the following port forwarding:

| Name          | Host port        | Guest port |
| ------------- |------------------|----------- |
| SSH           | 9122             | 22         |
| CB (REST)     | 9191             | 8091       |
| VNC           | 9159             | 5901       |


### Host only (recommended)

Host-only networking creates a network that is completely contained within the host computer.
It allows us to connect to our VM without using Port forwarding.

* Add a host-only "vboxnet1" interface in VirtualBox Preferences (Network)
* Change the default settings of vboxnet1 with the following:
  * IPv4 Address: 192.168.65.1
  * IPv4 Network Mask: 255.255.255.0
  * Enable DHCP Server
     * Server Address: 192.168.65.0
     * Server Mask: 255.255.255.0
     * Lower Address: 192.168.65.101
     * Upper Address: 192.168.65.254
* Under the VM settings, add 'vboxnet1' as Host-only Adapter and enable it
* Save the settings.

## Virtual Machine Setup

> If you use the provided VM image, then you can skip the following steps.

Perform the following steps in order to create the Virtual Machine:

### Creation of the VM

* In VirtualBox preferences (Network), configure the network => ([Configure the VM network](#configure-the-vm-network))
* Create Linux (Redhat 64 Bit) in VirtualBox and call it 'Couchbase-Dev'
* Assign 4GB RAM. (4096 MB)
* Create a virtual hard disk and assign a 25GB disk to it.
* Change the CPU configuration to use 4 cores of your host machine. (50% of your available cores)
* Add the media 'CentOS-7-x86_64-DVD-1611.iso' (The DVD CentOS7 media from https://www.centos.org/download/)
* Start the VM, choose 'Install' and follow the installation instructions
* Set the root password to 'couchbase'
* Choose the 25GB disk with automatic partitioning
* Choose to create a user 'couchbase' and password 'couchbase' during the installation
* Wait for the installation to complete.

### Network settings

> Run the command as root.

Boot the VM and disable the firewall:
```
systemctl disable firewalld
systemctl stop firewalld
```

Your VM should now have two network cards.
 You can double check and change the settings by using the following visual tool:

```
nmtui
```

* Make sure that all network devices are enabled at startup and that they are available to all users
* Check the name server settings
* Change the host name to 'couchbase-dev.localdomain'
```
hostname couchbase-dev.localdomain
```
* Show your IP addresses:
```
ip addr show
```

> In my case the NAT interface had the IP address 10.0.2.15 and the host only network was 192.168.56.101

* Make sure that you have access to the internet. The following ping should return a response:
```
ping 8.8.8.8
ping google.com
```

* Make sure that you can ssh from your host machine with NAT or Host-only interface.

### Software installation

#### Docker

* Install Docker as 'root' user:
```
yum install docker
systemctl start docker
systemctl enable docker
```

#### Couchbase

* Download the latest RHEL7/CentOS7 installation package
```
yum install wget
cd --
mkdir Downloads
cd Downloads
wget https://packages.couchbase.com/releases/5.0.0/couchbase-server-enterprise-5.0.0-centos7.x86_64.rpm
```

#### Graphical User Interface

We want to do some Development exercises on the VM, so we need a way to access a graphical user interface.
More information about VNC setup on CentOS7 => https://www.digitalocean.com/community/tutorials/how-to-install-and-configure-vnc-remote-access-for-the-gnome-desktop-on-centos-7

* Install 'Epel' (Extra packages for Enterprise Linux)
```
yum install epel-release
```

* Install X
```
yum groupinstall "X Window system"
```

* Install XFCE Desktop Environment
```
yum groupinstall xfce
```

* Install a VNC server
```
 yum install tigervnc-server
```

* Set VNC password for user root to 'couchbase'
```
vncpasswd
```

* Modify the VNC startup file
```
vi /root/.vnc/xstartup
--
#!/bin/sh

unset SESSION_MANAGER
unset DBUS_SESSION_BUS_ADDRESS
#exec /etc/X11/xinit/xinitrc
exec /usr/bin/startxfce4
--
```

* Include VNC at startup with XFCE on screen 1 => This gives the URL for VNC Connect: IP:5901
```
cp /lib/systemd/system/vncserver@.service /etc/systemd/system/vncserver@:1.service
```

* Edit the configuration file (replacing user & adding geometry)
Edit the [Service] section of the file, replacing instances of '<USER>' with couchbase.
Also, add the -geometry 1280x1024 clause at the end of the ExecStart parameter. This just tells VNC the screen size it should start in.
You will modify two lines in total.
```
vi /etc/systemd/system/vncserver@:1.service
```

* Reload & Start the VNC server
```
systemctl daemon-reload
systemctl enable vncserver@:1.service
```

* Use a VNC client to connect to the UI (The password is 'couchbase' & URL is IP:5901)

> The VNC Server can be stopped by using the command 'vncserver -kill :1'



#### Java IDE

* Install Maven
```
yum install maven
```

* Download the latest JDK installation package from Oracle's web site (jdk-8u121-linux-x64.tar.gz from http://www.oracle.com/technetwork/java/javase/downloads/) and place it under '/root/Downloads'

* Install the JDK
```
cd /root/Downloads
tar -xvf jdk-8u151-linux-x64.tar.gz
mv jdk1.8.0_151 /opt/jdk
echo ' ' >>  /root/.bash_profile
echo 'export PATH=/opt/jdk/bin:$PATH' >> /root/.bash_profile
echo 'export JAVA_HOME=/opt/jdk' >> /root/.bash_profile
source /root/.bash_profile
```

* Double check the version
```
java -version
```

This should return (version might differ):
```
Java version "1.8.0_151"
Java(TM) SE Runtime Environment (build 1.8.0_151-b12)
Java HotSpot(TM) 64-Bit Server VM (build 1.8.0_151-b12, mixed mode)
```

* Download Netbeans from https://netbeans.org/downloads/ to '/root/Downloads'

> I used the full distribution

* Change the script 'netbeans-8.2-linux.sh' to be executable
```
chmod +x netbeans-8.2-linux.sh
```

* Log-in to the graphical user interface (VNC) and install Netbeans under /opt/netbeans
```
./netbeans-8.2-linux.sh
```
> This might take a while.

* Ensure that Netbeans uses the right JDK by investigating the file /opt/netbeans/netbeans.conf
```
netbeans_jdkhome="/opt/jdk"
```
If not then set the right SDK!


* Start Netbeans
```
/opt/netbeans/bin/netbeans-8.2-linux.sh
```

Or use the Desktop link 'Netbeans 8.2'

* Create an empty Maven Java project and build it




#### C/C++ IDE

* Install the development tools
```
yum groupinstall 'Development Tools'
```

* Install Qt5
```
yum install qt5-* --skip-broken
```

> This worked in CentOS 6 w/o the flag --skip-broken. Let's hope the best.

* Install the Qt-Creator
```
yum install qt-creator
```

* Log-in to the graphical user interface (VNC) and run Qt-Creator
```
qtcreator
```

* Create an empty 'Qt Widgets Application' and run it



## Final steps

* Power off the VM
```
poweroff
```

* Export the Appliance & Import it over to another PC/Mac
* Start it there
* Double check the network access.

| Check         | Checked|
| ------------- |--------|
| The VM has at least a NAT IP address |?       |
| From the outside world, you can access the VM via SSH on it's host only IP address OR you can access it via the forwarded port 9122 on the VM's host |?       |
| You can access port 8091 on it's host-only IP address OR you can access it via the forwarded port 9191 on the VM's host |?       |
| You can access port 5901 on it's host-only IP address OR you can access it via the forwarded port 9159 on the VM's host |?       |
| From within the VM. you can ping 8.8.8.8 |?       |
| You can ping google.com |?       |

* If necessary repeat the steps in 'Configure the VM network'
