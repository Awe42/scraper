#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

int Queue_insert(char *url);
void Queue_delete();

struct MemoryStruct
{
    char *memory;
    size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr)
    {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Global variables
CURL *curl_handle;
CURLU *urlp;
struct MemoryStruct chunk;

void setup_curl()
{
    urlp = curl_url();

    chunk.memory = malloc(1); /* will be grown as needed by the realloc above */
    chunk.size = 0;           /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    /* some servers don't like requests that are made without a user-agent
       field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
}

void set_url(char *url)
{
    /* set URL using libcurl (can handle relative paths) */
    curl_url_set(urlp, CURLUPART_URL, url, 0);
}

int get_url()
{
    char *path;
    /* get parsed URL */
    curl_url_get(urlp, CURLUPART_URL, &path, 0);
    if (!Queue_insert(path))
        return 0;
    else
        return 1;
}

int fetch_html()
{
    CURLcode res;

    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_CURLU, urlp);

    /* get it! */
    res = curl_easy_perform(curl_handle);

    /* check for errors */
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        //return 1;
    }

    /*
         * Now, our chunk.memory points to a memory block that is chunk.size
         * bytes big and contains the remote file.
         *
         * Do something nice with it!
         */
    return 0;
}

void cleanup_curl()
{
    /* cleanup curl url */
    curl_url_cleanup(urlp);

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    free(chunk.memory);

    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();
}