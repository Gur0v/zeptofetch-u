#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/utsname.h>

#define VERSION "v1.0"

#define C1 "\033[1;34m"
#define C2 "\033[1;37m"
#define C3 "\033[1;38;5;208m"
#define CR "\033[0m"

#define W(s) write(1, s, sizeof(s) - 1)

int main(int argc, char **argv) {
    if (argc > 1 && (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v"))) {
        printf("zeptofetch-u %s\n", VERSION);
        return 0;
    }

    char os[64] = "Linux";
    char buf[512];
    
    int fd = open("/etc/os-release", O_RDONLY);
    if (fd >= 0) {
        ssize_t n = read(fd, buf, 511);
        close(fd);
        if (n > 0) {
            buf[n] = 0;
            char *p = strstr(buf, "PRETTY_NAME=\"");
            if (!p) p = strstr(buf, "PRETTY_NAME='");
            if (p) {
                p += 13;
                char *e = strchr(p, p[-1]);
                if (e && e - p < 64) {
                    size_t l = e - p;
                    memcpy(os, p, l);
                    os[l] = 0;
                }
            }
        }
    }

    struct utsname u;
    uname(&u);

    char *usr = getenv("USER");
    if (!usr) usr = getenv("LOGNAME");
    if (!usr) usr = "user";

    char *sh = getenv("SHELL");
    if (sh) {
        char *b = strrchr(sh, '/');
        sh = b ? b + 1 : sh;
    } else sh = "Unknown";

    char *wm = getenv("DESKTOP_SESSION");
    if (!wm) wm = getenv("XDG_CURRENT_DESKTOP");
    if (wm) {
        char *b = strrchr(wm, '/');
        wm = b ? b + 1 : wm;
        if (!strcmp(wm, "plasma")) wm = "kwin";
    } else wm = "Unknown";

    char term[64] = "Unknown";
    char path[64], buf2[512];
    snprintf(path, sizeof(path), "/proc/%d/stat", getppid());
    fd = open(path, O_RDONLY);
    if (fd >= 0) {
        ssize_t n = read(fd, buf2, 511);
        close(fd);
        if (n > 0) {
            buf2[n] = 0;
            char *p = strrchr(buf2, ')');
            if (p) {
                int term_pid;
                if (sscanf(p + 2, "%*c %d", &term_pid) == 1) {
                    snprintf(path, sizeof(path), "/proc/%d/exe", term_pid);
                    n = readlink(path, buf2, sizeof(buf2) - 1);
                    if (n > 0) {
                        buf2[n] = 0;
                        char *b = strrchr(buf2, '/');
                        strncpy(term, b ? b + 1 : buf2, sizeof(term) - 1);
                    }
                }
            }
        }
    }

    char title[128];
    int tlen = snprintf(title, sizeof(title), "%s@%s", usr, u.nodename);
    if (tlen < 0) tlen = 0;
    if (tlen > 127) tlen = 127;

    W(C1 "    ___ " CR "     " C1);
    write(1, title, tlen);
    W(CR "\n");
    
    W(C1 "   (" C2 ".· " C1 "|" CR "     ");
    for (int i = 0; i < tlen; i++) write(1, "-", 1);
    W("\n");
    
    W(C1 "   (" C3 "<>" CR " " C1 "|" CR "     " C3 "OS:" CR " ");
    write(1, os, strlen(os));
    W("\n");
    
    W(C1 "  / " C2 "__  " C1 "\\" CR "    " C3 "Kernel:" CR " ");
    write(1, u.release, strlen(u.release));
    W("\n");
    
    W(C1 " ( " C2 "/  \\ " C1 "/|" CR "   " C3 "Shell:" CR " ");
    write(1, sh, strlen(sh));
    W("\n");
    
    W(C3 "_" C1 "/\\ " C2 "__)" C1 "/" C3 "_" C1 ")" CR "   " C3 "WM:" CR " ");
    write(1, wm, strlen(wm));
    W("\n");
    
    W(C1 C3 "\\/" C1 "-____" C3 "\\/" CR "    " C3 "Terminal:" CR " ");
    write(1, term, strlen(term));
    W("\n\n");

    return 0;
}
