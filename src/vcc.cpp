
// **********************************************************************
// *                                                                    *
// *                     The Verge-C Compiler v.0.10                    *
// *                     Copyright (C)1997 BJ Eirich                    *
// *                                                                    *
// * Module: VCC.C                                                      *
// *                                                                    *
// * Description: Handles setup, command line parsing, determines       *
// * compilation target / mode, other misc things.                      *
// *                                                                    *
// * Portability: ANSI C - should compile on any compiler.              *
// *                                                                    *
// **********************************************************************

#include "compile.h"
#include "preproc.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LETTER 1
#define DIGIT 2
#define SPECIAL 3

// ============================== Variables ==============================

char quiet, verbose;        // output modes for VCC
char *strbuf, fname[100];   // string buffers (mostly temporary)
FILE *inf, *outf;           // input/output file handles.
char effect = 0, scrpt = 0; // compiling events/effects/misc scripts

// NEW CODE
char magic = 0;
// END NEW CODE

char *source; // ptr -> source file in memory.
char chr_tablebeta[256];
char tokenbeta[512];

// ================================ Code =================================

void
PostStartupFiles()
{
  if (!(inf = fopen("$$tmep$$.ma_", "rb")))
  {
    if (!quiet)
    {
      printf("*error* Could not open input file. \n");
    }
    exit(-1);
  }

  if (verbose)
  {
    printf("Reading source file into memory... \n");
  }
  memset(source, 0, 100000);
  memset(code, 0, 100000);
  fread(source, 1, 100000, inf);
  fclose(inf);
}

void
PreStartupFiles()
{
  int i;

  // Open input and output files; make sure filenames are correct.

  i = strlen(strbuf);
  strbuf[i] = '.';
  strbuf[i + 1] = 'v';
  strbuf[i + 2] = 'c';
  strbuf[i + 3] = 0;

  if (!(inf = fopen(strbuf, "rb")))
  {
    if (!quiet)
    {
      printf("*error* Could not open input file\n");
    }
    exit(-1);
  }

  if (verbose)
  {
    printf("Reading source file into memory... \n");
  }
  source = (char *)malloc(100000);
  code = (char *)malloc(100000);
  memset(source, 0, 100000);
  memset(code, 0, 100000);
  fread(source, 1, 100000, inf);
  fclose(inf);
}

void
WriteMagicOutput()
{
  FILE *f;

  f = fopen("magic.vcs", "wb");
  fwrite(&numscripts, 1, 4, f);
  fwrite(&scriptofstbl, 4, numscripts, f);
  fwrite(code, 1, (cpos - code), f);
  fclose(f);

  remove("error.txt");
  remove("$$tmep$$.ma_");
}

void
WriteEffectOutput()
{
  FILE *f;

  f = fopen("effects.vcs", "wb");
  fwrite(&numscripts, 1, 4, f);
  fwrite(&scriptofstbl, 4, numscripts, f);
  fwrite(code, 1, (cpos - code), f);
  fclose(f);

  remove("error.txt");
  remove("$$tmep$$.ma_");
}

void
WriteScriptOutput()
{
  FILE *f;

  f = fopen("startup.vcs", "wb");
  fwrite(&numscripts, 1, 4, f);
  fwrite(&scriptofstbl, 4, numscripts, f);
  fwrite(code, 1, (cpos - code), f);
  fclose(f);

  remove("error.txt");
  remove("$$tmep$$.ma_");
}

void
WriteOutput()
{
  FILE *f;
  char i;
  short int mx, my;
  int a;

  i = strlen(fname);
  memcpy(strbuf, fname, i);
  strcpy(strbuf + i, ".map");

  f = fopen(strbuf, "rb+");
  if (f == 0)
  {
    strcpy(strbuf + i, ".map");
    f = fopen(strbuf, "rb+");
  }

  fseek(f, 68, 0);
  fread(&mx, 1, 2, f);
  fread(&my, 1, 2, f);
  fseek(f, 100 + (mx * my * 5) + 7956, 0);
  fread(&a, 1, 4, f);
  fseek(f, 88 * a, 1);
  fread(&i, 1, 1, f);
  fread(&a, 1, 4, f);
  fseek(f, (i * 4) + a, 1);

  fwrite(&numscripts, 1, 4, f);
  fwrite(&scriptofstbl, 4, numscripts, f);
  fwrite(code, 1, (cpos - code), f);
  fclose(f);

  remove("error.txt");
  remove("$$tmep$$.ma_");
}

int
main(int argc, char *argv[])
{
  int i;

  // Test the command line and set any necessary flags.

  strbuf = (char *)malloc(100);
  switch (argc)
  {
    case 1:
    {
      printf("vcc v.04.Jun.98 Copyright (C)1997 BJ Eirich \n");
      printf("Usage: vcc <vc file> [flag] \n");
      printf("Where [flag] is one of the following: \n");
      printf("        q   Quiet mode - no output. \n");
      printf(
          "        v   Super-Verbose mode - more output than you want. \n \n");
      exit(-1);
    }
    case 3:
    {
      strbuf = argv[2];
      if (strbuf[0] == 'q')
      {
        quiet = 1;
      }
      if (strbuf[0] == 'v')
      {
        verbose = 1;
      }
    }
    case 2:
    {
      strbuf = argv[1];
      for (i = 0; i < 100; i++)
      {
        fname[i] = strbuf[i];
      }
      //  strupr (fname);
      if (!strcmp(fname, "effects"))
      {
        effect = 1;
      }
      // NEW CODE
      if (!strcmp(fname, "magic"))
      {
        magic = 1;
      }
      // END NEW CODE
      if (!strcmp(fname, "startup"))
      {
        scrpt = 1;
      }
      break;
    }
    default:
    {
      printf("vcc: Too many parameters. \n");
      exit(-1);
    }
  }

  if (!quiet)
  {
    printf("vcc v.04.Jun.98 Copyright (C)1997 BJ Eirich \n");
  }
  PreStartupFiles();  // startup for preprocessing
  FirstPass();        // preprocessing
  PostStartupFiles(); // startup for postprocessing? no,
                      // that doesn't make sense...
  InitCompileSystem();
  Compile();
  if (effect)
  {
    WriteEffectOutput();
  }
  else if (magic)
  {
    WriteMagicOutput();
  }
  else if (scrpt)
  {
    WriteScriptOutput();
  }
  else
  {
    WriteOutput();
  }
}
