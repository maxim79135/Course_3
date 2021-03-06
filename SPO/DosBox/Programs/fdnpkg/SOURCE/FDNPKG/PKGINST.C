/*
 * This file is part of FDNPKG
 * Copyright (C) 2012-2017 Mateusz Viste
 */

#include <stdio.h>
#include <stdlib.h>    /* system() */
#include <string.h>    /* strcpy() */
#include <unistd.h>    /* read() */
#include <sys/types.h> /* struct utimbuf */

#include "crc32.h"     /* all CRC32 related stuff */
#include "fdnpkg.h"    /* PKGINST_NOSOURCE, PKGINST_SKIPLINKS... */
#include "http.h"
#include "helpers.h"   /* slash2backslash(), strtolower() */
#include "fileexst.h"
#include "kprintf.h"
#include "libunzip.h"  /* zip_listfiles()... */
#include "showinst.h"  /* pkg_loadflist() */
#include "pkginst.h"   /* include self for control */
#include "version.h"


/* return 1 if fname looks like a link filename, 0 otherwise */
static int islinkfile(char *fname) {
  char *link1 = "LINKS\\";
  char *link2 = "links\\";
  int x;
  for (x = 0; ; x++) {
    if (link1[x] == 0) return(1);
    if ((fname[x] != link1[x]) && (fname[x] != link2[x])) return(0);
  }
}


/* validate a filename (8+3, no weird characters, etc). returns 0 on success,
 * nonzero otherwise. */
static int validfilename(char *fname) {
  int i, i2;
  char *validchars = "!#$%&'()-@^_`{}~";
  int elemlen = 0;
  int elemmaxlen = 8; /* switches from 8 to 3 depending wheter I am analyzing
                         a filename or an extension */
  /* look for invalid chars in the entire string, and check the length of the
   * element at the same time */
  for (i = 0; fname[i] != 0; i++) {
    /* director separators are threated specially */
    if (fname[i] == '\\') {
      elemlen = 0;
      elemmaxlen = 8;
      continue;
    }
    /* '.' switches the check into extension mode */
    if (fname[i] == '.') {
      if (elemlen == 0) return(-1); /* a file must have a base name */
      if (elemmaxlen == 3) return(-2); /* a file cannot have two extensions */
      elemlen = 0;
      elemmaxlen = 3;
      continue;
    }
    /* check that the element is not too long */
    if (++elemlen > elemmaxlen) return(-3);
    /* look for valid characters */
    if ((fname[i] >= 'a') && (fname[i] <= 'z')) continue;
    if ((fname[i] >= 'A') && (fname[i] <= 'Z')) continue;
    if ((fname[i] >= '0') && (fname[i] <= '9')) continue;
    if ((fname[i] & 128) != 0) continue; /* high bytes are okay (NLS) */
    /* look for other valid characters */
    for (i2 = 0; validchars[i2] != 0; i2++) {
      if (fname[i] == validchars[i2]) break;
    }
    if (validchars[i2] != 0) continue;
    /* if we are here, then the character is invalid */
    return(-4);
  }
  /* all checks passed */
  return(0);
}


/* processes a link file - that is, reads the target inside, and overwrite
 * the file with new content */
static void processlinkfile(char *linkfile, char *dosdir, struct customdirs *dirlist, char *buff) {
  char origtarget[512];
  int x;
  char *shortfile;
  unsigned char comstub[38] = { /* machine code of a COM stub launcher */
    0xB4,0x4A,0xBB,0x28,0x00,0xCD,0x21,0x8C,0x1E,0x1C,0x01,0xB8,0x00,0x4B,0xBA,0x26,
    0x01,0xBB,0x18,0x01,0xCD,0x21,0xCD,0x20,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00}; /* read comlink.asm for details */
  FILE *fd;
  /* open the link file and read the original target */
  fd = fopen(linkfile, "r");
  if (fd == NULL) {
    kitten_printf(3, 21, "Error: Failed to open link file '%s' for read access.", linkfile);
    puts("");
    return;
  }
  x = fread(origtarget, 1, sizeof(origtarget) - 1, fd);
  origtarget[x] = 0;
  fclose(fd);
  /* validate the original target (ltrim to first \r or \n) */
  for (x = 0; origtarget[x] != 0; x++) {
    if ((origtarget[x] == '\r') || (origtarget[x] == '\n')) {
      origtarget[x] = 0;
      break;
    }
  }
  /* translate the original target to a local path */
  shortfile = computelocalpath(origtarget, buff, dosdir, dirlist);
  /* write new content into the link file */
  fd = fopen(linkfile, "wb");
  if (fd == NULL) {
    kitten_printf(3, 22, "Error: Failed to open link file '%s' for write access.", linkfile);
    puts("");
    return;
  }
  fwrite(comstub, 1, sizeof(comstub), fd);
  fprintf(fd, "%s%s%c", buff, shortfile, 0);
  fclose(fd);
}


