#include <rq.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    rq_stream_t stream;
    const char *filename = "../data/stream1.txt";
    char path[4096];
    int ret = 0;

    if (argc >= 2)
        strcpy(path, argv[1]);
    else
    {
        char *srcdir = getenv("srcdir");
        if (srcdir)
        {
            /* printf("srcdir = '%s'\n", srcdir); */
            strcpy(path, srcdir);
            strcat(path, "/");
            strcat(path, filename);
            /* printf("path = '%s'\n", path); */
        }
        else
        {
            /* printf("Couldn't get srcdir environment variable\n"); */
            strcpy(path, filename);
        }
    }

    stream = rq_stream_file_alloc();
    rq_stream_file_set_filename(stream, path);
    rq_stream_file_set_openmode(stream, RQ_STREAM_FILE_OPENMODE_READING);

    if (rq_stream_open(stream) == RQ_OK)
    {
        char buf[20];
        int ret;
        do
        {
            ret = rq_stream_read_line(stream, buf, 20);
            printf("%s", buf);
        }
        while (ret > 0);

        rq_stream_close(stream);
    }
    rq_stream_free(stream);

    return 0;
}
