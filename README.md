lariza
======

A simple web browser using GTK+ 3, GLib and WebKit2GTK+.

This is a friendly fork of the
[original project](https://github.com/vain/lariza) by Vain in order to
test various stuffs.

There is no aim to release anytime soon. It is mainly for my personnal
use, to refresh my memory about C code and for fun. Do not hope any
support from me. You are completely free to use it though.

Done and different from upstream
--------------------------------

- Various different key bindings (see the code).
- An old fashioned throbber replace loading bar.
- ~~home page and download dir are configurable as parameters.~~
- Cookie management \o/
- Pref file to store home page, download dir... An exemple is given in
  `lariza.cfg`. It has to be saved as ~/.config/lariza/lariza.cfg


In my mind
----------

- A follow link mechanism?
- Stay tuned about embedding mozilla? [Some](http://chrislord.net/index.php/2016/03/08/state-of-embedding-in-gecko/) [bits](https://dutherenverseauborddelatable.wordpress.com/2016/03/07/the-gecko-monoculture/)
- ...

Note about throbbers
--------------------

The various pictures included in the media folder of this repository are
not free. Thus, they will never be included in the Makefile or any
package done by me.


Original README follows:

Features
--------

- A WebKit2 viewport
- An input box to change the URI or to search the current page
- Built-in launching of suckless' tabbed
- Built-in download manager
- Optimized hotkeys: Left hand on keyboard, right hand on mouse
- Keyword based searching: Opening "wi foo" will search wikipedia
- Global content zoom
- Cooperative instances using FIFOs
- Certificate trust store
- Support for Flash and Java
- Bundled web extensions:
  - Adblock

Installation
------------

The following C libraries are required:

- GTK+ 3
- WebKit2 API for GTK+ 3
- libconfig

lariza expects to be run on a POSIX-ish operating system.

To build the program and install it to /usr/local:

    $ make
    # make install

To use bundled web extensions, they must be copied or symlinked to the
appropriate path. Please refer to the manpage.


Running
-------

You simply invoke the main program:

    $ lariza

Refer to the manpage for all options.


Background information
----------------------

What lariza is and what it's not

lariza does what I need. It won't do other things. I'm open for pull
requests but please don't be upset if I turn them down -- which might
happen if it's a feature that I simply don't need.

Especially, it's very likely that lariza will never have a "follow mode"
like dwb, luakit or others have. I've used these browsers for quite some
time and I've also used Firefox extensions that add a "follow mode". The
point is, "follow mode" doesn't work anymore. This was a good thing ten
years ago. Today, a lot of websites make heavy use of JavaScript or
hovering. You NEED some kind of pointing device. I found using "follow
mode" to be very frustrating today, because you still have to reach for
the mouse all the time. So, you might as well just optimize your mousing
workflow.

lariza does not compete with powerful browsers like dwb or luakit, nor
with monstrous applications like Firefox or Chromium. lariza won't have
persistent storage, nor a plugin system, nor cloud sync, nor bookmarks.

lariza tries not to exceed 1000 lines of code.

About the name
--------------

"lariza" stems from a german sentence:

    Alle anderen waren mir zu anstrengend.
     l   a         r    i  z  a

That phrase basically means: "It was too tiresome to deal with any other
browser." I couldn't find a simple browser that does just what I
need. Most of them are utterly bloated, others lack essential
functions. Thus, I was forced to write scripts and tools and stuff to
deal with these issues. That is what was tiresome. I don't want to work
around bugs or nonsensical behavior anymore.

So, I wrote my own browser^W WebKit GUI. WebKit does all the dirty work.
