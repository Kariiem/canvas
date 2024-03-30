#ifndef MACROS_H
#define MACROS_H

#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define CHECK_SHADER_ERRORS(type, status, id, success)          \
    do {                                                        \
        GLchar info_log[1024];                                  \
        glGet##type##iv(id, status, &success);                  \
        if (!success) {                                         \
            glGet##type##InfoLog(id, 1024, NULL, info_log);     \
            eprintf("Error:: %s error :: %s \n",                \
                    #type, #id);                                \
            eprintf("InfoLog:: %s\n",info_log);                 \
        }                                                       \
    } while (0)

/* =========================================================  */


#define PRAGMA_DISABLE_INCOMPAT_TYPE_WARNING                            \
    _Pragma("GCC diagnostic push");                                     \
    _Pragma("GCC diagnostic ignored \"-Wincompatible-pointer-types\"")
#define PRAGMA_REENABLE_INCOMPAT_TYPE_WARNING   \
    _Pragma("GCC diagnostic pop")

#define PRAGMA_DISABLE_MISSING_BRACES_WARNING                   \
    _Pragma("GCC diagnostic push");                             \
    _Pragma("GCC diagnostic ignored \"-Wmissing-braces\"")
#define PRAGMA_REENABLE_MISSING_BRACES_WARNING  \
    _Pragma("GCC diagnostic pop")



#define CONCAT_(a, b) a##b
#define CONCAT(a, b) CONCAT_(a, b)

#define IGNORE(param) ((void)param)

#endif /* MACROS_H */
