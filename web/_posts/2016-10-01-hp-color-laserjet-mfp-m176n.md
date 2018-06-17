---
layout: post
title: HP Color LaserJet MFP m176n
date: 2016-10-01 00:00:00
categories: 
---

Linux support for this printer/scanner is terrible. For Manjaro, the following steps eventually resulted into a functional printer, with scanner support:

* Install hplip via `sudo pacman -S hplip`
* Install printer via `sudo hp-setup`, manually specifying the IP address
* Install the required plugins via `sudo hp-plugin`
* Removed installed printer (hpijs) and install the same one, but now hpcups
* Restart printer
