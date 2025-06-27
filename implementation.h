#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define SIZE 512
#define BYTE unsigned char
#define IMAGE_WIDTH 256
#define RGB_SIZE 256

void print_histogram(BYTE img_mat[][SIZE]);
void histogram_equalization(BYTE img_mat[][SIZE], BYTE equalized_img_mat[][SIZE]);
void write_image_raw(BYTE img_mat[][SIZE], const char name[]);
void basic_contrast_stretching(BYTE img_mat[][SIZE], BYTE basic_stretched_img_mat[][SIZE]);
BYTE get_max(BYTE img_mat[][SIZE]);
BYTE get_min(BYTE img_mat[][SIZE]);
void ends_in_contrast_stretching(BYTE img_mat[][SIZE], BYTE ends_in_stretched_img_mat[][SIZE], BYTE high, BYTE low);
void write_bmp_image(BYTE img_mat[][SIZE], const char name[]);

void write_bmp_image(BYTE img_mat[][SIZE], const char name[])
{
	FILE* fp, * infile;
	BITMAPFILEHEADER hFile;
	BITMAPINFOHEADER hInfo;
	RGBQUAD hRGB[RGB_SIZE];
	BYTE reversed_img_mat[SIZE][SIZE] = { 0, };

	// 이미지 뒤집기
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			reversed_img_mat[i][j] = img_mat[SIZE - i - 1][j];
		}
	}
	
	fp = fopen(name, "wb");
	
	if (!(infile = fopen("lena_bmp_512x512(8비트).bmp", "rb")))
	{
		printf("Bmp file open failed");
		return;
	}

	fread(&hFile, sizeof(BITMAPFILEHEADER), 1, infile);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile);
	fread(hRGB, sizeof(RGBQUAD), RGB_SIZE, infile);

	fclose(infile);

	fwrite(&hFile, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(hRGB, sizeof(RGBQUAD), RGB_SIZE, fp);
	fwrite(reversed_img_mat, sizeof(BYTE), SIZE * SIZE, fp);

	fclose(fp);
}

void ends_in_contrast_stretching(BYTE img_mat[][SIZE], BYTE ends_in_stretched_img_mat[][SIZE], BYTE high, BYTE low)
{
	double temp = 0;

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (img_mat[i][j] < low)
				ends_in_stretched_img_mat[i][j] = 0;
			else if (img_mat[i][j] > high)
				ends_in_stretched_img_mat[i][j] = IMAGE_WIDTH - 1;
			else
			{
				temp = (double)(img_mat[i][j] - low) / (high - low);
				ends_in_stretched_img_mat[i][j] = temp * (IMAGE_WIDTH - 1);
			}
		}
	}
}

void basic_contrast_stretching(BYTE img_mat[][SIZE], BYTE basic_stretched_img_mat[][SIZE])
{
	BYTE max = get_max(img_mat);
	BYTE min = get_min(img_mat);
	double temp = 0;

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			temp = (double)(img_mat[i][j] - min) / (max - min);
			basic_stretched_img_mat[i][j] = temp * (IMAGE_WIDTH-1);
		}
	}
}

BYTE get_min(BYTE img_mat[][SIZE])
{
	BYTE min = img_mat[0][0];

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (min > img_mat[i][j])
				min = img_mat[i][j];
		}
	}
	return min;
}

BYTE get_max(BYTE img_mat[][SIZE])
{
	BYTE max = img_mat[0][0];
	
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (max < img_mat[i][j])
				max = img_mat[i][j];
		}
	}
	return max;
}

void write_image_raw(BYTE img_mat[][SIZE], const char name[])
{
	FILE* fp;

	fp = fopen(name, "wb");

	fwrite(img_mat, sizeof(BYTE), SIZE * SIZE, fp);

	fclose(fp);
}

void print_histogram(BYTE img_mat[][SIZE])
{
	int histogram_data[IMAGE_WIDTH] = { 0, };

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			histogram_data[(img_mat[i][j])] = histogram_data[(img_mat[i][j])] + 1;
		}
	}

	// 출력
	for (int i = 0; i < IMAGE_WIDTH; i++)
	{
		printf("%d: ", i);
		for (int j = 0; j < histogram_data[i]; j++)
		{
			putchar('*');
		}
		putchar('\n');
	}
}

void histogram_equalization(BYTE img_mat[][SIZE], BYTE equalized_img_mat[][SIZE])
{
	int sum[IMAGE_WIDTH] = { 0, };
	BYTE nomalized_sum[IMAGE_WIDTH] = { 0, };
	double pmax_divied_na = 0;	// Pmax / Na를 임시로 저장할 변수
	int temp = 0;				// Nomalization 값이 오버플로우 될 때를 위한 임시변수

	// 기본 pixel 합
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			sum[(img_mat[i][j])] = sum[(img_mat[i][j])] + 1;
		}
	}

	// 누적합으로 변경
	for (int i = 1; i < IMAGE_WIDTH; i++)
	{
		sum[i] = sum[i] + sum[i - 1];
	}

	pmax_divied_na = (double)IMAGE_WIDTH / sum[IMAGE_WIDTH - 1];	// Pmax / Na

	for (int i = 0; i < IMAGE_WIDTH; i++)
	{
		temp = (pmax_divied_na * sum[i] + 0.5);
		if (temp >= IMAGE_WIDTH)
			nomalized_sum[i] = IMAGE_WIDTH - 1;
		else
			nomalized_sum[i] = (BYTE)(temp);
	}

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			equalized_img_mat[i][j] = nomalized_sum[(img_mat[i][j])];
		}
	}
}