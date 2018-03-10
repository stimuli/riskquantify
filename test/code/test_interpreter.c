#include <rq.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    rq_stream_t stream;
    const char *filename = "../data/test_interpreter.el";
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
        rq_interpreter_t interp = rq_interpreter_alloc();
        rq_object_t obj_script;
        rq_object_t obj_result = rq_object_nil;

        rq_interpreter_builtin_core_register(interp);
        rq_interpreter_builtin_math_register(interp);
        rq_interpreter_builtin_string_register(interp);

        obj_script = rq_interpreter_parse(interp, stream);

        while (!rq_object_is_nil(obj_script))
        {
            obj_result = rq_interpreter_eval(interp, rq_object_get_cons_car(obj_script));
            obj_script = rq_object_get_cons_cdr(obj_script);
        }

        printf("Result: %s\n", rq_object_coerce_string(obj_result));

        rq_interpreter_free(interp);

        rq_stream_close(stream);
    }
    else
    {
        printf("There was an error opening the file '%s'\n", path);
        ret = -1;
    }

    rq_stream_free(stream);
    
    return ret;
}
