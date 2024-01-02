// rocknation_types.h
#pragma once

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

typedef struct
{
    char name[MAX_NAME_LENGTH];
    char url[MAX_URL_LENGTH];
    char genre[MAX_GENRE_LENGTH];
} BandInfo;

typedef struct
{
    BandInfo bands[10];
    int count;
} BandInfoList;

typedef struct
{
    char name[MAX_NAME_LENGTH];
    char url[MAX_URL_LENGTH];
    char year[MAX_YEAR_LENGTH];
} AlbumInfo;

typedef struct
{
    AlbumInfo albums[50];
    int count;
} AlbumInfoList;

typedef struct
{
    char url[MAX_URL_LENGTH];
    char artist[MAX_NAME_LENGTH];
    char year[MAX_YEAR_LENGTH];
    char album[MAX_NAME_LENGTH];
    char name[MAX_SONG_NAME_LENGTH];
} SongInfo;

typedef struct
{
    SongInfo songs[50];
    int count;
} SongInfoList;

typedef struct
{
    char *memory;
    size_t size;
} MemoryStruct;
