#include "SimilarityMeasure.h"

using namespace cv;

double colorSSD(const Mat* firstImage, const Mat* secondImage, Point imagePoint, int windowSize, Point offset)
{
	// Return infinity if the image point in the first image is outside the image
	if (imagePoint.x < 0 || imagePoint.y < 0 ||
		imagePoint.x >= firstImage->cols || imagePoint.y >= firstImage->rows)
		return std::numeric_limits<double>::infinity();
	// Return infinity if the image point in the first image is outside the image
	if (imagePoint.x + offset.x < 0 || imagePoint.y + offset.y < 0 ||
		imagePoint.x + offset.x >= secondImage->cols || imagePoint.y + offset.y >= secondImage->rows)
		return std::numeric_limits<double>::infinity();

	int imageRows = firstImage->rows;
	int imageCols = firstImage->cols * firstImage->channels();

	long ssd = 0;
	int pixelsAdded = 0;
	// Iterate through the patch
	for (int cY = -windowSize; cY < windowSize; cY++)
	{
		Point imagePatchPixel = Point();
		Point offsetPatchPixel = Point();
		imagePatchPixel.y = imagePoint.y + cY;
		offsetPatchPixel.y = imagePoint.y + offset.y + cY;

		// Try if all pixel are inside the image
		if (imagePatchPixel.y < 0 || imagePatchPixel.y >= firstImage->rows ||
			offsetPatchPixel.y < 0 || offsetPatchPixel.y >= secondImage->rows)
			continue;

		// Create the pointer on the rows of the images
		const double* firstImageRowP = firstImage->ptr<double>(imagePatchPixel.y);
		const double* secondImageRowP = secondImage->ptr<double>(offsetPatchPixel.y);

		for (int cX = -windowSize; cX < windowSize; cX++)
		{
			imagePatchPixel.x = imagePoint.x + cX;
			offsetPatchPixel.x = imagePoint.x + offset.x + cX;

			// Try if all pixel are inside the image
			if (imagePatchPixel.x < 0 || imagePatchPixel.x >= firstImage->cols ||
				offsetPatchPixel.x < 0 || offsetPatchPixel.x >= secondImage->cols)
				continue;
			// Use the pointer to the rows/pointer to get the values
			Vec3d firstImageVec = Vec3d(firstImageRowP[imagePatchPixel.x * 3],
										firstImageRowP[imagePatchPixel.x * 3 + 1],
										firstImageRowP[imagePatchPixel.x * 3 + 2]);
			Vec3d secondImageVec = Vec3d(secondImageRowP[offsetPatchPixel.x * 3],
										 secondImageRowP[offsetPatchPixel.x * 3 + 1],
										 secondImageRowP[offsetPatchPixel.x * 3 + 2]);
			// Calculate the difference between the both values
			Vec3d difference = firstImageVec - secondImageVec;
			ssd += static_cast<long>(difference[0]) * static_cast<long>(difference[0]) +
				   static_cast<long>(difference[1]) * static_cast<long>(difference[1]) +
				   static_cast<long>(difference[2]) * static_cast<long>(difference[2]);
			pixelsAdded++;
		}

	}
	return static_cast<double>(ssd) / static_cast<double>(pixelsAdded);
}