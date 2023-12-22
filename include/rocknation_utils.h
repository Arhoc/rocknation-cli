// rocknation_utils.h
#pragma once
#include "rocknation_types.h"

char hex_to_char(const char *hex);
char *url_encode(const char *input);
char *url_encode_spaces(char *input);
char *url_decode(const char *input);
char *get_filename_from_url(const char *url);
char *replace_http(const char *url);


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