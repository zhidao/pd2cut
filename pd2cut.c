#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *pd2cut_open_src(char *name, char *basename)
{
  FILE *fp;
  char *cp;

  if( !( fp = fopen( name, "r" ) ) ){
    perror( name );
    exit( 1 );
  }
  if( ( cp = strrchr( name, '/' ) ) )
    cp++;
  else
    cp = name;
  strcpy( basename, cp );
  if( ( cp = strchr( basename, '.' ) ) ) *cp = '\0';
  return fp;
}

int pd2cut_file_num(FILE *fp, unsigned int *num)
{
  rewind( fp );
  if( fread( num, 4, 1, fp ) != 4 ) return -1;
  *num /= 8;
  rewind( fp );
  return 0;
}

int pd2cut_output(FILE *fp, char *basename, unsigned int ident)
{
  unsigned int offset, size, i;
  unsigned char data;
  FILE *fout;
  char filename[BUFSIZ];

  fseek( fp, ident*8, SEEK_SET );
  if( fread( &offset, 4, 1, fp ) != 1 ) return -1;
  if( fread( &size, 4, 1, fp ) != 1 ) return -1;

  sprintf( filename, "%s%04d.bmp", basename, ident );
  fprintf( stderr, "jump to %X, read %u bytes, output to %s ... ", offset, size, filename );
  fseek( fp, offset, SEEK_SET );
  if( !( fout = fopen( filename, "w" ) ) ){
    perror( filename );
    exit( 1 );
  }
  for( i=0; i<size; i++ ){
    if( fread( &data, 1, 1, fp ) != 1 ) break;
    fwrite( &data, 1, 1, fout );
  }
  fclose( fout );
  fprintf( stderr, "done.\r" );
  fflush( stderr );
  return 0;
}

void pd2cut(FILE *fp, char *basename)
{
  unsigned int num, ident;

  pd2cut_file_num( fp, &num );
  fprintf( stderr, "%d files contained in the archive\n", num );
  for( ident=0; ident<num; ident++ )
    pd2cut_output( fp, basename, ident );
  fprintf( stderr, "\n" );
}

int main(int argc, char *argv[])
{
  FILE *fp;
  char basename[BUFSIZ];

  while( --argc > 0 ){
    fp = pd2cut_open_src( *++argv, basename );
    pd2cut( fp, basename );
    fclose( fp );
  }
  return 0;
}
