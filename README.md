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

And then you compile it like:
```
gcc main.c -o rocknation-cli -lcurl -luriparser -lpcre
````

## TO DO:

- [x] Reformat the headers to make it more readable
- [ ] Reformat the code itself to make some optimizations
- [ ] Rewrite parts of the code to move from PCRE to PCRE2
- [ ] Extend functionality
- [ ] Write a nice Graphical Interface using GTK and Glade
