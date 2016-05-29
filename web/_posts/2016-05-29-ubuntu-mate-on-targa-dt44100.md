---
layout: post
title: Ubuntu-MATE on TARGA DT44100
date: 2016-05-29 15:43:00
categories: 
---

Installation of ubuntu-MATE went fine, but the graphics resolution was to small. This can be fixed by:

* Create the file /usr/share/X11/xorg.conf.d/use-vesa.conf with content:
    Section "Device"
        Identifier "Configured Video Device"
        Driver "vesa"
    EndSection
* Reboot
