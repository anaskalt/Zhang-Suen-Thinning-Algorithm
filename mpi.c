#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <mpi.h>
#define height 256
#define width 256

int size;
int localheight;

bool conditionsab(int p[3][3]){
	int i, j;
	int count1 = 0, count2 = 0;
	for(i = 0; i<3; i++){
		for(j = 0; j<3; j++){
			if(p[i][j] != 0) {
				if(!(i == 1 && j == 1)) count1++;
			}
		}
	}
	if(p[0][1] == 0 && p[0][2] != 0) count2++;
	if(p[0][2] == 0 && p[1][2] != 0) count2++;
	if(p[1][2] == 0 && p[2][2] != 0) count2++;
	if(p[2][2] == 0 && p[2][1] != 0) count2++;
	if(p[2][1] == 0 && p[2][0] != 0) count2++;
	if(p[2][0] == 0 && p[1][0] != 0) count2++;
	if(p[1][0] == 0 && p[0][0] != 0) count2++;
	if(p[0][0] == 0 && p[0][1] != 0) count2++;
	if(count1 >= 2 && count1 <= 6 && count2 == 1) return true;
	else return false;
}

bool checkconditions1(int p[3][3]){
	int i, j, k;
	bool check3, check4;
	check3 = p[0][1]*p[1][2]*p[2][1] == 0;
	check4 = p[1][2]*p[2][1]*p[1][0] == 0; 
	if(conditionsab(p) && check3 && check4) return true;
	else return false;
}

bool checkconditions2(int p[3][3]){
	int i, j, k;
	bool check3, check4;
	check3 = p[0][1]*p[1][2]*p[1][0] == 0;
	check4 = p[0][1]*p[2][1]*p[1][0] == 0; 
	if(conditionsab(p) && check3 && check4) return true;
	else return false;
}

void cut(int temp[3][3], unsigned char A[localheight][width], int i, int j){
	int k, m, row, col;
	for(k = 0; k<3; k++){
		row = i-1+k;
		for(m = 0; m<3; m++){
			col = j-1+m;
			temp[k][m] = (col>-1 && col<width) ? (int)A[row][col] : 0;
		}
	}
}

void apply(unsigned char A[localheight][width], bool b[localheight][width]){
	int i, j;
	for(i = 1; i<localheight - 1; i++){
		for(j = 0; j<width; j++){
			if(b[i][j]) A[i][j] = 0;
		}
	}
}

bool checkend(int a[size]){
	int o;
	for(o = 0; o < size; o++){
		if(a[o] != 0) return false;
	}
	return true;
}

