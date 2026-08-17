# Chapter 3 - IP Addresses, structs, and Data Munging

# IP Addresses, versions 4 and 6

Internet Protocol Version 4 (IPv4) - addresses made up of 4 bytes (AKA four octets), commonly written as dots and numbers, like so `192.0.2.111`

Problem: we're running out of IPv4 addresses FAST - only 2^32, some billion addresses, and we DO have billions of computers now -> we need more addresses -> IPv6

Hexadecimal represenntation, with each two-byte chunk seperated by a colon, like this:

```
2001:0db8:c9d2:aee5:73e3:934a:a5ae:9551
```

That’s not all! Lots of times, you’ll have an IP address with lots of zeros in it, and you can compress them between two colons. And you can leave off leading zeros for each byte pair. For instance, each of these pairs of addresses are equivalent:

```
2001:0db8:c9d2:0012:0000:0000:0000:0051
2001:db8:c9d2:12::51

2001:0db8:ab00:0000:0000:0000:0000:0000
2001:db8:ab00::

0000:0000:0000:0000:0000:0000:0000:0001
::1
```

The address `::1` is the loopback address. It always means “this machine I’m running on now”. In IPv4, the loopback address is `127.0.0.1`.

Finally, there’s an IPv4-compatibility mode for IPv6 addresses that you might come across. If you want, for example, to represent the IPv4 address `192.0.2.33` as an IPv6 address, you use the following notation: `::ffff:192.0.2.33`

## Subnets

In the Ancient Times, there were “classes” of subnets, where the first one, two, or three bytes of the address was the network part. If you were lucky enough to have one byte for the network and three for the host, you could have 24 bits-worth of hosts on your network (16 million or so). That was a “Class A” network. On the opposite end was a “Class C”, with three bytes of network, and one byte of host (256 hosts, minus a couple that were reserved).

So as you can see, there were just a few Class As, a huge pile of Class Cs, and some Class Bs in the middle.

The network portion of the IP address is described by something called the netmask, which you bitwise AND with the IP address to get the network number out of it. The netmask usually looks something like `255.255.255.0`. (E.g. with that netmask, if your IP is `192.0.2.12` , then your network is `192.0.2.12` AND `255.255.255.0` which gives `192.0.2.0`.)

Unfortunately, it turned out that this wasn’t fine-grained enough for the eventual needs of the Internet -> allowed for the netmask to be an arbitrary number of bits, not just 8, 16, or 24

So you might have a netmask of, say `255.255.255.252`, which is 30 bits of network, and 2 bits of host allowing for four hosts on the network. (Note that the netmask is ALWAYS a bunch of 1-bits followed by a bunch of 0-bits.)

But it’s a bit unwieldy to use a big string of numbers like as a netmask. You just put a slash after the IP address, and then follow that by the number of network bits in decimal. Lik this: `192.0.2.12/30`

Or, for IPv6, something like this: `2001:db8::/32` or `2001:db8:5413:4028::9db9/64`.

## Port Numbers

Besides an IP address (used by the IP layer), there is another address that is used by TCP and UDP. It is the port number. It’s a 16-bit number that’s like the local address for the connection.

Think of the IP address as the street address of a hotel, and the port number as the room number. 

Different services on the Internet have different well-known port numbers. You can see them all in the Big IANA Port List or, if you’re on a Unix box, in your `/etc/services` file. 

HTTP (the web) is port 80, telnet is port 23, SMTP is port 25, the game DOOM4 used port 666, etc. and so on. Ports under 1024 are often considered special, and usually require special OS privileges to use.