---
layout: post
title: Sony WH-1000XM4 Headphones
date: 2020-09-11 00:00:00
categories: 
---

Using my brand-new [Sony WH-1000XM4 Headphones](https://www.sony.com/electronics/headband-headphones/wh-1000xm4) was not trivial. Pressing the power-button for 7 seconds brings the device in pairing mode.

## Connecting to Nokia 8 Sirocco

When paired with the phone, I keep hearing "bluetooth connected" when playing music, and no music. Apparently, this is caused by [Nokia's bluetooth manager](https://community.phones.nokia.com/discussion/7778/bluetooth-not-working/p2) that crashes when using `HD audio`. Disabling the `HD audio` did the trick.

## Connecting to Manjaro

For some reason, the HP cannot pair with the bluetooth on my `MSI MPG X570 GAMING PRO CARBON WIFI` mobo. Inserting a bluetooth dongle with chipset `Broadcom Corp. BCM20702A0 Bluetooth 4.0` resolved that. To properly configure bluetooth devices, best is to install `blueman-manager` from the `blueman` package.

With the HP in pairing mode, perform a search on the BT dongle and pair `WH-1000XM4` (the `LE_WH-1000XM4` is a low-energy connection that cannot transport audio). After pairing, run the setup and indicate the device as _trusted_. Then, right-click on `WH-1000XM4` and press `Connect`. You should now hear _bluetooth connected_ on the HP and see the WH-1000XM4 in the sound settings.

When selecting the WH-1000XM4 as output device, you can select between `HSP/HFP` and `A2DP Sink`. For me, only the `A2DP Sink` works and gives stereo audio. When selecting the `HSP/HFP`, the `Test` button presents a mono source without actual audio. Unfortunately, the mic currently does not work. I managed to get the mic working by replacing the `manjaro-firmware` package with `bcm20702a1-firmware` yesterday on my laptop, but today, installation of the same package fails on my desktop due to a 404 on `http://dlcdnet.asus.com/pub/ASUS/wireless/USB-BT400/DR_USB_BT400_1201710_Windows.zip`. Maybe that link comes back one day, maybe the `HSP/HFP` will be fixed in the `manjaro-firmware` or kernel.

Other links on the web advice to install `pulseaudio-modules-bt` to have `LDAC` support. This did not work for me, and I rolled-back to `pulseaudio-bluetooth`.
