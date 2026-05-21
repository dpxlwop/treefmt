/*
 * treefmt.c
 *
 * Simple recursive clang-format runner.
 *
 * Author: dpxlwop
 */

#include "treefmt.h"

int main(int argc, char** argv) {
    treefmt_flags flags = {0};
    flags.depth = -1;
    int rez;
    while ((rez = getopt_long(argc, argv, "ishgd:", long_options, NULL)) != -1) {
        switch (rez) {
            case 'i':
                flags.i = 1;
                break;
            case 's':
                flags.s = 1;
                break;
            case 'g':
                generateClangConfig();
                return 0;
            case 'd': {
                char* endptr;
                long depth = strtol(optarg, &endptr, 10);
                if (*endptr != '\0' || depth < 0 || depth > INT_MAX) {
                    fprintf(stderr, "Invalid depth value\n");
                    return 1;
                }
                flags.depth = (int)depth;
                break;
            }
            case 'h':
                fprintf(stderr, "Usage: %s [options] [start_directory] ...\n\n", argv[0]);
                fprintf(stderr, "By default, treefmt checks formatting without modifying files\n");
                fprintf(stderr,
                        "Flags:\n  -i                       - Inplace edit <file>s, if specified.\n  "
                        "-d, --depth <depth>                - Set maximum depth\n  "
                        "-s, --silent                       - Disable verbose output\n  "
                        "-g, --generate-config              - generate defaultclang-format configuration and exit\n  "
                        "-h, --help                         - display this help and exit\n");
                fprintf(stderr, "\nSupported extensions: ");
                for (int i = 0; extensions[i] != NULL; i++) {
                    fprintf(stderr, "%s ", extensions[i]);
                }
                fprintf(stderr, "\n");
                return 0;
            case '?':
                fprintf(stderr, "Unknown option '%c'\n", optopt);
                return 1;
        }
    }
    if (access(".clang-format", F_OK) != 0) {
        fprintf(stderr, "No .clang-format found. Using default clang-format configuration\n");
    }
    const char* startPath = handleStartPath(optind, argc, argv);
    recursiveWalker(startPath, flags, 0);
    return 0;
}

const char* handleStartPath(int optind, int argc, char** argv) {
    if (optind < argc) {
        return argv[optind];
    }
    return ".";
}

void generateClangConfig() {
    FILE* file = fopen(".clang-format", "w");
    if (!file) {
        fprintf(stderr, "Error: Could not create .clang-format\n");
        return;
    }
    fprintf(file,
            "---\n"
            "BasedOnStyle: Google\n"
            "IndentWidth: 4\n"
            "ColumnLimit: 110\n");
    fclose(file);
}

void recursiveWalker(const char* path, treefmt_flags flags, int currentDepth) {
    DIR* dir = opendir(path);
    if (!dir) {
        perror(path);
        return;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || entry->d_name[0] == '.') {
            continue;
        }
        char fullpath[PATH_MAX];
        int written = snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
        if (written < 0 || written >= (int)sizeof(fullpath)) {
            fprintf(stderr, "Path too long: %s/%s\n", path, entry->d_name);
            continue;
        }
        struct stat st;
        if (lstat(fullpath, &st) == -1) {
            perror(fullpath);
            continue;
        }
        if (S_ISLNK(st.st_mode)) {
            continue;
        }
        if (S_ISDIR(st.st_mode) && (flags.depth == -1 || currentDepth < flags.depth)) {
            recursiveWalker(fullpath, flags, currentDepth + 1);
        } else if (S_ISREG(st.st_mode)) {
            if (isSupportedExtension(entry->d_name)) {
                clangProcessor(fullpath, flags);
            }
        }
    }
    closedir(dir);
}

int isSupportedExtension(const char* filename) {
    for (int i = 0; extensions[i] != NULL; i++) {
        if (endsWith(filename, extensions[i])) {
            return 1;
        }
    }
    return 0;
}

void clangProcessor(const char* filepath, treefmt_flags flags) {
    if (!flags.s) {
        printf("Processing: %s\n", filepath);
    }
    if (flags.i) {
        runClangFormat(filepath, "-i");
    } else {
        runClangFormat(filepath, "-n");
    }
}

void runClangFormat(const char* filepath, const char* flag) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    if (pid == 0) {
        execlp("clang-format", "clang-format", flag, filepath, NULL);
        perror("execlp");
        exit(1);
    }
    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        fprintf(stderr, "clang-format failed: %s\n", filepath);
    }
}

int endsWith(const char* str, const char* suffix) {
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    if (suffix_len > str_len) return 0;
    return strcmp(str + str_len - suffix_len, suffix) == 0;
}