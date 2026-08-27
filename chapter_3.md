# Chapter 3 - IP Addresses, structs, and Data Munging

## Summary
IP Addresses include IPv4 (made of 4 bytes, written as dots and numbers) and IPv6 (8 chunks of 2 bytes, written as hexadecimals), out of concern for running out of IPv4 addresses.
Subnets divide the address into a network address and a host address, using a netmask, and denoted as a decimal behind the IP. Port Numbers are to denote the local connection to an IP addreess, if a device has multiple services.

Byte Order includes Big-Endian and Little-Endian. Network Byte Order is always Big-Endian and Host Byte Order depends on architecture. Always run an address through a function before assigning it to the network.

Structs include `struct addrinfo` to prep the socket address structures for subsequent use. The socket descriptor is just an `int`. `struct sockaddr_in` and `struct sockaddr_in6` denotes the socket addresses for `AF_INET` and `AF_INET6` as IPv4 and IPv6.

IP manip functions exist so we don't have to broadcast and shift. `inet_pton()` converts address in printable form to network binary, and `inet_ntop()` does the reverse.

Private networks and firewalls use Network Address Translation to map public IPs into reversed IPs, such as `10.x.x.x` and `fdXX:` 

## IP Addresses, versions 4 and 6

Internet Protocol Version 4 (IPv4) - addresses made up of 4 bytes (AKA four octets), commonly written as dots and numbers, like so `192.0.2.111`

Problem: we're running out of IPv4 addresses FAST - only 2^32, some billion addresses, and we DO have billions of computers now -> we need more addresses -> IPv6

Hexadecimal representation, with each two-byte chunk seperated by a colon, like this:

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

### Subnets

In the Ancient Times, there were “classes” of subnets, where the first one, two, or three bytes of the address was the network part. If you were lucky enough to have one byte for the network and three for the host, you could have 24 bits-worth of hosts on your network (16 million or so). That was a “Class A” network. On the opposite end was a “Class C”, with three bytes of network, and one byte of host (256 hosts, minus a couple that were reserved).

So as you can see, there were just a few Class As, a huge pile of Class Cs, and some Class Bs in the middle.

The network portion of the IP address is described by something called the netmask, which you bitwise AND with the IP address to get the network number out of it. The netmask usually looks something like `255.255.255.0`. (E.g. with that netmask, if your IP is `192.0.2.12` , then your network is `192.0.2.12` AND `255.255.255.0` which gives `192.0.2.0`.)

Unfortunately, it turned out that this wasn’t fine-grained enough for the eventual needs of the Internet -> allowed for the netmask to be an arbitrary number of bits, not just 8, 16, or 24

So you might have a netmask of, say `255.255.255.252`, which is 30 bits of network, and 2 bits of host allowing for four hosts on the network. (Note that the netmask is ALWAYS a bunch of 1-bits followed by a bunch of 0-bits.)

But it’s a bit unwieldy to use a big string of numbers like as a netmask. You just put a slash after the IP address, and then follow that by the number of network bits in decimal. Lik this: `192.0.2.12/30`

Or, for IPv6, something like this: `2001:db8::/32` or `2001:db8:5413:4028::9db9/64`.

### Port Numbers

Besides an IP address (used by the IP layer), there is another address that is used by TCP and UDP. It is the port number. It’s a 16-bit number that’s like the local address for the connection.

Think of the IP address as the street address of a hotel, and the port number as the room number. 

Different services on the Internet have different well-known port numbers. You can see them all in the Big IANA Port List or, if you’re on a Unix box, in your `/etc/services` file. 

HTTP (the web) is port 80, telnet is port 23, SMTP is port 25, the game DOOM4 used port 666, etc. and so on. Ports under 1024 are often considered special, and usually require special OS privileges to use.

## Byte Order

Little Endian, Big Endian

Big-Endian = Network Byte Order

Computer stores Host Byte Order, which can be Little or Big Endian depending on architecture (Intel80x86 ues Little Endian, Motorola 68k uses Big Endian, etc.)

Need to make sure building packets or filling out data structures with Network Byte Order 

Always assume Host Byte Order isn't right, run the value through a function to set to Network Byte Order

Two type of numbers: `short` (two bytes) and `long` (four bytes)

Convert `short` from Host Byte Order to Network Byte Order -> start with "h" (host), "to", "n" (network) and "s" (short), or htons() (Host to Network Short)

The functions:
- htons() - Host to Network Short 
- htonl() - Host to Network Long
- ntohs() - Network to Host Short
- ntohl() - Network to Host Long

No standard 64-bit variants in the sockets API

## `struct`s

A socket descriptor is just: `int`

Code: [src/structs.c](src/structs.c)

`struct addrinfo` prep the socket address structures for subsequent use. Load the struct and call `getaddrinfo` to return a pointer to a new linked list;

Force to use IPv4 or IPv6 in `ai_family`, or leave as `AF_UNSPEC`

`ai_next` as a linked list

`ai_addr` is a pointer to a `struct sockaddr`

`struct sockaddr` holds socket address information

`sa_family` can be `AF_INET` (IPv4) or `AF_INET6` (IPv6), `sa_data` contains a destination address and port number but rather too weieldy -> made `struct sockaddr_in` and `struct sockaddr_in6`

`struct sockaddr_storage` designed to be large enough to hold both IPv4 and IPv6 structures

## IP Addresses, Part Deux

Already a bunch of functions to use for IP manip

`inet_pton()` converts an IP address in numbers-and-dots notation into either a `struct in_addr` or `struct in6_addr` depending on whether you specify `AF_INET` or `AF_INET6`

`pton` stands for "presentation to network" or "printable to network"

```c
struct sockaddr_in sa; // IPv4
struct sockaddr_in6 sa6; // IPv6

inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr));
inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr));
```

Returns -1 on error, or 0 if address is bad. Check output before using.

`inet_ntop()` to convert binary address to numbers-and-dots notation (network to printable)

```c
// IPv4:

char ip4[INET_ADDRSTRLEN]; // space to hold the IPv4 string
struct sockaddr_in sa; // pretend this is loaded with something

inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);

printf("The IPv4 address is: %s\n", ip4);


// IPv6:

char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
struct sockaddr_in6 sa6; // pretend this is loaded with something

inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);

printf("The address is: %s\n", ip6);
```

These functions only work with numeric IP adresses, they won't do any nameserver DNS lookup on a hostname like "www.example.com". We will use `getaddrinfo()` to do that.

### Private (Or Disconnected Networks)

Lots of places have a firewall that hides the network from the rest of the world for their own protection. And often times, the firewall translates “internal” IP addresses to “external” (that everyone else in the world knows) IP addresses using a process called Network Address Translation, or NAT.

10.x.x.x is one of a few reserved networks that are only to be used either on fully disconnected networks, or on networks that are behind firewalls. The details of which private network numbers are available for you to use are outlined in RFC 19186, but some common ones you’ll see are 10.x.x.x and 192.168.x.x, where x is 0-255, generally. Less common is 172.y.x.x , where y goes between 16 and 31.

Networks behind a NATing firewall don’t need to be on one of these reserved networks, but they commonly are.

Fun fact! My external IP address isn’t really 192.0.2.33 . The 192.0.2.x network is reserved for make believe “real” IP addresses to be used in documentation, just like this guide! Wowzers!

IPv6 has private networks, too, in a sense. They’ll start with fdXX: (or maybe in the future fcXX: ), as per RFC 4193