/* returns 0 if pkgname is not installed, non-zero otherwise */
int is_package_installed(char *pkgname, char *dosdir) {
  char fname[512];
  sprintf(fname, "%s\\packages\\%s.lst", dosdir, pkgname);
  if (fileexists(fname) != 0) { /* file exists -> package is installed */
    return(1);
  } else {
    return(0);
  }
}


/* checks that pkgname is NOT installed. return 0 on success, non-zero otherwise. */
int validate_package_not_installed(char *pkgname, char *dosdir) {
  if (is_package_installed(pkgname, dosdir) != 0) {
    kitten_printf(3, 18, "Package %s is already installed! You might want to use the 'update' action.", pkgname);
    puts("");
    return(-1);
  }
  return(0);
}


/* find a filename in a flist linked list, and returns a pointer to it */
struct flist_t *findfileinlist(struct flist_t *flist, char *fname) {
  while (flist != NULL) {
    if (strcmp(flist->fname, fname) == 0) return(flist);
    flist = flist->next;
  }
  return(NULL);
}


/* prepare a package for installation. this is mandatory before actually installing it!
 * returns a pointer to the zip file's index on success, NULL on failure. the *zipfile pointer is updated with a file descriptor to the open zip file to install. */
struct ziplist *pkginstall_preparepackage(struct pkgdb *pkgdb, char *pkgname, char *tempdir, char *localfile, int flags, char **repolist, FILE **zipfd, char *proxy, int proxyport, char *downloadingstring, char *dosdir, struct customdirs *dirlist, char *buffmem1k) {
  char *fname;
  char *zipfile;
  char *appinfofile;
  int appinfopresence;
  char *shortfile;
  struct ziplist *ziplinkedlist, *curzipnode, *prevzipnode;
  struct flist_t *flist;

  fname = buffmem1k;
  zipfile = buffmem1k + 256;
  appinfofile = buffmem1k + 512;

  strtolower(pkgname); /* convert pkgname to lower case, because the http repo is probably case sensitive */
  sprintf(appinfofile, "appinfo\\%s.lsm", pkgname); /* Prepare the appinfo/xxxx.lsm filename string for later use */

  /* check if not already installed, if already here, print a message "you might want to use update instead"
   * of course this must not be done if we are in the process of upgrading said package */
  if (((flags & PKGINST_UPDATE) == 0) && (validate_package_not_installed(pkgname, dosdir) != 0)) {
    return(NULL);
  }

