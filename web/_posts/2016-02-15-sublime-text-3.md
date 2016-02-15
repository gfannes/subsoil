---
layout: post
title: Sublime text 3
date: 2016-02-15 18:00:00
categories: 
---

To get [sublime text](http://www.sublimetext.com) up-and-running and usable for a [vim](http://www.vim.org) addict, following steps can help:

* Install and buy sublime text 3. Yes, just buy it, poor developers have to eat as well.
* Install [package control](https://packagecontrol.io/installation#st3):

    * Open the console via ``Ctrl+```
    * Paste the installation script:

    import urllib.request,os,hashlib; h = '2915d1851351e5ee549c20394736b442' + '8bc59f460fa1548d1514676163dafc88'; pf = 'Package Control.sublime-package'; ipp = sublime.installed_packages_path(); urllib.request.install_opener( urllib.request.build_opener( urllib.request.ProxyHandler()) ); by = urllib.request.urlopen( 'http://packagecontrol.io/' + pf.replace(' ', '%20')).read(); dh = hashlib.sha256(by).hexdigest(); print('Error validating download (got %s instead of %s), please try manual install' % (dh, h)) if dh != h else open(os.path.join( ipp, pf), 'wb' ).write(by)

    * Restart sublime text 3.
* Install [Vintageous](https://github.com/guillermooo/Vintageous) via package control:

    * Open the _palette_ via `Ctrl+Shift+p`.
    * Select `Package Control: Install Package`.
    * Select `Vintageous`.
    * Restart sublime text 3, you should have vi keybindings now.
* Add _save on focus lost_:
    
    * Copy the `save_on_focus_lost` settings line from `Preferences->Settings Default`.
    * Paste it in `Preferences->Settings User`, set it to `true` and save it, making sure there is no JSON mistake with respect to the comma.
    * Restart sublime text 3: changing to another app should save your work, except for _new_ files; you still have to save these the first time.

* Change color scheme: the _SpaceCadet_ is nice, but I find it hard to see the cursor (_caret_). To change its color for sublime text 3, do the following:

    * Install `PackageResourceViewer` via the palette.
    * Select `PackageResourceViewer: Open Resource` via the palette.
    * Select `Color Scheme - Default->SpaceCadet.tmTheme`.
    * Adjust the color for _selection_ from `#40002F` to e.g., `#2eaa40`.
