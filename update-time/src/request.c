#include <curl/curl.h>
#include <stdint.h>
#include <stdlib.h>

#include "request.h"

static uint64_t time_ms = 0;

size_t rspCallback(void *buffer, size_t size, size_t nmemb, void *userp)
{
    char *pEnd;
    if (size * nmemb == 13) {
        time_ms = strtoll((const char *)buffer, &pEnd, 10);
        /* Sanity check for time  - sometime between 2016 and 2030 */
        if (!((1450656000000 < time_ms) && (time_ms < 1892160000000)))
            time_ms = 0;
    }
    return nmemb * size;
}

uint64_t get_time()
{
    return time_ms;
}

int my_curl_init(char *target_server, CURL **curl)
{
    *curl = curl_easy_init();
    if (*curl) {
        curl_easy_setopt(*curl, CURLOPT_URL, target_server);
        // curl_easy_setopt(*curl, CURLOPT_SSL_VERIFYPEER,  1L);
        // curl_easy_setopt(*curl, CURLOPT_SSL_VERIFYHOST, 2L);
        curl_easy_setopt(*curl, CURLOPT_SSL_VERIFYPEER, 0 );
        curl_easy_setopt(*curl, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, rspCallback);
        return 0;
    }
    return 1;
}