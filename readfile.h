#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>


#define USE_READ

static inline char* read_file(char const * filepath);

#ifdef USE_READ
static inline char* read_file(char const* filepath)
{
     int fd = open(filepath, O_RDONLY);
     off_t len = lseek(fd, 0, SEEK_END);
     if(len < 0) {
          fprintf(stderr, "lseek failed on file: %s with error: %s", filepath,strerror(errno));
          exit(1);
     }
     char * data = malloc(len + 1);
     lseek(fd,0,SEEK_SET);
     ssize_t read_len = read(fd, (void*) data, len);
     if (read_len != len) {
          fprintf(stderr, "expected length: %ld, read length: %ld\n", len, read_len);
          fprintf(stderr, "Read length not equal to the expected file length\n");
          exit(1);
     }
     close(fd);
     data[len]='\0';
     return data;
}

#elif defined USE_FREAD
static inline char* read_file(char const *filepath)
{
     FILE *fp = fopen(filepath, "r");
     int seek_status = fseeko(fp, 0, SEEK_END);
     if(seek_status < 0) {
          fprintf(stderr, "fseeko failed on file: %s with error: %s", filepath, strerror(errno));
          exit(1);
     }
     off_t len = ftello(fp);
     fseeko(fp, 0, SEEK_SET);
     char *data = malloc(len + 1);
     ssize_t read_len = fread(data, sizeof(char), len, fp);
     fclose(fp);
     if (read_len != len) {
          fprintf(stderr, "expected length: %ld, read length: %ld\n", len, read_len);
          fprintf(stderr, "Read length not equal to the expected file length\n");
          exit(1);
     }
     data[len]='\0';
     return data;
}

#elif defined USE_MMAP
static inline char* read_file(char const *filepath)
{
     int fd = open(filepath, O_RDONLY);
     struct stat sb;
     if(fstat(fd, &sb)){
          fprintf(stderr, "fstat error: %s", strerror(errno));
     }
     size_t len = sb.st_size;
     char *data = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
     close(fd);
     if(data == MAP_FAILED){
          fprintf(stderr, "mmap failed with error: %s", strerror(errno));
          exit(1);
     }
     return data;
}
#endif
