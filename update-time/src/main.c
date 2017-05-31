#include <curl/curl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifndef __arm__
#else
#include "nb/system.h"

#endif

#include "request.h"

char *parse_args(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    long response_code;
    CURL *curl;
    CURLcode curl_code;
    uint64_t time_ms = 0;
    char *name;

    /* verify correct command line arguments */
    name = parse_args(argc, argv);
    if (name == NULL)
        return 1;

    /* perform initialization, and set options */
    my_curl_init(name, &curl);

    /* perform request; check for error */
    curl_code = curl_easy_perform(curl);
    if (curl_code != CURLE_OK)
    {
        printf("Request error: %s\n", curl_easy_strerror(curl_code));
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 2;
    }

    /* check http response status code */
    curl_code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code != 200) /* Http response code 200 = good response */
    {
        printf("Bad response: Code %ld \n", response_code);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 3;
    }
    else
    {
        time_ms = get_time();

#ifndef __arm__
        printf("The current time is  %ld \n", time_ms);
#else
        printf("The current time is  %lld \n", time_ms);
/* so we have a good response, what should we do  ? */
         /* if( time != 0 ){
         }*/

#endif
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}

char *parse_args(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("usage %s url \n", argv[0]);
    }
    else
    {
        /*basic check for url format */
        if (strncmp(argv[1], "https://", 8) == 0)
            return argv[1];
        else
            printf("argument should be a url in the form https://...\n");
    }
    return NULL;
}