/*
 * FDINST - lightweigth FreeDOS package installer
 * Copyright (C) 2015-2017 Mateusz Viste
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>    /* printf() */
#include <stdlib.h>   /* malloc() and friends */
#include <string.h>   /* strcasecmp() */

#include "libunzip.h"
#include "pkginst.h"
#include "pkgrem.h"
#include "readenv.h"
#include "version.h"


enum ACTIONTYPES {
  ACTION_INSTALL,
  ACTION_REMOVE,
  ACTION_HELP
};


static int showhelp(void) {
  printf("FDINST v" PVER " Copyright (C) " PDATE " Mateusz Viste\n"
         "\n"
         "FDINST is a lightweigth package installer for FreeDOS. It is an alternative\n"
         "to FDNPKG, when only basic, local install/remove actions are necessary. FDINST\n"
         "is a 16-bit, 8086-compatible application running in real mode.\n"
         "\n"
         "Usage: FDINST install package.zip\n"
         "       FDINST remove package\n"
         "\n"
         "FDINST is published under the GNU/GPL v3 license, and shares most of its source\n"
         "code with FDNPKG to guarantee consistent behaviour of both tools. It also uses\n"
         "FDNPKG's configuration file.\n"
         );
  return(1);
}


static enum ACTIONTYPES parsearg(int argc, char **argv) {
  int extpos, i;
  enum ACTIONTYPES res = ACTION_HELP;
  /* I expect exactly 2 arguments (ie argc == 3) */
  if (argc != 3) return(ACTION_HELP);
  /* look for valid actions */
  if (strcasecmp(argv[1], "install") == 0) {
    res = ACTION_INSTALL;
  } else if (strcasecmp(argv[1], "remove") == 0) {
    res = ACTION_REMOVE;
  }
  /* the argument should never be empty */
  if (argv[2][0] == 0) return(ACTION_INSTALL);
  /* for 'install', validate that the extension is '.zip' */
  if (res == ACTION_INSTALL) {
    /* find where the file's extension starts */
    extpos = 0;
    for (i = 0; argv[2][i] != 0; i++) {
      if (argv[2][i] == '.') extpos = i + 1;
    }
    if (extpos == 0) return(ACTION_HELP);
    if (strcasecmp(argv[2] + extpos, "zip") != 0) return(ACTION_HELP);
  }
  /* return the result */
  return(res);
}


static int pkginst(char *file, int flags, char *dosdir, char *tempdir, struct customdirs *dirlist) {
  char pkgname[32];
  int t, lastpathdelim = -1, u = 0;
  char *buffmem1k;
  struct ziplist *zipfileidx;
  FILE *zipfilefd;
  for (t = 0; file[t] != 0; t++) {
    if ((file[t] == '/') || (file[t] == '\\')) lastpathdelim = t;
  }
  /* copy the filename into pkgname (without path elements) */
  for (t = lastpathdelim + 1; file[t] != 0; t++) pkgname[u++] = file[t];
  pkgname[u] = 0; /* terminate the string */
  /* truncate the file's extension (.zip) */
  for (t = u; t > 0; t--) {
    if (pkgname[t] == '.') {
      pkgname[t] = 0;
      break;
    }
  }
  /* allocate some memory for pkginst_preparepackage() to do its job */
  buffmem1k = malloc(1024);
  if (buffmem1k == NULL) {
    puts("ERROR: Out of memory");
    return(1);
  }
  /* prepare the zip file and install it */
  zipfileidx = pkginstall_preparepackage(NULL, pkgname, tempdir, file, flags, NULL, &zipfilefd, NULL, 0, NULL, dosdir, dirlist, buffmem1k);
  free(buffmem1k);
  if (zipfileidx != NULL) {
    int res = 0;
    if (pkginstall_installpackage(pkgname, dosdir, dirlist, zipfileidx, zipfilefd) != 0) res = 1;
    fclose(zipfilefd);
    return(res);
  } else {
    fclose(zipfilefd);
    return(1);
  }
}


int main(int argc, char **argv) {
  int res, flags;
  enum ACTIONTYPES action;
  char *dosdir, *tempdir, *cfgfile;
  struct customdirs *dirlist;

  action = parsearg(argc, argv);
  if (action == ACTION_HELP) return(showhelp());

  /* allocate some bits for cfg file's location */
  cfgfile = malloc(256);
  if (cfgfile == NULL) {
    puts("ERROR: Out of memory");
    return(1);
  }

  /* read all necessary environment variables */
  if (readenv(&dosdir, &tempdir, cfgfile, 256) != 0) {
    free(cfgfile);
    return(1);
  }

  /* load configuration */
  flags = 0;
  dirlist = NULL;
  if (loadconf(cfgfile, NULL, 0, NULL, NULL, &dirlist, &flags, NULL, NULL) < 0) return(5);

  /* free the cfgfile buffer, I won't need the config file's location any more */
  free(cfgfile);
  cfgfile = NULL;

  switch (action) {
    case ACTION_INSTALL:
      res = pkginst(argv[2], flags, dosdir, tempdir, dirlist);
      break;
    case ACTION_REMOVE:
      res = pkgrem(argv[2], dosdir);
      break;
    default:
      res = showhelp();
      break;
  }

  if (res != 0) return(1);
  return(0);
}
