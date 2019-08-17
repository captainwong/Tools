# putty_launcher_MFC

![img](https://github.com/captainwong/Tools/blob/master/putty_launcher_MFC/putty_launcher_MFC.png?raw=true)

A small tool stores sessions in file and calls `putty` with `username`/`password` or `ppk`. 

Portable, small, easy to use.

Configuration file is stored in file `./putty_launcher/putty_launcher.json`, feel free to modify it. 

## Trick to modify session experience

Under command line, you can only set the user/host/password/etc, but there is no way to change the experience. The default experience's font is too small, making me blind.

So I found a way to mofity the default experience:

Open REGEDIT, locate to `HKEY_CURRENT_USER\Software\SimonTatham\PuTTY\Sessions\Default%20Settings`, change anything you need.

For example, change `Font` to `Consolas`, change `FontHeight` to `c`, etc.

For portable, export the `Default%20Settings` to a `reg` file, under a new machine, double click it to import.

Now every putty session will look the same as you configured.

## Todo

* Delete folder/session
* Move folder/session
* Password encryption

