#include <rq.h>

int
main(int argc, char **argv)
{
    rq_type_id_mgr_t type_id_mgr = rq_type_id_mgr_alloc(RQ_ID_TYPE_BITMAP);
    long i;
    int failed = 0;

    for (i = 1; i <= 0x04000000; i <<= 1)
    {
        char buf[1024];
        long id;
        sprintf(buf, "%lu", i);
        id = rq_type_id_mgr_add_type(
            type_id_mgr,
            buf
            );

        if (id != atol(buf))
        {
            printf("Error: Bad return value for bitmap type ID %ld instead of %ld\n", id, i);
            failed = 1;
            break;
        }

        /* printf("Got type id %lu for type '%s'\n", id, buf); */
    }

    if (!failed)
    {
        for (i = 1; i <= 0x04000000; i <<= 1)
        {
            char buf[1024];
            long id;
            sprintf(buf, "%lu", i);
            id = rq_type_id_mgr_get_id_for_type(
                type_id_mgr,
                buf
                );

            if (atol(buf) != id)
            {
                printf("The ID for %s doesnt match it's returned ID %lu\n", buf, id);
                failed = 1;
            }
        }
    }

    if (!failed)
    {
        for (i = 1; i <= 0x04000000; i <<= 1)
        {
            const char *name = rq_type_id_mgr_get_type_for_id(
                type_id_mgr,
                i
                );

            if (atol(name) != i)
            {
                printf("The returned name '%s' doesnt match the requested ID %lu\n", name, i);
                failed = 1;
            }
        }
    }

    rq_type_id_mgr_free(type_id_mgr);

    return failed;
}
