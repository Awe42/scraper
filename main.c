#include "curl.c"
#include "htmlparser.c"
#include "crawl_queue.h"

int main()
{
    char *base_url = "https://www.iana.org";
    int return_code;

    create_Queue(base_url);
    setup_curl();
    int k = 0;

    while (front || MAX_SIZE)
    {
        //printf("Inside Loop%d",++k);
        set_url(front->url);

        return_code = fetch_html();

        if (return_code == 0)
        {
            if (!searchForImg(chunk.memory, chunk.size, front->url))
                goto delete;
        }
        delete : Queue_delete();
    }

    cleanup_curl();

    return 0;
}