---
layout: post
title: Ergodox shifted keys and XKB
date: 2017-12-22 00:00:00
categories: 
---

After a recent [manjaro](https://manjaro.org/) update, something in X/XKB must have changed, influencing the keypress event order perceived by X11. Console terminal is not affected.

My [ergodox](https://ergodox-ez.com/) keyboard is [configured](https://www.massdrop.com/configurator/ergodox) to use the `_SHIFTED_Underscore` and `_SHIFTED_Colon` for easy software development with [vim](http://www.vim.org/). These keys basically first send a _left shift_, followed by the _semicolon-colon_ key: you get the _colon_ without the need to press _shift_.

Using [xev](https://www.x.org/archive/X11R7.7/doc/man/man1/xev.1.xhtml) from the `xorg-xev` package, it became clear that a software upgrade _reversed_ the order _xev_ processed the _shift_ and _colon_ keypress: before, _shift_ came before _colon_, after, _colon_ came before _shift_, losing its _shifted_ behaviour. I have no clue why this happened, or what is causing this, but I do want my easy-to-reach _colon_ and _underscore_ back!

After reading [this nice introduction](https://medium.com/@damko/a-simple-humble-but-comprehensive-guide-to-xkb-for-linux-6f1ad5e13450) on xkb, I found the following work-around:

* Using the ergodox configurator, replace the shifted keys as:
    * `_SHIFTED_Underscore` => `KEY_Pause`
    * `_SHIFTED_Colon` => `KEY_NonUS_Backslash_Pipe`
* Edit the `/usr/share/X11/xkb/symbols/us` file as root, adding following line to the `xkb_symbols "basic"` section:

```
key <LSGT> {	[ colon,         semicolon	]	};
key <PAUS> {	[ underscore,         minus	]	};
```

* Reload the `us` layout via the command `setxkbmap -layout us`

Now, xkb is responsible for translating the two keys I don't actually use into the desired symbols, and the original behaviour of the _semicolon-colon_ and _dash-underscore_ is unaffected; I have them on other locations in the non-shifted version: I could not simply change the translation of `<AC10>` (_semicolon-colon_) and `<AE11>` (_dash-underscore_).
