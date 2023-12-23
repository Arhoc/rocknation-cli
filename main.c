#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "include/rocknation_types.h"
#include "include/rocknation_utils.h"
#include "include/rocknation_curl.h"

#ifdef _WIN32
    #include <direct.h>
    #define mkdir(directory, mode) _mkdir(directory)
    #define PATH_SEPARATOR '\\'
#else 
    #include <sys/stat.h>
    #include <sys/types.h>
    #define PATH_SEPARATOR '/'
#endif

char program_name[256];

void print_usage() {
    puts("[USAGE]");
    printf("%s <option> <argument_to_option>\n", program_name);
    puts("[OPTIONS]");
    puts("\tsearch-band <BAND_NAME>");
    puts("\tlist-albums <BAND_NAME/BAND_URL>");
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

void listAndPrintAlbums(const char *band) {
    AlbumInfoList albumList;

    if(strstr(band, "rocknation.su") != NULL) {
        get_albums(band, &albumList);
    } else {
        get_albums_by_name(band, &albumList);
    }

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
                #ifdef _WIN32
                    if (_mkdir(outputFolder) == 0) {
                        printf("[?] Seems like directory didn't exist yet, so we created it.\n");
                    } else {
                        printf("[!] Error creating the directory.\n");
                    }
                #else
                    if (mkdir(outputFolder, 0777) == 0) {
                        printf("[?] Seems like directory didn't exist yet, so we created it.\n");
                    } else {
                        printf("[!] Error creating the directory.\n");
                    }
                #endif

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
    strncpy(program_name, argv[0], strlen(argv[0]) + 1);

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