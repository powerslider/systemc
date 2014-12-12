#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <linux/inotify.h>

#define EVENT_SIZE (sizeof (struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

int main(int argc, char **argv) {
    int file_desc, watch_dir, length, i = 0;
    char event_buffer[EVENT_BUF_LEN];
    char* dir = argv[1];

    if (dir == NULL) {
        perror("Please specify a directory to be watched by INotify!");
        exit(1);
    }

    file_desc = inotify_init();
    if (file_desc < 0) {
        perror("Error cannot initialise INotify instance!");
    }

    watch_dir = inotify_add_watch(file_desc, dir, IN_CREATE | IN_DELETE);

    length = read(watch_dir, event_buffer, EVENT_BUF_LEN);
    if (length < 0) {
        perror("Error reading inotify_event stuctures!");
    }

    while (i < length) {
        struct inotify_event *event = (struct inotify_event *) &event_buffer[i];
        if ( event->len ) {
            if (event->mask & IN_CREATE) {
                if (event->mask & IN_ISDIR) {
                     printf( "New directory %s created.\n", event->name );
                } else {
                    printf("New file %s created.\n", event->name);
                }
            } else if ( event->mask & IN_DELETE ) {
                if ( event->mask & IN_ISDIR ) {
                    printf( "Directory %s deleted.\n", event->name );
                } else {
                    printf( "File %s deleted.\n", event->name );
                }
            }
        }
        i += EVENT_SIZE + event->len;
    }

   inotify_rm_watch(file_desc, watch_dir);

   close(file_desc);
}
