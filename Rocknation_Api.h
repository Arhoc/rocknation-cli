#pragma once
#ifndef ROCKNATION_API
#define ROCKNATION_API

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curl/curl.h>
#include <pcre.h>
#include <uriparser/Uri.h>

#define MAX_NAME_LENGTH 100
#define MAX_URL_LENGTH 512
#define MAX_YEAR_LENGTH 5
#define MAX_GENRE_LENGTH 100
#define MAX_SONG_NAME_LENGTH 50

typedef struct {
    char name[MAX_NAME_LENGTH];
    char url[MAX_URL_LENGTH];
    char genre[MAX_GENRE_LENGTH];
} BandInfo;

typedef struct {
    BandInfo bands[10]; // Array to store multiple bands
    int count;
} BandInfoList;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char url[MAX_URL_LENGTH];
    char year[MAX_YEAR_LENGTH];
} AlbumInfo;

typedef struct {
    AlbumInfo albums[50];
    int count;
} AlbumInfoList;

typedef struct {
    char url[MAX_URL_LENGTH];
    char artist[MAX_NAME_LENGTH];
    char year[MAX_YEAR_LENGTH];
    char album[MAX_NAME_LENGTH];
    char name[MAX_SONG_NAME_LENGTH];
} SongInfo;

typedef struct {
    SongInfo songs[50];
    int count;
} SongInfoList;

typedef struct {
    char *memory;
    size_t size;
} MemoryStruct;

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    /* Function  : static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
     * Input     : contents - pointer to the received data
     *             size - size of each data element
     *             nmemb - number of data elements
     *             userp - pointer to a MemoryStruct structure
     * Output    : Returns the total size of the received data (in bytes)
     * Procedure : This function is a callback used with libcurl to handle the received data. It is designed to be used as the write callback for CURLOPT_WRITEFUNCTION option. The function reallocates memory to store the received data and updates the MemoryStruct structure accordingly. If a memory allocation error occurs, an error message is printed to stderr.
     */
    
    size_t real_size = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    // Reallocate memory to accommodate new data
    char *ptr = realloc(mem->memory, mem->size + real_size + 1);
    if (ptr == NULL) {
        // Error: Memory allocation failure
        fprintf(stderr, "Error de asignación de memoria\n");
        return 0;
    }

    // Update MemoryStruct with the new memory block
    mem->memory = ptr;

    // Copy the received data to the allocated memory
    memcpy(&(mem->memory[mem->size]), contents, real_size);

    // Update the total size of received data
    mem->size += real_size;

    // Null-terminate the memory block to ensure it can be treated as a string
    mem->memory[mem->size] = 0;

    // Return the total size of the received data (in bytes)
    return real_size;
}

char hex_to_char(const char *hex) {
    /* Function  : char hex_to_char(const char *hex)
     * Input     : hex - pointer to a two-character string representing a hexadecimal number
     * Output    : Returns the corresponding ASCII character value
     * Procedure : This function converts a two-character hexadecimal string to its corresponding ASCII character. It iterates through each character of the input string, converting the hexadecimal digits to their decimal equivalent. The result is the ASCII value of the represented character. If the input is not a valid hexadecimal string, the behavior is undefined.
     */
    
    int value = 0;

    // Iterate through each character in the two-character hexadecimal string
    for (int i = 0; i < 2; i++) {
        char c = hex[i];

        // Convert hexadecimal digits to their decimal equivalent
        if (isdigit(c)) {
            value = value * 16 + (c - '0');
        } else if (isxdigit(c)) {
            value = value * 16 + (tolower(c) - 'a' + 10);
        }
    }

    // Return the corresponding ASCII character value
    return (char)value;
}


char *url_encode(const char *input) {
    /*
     * Function  : char *url_encode(const char *input)
     * Input     : input - pointer to the string to be URL-encoded
     * Output    : Returns a newly allocated URL-encoded string
     * Procedure : This function URL-encodes the input string, replacing special characters with their percent-encoded equivalents. The resulting string should be freed by the caller.
     */
    
    size_t len = strlen(input);
    char *output = malloc(3 * len + 1); // Maximum possible length for URL encoding

    if (output) {
        size_t j = 0;
        for (size_t i = 0; i < len; i++) {
            if (isalnum((unsigned char)input[i]) || input[i] == '-' || input[i] == '_' || input[i] == '.' || input[i] == '~') {
                output[j++] = input[i];
            } else {
                snprintf(output + j, 4, "%%%02X", (unsigned char)input[i]);
                j += 3;
            }
        }
        output[j] = '\0';
    }

    return output;
}