  if (localfile != NULL) {  /* if it's a local file, then we will have to skip all the network stuff */
    strcpy(zipfile, localfile);
  } else {
    zipfile[0] = 0;
  }

#ifndef NOREPOS
  if (zipfile[0] == 0) { /* need to download the package from a repository */
    char *instrepo;
    struct pkgdb *pkgnode, *lastnode;
    struct pkgrepo *pkgrepo;
    int repoid;
    unsigned long zipfilecrc, remotecrc;
    unsigned char *buff;
    int buffreadres;
    char *pkgext; /* zip or zib */

    /* look into the db to find the package */
    pkgnode = findpkg(pkgdb, pkgname, &lastnode);
    if (pkgnode == NULL) { /* no such package found in repositories */
      kitten_printf(3, 1, "No package '%s' found in online repositories.", pkgname);
      puts("");
      return(NULL);
    }

    /* if found - check the list of repositories */
    if (pkgnode->repolist == NULL) {
      kitten_printf(3, 2, "Package '%s' is not available in repositories.", pkgname);
      puts("");
      return(NULL);
    }

    if (pkgnode->repolist->nextrepo != NULL) { /* available from more than 1 repo.. */
      char userchoicestr[8];
      int userchoice, latestver_repoid = 1;
      struct pkgrepo *xrepo, *latestver = pkgnode->repolist;
      /* check out if we are able to find out the newest version */
      repoid = 2; /* setting to 2, because we start iterating at the second repo entry */
      for (xrepo = pkgnode->repolist->nextrepo; xrepo != NULL; xrepo = xrepo->nextrepo) {
        int versionnewerres = isversionnewer(latestver->version, xrepo->version);
        if (versionnewerres > 0) {
          latestver = xrepo;
          latestver_repoid = repoid;
        } else if (versionnewerres < 0) {  /* unable to tell which version is newer */
          latestver_repoid = -1;
          break;
        }
        repoid += 1;
      }
      if (latestver_repoid > 0) {
        repoid = latestver_repoid;
      } else { /* newest version could not be figured out, so let's ask the user to choose */
        puts("");
        kitten_printf(3, 3, "%s is available from several repositories. Choose which one to use:", pkgname);
        puts("");
        repoid = 1;
        for (xrepo = pkgnode->repolist; xrepo != NULL; xrepo = xrepo->nextrepo) {
          printf(" %d) %s %s (%s)\n", repoid, pkgnode->name, xrepo->version, repolist[xrepo->repo]);
          repoid += 1;
        }
        for (;;) {
          kitten_printf(3, 4, "Your choice:");
          printf(" ");
          fgets(userchoicestr, 6, stdin);
          userchoice = atoi(userchoicestr);
          if ((userchoice < 1) || (userchoice >= repoid)) {
            kitten_puts(3, 5, "Invalid choice!");
          } else {
            break;
          }
        }
        repoid = userchoice;
      }
    } else { /* available only from one repository - get it there */
      repoid = 1;
    }
    pkgrepo = pkgnode->repolist;
    for (; repoid > 1; repoid--) pkgrepo = pkgrepo->nextrepo;
    instrepo = repolist[pkgrepo->repo];

    /* select the package extension: zip or zib */
    if ((flags & PKGINST_NOSOURCE) && (pkgrepo->crc32zib != 0)) { /* use zib if available and if no sources needed */
      pkgext = "zib"; /* zib is the same thing as zip, but binary-only (no sources) */
      remotecrc = pkgrepo->crc32zib;
    } else { /* otherwise use the full-blown zip package */
      pkgext = "zip";
      remotecrc = pkgrepo->crc32zip;
    }

    /* if it's a network repo, download the package from repoid into the temp directory */
    if (detect_localpath(instrepo) == 0) {
      sprintf(fname, "%s%s.%s", instrepo, pkgname, pkgext);
      sprintf(zipfile, "%s\\fdnpkg.tmp", tempdir);
      kitten_printf(3, 6, "Downloading package %s...", fname);
      puts("");
      if (http_get(fname, zipfile, proxy, proxyport, downloadingstring) <= 0) {
        kitten_puts(3, 7, "Error downloading package. Aborted.");
        return(NULL);
      }
    } else { /* else it's an on-disk repo, so we can use the package right from there */
      sprintf(zipfile, "%s%s.%s", instrepo, pkgname, pkgext);
    }
    /* check the CRC of the downloaded file */
    buff = malloc(4096);  /* use a 4K buffer to compute file's CRC */
    if (buff == NULL) {
      kitten_puts(3, 15, "Error: Out of memory while computing the CRC of the package!");
      return(NULL);
    }
    *zipfd = fopen(zipfile, "rb");
    if (*zipfd == NULL) {
      kitten_puts(3, 14, "Error: Failed to open the downloaded package. Installation aborted.");
      free(buff);
      return(NULL);
    }
    zipfilecrc = crc32_init();
    while ((buffreadres = read(fileno(*zipfd), buff, 4096)) > 0) {
      crc32_feed(&zipfilecrc, buff, buffreadres);
    }
    crc32_finish(&zipfilecrc);
    fclose(*zipfd);
    free(buff);
    if (zipfilecrc != remotecrc) {
      kitten_puts(3, 13, "Error: Downloaded package had wrong CRC. Installation aborted.");
      return(NULL);
    }
  } /* if (zipfile[0] == 0) */
#endif

  /* Now let's check the content of the zip file */

