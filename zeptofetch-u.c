#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/utsname.h>

#define VERSION "v1.1"

#define C1 "\033[1;34m"
#define C2 "\033[1;37m"
#define C3 "\033[1;38;5;208m"
#define CR "\033[0m"

static char out[2048];
static int olen = 0;

static void emit(const char *s, int n) {
    if (olen + n < (int)sizeof(out)) {
        memcpy(out + olen, s, n);
        olen += n;
    }
}

#define W(s)     emit(s, sizeof(s) - 1)
#define WS(s, n) emit(s, n)

static ssize_t read_file(const char *path, char *buf, size_t sz) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) return -1;
    ssize_t n = read(fd, buf, sz - 1);
    close(fd);
    if (n > 0) buf[n] = 0;
    return n;
}

int main(int argc, char **argv) {
    if (argc > 1 && (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v"))) {
        printf("zeptofetch-u %s\n", VERSION);
        return 0;
    }

    char buf[512];
    char os[64] = "Linux";
    size_t os_len = 5;

    if (read_file("/etc/os-release", buf, sizeof(buf)) > 0) {
        char *p = strstr(buf, "PRETTY_NAME=");
        if (p) {
            p += 12;
            char q = (*p == '"' || *p == '\'') ? *p++ : 0;
            char *e = q ? strchr(p, q) : strpbrk(p, "\n");
            if (e && (size_t)(e - p) < sizeof(os)) {
                os_len = e - p;
                memcpy(os, p, os_len);
                os[os_len] = 0;
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
        if (strchr(wm, '/')) wm = strrchr(wm, '/') + 1;
        if (!strcmp(wm, "plasma")) wm = "kwin";
    } else wm = "Unknown";

    char *term = "Unknown";
    size_t term_len = 7;
    char path[64];

    snprintf(path, sizeof(path), "/proc/%d/stat", getppid());
    ssize_t n = read_file(path, buf, sizeof(buf));
    if (n > 0) {
        char *p = strrchr(buf, ')');
        if (p) {
            int term_pid;
            if (sscanf(p + 2, "%*c %d", &term_pid) == 1) {
                snprintf(path, sizeof(path), "/proc/%d/exe", term_pid);
                n = readlink(path, buf, sizeof(buf) - 1);
                if (n > 0) {
                    buf[n] = 0;
                    char *b = strrchr(buf, '/');
                    if (b) { term = b + 1; term_len = n - (b + 1 - buf); }
                    else   { term = buf;   term_len = n; }
                }
            }
        }
    }

    char title[128];
    int tlen = snprintf(title, sizeof(title), "%s@%s", usr, u.nodename);
    if (tlen < 0) tlen = 0;
    if (tlen > 127) tlen = 127;

    char dashes[128];
    memset(dashes, '-', tlen);

    size_t sh_len     = strlen(sh);
    size_t wm_len     = strlen(wm);
    size_t kernel_len = strlen(u.release);

    W(C1 "    ___ " CR "     " C1); WS(title, tlen);          W(CR "\n");
    W(C1 "   (" C2 ".· " C1 "|" CR "     ");  WS(dashes, tlen);          W("\n");
    W(C1 "   (" C3 "<>" CR " " C1 "|" CR "     " C3 "OS:" CR " ");        WS(os, os_len);        W("\n");
    W(C1 "  / " C2 "__  " C1 "\\" CR "    " C3 "Kernel:" CR " ");         WS(u.release, kernel_len); W("\n");
    W(C1 " ( " C2 "/  \\ " C1 "/|" CR "   " C3 "Shell:" CR " ");          WS(sh, sh_len);        W("\n");
    W(C3 "_" C1 "/\\ " C2 "__)" C1 "/" C3 "_" C1 ")" CR "   " C3 "WM:" CR " "); WS(wm, wm_len); W("\n");
    W(C1 C3 "\\/" C1 "-____" C3 "\\/" CR "    " C3 "Terminal:" CR " ");   WS(term, term_len);    W("\n\n");

    write(1, out, olen);
    return 0;
}
