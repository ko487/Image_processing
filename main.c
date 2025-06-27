#include "implementation.h"

int main()
{
	FILE* fp;
	BYTE img_mat[SIZE][SIZE] = { 0, };
	BYTE processed_img_mat[SIZE][SIZE] = { 0, };

	if ((fp = fopen("lena_raw_512x512.raw", "rb")) == NULL)
	{
		printf("File open failed");
		return 0;
	}

	fread(img_mat, sizeof(BYTE), SIZE * SIZE, fp);
	
	fclose(fp);

	//histogram_equalization(img_mat, processed_img_mat);
	//basic_contrast_stretching(img_mat, processed_img_mat);
	ends_in_contrast_stretching(img_mat, processed_img_mat, 200, 30);
	
	print_histogram(processed_img_mat);

	return 0;
}