  *zipfd = fopen(zipfile, "rb");
  if (*zipfd == NULL) {
    kitten_puts(3, 8, "Error: Invalid zip archive! Package not installed.");
    return(NULL);
  }
  ziplinkedlist = zip_listfiles(*zipfd);
  if (ziplinkedlist == NULL) {
    kitten_puts(3, 8, "Error: Invalid zip archive! Package not installed.");
    fclose(*zipfd);
    return(NULL);
  }
  /* if updating, load the list of files belonging to the current package */
  if ((flags & PKGINST_UPDATE) != 0) {
    flist = pkg_loadflist(pkgname, dosdir);
  } else {
    flist = NULL;
  }
  /* Verify that there's no collision with existing local files, look for the appinfo presence, get rid of sources if required, and rename BAT links into COM files */
  appinfopresence = 0;
  prevzipnode = NULL;
  for (curzipnode = ziplinkedlist; curzipnode != NULL;) {
    /* change all slashes to backslashes, and switch into all-lowercase */
    slash2backslash(curzipnode->filename);
    strtolower(curzipnode->filename);
    /* remove 'directory' ZIP entries to avoid false alerts about directory already existing */
    if ((curzipnode->flags & ZIP_FLAG_ISADIR) != 0) {
      curzipnode->filename[0] = 0; /* mark it "empty", will be removed in a short moment */
    }
    /* if --nosource specified, skip sources */
    if ((flags & PKGINST_NOSOURCE) != 0) {
      if (fdnpkg_strcasestr(curzipnode->filename, "source\\") == curzipnode->filename) { /* drop this file */
        curzipnode->filename[0] = 0; /* in fact, we just mark the file as 'empty' on the filename.. see below */
      }
    }
    /* is it a "link file"? */
    if (fdnpkg_strcasestr(curzipnode->filename, "links\\") == curzipnode->filename) {
      /* skip links, if that's what the user wants */
      if ((flags & PKGINST_SKIPLINKS) != 0) {
        curzipnode->filename[0] = 0; /* in fact, we just mark the file as 'empty' on the filename.. see later below */
      } else {
        /* if it's a *.BAT link, then rename it to *.COM */
        char *ext = getfext(curzipnode->filename);
        if (strcasecmp(ext, "bat") == 0) sprintf(ext, "com");
      }
    }

    if (curzipnode->filename[0] == 0) { /* ignore empty filenames (maybe it was empty originally, or has been emptied because it's a dropped source or link) */
      if (prevzipnode == NULL) {  /* take the item out of the list */
        ziplinkedlist = curzipnode->nextfile;
        free(curzipnode); /* free the item */
        curzipnode = ziplinkedlist;
      } else {
        prevzipnode->nextfile = curzipnode->nextfile;
        free(curzipnode); /* free the item */
        curzipnode = prevzipnode->nextfile;
      }
      continue; /* go to the next item */
    }
    /* validate that the file has a valid filename (8+3, no shady chars...) */
    if (validfilename(curzipnode->filename) != 0) {
      kitten_puts(3, 23, "Error: Package contains an invalid filename:");
      printf(" %s\n", curzipnode->filename);
      zip_freelist(&ziplinkedlist);
      fclose(*zipfd);
      return(NULL);
    }
    /* look out for collisions with already existing files (unless we are
     * updating the package and the local file belongs to it */
    shortfile = computelocalpath(curzipnode->filename, fname, dosdir, dirlist);
    strcat(fname, shortfile);
    if ((findfileinlist(flist, fname) == NULL) && (fileexists(fname) != 0)) {
      kitten_puts(3, 9, "Error: Package contains a file that already exists locally:");
      printf(" %s\n", fname);
      zip_freelist(&ziplinkedlist);
      fclose(*zipfd);
      return(NULL);
    }
    /* abort if any entry is encrypted */
    if ((curzipnode->flags & ZIP_FLAG_ENCRYPTED) != 0) {
      kitten_printf(3, 20, "Error: Package contains an encrypted file:");
      puts("");
      printf(" %s\n", curzipnode->filename);
      zip_freelist(&ziplinkedlist);
      fclose(*zipfd);
      return(NULL);
    }
    /* abort if any file is compressed with an unsupported method */
    if ((curzipnode->compmethod != 0/*store*/) && (curzipnode->compmethod != 8/*deflate*/) && (curzipnode->compmethod != 14/*lzma*/)) { /* unsupported compression method */
      kitten_printf(8, 2, "Error: Package contains a file compressed with an unsupported method (%d):", curzipnode->compmethod);
      puts("");
      printf(" %s\n", curzipnode->filename);
      zip_freelist(&ziplinkedlist);
      fclose(*zipfd);
      return(NULL);
    }
    if (strcmp(curzipnode->filename, appinfofile) == 0) appinfopresence = 1;
    prevzipnode = curzipnode;
    curzipnode = curzipnode->nextfile;
  }
  /* if appinfo file not found, this is not a real FreeDOS package */
  if (appinfopresence != 1) {
    kitten_printf(3, 12, "Error: Package do not contain the %s file! Not a valid FreeDOS package.", appinfofile);
    puts("");
    zip_freelist(&ziplinkedlist);
    fclose(*zipfd);
    return(NULL);
  }

