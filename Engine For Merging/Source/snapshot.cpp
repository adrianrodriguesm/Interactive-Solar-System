#include "../Headers/snapshot.h"

Snapshot::Snapshot()
{
}

/*This helps to flip the image vertically. It's necessary because in
  OpenGL the origin is at the bottom left corner, and most image formats
  have their origin at the top left corner.
*/
void Snapshot::flipVertically(int width, int height, char* data)
{
	char rgb[3];

	for (int y = 0; y < height / 2; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int top = (x + y * width) * 3;
			int bottom = (x + (height - y - 1) * width) * 3;

			memcpy(rgb, data + top, sizeof(rgb));
			memcpy(data + top, data + bottom, sizeof(rgb));
			memcpy(data + bottom, rgb, sizeof(rgb));
		}
	}
}

int Snapshot::saveSnapshot(const char* filename)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	int x = viewport[0];
	int y = viewport[1];
	int width = viewport[2];
	int height = viewport[3];

	char* data = (char*)malloc((size_t)(width * height * 3)); // 3 components (R, G, B)
	
	if (!data)
		return 0;
	
	//Necessary in order to get to correct picture.
	glReadBuffer(GL_FRONT);
	//Necessary for reading the pixels
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	//We'll read all the pixels from the framebuffer and save them in data.
	glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	// This is done toavoid getting the picture upside down
	flipVertically(width, height, data);
	//We save the picture in a png format.
	int saved = stbi_write_png(filename, width, height, 3, data, 0);

	free(data);

	return saved;
}

const char* Snapshot::createSnapshotBasename()
{
	static char basename[30];

	time_t t = time(NULL);
	//Format is: YearMonthDay_HourMinuteSecond
	strftime(basename, 30, "%Y%m%d_%H%M%S.png", localtime(&t));

	return basename;
}

int Snapshot::captureSnapshot()
{
	char filename[50];

	strcpy(filename, "../../Snapshots/");
	strcat(filename, createSnapshotBasename());

	int saved = saveSnapshot(filename);

	if (saved)
		printf("Successfully Saved Image: %s\n", filename);
	else
		fprintf(stderr, "Failed Saving Image: %s\n", filename);

	return saved;
}
