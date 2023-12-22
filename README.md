# rocknation-cli
Experimental CLI tool to interact with rocknation.su written in C

Since this is just experimental for now, there could be changes, i'm working on a GUI On-Streaming too.

```[USAGE]
./rocknation-cli <option> <argument_to_option>

[OPTIONS]
        search-band <NAME>
        list-albums <BAND_URL>
        download-song <URL> [OUTPUT_FILE]
        download-album <URL> [OUTPUT_FOLDER]
```

## Installation
First, you need to install the required libraries with your favourite package manager:
- Libcurl
- Uriparser
- PCRE

```
$ sudo apt install libcurl4-openssh-dev liburiparser-dev libpcre-dev # Debian GNU/Linux
$ sudo zypper install libcurl-dev liburiparser-dev libpcre-dev # OpenSUSE GNU/Linux
$ sudo pacman -S libcurl-dev liburiparser-dev libpcre-dev # Arch GNU/Linux
```

Or you can compile their source downloading the .zip/.gz/.xz and using make OR cmake, for example:
```
# Unmantained Mirror
$ wget https://sourceforge.net/projects/pcre/files/pcre/8.45/pcre-8.45.tar.gz/download -O pcre.tar.gz
$ tar -xvf pcre.tar.gz
$ cd pcre-8.45
$ ./configure # or ./config
$ make
$ make install
```

And then you compile it like:
```
gcc main.c -o rocknation-cli -lcurl -luriparser -lpcre
```

## TO DO:

- [x] Reformat the headers to make it more readable
- [ ] Reformat the code itself to make some optimizations
- [ ] Rewrite parts of the code to move from PCRE to PCRE2
- [ ] Extend functionality
- [ ] Write a nice Graphical Interface using GTK and Glade