char *url_encode_spaces(char *input) {
    /*
     * Function  : char *url_encode_spaces(char *input)
     * Input     : input - pointer to the string containing spaces to be URL-encoded
     * Output    : Returns a newly allocated URL-encoded string with spaces replaced by "%20"
     * Procedure : This function URL-encodes the input string, replacing spaces with "%20". The resulting string should be freed by the caller.
     */
    
    // Count the number of spaces in the input string
    int spaceCount = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == ' ') {
            spaceCount++;
        }
    }

    // Calculate the length of the new string with "%20"
    int originalLength = strlen(input);
    int newLength = originalLength + (spaceCount * 2); // Each space is replaced by "%20"

    // Allocate memory for the new string
    char *newString = (char *)malloc(newLength + 1); // +1 for the null terminator

    if (newString == NULL) {
        // Memory allocation failed
        return NULL;
    }

    // Copy characters from the original string to the new string
    int newIndex = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == ' ') {
            // Replace space with "%20"
            newString[newIndex++] = '%';
            newString[newIndex++] = '2';
            newString[newIndex++] = '0';
        } else {
            // Copy other characters as-is
            newString[newIndex++] = input[i];
        }
    }

    // Add the null terminator at the end of the new string
    newString[newIndex] = '\0';

    return newString;
}

char *url_decode(const char *input) {
    /*
     * Function  : char *url_decode(const char *input)
     * Input     : input - pointer to the URL-encoded string
     * Output    : Returns a newly allocated URL-decoded string
     * Procedure : This function URL-decodes the input string, replacing percent-encoded sequences with their original characters. The resulting string should be freed by the caller.
     */
    
    size_t len = strlen(input);
    char *output = malloc(len + 1); // Maximum possible length for URL decoding

    if (output) {
        size_t j = 0;
        for (size_t i = 0; i < len; i++) {
            if (input[i] == '%') {
                if (i + 2 < len && isxdigit(input[i + 1]) && isxdigit(input[i + 2])) {
                    char hex[3];
                    hex[0] = input[i + 1];
                    hex[1] = input[i + 2];
                    hex[2] = '\0';
                    output[j++] = hex_to_char(hex);
                    i += 2;
                } else {
                    // Invalid encoding, copy as is
                    output[j++] = input[i];
                }
            } else if (input[i] == '+') {
                output[j++] = ' ';
            } else {
                output[j++] = input[i];
            }
        }
        output[j] = '\0';
    }

    return output;
}

char *get_filename_from_url(const char *url) {
    /*
     * Function  : char *get_filename_from_url(const char *url)
     * Input     : url - pointer to the URL containing the filename
     * Output    : Returns a newly allocated string containing the filename extracted from the URL
     * Procedure : This function extracts the filename from the provided URL. The resulting string should be freed by the caller.
     */
    
    // Find the last '/' character in the URL
    const char *lastSlash = strrchr(url, '/');

    if (lastSlash != NULL) {
        // Extract the file name (after the last '/')
        const char *fileName = lastSlash + 1;

        // Decode the file name using the url_decode function
        char *decodedFileName = url_decode(fileName);

        return decodedFileName;
    } else {
        // No '/' character found, return NULL to indicate an error
        return NULL;
    }
}

char *replace_http(const char *url) {
    /*
     * Function  : char *replace_http(const char *url)
     * Input     : url - pointer to the URL to be checked and possibly modified
     * Output    : Returns a newly allocated string containing the modified URL (if replaced)
     * Procedure : This function checks if the URL starts with "http://" and replaces it with "https://". The resulting string should be freed by the caller.
     */
    
    // Check if the URL starts with "http://" and replace it with "https://"
    if (strncmp(url, "http://", 7) == 0) {
        char *https_url = (char *)malloc(strlen(url) + 1); // +1 for null terminator
        if (https_url == NULL) {
            return NULL; // Memory allocation failed
        }
        strcpy(https_url, "https://");
        strcat(https_url, url + 7); // Skip "http://"
        return https_url;
    }
    return strdup(url); // Return a copy of the original URL if it doesn't start with "http://"
}

