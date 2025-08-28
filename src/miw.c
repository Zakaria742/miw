#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/ioctl.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../libs/stb_image_resize2.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb_image_write.h"


struct terminal_size
{
	int cols;
	int rows;
};


struct terminal_size getTermSize();
void printImage(const char* );



int main(int argc, char **argv)
{

	printImage(argv[1]);
	return 0;
}



struct terminal_size getTermSize()
{
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return (struct terminal_size){.cols = w.ws_col, .rows = w.ws_row};
}

void printImage(const char *filename)
{
	system("reset");
	int width, height, channels;
	unsigned char *image = stbi_load(filename, &width, &height, &channels, 3);

	if (image == NULL)
	{
		fprintf(stderr, "Error loading : %s\n%s", filename, stbi_failure_reason());
		stbi_image_free(image);
		return;
	}
	printf("IMAGE %s LOADED SUCCESSFULLY\n", filename);
	struct terminal_size TS = getTermSize();
	float aspect_ratio = (float)width / (float)height;

	int w = TS.cols;
	int h = (int)(w / aspect_ratio * 0.5);

	if (h > TS.rows)
	{
		h = TS.rows;
		w = (int)(h * aspect_ratio / 0.5);
	}

	unsigned char *new = stbir_resize_uint8_linear(image, width, height, 0, NULL, w, h, 0, STBIR_RGB);
	FILE *img = fopen("./output_img.txt", "w");
	if (new == NULL)
	{
		fprintf(stderr, "Error resizing the image: %s", stbi_failure_reason());
		stbi_image_free(image);
		return;
	}

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int idx = (i * w + j) * 3;
			int r = new[idx];
			int g = new[idx + 1];
			int b = new[idx + 2];
			fprintf(stderr, "\x1b[38;2;%d;%d;%dm█\x1b[0m", r, g, b);
			fprintf(img, "\x1b[38;2;%d;%d;%dm█\x1b[0m", r, g, b);
		}
		printf("\n");
		fprintf(img, "\n");
	}
	
	new = stbir_resize_uint8_linear(new, w, h, 0, NULL, width, height, 0, 3);

	int res = stbi_write_png("./image.png", width, height, STBIR_RGB, new, 0);
	fclose(img);
	

	if(!res){
		fprintf(stderr, "Error writing to image.png\n");
		stbi_image_free(image);
        	free(new);
		return ;
	}

	printf("The image was output it successfully to both the output_image.txt and image.png\n");

	stbi_image_free(image);
	free(new);
	return ;
}