//send changed lines from one process to another process that needs them 
void refreshlines(unsigned char localinimg[localheight][width], int rank){
	//all processes, except the last, send their last line(that they changed) to the process with higher rank than them 
	if(rank < size - 1) MPI_Send(&localinimg[localheight - 2][0], width, MPI_UNSIGNED_CHAR, rank + 1, rank, MPI_COMM_WORLD);
	//all processes, except the first, receive the last line that was sent above from the process with rank lower than them  
	if(rank > 0) {
		MPI_Recv(&localinimg[0][0], width, MPI_UNSIGNED_CHAR, rank - 1, rank - 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//send the first line(that they changed) to the process with rank lower than them
		MPI_Send(&localinimg[1][0], width, MPI_UNSIGNED_CHAR, rank - 1, rank, MPI_COMM_WORLD);
	}
	//all processes, except the last, receive the first line that was sent above from processes with higher rank than them
	if(rank < size - 1) MPI_Recv(&localinimg[localheight - 1][0], width, MPI_UNSIGNED_CHAR, rank + 1, rank + 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	
}

int main(int argc, char* argv[]){
	int temp[3][3], recvcount, sendcount, rank;                  //recvcount = sendcount, rank needed to scatter/send 
	unsigned char inimg[height][width] ,outimg[height][width];  
	FILE *infile, *outfile;
	int i, j;
	bool cond1, cond2;
	char infname[50] = "bin_A1_256x256.raw", outfname[50] = "out";
	//Initialize MPI
	MPI_Init(&argc,&argv);	
	//get rank
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	//get size
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	//calculate localheight
	localheight = height/size + 2;
	//declare C array (this time not a single variable as before)
	int C[size];
	//declare boolean array, needed to mark pixels
	bool a[localheight][width];
	//declare localinimg to store the lines for each process
	unsigned char localinimg[localheight][width];
	//rank 0 tasks
	//only process with rank 0 reads the file 
	if(rank == 0){
		//printf("input image filename: "); 
		//scanf("%s", infname);
		//printf("output image filename: "); 
		//scanf("%s",outfname);
		infile=fopen(infname,"r");
		for (i=0; i<height; i++) fread(inimg[i], 1, width, infile);
		fclose(infile);
		//fill with zeros line 0 and copy the line height/size from the image array
		for(j = 0; j < width; j++) {
			localinimg[0][j] = 0;
			localinimg[localheight - 1][j] = inimg[localheight - 2][j];	 
		}
	}
	//the last process fills the last line of its localinimg array with 0
	else if(rank == size - 1) for(j = 0; j < width; j++) localinimg[localheight - 1][j] = 0;	 
	//the number of elements to be sent to other processes
	sendcount = recvcount = (localheight - 2)*width; 
	//send the elements to all processes
	MPI_Scatter(inimg, sendcount, MPI_UNSIGNED_CHAR, &localinimg[1][0], recvcount, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
	//update changed lines
	refreshlines(localinimg, rank);
	do{
		//reset C array
		for (i = 0; i<size; i++) C[i] = 0;
		for(i = 1; i<localheight - 1; i++){
			for(j = 0; j<width; j++){
				a[i][j] = 0;
				if(localinimg[i][j] != 0){
					cut(temp, localinimg, i, j);
					cond1 = checkconditions1(temp);
					if(cond1) {
						a[i][j] = 1;
						//only increment the element of C with index rank
						C[rank]++;
					}
				}
			}
		}		
		//all processes get the elements of array C and check if all of the elements are 0		
		MPI_Allgather(&C[rank], 1, MPI_INT, C, 1, MPI_INT, MPI_COMM_WORLD);
		//if they are then the process quits the loop
		if(checkend(C)) break;
		//remove marked pixels
		apply(localinimg, a);
		//update changed lines
		refreshlines(localinimg, rank);
		//reset C array
		for (i = 0; i<size; i++) C[i] = 0;
		for(i = 1; i<localheight - 1; i++){
			for(j = 0; j<width; j++){
				a[i][j] = 0;
				if(localinimg[i][j] != 0){
					cut(temp, localinimg, i, j);
					cond2 = checkconditions2(temp);
					if(cond2) {
						a[i][j] = 1;
						C[rank]++;
					}	
				}
			}
		}
		//all processes get the elements of array C and check if all of the elements are 0	
		MPI_Allgather(&C[rank], 1, MPI_INT, C, 1, MPI_INT, MPI_COMM_WORLD); 
		//if they are then the process quits the loop
		if(checkend(C)) break;
		//remove marked pixels
		apply(localinimg, a);
		//update changed lines
		refreshlines(localinimg, rank);
	}while(true);
	//process 0 gets all the lines from the other process and puts them together in the array inimg
	MPI_Gather(&localinimg[1][0], sendcount, MPI_UNSIGNED_CHAR, inimg, recvcount, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
	//rank 0 process then writes to output file
	if(rank == 0){
		for (i=0; i< height; i++)for (j=0; j<width; j++) outimg[i][j] = inimg[i][j];
		outfile=fopen(outfname,"w");
		for (i=0; i< height; i++) fwrite(outimg[i], 1, width, outfile);
		fclose (outfile);
	}
	//Finalize MPI
	MPI_Finalize();
	return 0;
}
