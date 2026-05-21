#include <dirent.h>
#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
    int i;
    int s;
    int depth;
} treefmt_flags;

const char* extensions[] = {".c",  ".h",    ".cpp", ".hpp", ".cc", ".hh",  ".cxx", ".hxx", ".m",
                            ".mm", ".java", ".js",  ".cs",  ".ts", ".tsx", ".js",  ".jsx", NULL};

struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                {"generate-config", no_argument, 0, 'g'},
                                {"silent", no_argument, 0, 's'},
                                {"depth", required_argument, 0, 'd'},
                                {0, 0, 0, 0}};

const char* handleStartPath(int optind, int argc, char** argv);
void recursiveWalker(const char* path, treefmt_flags flags, int currentDepth);
void clangProcessor(const char* filepath, treefmt_flags flags);
void runClangFormat(const char* filepath, const char* flag);
int endsWith(const char* str, const char* suffix);
int isSupportedExtension(const char* filename);
void generateClangConfig();