---
layout: post
title: Belgium eID
date: 2018-09-16 00:00:00
categories: 
---

Getting the Belgian eID cardreader up and running under linux can be painful. These steps worked for me:

* Install [manjaro](https://manjaro.org)
* Install and run `sudo pcscd`
* Install eID card reader driver via `yaourt acsccid`
* Install `eid-mw` via yaourt
  * `yaourt eid-mw`
  * Install any unknown public key via `gpg --recv-keys <missing-key>`
* Test eID card reader via `eid-viewer`
  * Insert eID card in reader
  * Insert card reader in USB port
  * Running `eid-viewer` should display the photo stored on the eID
* Start firefox
* Install [addon](https://addons.mozilla.org/nl/firefox/addon/belgium-eid/)
* Restart firefox
* Goto [https://eid.belgium.be/nl](https://eid.belgium.be/nl) and test via "Aanmelden met CSAM"
  * Accept the certificate stuff
  * Enter pin code
