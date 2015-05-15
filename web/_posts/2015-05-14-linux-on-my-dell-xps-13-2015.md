---
layout: post
title: Linux on my Dell XPS 13 2015
date: 2015-05-14 17:24:20
categories: 
---

I recently bought a Dell XPS 13 2015 developer edition laptop (BIOS A03). Since it came with linux preinstalled, I expected flawless linux support, but that was not really the case: the initial wifi configuration did not work, its ubuntu 14.04 OS kept mentioning internal errors, waking up from hibernation froze the keyboard and touchscreen. The font size was very small due to its HiDPI touchscreen with a resolution of 3200 x 1800 not being compensated.

I was hoping that upgrading to ubuntu 2015.04 would fix all this, but the upgrade procedure failed. Installing ubuntu 2015.04 from scratch was even worse: the live USB worked very well, even wifi worked out of the box, installation was fine, but my laptop would not boot and kept displaying something along the lines: No bootable devices found. I though it had to do with UEFI and secure boot, a technology I'm not familiar with, but after some experimentation, I think it is a problem with unubntu.

Ubuntu had its chance, time for something else: [manjaro](http://manjaro.org), an arch-based distro I've been using successfully for years. The live USB stick did boot, but the touchpad was very jumpy, wifi did not work out of the box, and XFCE is not very suitable for HiDPI. The wifi issue was easily solved following the suggestions from the arch community, and an upgrade fixed the touchpad issues.

Hibernation and suspension still gave problems. Replacing the SNA graphics driver for the older UXA changed its behaviour a bit, but problems remained. Upgrading to linux kernel version 4 did fix these issues, and can easily be done from inside the Manjaro Settings Manager.

After an unsuccessfull test with i3, I exchanged XFCE for Gnome3 which works very well in combination with the HiDPI touchscreen, only the screen keyboard keeps popping up from time to time, but I don't mind that, it should be easy to fix. Hibernation was back though but in a different shape: Gnome3 Power settings gives no options to handle closing the lid. After installing the _gnome-power-manager_, I could manually indicate in /etc/systemd/logind.conf that my machine should _hibernate_ when closing its lid, and _shutdown_ when pressing the power button.

Finally, the printer setup failed although I could configure my HP Photosmart 2610 before. It turns out to be a problem with the foomatic-\* and manjaro-printer package. After uninstallation, I could successfully intall my printer.

I have a fully functional laptop now with readable fonts and great HiDPI support! Scrolling via the touchscreen works in most applications, as is two finger zoom in the default web browser (some browser called _web_, based on webkit). The only problem I still have has to do with the microphone, it shows up, but I cannot get sound out of it. I use a Logitech C920 webcam instead which works fine out of the box.

I hope these directions can help other people configure their Dell XPS 13 for linux. It took me some time and experimentation, but now I'm very happy with it. Quite a shame for Dell though to put such a badly configured machine on the market...

This article is not very high in details, mostly because I don't want to go through the process again just to take detailed notes. If you want more details or are stuck, don't hesitate to contact me.
