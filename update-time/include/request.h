#ifndef request_h
#define request_h

/************************************************************************
* tartget server - full url string for target time server 
* curl - simulated pass by reference pointer to CURL* 
* for error response, returns 0xFFFFFFFF 
************************************************************************/
int my_curl_init(char *targetServer , CURL **curl );

/************************************************************************
* 
* returns time in milliseconds from the last response 
* for error response, returns 0xFFFFFFFF 
************************************************************************/
uint64_t get_time();

#endif 