void search_band(const char *search_text, BandInfoList *band_list) {
    /*
     * Function  : void search_band(const char *search_text, BandInfoList *band_list)
     * Input     : search_text - pointer to the text used for band search
     *             band_list - pointer to the BandInfoList structure to store search results
     * Output    : Updates the band_list with search results
     * Procedure : This function searches for bands on rocknation.su based on the provided search_text. It uses libcurl to perform an HTTP request, processes the HTML response using PCRE regular expressions, and populates the BandInfoList structure with the found bands.
     */
    
    CURL *curl;
    CURLcode res;

    MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Host: rocknation.su");
    // Add other headers here

    curl = curl_easy_init();
    if (curl) {
        char url[] = "https://rocknation.su/mp3/searchresult/";
        char postdata[MAX_URL_LENGTH];
        snprintf(postdata, sizeof(postdata), "text_mp3=%s&enter_mp3=Search", search_text);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            const char *pattern = "<a href=\"(\\/mp3\\/band-[0-9]+)\">([a-zA-Z0-9 \\/]+)<\\/a><\\/td><td>([a-zA-Z0-9 ]+)<\\/td>";
            const int options = PCRE_CASELESS;

            pcre *re;
            const char *error;
            int erroffset;

            re = pcre_compile(pattern, options, &error, &erroffset, NULL);
            if (re != NULL) {
                int ovector[30];
                int rc;
                band_list->count = 0; // Counter for bands found

                rc = pcre_exec(re, NULL, chunk.memory, chunk.size, 0, 0, ovector, 30);
                while (rc >= 0 && band_list->count < 10) {
                    char *band_url;
                    char *band_name;
                    char *genre;

                    pcre_get_substring(chunk.memory, ovector, rc, 1, &band_url);
                    pcre_get_substring(chunk.memory, ovector, rc, 2, &band_name);
                    pcre_get_substring(chunk.memory, ovector, rc, 3, &genre);

                    snprintf(band_list->bands[band_list->count].url, sizeof(band_list->bands[band_list->count].url),
                             "https://rocknation.su%s", band_url);
                    strncpy(band_list->bands[band_list->count].name, band_name,
                            sizeof(band_list->bands[band_list->count].name) - 1);
                    band_list->bands[band_list->count].name[sizeof(band_list->bands[band_list->count].name) - 1] = '\0';
                    strncpy(band_list->bands[band_list->count].genre, genre,
                            sizeof(band_list->bands[band_list->count].genre) - 1);
                    band_list->bands[band_list->count].genre[sizeof(band_list->bands[band_list->count].genre) - 1] = '\0';

                    pcre_free_substring(band_url);
                    pcre_free_substring(band_name);
                    pcre_free_substring(genre);

                    rc = pcre_exec(re, NULL, chunk.memory, chunk.size, ovector[1], 0, ovector, 30);
                    band_list->count++;
                }

                pcre_free(re);
            }
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        free(chunk.memory);
    }
}

void get_albums(char *band_url, AlbumInfoList *album_list) {
    /*
     * Function  : void get_albums(char *band_url, AlbumInfoList *album_list)
     * Input     : band_url - pointer to the URL of the band
     *             album_list - pointer to the AlbumInfoList structure to store album information
     * Output    : Updates the album_list with album information
     * Procedure : This function retrieves the list of albums for a given band from rocknation.su. It performs HTTP requests to each page of the band's albums, processes the HTML response using PCRE regular expressions, and populates the AlbumInfoList structure with the found albums.
     */
    
    int page_index = 1; // Índice de la página
    album_list->count = 0;

    while (1) {
        char page_url[MAX_URL_LENGTH];
        snprintf(page_url, sizeof(page_url), "%s/%d", band_url, page_index);

        // Realizar la solicitud HTTP a la página de la banda y buscar álbumes

        CURL *curl;
        CURLcode res;

        MemoryStruct chunk;
        chunk.memory = malloc(1);
        chunk.size = 0;

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Host: rocknation.su");
        // Agregar otros encabezados aquí
        //

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, page_url);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

            res = curl_easy_perform(curl);

            if (res == CURLE_OK) {
                const char *pattern = "<a href=\"(\\/mp3\\/album-[0-9]+)\">([0-9]+) - (.*?)<\\/a>";
                const int options = PCRE_CASELESS;

                pcre *re;
                const char *error;
                int erroffset;

                re = pcre_compile(pattern, options, &error, &erroffset, NULL);
                if (re != NULL) {
                    int ovector[30];
                    int rc;

                    rc = pcre_exec(re, NULL, chunk.memory, chunk.size, 0, 0, ovector, 30);
                    if (rc == -1) {
                        // No hay más álbumes en esta página, terminar el bucle
                        break;
                    }

                    while (rc >= 0) {
                        char *album_url;
                        char *album_name;
                        char *album_year;

                        pcre_get_substring(chunk.memory, ovector, rc, 1, &album_url);
                        pcre_get_substring(chunk.memory, ovector, rc, 2, &album_year);
                        pcre_get_substring(chunk.memory, ovector, rc, 3, &album_name);

                        snprintf(album_list->albums[album_list->count].url, sizeof(album_list->albums[album_list->count].url),
                                 "https://rocknation.su%s", album_url);
                        strncpy(album_list->albums[album_list->count].name, album_name,
                                sizeof(album_list->albums[album_list->count].name) - 1);
                        album_list->albums[album_list->count].name[sizeof(album_list->albums[album_list->count].name) - 1] = '\0';
                        strncpy(album_list->albums[album_list->count].year, album_year,
                                sizeof(album_list->albums[album_list->count].year) - 1);
                        album_list->albums[album_list->count].year[sizeof(album_list->albums[album_list->count].year) - 1] = '\0';

                        pcre_free_substring(album_url);
                        pcre_free_substring(album_year);
                        pcre_free_substring(album_name);

                        rc = pcre_exec(re, NULL, chunk.memory, chunk.size, ovector[1], 0, ovector, 30);
                        album_list->count++;
                    }

                    pcre_free(re);
                }
            }

            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            free(chunk.memory);
        }

        page_index++;
    }
}

