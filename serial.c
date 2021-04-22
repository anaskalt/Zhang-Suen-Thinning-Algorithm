#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define height 256
#define width 256

//check conditions a & b
bool conditionsab(int p[3][3]){
	int i, j;
	int count1 = 0, count2 = 0;
	//count the number of black neighbours
	for(i = 0; i<3; i++){
		for(j = 0; j<3; j++){
			if(p[i][j] != 0) {
				if(!(i == 1 && j == 1)) count1++;
			}
		}
	}
	//count the number of 0 to nonzero transitions
	if(p[0][1] == 0 && p[0][2] != 0) count2++;
	if(p[0][2] == 0 && p[1][2] != 0) count2++;
	if(p[1][2] == 0 && p[2][2] != 0) count2++;
	if(p[2][2] == 0 && p[2][1] != 0) count2++;
	if(p[2][1] == 0 && p[2][0] != 0) count2++;
	if(p[2][0] == 0 && p[1][0] != 0) count2++;
	if(p[1][0] == 0 && p[0][0] != 0) count2++;
	if(p[0][0] == 0 && p[0][1] != 0) count2++;
	//return true if the conditions a & b are met
	if(count1 >= 2 && count1 <= 6 && count2 == 1) return true;
	else return false;
}

//check conditions a, b, c, d
bool checkconditions1(int p[3][3]){
	int i, j, k;
	bool check3, check4;
	//calculate products according to the conditions c, d
	check3 = p[0][1]*p[1][2]*p[2][1] == 0;
	check4 = p[1][2]*p[2][1]*p[1][0] == 0; 
	//return true if the conditions a, b, c, d are met
	if(conditionsab(p) && check3 && check4) return true;
	else return false;
}

//check conditions a, b, c', d'
bool checkconditions2(int p[3][3]){
	int i, j, k;
	bool check3, check4;
	//calculate products according to the conditions c', d'
	check3 = p[0][1]*p[1][2]*p[1][0] == 0;
	check4 = p[0][1]*p[2][1]*p[1][0] == 0; 
	//return true if the conditions a, b, c', d' are met
	if(conditionsab(p) && check3 && check4) return true;
	else return false;
}

//save the current pixel with indeces i, j and its 8 neighbours in the temp array
void cut(int temp[3][3], unsigned char A[height][width], int i, int j){
	int k, m, row, col;
	for(k = 0; k<3; k++){
		//calculate row of neighbours
		row = i-1+k;
		for(m = 0; m<3; m++){
			//calculate column of neighbour
			col = j-1+m;
			//check if the neighbour is within the boundaries of the initial array inimg
			//if it is then save the value, else save 0
			temp[k][m] = (col>-1 && col<width && row>-1 && row<height) ? (int)A[row][col] : 0;
		}
	}
}

//remove marked pixels in the image array, using the boolean array b
void apply(unsigned char A[height][width], bool b[height][width]){
	int i, j;
	for(i = 0; i<height; i++){
		for(j = 0; j<width; j++){
			//if the pixel with indeces i, j is marked then remove it(place a zero) in the image array 
			if(b[i][j]) A[i][j] = 0;
		}
	}
}

int main(){
	//declare variables
	int temp[3][3]; 		//used for cutting the array to 3x3 windows
	int C;					//used to count the number of pixels that meet the conditions
	bool a[height][width];	//used to mark pixels
	bool cond1, cond2;		//boolean values, to check if conditions are met
	//read from file
	char infname[50], outfname[50];
	unsigned char inimg[height][width], outimg[height][width];
	short i, j;
	FILE *infile, *outfile;
	printf("input image filename: "); 
	scanf("%s", infname);
	printf("output image filename: "); 
	scanf("%s",outfname);
	infile=fopen(infname,"r");
	for (i=0; i<height; i++) fread(inimg[i], 1, width, infile);
	fclose(infile);
	//flag, to return to this loop if needed 
	here:
	//reset C
	C = 0;
	//check for each pixel in the image array, if the conditions a, b, c, d are met
	for(i = 0; i<height; i++){
		for(j = 0; j<width; j++){
			//default value for a[i][j] = 0
			a[i][j] = 0;
			//only check non zero pixels
			if(inimg[i][j] != 0){
				//cut to 3x3 window
				cut(temp, inimg, i, j);
				//check conditions
				cond1 = checkconditions1(temp);
				if(cond1) {
					//mark pixel
					a[i][j] = 1;
					//increase C by 1
					C++;
				}
			}
		}
	}
	//if no pixels found(C == 0) then quit loop
	//else continue to the next loop
	if(C == 0) goto write;
	//remove marked pixels
	apply(inimg, a);
	//reset C
	C = 0;
	//check for each pixel in the image array, if the conditions a, b, c', d' are met
	for(i = 0; i<height; i++){
		for(j = 0; j<width; j++){
			//default value for a[i][j] = 0
			a[i][j] = 0;
			if(inimg[i][j] != 0){
				//cut to 3x3 window
				cut(temp, inimg, i, j);
				//check conditions
				cond2 = checkconditions2(temp);
				if(cond2) {
					//mark pixel
					a[i][j] = 1;
					//increase C by 1
					C++;
				}	
			}
		}
	}
	//remove marked pixels
	apply(inimg, a);
	//if pixels, that meet the conditions, are found(C != 0) then go to the first loop, and start again
	if(C != 0) goto here;
	//if C == 0 go here to write the output file
	write:
	//write
	for (i=0; i< height; i++)for (j=0; j<width; j++) outimg[i][j] = inimg[i][j];
	outfile=fopen(outfname,"w");
	for (i=0; i< height; i++) fwrite(outimg[i], 1, width, outfile);
	fclose (outfile);
	return 0;
}
