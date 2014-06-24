/*
 * test_curl.c
 *
 *  Created on: Jun 6, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"
#include <curl/curl.h>

int main (int argc, char **argv)
{
	test_curl(SERVER_URL);
	return 0;
}

void test_curl(char * url)
{
	  CURL *curl;
	  CURLcode res;

	  curl = curl_easy_init();
	  if(curl) {
	    curl_easy_setopt(curl, CURLOPT_URL, url);

	    /* http://google.com is redirected, so we tell libcurl to follow redirection */
	    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	    /* perform the request, res will get the return code */
	    res = curl_easy_perform(curl);

	    /* Check for errors */
	    if(res != CURLE_OK)
	      fprintf(stderr, "curl_easy_perform() failed: %s\n",
	              curl_easy_strerror(res));

	    /* cleanup */
	    curl_easy_cleanup(curl);
	  }
}
