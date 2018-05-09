# Couchbase Development Workshop - 2 days

Couchbase is the most powerful NoSQL data platform.

This course is designed to give an introduction into Couchbase's main use cases, teach the Couchbase Server's administration basics and it's architecture. The main focus of this 2 day workshop is to learn how to develop with Couchbase's standard development kits. You will learn how to manage connections, how to work with documents and how to model your data best for Couchbase.

This is a combined C/C++ & Java workshop. 

## Requirements

Each training computer should have at least the following HW configuration.

### Hardware

  * 4 CPU cores >=2GHz
  * 8 GB RAM
  * 50 GB free disk space

The following connectivity is expected:

### Internet access

The following software needs to be installed on the attendee's computer:

  * VirtualBox >= 4.3 (https://www.virtualbox.org/wiki/Downloads)
  * Hardware virtualization enabled. (AMD-V/VT-x) => [Troubleshooting Guide](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Workshop-Troubleshoot-Guide.pdf)
  * Putty and WinSCP (Windows only)
  * A VNC Viewer (https://github.com/TigerVNC/tigervnc/releases)
  * Firefox or Chrome

The attendee should have all required permissions to create Virtual Machines and Virtual Machine networks on his box.

### Room Facilities

The room should provide a video-projector (HDMI, DVI or VGA) & a whiteboard with eraser & pens.

## VM Setup

Details regarding the setup of the workshop VM can be found here:

* [VMSETUP](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/VMSETUP.md)

You can also directly download the VM and import the Appliance in VirtualBox:  
Here is the link:  https://drive.google.com/file/d/0B2k_ASGIBGIiN1VJbUUzUnFtUTg/view?usp=sharing

[Troubleshooting Guide](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Workshop-Troubleshoot-Guide.pdf)

### Alternative to VM (Docker)

In case that you have issues running the VM you could run a Docker container  with a 1 node cluster using the following command:

```
docker run -d -p 8091-8094:8091-8094 -p 11210:11210 --name cb1 -v /[FULL_PATH]/cb1:/opt/couchbase/var couchbase
```

> Note that the [FULL_PATH] is a real path on your host machine e.g. /tmp/couchbase
## Slides & Labs

* Day 1.
  * Slides: [Workshop-Day1-0-Agenda.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day1/Workshop-Day1-0-Agenda.pdf)
  * Slides: [Workshop-Day1-1-Introduction.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day1/Workshop-Day1-1-Introduction.pdf)
  * Slides: [Workshop-Day1-2-Architecture.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day1/Workshop-Day1-2-Architecture.pdf)
  * Slides: [Workshop-Day1-5-Security.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day1/Workshop-Day1-5-Security.pdf)
  * Slides: [Workshop-Day1-6-BackupRestore.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day1/Workshop-Day1-6-BackupRestore.pdf)
  * Slides: [Workshop-Day1-7-XDCR.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day1/Workshop-Day1-7-XDCR.pdf)
  * Slides: [Workshop-Day1-8-FTS.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day1/Workshop-Day1-8-FTS.pdf)
  * Labs: [Workshop-Day1-9-Labs.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day1/Workshop-Day1-9-Labs.pdf)

* Day 2.
  * Slides: [Workshop-Day2-1-Document Modeling.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day2/Workshop-Day2-1-Document%20Modeling.pdf)
  * Slides: [Workshop-Day2-2-Json Data Modeling.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day2/Workshop-Day2-2-Json%20Data%20Modeling.pdf)
  * Slides: [Workshop-Day2-3-Indexes.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day2/Workshop-Day2-3-Indexes.pdf)
  * Slides: [Workshop-Day2-4-N1QL.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day2/Workshop-Day2-4-N1QL.pdf)
  * Slides: [Workshop-Day2-5-FTS.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day2/Workshop-Day2-5-FTS.pdf)
  * Labs C: [Workshop-Day2-9-CCCP-Labs.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day2/Workshop-Day2-9-CCCP-Labs.pdf)
  * Labs Java: [Workshop-Day2-9-Java-Labs.pdf](https://github.com/dufrenoyl/cb-workshop-2d/blob/master/slides/Day2/Workshop-Day2-9-Java-Labs.pdf)

## Resources

* [RXJava Cheat Sheet](http://files.zeroturnaround.com/pdf/zt-rxjava-cheat-sheet.pdf)
* [Couchbase in Docker](https://developer.couchbase.com/documentation/server/current/install/docker-deploy-single-node-cluster.html)