  return(ziplinkedlist);
}


/* install a package that has been prepared already. returns 0 on success,
 * or a negative value on error, or a positive value on warning */
int pkginstall_installpackage(char *pkgname, char *dosdir, struct customdirs *dirlist, struct ziplist *ziplinkedlist, FILE *zipfd) {
  char *buff;
  char *fulldestfilename;
  char packageslst[64];
  char *shortfile;
  long filesextractedsuccess = 0, filesextractedfailure = 0;
  struct ziplist *curzipnode;
  FILE *lstfd;

  sprintf(packageslst, "packages\\%s.lst", pkgname); /* Prepare the packages/xxxx.lst filename string for later use */

  buff = malloc(512);
  fulldestfilename = malloc(1024);
  if ((buff == NULL) || (fulldestfilename == NULL)) {
    kitten_puts(8, 0, "Out of memory!");
    zip_freelist(&ziplinkedlist);
    free(buff);
    free(fulldestfilename);
    return(-1);
  }

  /* create the %DOSDIR%/packages directory, just in case it doesn't exist yet */
  sprintf(buff, "%s\\packages\\", dosdir);
  mkpath(buff);

  /* open the lst file */
  sprintf(buff, "%s\\%s", dosdir, packageslst);
  lstfd = fopen(buff, "wb"); /* opening it in binary mode, because I like to have control over line terminators (CR/LF) */
  if (lstfd == NULL) {
    kitten_printf(3, 10, "Error: Could not create %s!", buff);
    puts("");
    zip_freelist(&ziplinkedlist);
    free(buff);
    free(fulldestfilename);
    return(-2);
  }

  /* write list of files in zip into the lst, and create the directories structure */
  for (curzipnode = ziplinkedlist; curzipnode != NULL; curzipnode = curzipnode->nextfile) {
    int unzip_result;
    if ((curzipnode->flags & ZIP_FLAG_ISADIR) != 0) continue; /* skip directories */
    if (strcasecmp(curzipnode->filename, packageslst) == 0) continue; /* skip silently the package.lst file, if found */
    shortfile = computelocalpath(curzipnode->filename, buff, dosdir, dirlist); /* substitute paths to custom dirs */
    fprintf(lstfd, "%s%s\r\n", buff, shortfile); /* log the filename to packages\pkg.lst */
    /* create the path, just in case it doesn't exist yet */
    mkpath(buff);
    sprintf(fulldestfilename, "%s%s", buff, shortfile);
    /* Now unzip the file */
    unzip_result = zip_unzip(zipfd, curzipnode, fulldestfilename);
    if (unzip_result != 0) {
      kitten_printf(8, 3, "Error while extracting '%s' to '%s'!", curzipnode->filename, fulldestfilename);
      printf(" [%d]\n", unzip_result);
      filesextractedfailure += 1;
    } else {
      printf(" %s -> %s\n", curzipnode->filename, buff);
      filesextractedsuccess += 1;
    }
    /* if it's a LINK file, recompute a new content */
    if (islinkfile(curzipnode->filename) != 0) {
      processlinkfile(fulldestfilename, dosdir, dirlist, buff);
    }
  }
  fclose(lstfd);

  /* free the ziplist and close file descriptor */
  zip_freelist(&ziplinkedlist);
  free(buff);
  free(fulldestfilename);

  kitten_printf(3, 19, "Package %s installed: %ld files extracted, %ld errors.", pkgname, filesextractedsuccess, filesextractedfailure);
  puts("");
  return(filesextractedfailure);
}
