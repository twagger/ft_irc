# Welcome to ft_irc 💬

![](https://www.aurone.com/wp-content/uploads/2018/10/livechat-conversions-ecommerce.gif)

This project is about creating a small IRC server that can be used with a real IRC client to chat, create chat rooms, send private messages to one or multiple users, etc ...

We tested it with Netcat (TCP/UDP client) and IRSSI (IRC client)

# Installation instructions

# Usage instruction

### Launch the server
```sh
./ircserv [port number] [password]
```

*For example*
```sh
./ircserv 6667 my-password
```

## Connect to the server with a Netcat
```sh
nc -C 127.0.0.1 6667
```

**note** : *we use the -C option because IRC messages end with **\r\n** as defined in the RFC 2812 standards.*

**note** : *We used 127.0.0.1 since our server is launched in localhost.*

## Authenticate

You need to authenticate to the server otherwise you will be deconnected quickly :

```sh
pass my-password
user tom 0 * :tom
nick tom
```

**note** : *To understand more abour the authentification process please look [here](https://www.rfc-editor.org/rfc/rfc2812#section-3.1.1).*

## Connect to the server with IRSSI
```sh
irssi
/connect 127.0.0.1 6667 my-password
```

**note** : *The authentification process is automatically done by irssi.*

**note** : *Your user will be known in the server as plop.*

# Commands

We implemented a lot of command from the [RFC 2812](https://www.rfc-editor.org/rfc/rfc2812#section-3.1.1). We didn't implemented the multi server commands as it was not required in the project.

| Category        | Commands                                                 |
| --------------- | -------------------------------------------------------- |
| Server          | DIE, INFO, MOTD, PING, PONG, QUIT, SERVICES, TIME, VERSION |
| User            | USER, KILL, MODE (B, i, o, r, s), NICK, OPER, PASS, WHO, WHOIS |
| Channel         | INVITE, JOIN, KICK, LIST, MODE (b, i, k, o), NAMES, PART, TOPIC |
| Private message | PRIVMSG, NOTICE                                          |

# Extra features

## file transfer : XDCC

We where asked to implement file transfer. If you are using a proper IRC client like IRSSI, you can use XDCC (eXtended DCC) which use direct messages to establish a direct connection between two clients so they can share files.

**Sending file to another client**

```irssi
/dcc send [nickname of user] [path of the file between quotes]
```

*Example*
```irssi
/dcc send plop_ "Documents/test.txt"
```

**Receiving file from a client**
```irssi
/dcc get [nickname of user][name of the file between quotes]
```

*example* 
```irssi
/dcc get plop "test.text"
```

## Answering bot

We implemented it as a simple client using the command services. This bot can be contacted by private message and answer to specific commands only :

* QUOTE : give the quote of the day
* TIME : give the exact time of the sercer

The bot is a second separate program that need to be launched and connected to  server with a specific command : 

```irssi
./ircbot [hostname] [password]
```

Then the bot is available on the server and can interact with real users. It is a very simple bot but it could be enhanced a lot to be more useful. We just wanted to understand the concept here.

## Resources

* [RFC](https://www.rfc-editor.org/rfc/rfc2812#section-3.1.1)
* [Channel related RFC](https://www.rfc-editor.org/rfc/rfc2811)
* [Tutorial on how to implement a server in french](http://vidalc.chez.com/lf/socket.html)
* [Website made by 42 students from Paris](https://ircgod.com/)

## Credits

* 🖼️ Illustrative image from : [here](https://www.aurone.com/blog/27-astuces-pour-que-le-livechat-augmente-les-conversions-des-sites-e-commerce/)