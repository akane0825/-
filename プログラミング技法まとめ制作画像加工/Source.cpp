#define _CRT_SECURE_NO_WARNINGS

#include<stdlib.h>
#include<stdio.h>

#define IMG_MAX_SIZE 512

typedef struct {
	unsigned char* red;
	unsigned char* blue;
	unsigned char* green;
}ImageData;

typedef struct {
	unsigned char FileHeader[14];
	unsigned int size;
	int	 width, height;
	unsigned char InfoHeader[28]; //上3つ以外の情報ヘッダ
	ImageData img;
}BMP;

//BMPファイル読み込み
void ReadBmp(const char FileName[], BMP* bmp);

//グレースケール
void grayBmp(BMP* bmp);

//画像を書き込む
void WriteBmp(const char FileName[], BMP* bmp);

int main() {
	BMP bmp, graybmp;

	char out_file[] = "img/photo00out.bmp";
	int i, cnt;

	printf("画像の枚数を入力");
	scanf("%d", &cnt);

	for (i = 1; i <= cnt; i++)
	{
		out_file[9] = '0' + i/10;
		out_file[10] = '0' + i%10;

		printf("%s", out_file);

		ReadBmp(out_file, &graybmp);

		grayBmp(&graybmp);

		WriteBmp(out_file, &graybmp);
	}

	printf("完了しました\n");

	return 0;

	free(bmp.img.red);
	free(bmp.img.green);
	free(bmp.img.blue);

	//return 0;
}

//BMPファイル読み込み
void ReadBmp(const char FileName[], BMP* bmp)
{
	FILE* fp;

	fopen_s(&fp, FileName, "rb");
	if (fp == NULL)
	{
		printf("ファイルが見つかりません\n");
		exit(1);
	}
	else
	{
		printf("ファイルが見つかりました\n");
	}

	//ヘッダー情報を読み込み
	fread(bmp->FileHeader, sizeof(unsigned char), 14, fp);
	fread(&bmp->size, sizeof(int), 1, fp);
	fread(&bmp->width, sizeof(int), 1, fp);
	fread(&bmp->height, sizeof(int), 1, fp);
	fread(bmp->InfoHeader, sizeof(unsigned char), 28, fp);

	//画像本体の読み込み
	bmp->img.red = (unsigned char*)malloc((bmp->width) * (bmp->height) * sizeof(unsigned char));
	bmp->img.blue = (unsigned char*)malloc((bmp->width) * (bmp->height) * sizeof(unsigned char));
	bmp->img.green = (unsigned char*)malloc((bmp->width) * (bmp->height) * sizeof(unsigned char));

	if (bmp->img.red == NULL)
	{
		printf("失敗1\n");
		exit(1);
	}
	if (bmp->img.blue == NULL)
	{
		printf("失敗2\n");
		exit(1);
	}
	if (bmp->img.green == NULL)
	{
		printf("失敗3\n");
		exit(1);
	}

	int stride = (bmp->width * 3 + 3) / 4 * 4;
	unsigned char padding;

	for (int h = 0; h < bmp->height; h++)
	{
		for (int w = 0; w < bmp->width; w++)
		{
			fread(&bmp->img.red[w + h * bmp->width], sizeof(unsigned char), 1, fp);
			fread(&bmp->img.green[w + h * bmp->width], sizeof(unsigned char), 1, fp);
			fread(&bmp->img.blue[w + h * bmp->width], sizeof(unsigned char), 1, fp);
		}
		for (int i = 0; i < stride - bmp->width * 3; i++)
		{
			fread(&padding, sizeof(unsigned char), 1, fp);
		}
	}
	//読みこんだファイルを閉じる
	fclose(fp);
}

//画像を書き込む
void WriteBmp(const char FileName[], BMP * bmp)
{
	FILE* fp;
	fopen_s(&fp, FileName, "wb");
	if (fp == NULL)
	{
		printf("ファイルが見つかりません\n");
		exit(1);
	}

	fwrite(bmp->FileHeader, sizeof(unsigned char), 14, fp);
	fwrite(&bmp->size, sizeof(int), 1, fp);
	fwrite(&bmp->width, sizeof(int), 1, fp);
	fwrite(&bmp->height, sizeof(int), 1, fp);
	fwrite(bmp->InfoHeader, sizeof(unsigned char), 28, fp);

	int stride = (bmp->width * 3 + 3) / 4 * 4;
	unsigned char padding = '00';

	//画像本体の読み込み
	for (int h = 0; h < bmp->height; h++)
	{
		for (int w = 0; w < bmp->width; w++)
		{
			fwrite(&bmp->img.red[w + h * bmp->width], sizeof(unsigned char), 1, fp);
			fwrite(&bmp->img.green[w + h * bmp->width], sizeof(unsigned char), 1, fp);
			fwrite(&bmp->img.blue[w + h * bmp->width], sizeof(unsigned char), 1, fp);
		}
		for (int i = 0; i < stride - bmp->width * 3; i++)
		{
			fwrite(&padding, sizeof(unsigned char), 1, fp);
		}
	}

	printf("書き込み完了\n");

	fclose(fp);
}

//グレースケール
void grayBmp(BMP * bmp)
{
	for (int h = 0; h < bmp->height; h++) {
		for (int w = 0; w < bmp->width; w++) {
			unsigned char Average = 0;
			Average = (bmp->img.red[w + h * bmp->width]
				+ bmp->img.green[w + h * bmp->width]
				+ bmp->img.blue[w + h * bmp->width]) / 3;

			bmp->img.red[w + h * bmp->width] = Average;
			bmp->img.green[w + h * bmp->width] = Average;
			bmp->img.blue[w + h * bmp->width] = Average;
		}
	}
	printf("グレースケール完了\n");
}
