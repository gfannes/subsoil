---
layout: post
title: HP Color LaserJet MFP m176n
date: 2016-10-01 00:00:00
categories: 
---

Linux support for this printer/scanner is terrible. For Manjaro, the following steps eventually resulted into a functional printer, with scanner support:

* Install hplip via `sudo pacman -S hplip`
* Install printer via `sudo hp-setup` as a network printer, manually specifying the IP address
  * Make sure the following _ppd_ file is used: `drv:///hp/hpcups.drv/hp-color_laserjet_pro_mfp_m176n.ppd`
* Install the required plugins via `sudo hp-plugin`
  * Indicate to download the plugin from HP
* ~~Removed installed printer (hpijs) and install the same one, but now hpcups~~
* Restart printer seems to needed from time to time

Note: you might need to repeat the above steps when upgrading your distro.
