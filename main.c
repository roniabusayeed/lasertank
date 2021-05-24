#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
    /* Declare filenames and file pointers. */
    char const* map_filename = NULL;
    const char* log_filename = NULL;
    FILE* map = NULL;
    FILE* log = NULL;

    /* Ensure proper usage. */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s %s %s\n", argv[0], "<map-filename>","<log-filename>");
        return EXIT_FAILURE;
    }

    /* Remember map filename. */
    map_filename = argv[1];
    
    /* Remember log filename. */
    log_filename = argv[2];

    /* Open map file for reading. */
    map = fopen(map_filename, "r");
    if (! map)
    {
        fprintf(stderr, "Couldn't open %s for reading.\n", map_filename);
        return EXIT_FAILURE;
    }

    /* Open log file for writing. */
    log = fopen(log_filename, "w");
    if (! log)
    {
        fprintf(stderr, "Couldn't open %s for writing.\n", log_filename);
        return EXIT_FAILURE;
    }

    

    /* Success. */
    return EXIT_SUCCESS;
}