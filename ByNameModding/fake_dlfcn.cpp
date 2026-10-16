#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>
#include <dlfcn.h>
#include <sys/system_properties.h>
#include <android/log.h>

#define TAG_NAME "dlfcn_ex"
#define LOG_DBG
#ifdef LOG_DBG
#define log_info(fmt, args...) __android_log_print(ANDROID_LOG_INFO, TAG_NAME, (const char *) fmt, ##args)
#define log_err(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG_NAME, (const char *) fmt, ##args)
#define log_dbg log_info
#else
#define log_dbg(...)
#define log_info(fmt, args...)
#define log_err(fmt, args...)
#endif

#ifdef __LP64__
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Shdr Elf64_Shdr
#define Elf_Sym  Elf64_Sym
#define Elf_Phdr Elf64_Phdr
#else
#define Elf_Ehdr Elf32_Ehdr
#define Elf_Shdr Elf32_Shdr
#define Elf_Sym  Elf32_Sym
#define Elf_Phdr Elf32_Phdr
#endif

struct ctx {
    void *load_addr;
    void *dynstr;
    void *dynsym;
    int nsyms;
    off_t bias;
};

extern "C" {

static int fake_dlclose(void *handle) {
    if (handle) {
        struct ctx *ctx = (struct ctx *) handle;
        if (ctx->dynsym) free(ctx->dynsym);    /* we're saving dynsym and dynstr */
        if (ctx->dynstr) free(ctx->dynstr);    /* from library file just in case */
        free(ctx);
    }
    return 0;
}

/* flags are ignored */
static void *fake_dlopen_with_path(const char *libpath, int flags) {
    FILE *maps;
    char buff[256], name[256], chosen[256], fallback[256];
    struct ctx *ctx = 0;
    off_t load_addr = 0, map_off = 0, size;
    int k, fd = -1, found = 0, have_fallback = 0;
    char *shoff;
    Elf_Ehdr *elf = (Elf_Ehdr *) MAP_FAILED;

#define fatal(fmt, args...) do { log_err(fmt,##args); goto err_exit; } while(0)

    maps = fopen("/proc/self/maps", "r");
    if (!maps) fatal("failed to open maps");

    /* Pick the *executable* mapping. A read-only (r--p) mapping with file
     * offset 0 may be a plain mmap of the whole file (e.g. created to parse
     * its ELF); resolving symbols against it yields non-executable addresses
     * and crashes with SEGV_ACCERR on the first call. Only fall back to r--p
     * if no executable mapping exists. */
    chosen[0] = fallback[0] = '\0';
    while (fgets(buff, sizeof(buff), maps)) {
        if (!strstr(buff, libpath)) continue;
        if (strstr(buff, "r-xp")) { strncpy(chosen, buff, sizeof(chosen) - 1); chosen[sizeof(chosen)-1] = '\0'; found = 1; break; }
        if (!have_fallback && strstr(buff, "r--p")) {
            strncpy(fallback, buff, sizeof(fallback) - 1); fallback[sizeof(fallback)-1] = '\0';
            have_fallback = 1;
        }
    }
    if (!found && have_fallback) {
        strncpy(chosen, fallback, sizeof(chosen) - 1); chosen[sizeof(chosen)-1] = '\0';
        found = 1;
    }
    fclose(maps);

    if (!found) fatal("%s not found in my userspace", libpath);

    if (sscanf(chosen, "%lx-%*lx %*s %lx %*s %*s %s", &load_addr, &map_off, name) != 3)
        fatal("failed to read load address for %s", libpath);

    log_info("%s loaded in Android at 0x%08lx (file offset 0x%08lx)", libpath, load_addr, map_off);

    libpath = name;

    /* Now, mmap the same library once again */

    fd = open(libpath, O_RDONLY);
    if (fd < 0) fatal("failed to open %s", libpath);

    size = lseek(fd, 0, SEEK_END);
    if (size <= 0) fatal("lseek() failed for %s", libpath);

    elf = (Elf_Ehdr *) mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    fd = -1;

    if (elf == MAP_FAILED) fatal("mmap() failed for %s", libpath);

    ctx = (struct ctx *) calloc(1, sizeof(struct ctx));
    if (!ctx) fatal("no memory for %s", libpath);

    ctx->load_addr = (void *) load_addr;
    shoff = ((char *) elf) + elf->e_shoff;

    for (k = 0; k < elf->e_shnum; k++, shoff += elf->e_shentsize) {

        Elf_Shdr *sh = (Elf_Shdr *) shoff;
        log_dbg("%s: k=%d shdr=%p type=%x", __func__, k, sh, sh->sh_type);

        switch (sh->sh_type) {

            case SHT_DYNSYM:
                if (ctx->dynsym) fatal("%s: duplicate DYNSYM sections", libpath); /* .dynsym */
                ctx->dynsym = malloc(sh->sh_size);
                if (!ctx->dynsym) fatal("%s: no memory for .dynsym", libpath);
                memcpy(ctx->dynsym, ((char *) elf) + sh->sh_offset, sh->sh_size);
                ctx->nsyms = (sh->sh_size / sizeof(Elf_Sym));
                break;

            case SHT_STRTAB:
                if (ctx->dynstr) break;    /* .dynstr is guaranteed to be the first STRTAB */
                ctx->dynstr = malloc(sh->sh_size);
                if (!ctx->dynstr) fatal("%s: no memory for .dynstr", libpath);
                memcpy(ctx->dynstr, ((char *) elf) + sh->sh_offset, sh->sh_size);
                break;

            case SHT_PROGBITS:
                if (!ctx->dynstr || !ctx->dynsym) break;
                /* don't derive the load bias from an arbitrary section: a
                 * non-allocated PROGBITS (e.g. .comment, sh_addr == 0) placed
                 * after .dynsym/.dynstr produces a huge negative bias that
                 * shifts every resolved symbol into non-executable memory.
                 * The bias comes from the program headers below. */
                k = elf->e_shnum;  /* exit for */
                break;
        }
    }

    /* Derive the load bias from the loadable segment that matches the chosen
     * mapping's file offset: runtime(sym) = load_addr + st_value - bias, where
     * load_addr corresponds to p_vaddr of the matched PT_LOAD segment. This is
     * correct regardless of which segment the mapping belongs to. */
    if (elf->e_phoff && elf->e_phnum) {
        Elf_Phdr *ph = (Elf_Phdr *) (((char *) elf) + elf->e_phoff);
        off_t min_vaddr = 0;
        int have_load = 0, matched = 0;
        for (k = 0; k < elf->e_phnum; k++) {
            if (ph[k].p_type != PT_LOAD) continue;
            if (!have_load || (off_t) ph[k].p_vaddr < min_vaddr) {
                min_vaddr = (off_t) ph[k].p_vaddr;
                have_load = 1;
            }
            if ((off_t) ph[k].p_offset == map_off) {
                ctx->bias = (off_t) ph[k].p_vaddr;
                matched = 1;
            }
        }
        if (!matched) ctx->bias = have_load ? min_vaddr : 0;
    }

    munmap(elf, size);
    elf = 0;

    if (!ctx->dynstr || !ctx->dynsym) fatal("dynamic sections not found in %s", libpath);

#undef fatal

    log_dbg("%s: ok, dynsym = %p, dynstr = %p, load_addr = %p, bias = %ld",
            libpath, ctx->dynsym, ctx->dynstr, ctx->load_addr, (long) ctx->bias);

    return ctx;

    err_exit:
    if (fd >= 0) close(fd);
    if (elf != MAP_FAILED) munmap(elf, size);
    fake_dlclose(ctx);
    return 0;
}


#if defined(__LP64__)
static const char *const kSystemLibDir = "/system/lib64/";
static const char *const kOdmLibDir = "/odm/lib64/";
static const char *const kVendorLibDir = "/vendor/lib64/";
static const char *const kApexLibDir = "/apex/com.android.runtime/lib64/";
static const char *const kApexArtNsLibDir = "/apex/com.android.art/lib64/";
#else
static const char *const kSystemLibDir = "/system/lib/";
static const char *const kOdmLibDir = "/odm/lib/";
static const char *const kVendorLibDir = "/vendor/lib/";
static const char *const kApexLibDir = "/apex/com.android.runtime/lib/";
static const char *const kApexArtNsLibDir = "/apex/com.android.art/lib/";
#endif

static void *fake_dlopen(const char *filename, int flags) {
    if (strlen(filename) > 0 && filename[0] == '/') {
        return fake_dlopen_with_path(filename, flags);
    } else {
        char buf[512] = {0};
        void *handle = NULL;
        //sysmtem
        strcpy(buf, kSystemLibDir);
        strcat(buf, filename);
        handle = fake_dlopen_with_path(buf, flags);
        if (handle) {
            return handle;
        }

        // apex in ns com.android.runtime
        memset(buf, 0, sizeof(buf));
        strcpy(buf, kApexLibDir);
        strcat(buf, filename);
        handle = fake_dlopen_with_path(buf, flags);
        if (handle) {
            return handle;
        }

        // apex in ns com.android.art
        memset(buf, 0, sizeof(buf));
        strcpy(buf, kApexArtNsLibDir);
        strcat(buf, filename);
        handle = fake_dlopen_with_path(buf, flags);
        if (handle) {
            return handle;
        }

        //odm
        memset(buf, 0, sizeof(buf));
        strcpy(buf, kOdmLibDir);
        strcat(buf, filename);
        handle = fake_dlopen_with_path(buf, flags);
        if (handle) {
            return handle;
        }

        //vendor
        memset(buf, 0, sizeof(buf));
        strcpy(buf, kVendorLibDir);
        strcat(buf, filename);
        handle = fake_dlopen_with_path(buf, flags);
        if (handle) {
            return handle;
        }

        return fake_dlopen_with_path(filename, flags);
    }
}

static void *fake_dlsym(void *handle, const char *name) {
    int k;
    struct ctx *ctx = (struct ctx *) handle;
    Elf_Sym *sym = (Elf_Sym *) ctx->dynsym;
    char *strings = (char *) ctx->dynstr;

    for (k = 0; k < ctx->nsyms; k++, sym++) {
        if (sym->st_shndx == SHN_UNDEF) continue;   /* undefined import, not a definition */
        if (strcmp(strings + sym->st_name, name) == 0) {
            /*  NB: sym->st_value is an offset into the section for relocatables,
            but a VMA for shared libs or exe files, so we have to subtract the bias */
            void *ret = (char *) ctx->load_addr + sym->st_value - ctx->bias;
            log_info("%s found at %p (load_addr=%p bias=%ld st_value=%lx)", name, ret,
                     ctx->load_addr, (long) ctx->bias, (unsigned long) sym->st_value);
            return ret;
        }
    }
    log_info("%s NOT found", name);
    return 0;
}

static const char *fake_dlerror() {
    return NULL;
}

// =============== implementation for compat ==========
static int SDK_INT = -1;
static int get_sdk_level() {
    if (SDK_INT > 0) {
        return SDK_INT;
    }
    char sdk[PROP_VALUE_MAX] = {0};;
    __system_property_get("ro.build.version.sdk", sdk);
    SDK_INT = atoi(sdk);
    return SDK_INT;
}

int dlclose_ex(void *handle) {
    if (get_sdk_level() >= 24) {
        return fake_dlclose(handle);
    } else {
        return dlclose(handle);
    }
}

void *dlopen_ex(const char *filename, int flags) {
    log_info("dlopen: %s", filename);
    if (get_sdk_level() >= 24) {
        return fake_dlopen(filename, flags);
    } else {
        return dlopen(filename, flags);
    }
}

void *dlsym_ex(void *handle, const char *symbol) {
    if (get_sdk_level() >= 24) {
        return fake_dlsym(handle, symbol);
    } else {
        return dlsym(handle, symbol);
    }
}

const char *dlerror_ex() {
    if (get_sdk_level() >= 24) {
        return fake_dlerror();
    } else {
        return dlerror();
      }
   }
}
