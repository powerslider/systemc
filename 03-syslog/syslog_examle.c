#include <syslog.h>

int main() {
    openlog ("MyProgram", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
    syslog (LOG_INFO, "Program started by User %d", getuid ());
    syslog (LOG_ERR, "Ouch an ERROR!");
    closelog ();
}
