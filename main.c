#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Rocknation_Api.h"

void print_usage() {
    puts("[USAGE]");
    puts("./rocknation-cli <option> <argument_to_option>\n");
    puts("[OPTIONS]");
    puts("\tsearch-band <NAME>");
    puts("\tlist-albums <BAND_URL>");
    puts("\tdownload-song <URL> [OUTPUT_FILE]");
    puts("\tdownload-album <URL> [OUTPUT_FOLDER]");
}

void searchAndPrintBands(const char *searchQuery) {
    printf("Searching '%s'...\n", searchQuery);

    BandInfoList bandList;
    search_band(searchQuery, &bandList);

    if (bandList.count > 0) {
        for (int i = 0; i < bandList.count; i++) {
            printf("[*]: %s\n\tGenre: %s\n\tUrl: %s\n-----\n", bandList.bands[i].name, bandList.bands[i].genre, bandList.bands[i].url);
        }
    } else {
        printf("[!] No search results for the query '%s'.\n", searchQuery);
    }
}

void listAndPrintAlbums(const char *bandUrl) {
    AlbumInfoList albumList;
    get_albums(bandUrl, &albumList);

    if (albumList.count > 0) {
        for (int i = 0; i < albumList.count; i++) {
            printf("[*] %s:\n\tName: %s\n\tUrl: %s\n-----\n", albumList.albums[i].year, albumList.albums[i].name, albumList.albums[i].url);
        }
    } else {
        printf("[!] No album found for that band.\n");
    }
}

void downloadSong(const char *songUrl, const char *outputFile) {
    char *encodedUrl = url_encode_spaces(songUrl);
    download_file(encodedUrl, outputFile);
    free(encodedUrl);
}

void downloadAlbum(const char *albumUrl, const char *outputFolder) {
    printf("Hang on, we're downloading album\n");

    SongInfoList songList;
    get_songs(albumUrl, &songList);

    if (songList.count > 0) {
        for (int i = 0; i < songList.count; i++) {
            printf("[?] Downloading...\n\t[*] Name: %s\n\t[*] Album: %s\n\t[*] Artist: %s\n-----\n", songList.songs[i].name, songList.songs[i].album, songList.songs[i].artist);

            if (outputFolder != NULL) {
                char outputFilePath[256] = "";
                strcat(outputFilePath, outputFolder);
                strcat(outputFilePath, "/");
                strcat(outputFilePath, songList.songs[i].name);
                downloadSong(songList.songs[i].url, outputFilePath);
            } else {
                print_usage();
                return;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    if (strcmp(argv[1], "search-band") == 0) {
        searchAndPrintBands(argv[2]);
    } else if (strcmp(argv[1], "list-albums") == 0) {
        listAndPrintAlbums(argv[2]);
    } else if (strcmp(argv[1], "download-song") == 0) {
        if (argc < 3) {
            printf("Missing song URL.\n");
            print_usage();
            return 1;
        }
        const char *outputFile = (argc >= 4) ? argv[3] : NULL;
        downloadSong(argv[2], outputFile);
    } else if (strcmp(argv[1], "download-album") == 0) {
        if (argc < 3) {
            printf("Missing album URL.\n");
            print_usage();
            return 1;
        }
        const char *outputFolder = (argc >= 4) ? argv[3] : NULL;
        downloadAlbum(argv[2], outputFolder);
    } else {
        printf("Invalid option: %s\n", argv[1]);
        print_usage();
        return 1;
    }

    return 0;
}