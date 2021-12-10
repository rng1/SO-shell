/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

#ifndef SHELL_P1_H
#define SHELL_P1_H

#include <stdbool.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdbool.h>
#include <libgen.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define ARRAY 4096
#define BUFFERSIZE 1024

int cmd_crear(char **tr);
int aux_createFile(char *f);
/*
 * Creates a file or directory in the file system.
 *  crear [-f] name     creates a directory (or file with the -f flag) with the given name.
 */

int cmd_borrar(char **tr);
/*
 * Delete files or empty directories.
 */

int cmd_borrarRec(char **tr);
void aux_rmTraverseDir(char *dirname);
/*
 * Walks through a file tree and delete recursively folders and files in the given directory.
 */

int cmd_listFich(char **tr);
/*
 * Gives info on files in one line per file. If no options are given, it prints the size and name of each file.
 * If no name is given, the cwd will be printed.
 *  listfich [-long] name           stands for long listing, printing more information about the file.
 *  listfich -long [-link] name     display the name of the file a link points to.
 *  listfich -long [-acc] name      last access time will be used instead of last modification time.
 */

int cmd_listDir(char **tr);
void aux_listTraverseDir(char *dirname, bool long_l, bool link, bool acc, bool hid, bool reca, bool recb);
void aux_lookForDir(char *dirname, bool long_l, bool link, bool acc, bool hid, bool reca, bool recb);
/*
 * List all contents and info of the given directories, or as with listfich if it isn't.
 * If no name is given, the cwd will be printed.
 * listDir [-long] [-link] [-acc] has the same meaning as in listfich.
 *  listDir [-hid] name     hidden files and directories will also get listed.
 *  listDir [-reca] name    when listing a directory's contents, subdirectories will be listed AFTER all the prior files.
 *  listDir [-recb] name    when listing a directory's contents, subdirectories will be listed BEFORE all the prior files.
 */

void aux_filePrint(char *fname, bool long_l, bool link, bool acc);
void aux_getMode(mode_t m);
char aux_getType(mode_t m);
/*
 * Get all info on a file and print it in the desired format.
 */

#endif
