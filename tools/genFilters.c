/*
================================================================

	Generates Filter Definitions from g.Tec DSP Filter Files
	The Myelin Project
	
	Copyright (C) 2010 Saumitro Dasgupta.
	This code is made available under the MIT License.
	<http://www.opensource.org/licenses/mit-license.html>

================================================================
*/

#include <stdio.h>

#define DETAIL_LENGTH 0x90
#define HEADER_LENGTH 0x2A

//-----------------------------------------------------------------------------

typedef struct filterSpec {

  double hf; //Highpass frequency
  double lf; //Lowpass frequency
  double fs; //Sampling frequency
  double type; 
  double order;
  
  unsigned char details[DETAIL_LENGTH];

} FilterSpec;

//-----------------------------------------------------------------------------

void displayFilterSpec(FilterSpec* spec) {

  printf("{ %0.2f, %0.1f, %d, %d, %d, {",spec->hf, spec->lf, (int)spec->fs, (int)spec->type, (int)spec->order);
  
  int i;
  for(i=0;i<DETAIL_LENGTH;++i)
    printf("0x%x, ", (unsigned char)spec->details[i]);

  printf("} } , \n\n");

}

//-----------------------------------------------------------------------------

void generateFilterList(const char* filename) {

  FILE* fp = fopen(filename,"rb");

  FilterSpec spec;

  if(fp==NULL) {

    printf("Unable to open input file.\n");
    return;

  }

  //Skip over the header (0x29 bytes in size)
  fseek(fp, HEADER_LENGTH, SEEK_SET);

  printf("\nFilterSpec filters[] = {\n\n");

  int count=0;

  while(1) {

    fread(&spec, sizeof(FilterSpec), 1, fp);
    if(feof(fp)) break;
    displayFilterSpec(&spec);
    ++count;

  }

  printf("};\n\n");

  printf("Total filters found : %d\n\n",count);

  fclose(fp);

}

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {

  if(argc!=2) 
    printf("\nusage : genFilters filterFile\n");
  else
    generateFilterList(argv[1]);

 
  return 0;


}

//-----------------------------------------------------------------------------