void get_songs(const char *album_url, SongInfoList *song_list) {
    /*
     * Function  : void get_songs(const char *album_url, SongInfoList *song_list)
     * Input     : album_url - pointer to the URL of the album
     *             song_list - pointer to the SongInfoList structure to store song information
     * Output    : Updates the song_list with song information
     * Procedure : This function retrieves the list of songs for a given album from rocknation.su. It performs an HTTP request to the album page, processes the HTML response using PCRE regular expressions, and populates the SongInfoList structure with the found songs.
     */
    
    CURL *curl;
    CURLcode res;

    MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    song_list->count = 0;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Host: rocknation.su");
    // Add other headers here
    //

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, album_url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            const char *pattern = "(http:\\/\\/rocknation.su\\/upload\\/mp3\\/([a-zA-Z0-9 %]+)\\/([0-9]{4}) - ([a-zA-Z0-9 %]+)\\/([a-zA-Z0-9 %\\.]+))";
            pcre *re;
            const char *error;
            int erroffset;

            re = pcre_compile(pattern, NULL, &error, &erroffset, NULL);
            if (re != NULL) {
                int ovector[30];
                int rc;

                rc = pcre_exec(re, NULL, chunk.memory, chunk.size, 0, 0, ovector, 30);
                while (rc >= 0) {
                    char *mp3_url;
                    char *artist;
                    char *year;
                    char *album;
                    char *song_name;

                    pcre_get_substring(chunk.memory, ovector, rc, 1, &mp3_url);
                    pcre_get_substring(chunk.memory, ovector, rc, 2, &artist);
                    pcre_get_substring(chunk.memory, ovector, rc, 3, &year);
                    pcre_get_substring(chunk.memory, ovector, rc, 4, &album);
                    pcre_get_substring(chunk.memory, ovector, rc, 5, &song_name);

                    SongInfo *song = &song_list->songs[song_list->count];
                    strncpy(song->url, mp3_url, sizeof(song->url) - 1);
                    song->url[sizeof(song->url) - 1] = '\0';
                    strncpy(song->artist, artist, sizeof(song->artist) - 1);
                    song->artist[sizeof(song->artist) - 1] = '\0';
                    strncpy(song->year, year, sizeof(song->year) - 1);
                    song->year[sizeof(song->year) - 1] = '\0';
                    strncpy(song->album, album, sizeof(song->album) - 1);
                    song->album[sizeof(song->album) - 1] = '\0';
                    strncpy(song->name, url_decode(song_name), sizeof(song->name) - 1);
                    song->name[sizeof(song->name) - 1] = '\0';

                    pcre_free_substring(mp3_url);
                    pcre_free_substring(artist);
                    pcre_free_substring(year);
                    pcre_free_substring(album);
                    pcre_free_substring(song_name);

                    rc = pcre_exec(re, NULL, chunk.memory, chunk.size, ovector[1], 0, ovector, 30);
                    song_list->count++;
                }

                pcre_free(re);
            }
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        free(chunk.memory);
    }
}

int download_file(const char *url, char *output_file) {
    /*
     * Function  : int download_file(const char *url, char *output_file)
     * Input     : url - pointer to the URL of the file to download
     *             output_file - pointer to the name of the file to save the downloaded content
     * Output    : Downloads the file and returns 0 on success, -1 on failure
     * Procedure : This function downloads a file from the given URL using libcurl. It writes the content to the specified output file. If the output_file is NULL, the function attempts to derive the filename from the URL. The function returns 0 on success and -1 on failure.
     */
    
    CURL *curl;
    CURLcode res;

    MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    if (output_file == NULL) {
        output_file = get_filename_from_url(url);
    }

    url = replace_http(url);

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            FILE *file = fopen(output_file, "wb");
            if (file) {
                size_t bytes_written = fwrite(chunk.memory, 1, chunk.size, file);
                fclose(file);

                if (bytes_written == chunk.size) {
                    printf("File downloaded successfully: %s\n", output_file);
                } else {
                    printf("Error writing file\n");
                }
            } else {
                printf("Error opening file for writing\n");
            }
        } else {
            printf("curl_easy_perform failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        free(chunk.memory);
    }

    return 0;
}

#endif
