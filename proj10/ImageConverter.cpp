/*
 * ImageConverter.cpp declares a class for
 *  various image conversion operations.
 *
 *  Author:  Joel Adams, CS112 @ Calvin College.
 *  Created: March 2015
 *  Revised: November 2016
 */

#include "ImageConverter.h"
#include <unistd.h>            // sleep()

//-----------------  Method definitions ------------------------------

/* explicit constructor
 *
 * @param imageFileName, a string.
 * @param width, an int.
 * @param height, an int.
 *
 * Precondition: imageFileName contains the name of a valid image file
 *               && width > 0 && height > 0
 * Postcondition: myImageFileName == imageFileName
 *                && myWidth == width && myHeight = height
 *                && width x height Canvas containing the image
 *                    from imageFileName has been displayed on screen.
 */
ImageConverter::ImageConverter(const std::string& imageFileName, int width, int height)
: myCanvas1(0, 0, width, height, imageFileName),
  myImageFileName(imageFileName),
  myWidth(width),
  myHeight(height)
{
	myCanvas1.start();
	myCanvas1.drawImage(myImageFileName, 0, 0, width, height);
	sleep(1);
}

/* destructor
 *
 * Precondition: this ImageConverter object is going out of scope.
 * Postcondition: myCanvas1 has disappeared from the screen.
 */
ImageConverter::~ImageConverter() {
	myCanvas1.wait();
	cout << "\nImageConverter terminated normally." << endl;
}

/* invertImage() inverts the image from myImageFileName.
 *
 * Precondition: myCanvas1 contains a TSGL Image that is to be inverted
 *               && myWidth contains the number of columns in which to display the image
 *               && myHeight contains the number of rows in which to display the image.
 *
 * Postcondition: a myWidth x myHeight Canvas has been displayed
 *                 containing the inverse of the image from myCanvas1
 *                 (which contains the image from myImageFileName).
 */
void ImageConverter::invertImage() {
	// record starting time
	double startTime = omp_get_wtime();

	Canvas canvas2(myWidth+50, 0, myWidth, myHeight, myImageFileName + " Inverted");
	canvas2.start();

	// loop through the image rows
	#pragma omp parallel for
	for (int row = 0; row < myHeight; row++) {
		// slow the processing to simulate a very large image
		canvas2.sleep();
		// loop through the image columns
		for (int col = 0; col < myWidth; col++) {
			// read the pixel at canvas1[row][col]
			ColorInt pixelColor = myCanvas1.getPixel(row, col);
			// compute its inverse
			int invertedR = 255 - pixelColor.R;
			int invertedG = 255 - pixelColor.G;
			int invertedB = 255 - pixelColor.B;
			// draw the inverse at the same spot in canvas2
			canvas2.drawPixel(row, col, ColorInt(invertedR,invertedG,invertedB) );
		} // inner for
	} // outer for

	// compute and display the time difference
	double totalTime = omp_get_wtime() - startTime;
	cout << "\n\nImage inversion took "
			<< totalTime << " seconds.\n" << endl;

	//save converted image
	usleep(20000); // allows time for loading w/o lost pixels
	canvas2.takeScreenShot();
	canvas2.wait();
} // invertImage


// The simpler "chunk-size 1" approach is shown below.
void ImageConverter::invertImage2() {

	Canvas canvas3(myWidth+50, 0, myWidth, myHeight, myImageFileName + " Inverted, Chunk-Size 1");
	canvas3.start();

	// launch additional threads
	#pragma omp parallel
	{
		// how many workers do we have?
		unsigned numThreads = omp_get_num_threads();
		// which one am I?
		unsigned id = omp_get_thread_num();

		// loop through the image rows
		for (int row = id; row < myHeight; row += numThreads) {
			// slow the processing to simulate a very large image
			canvas3.sleep();
			// loop through the image columns
			for (int col = 0; col < myWidth; col++) {
				// read the pixel at canvas1[row][col]
				ColorInt pixelColor = myCanvas1.getPixel(row, col);
				// compute its inverse
				int invertedR = 255 - pixelColor.R;
				int invertedG = 255 - pixelColor.G;
				int invertedB = 255 - pixelColor.B;
				// draw the inverse at the same spot in canvas2
				canvas3.drawPixel(row, col, ColorInt(invertedR,invertedG,invertedB) );
			} // inner for
		} // outer for
	} // #pragma omp parallel

	canvas3.wait();
} // invertImage2


//  parallel processing using the "equal-sized chunks" approach
void ImageConverter::invertImage3() {
	Canvas canvas4(myWidth+50, 0, myWidth, myHeight, myImageFileName + " Inverted, Equal-Sized Chunks ");
	canvas4.start();

	// launch additional threads
	#pragma omp parallel
	{
		// how many workers do we have?
		unsigned numThreads = omp_get_num_threads();
		// which one am I?
		unsigned id = omp_get_thread_num();

		// compute size of chunks (iterations % numThreads may != 0)
		double iterations = myHeight;
		unsigned chunkSize = (unsigned)ceil(iterations / numThreads);

		// compute starting and stopping index values
		int start = id * chunkSize;
		int stop = 0;
		if (id < numThreads-1) {
			stop = start + chunkSize;
		} else {
			stop = myHeight;
		}

		// loop through image rows in equal-sized chunks
		for (int row = start; row < stop; row++) {
			// slow the processing to simulate a very large image
			canvas4.sleep();
			// loop through the image columns
			for (int col = 0; col < myWidth; col++) {
				// read the pixel at canvas1[row][col]
				ColorInt pixelColor = myCanvas1.getPixel(row, col);
				// compute its inverse
				int invertedR = 255 - pixelColor.R;
				int invertedG = 255 - pixelColor.G;
				int invertedB = 255 - pixelColor.B;
				// draw the inverse at the same spot in canvas2
				canvas4.drawPixel(row, col, ColorInt(invertedR,invertedG,invertedB) );
			} // inner for
		} // outer for
	} // #pragma omp parallel

	canvas4.wait();
} // invertImage3



/* Image Converter Horizontal Flip
 * @param:			none
 *
 * 	Suppose an image, where p_n is a pixel:
 * 	Precondition:			Postcondition:
 * 	p0  p1  p2  p3		->	p3  p2  p1  p0
 * 	p4  p5  p6  p7		->	p7  p6  p5  p4
 * 	p8  p9  p10 p11		->	p11 p10 p9  p8
 * 	p12 p13 p14 p15		->	p15 p14 p13 p12
 *
 * 	Postcondition: new Image is copied to file
 *
 * AUTHORED BY:		Michael Sisko
 */
void ImageConverter::horizFlip() {
	double startTime = omp_get_wtime();

	Canvas canvas5(myWidth+50, 0, myWidth, myHeight, myImageFileName + " Flipped Horizontally");
	canvas5.start();

	// loop through the image rows
	#pragma omp parallel for
	for (int row = 0; row < myHeight; row++) {
		// slow the processing to simulate a very large image
		canvas5.sleep();
		// loop through the image columns
		for (int col = 0; col < myWidth; col++) {
			// read the pixel at canvas1[row][col]
			ColorInt pixelColor = myCanvas1.getPixel(row, col);
			// draw the horizFlip at the same spot in canvas5
			canvas5.drawPixel( row, myHeight-col, ColorInt(pixelColor.R,pixelColor.G,pixelColor.B) );
		} // inner for
	} // outer for

	// compute and display the time difference
	double totalTime = omp_get_wtime() - startTime;
	cout << "\n\nImage horizontal flip took "
			<< totalTime << " seconds.\n" << endl;

    //save converted image
	usleep(20000); // allows time for loading w/o lost pixels
    canvas5.takeScreenShot();
	canvas5.wait();
} // horizFlip



/* Image Converter Vertical Flip
 * @param:			none
 *
 * 	Suppose an image, where p_n is a pixel:
 * 	Precondition:			Postcondition:
 * 	p0  p1  p2  p3		->	p12 p13 p14 p15
 * 	p4  p5  p6  p7		->	p8  p9  p10 p11
 * 	p8  p9  p10 p11		->	p4  p5  p6  p7
 * 	p12 p13 p14 p15		->	p0  p1  p2  p3
 *
 * 	Postcondition: new Image is copied to file
 *
 * AUTHORED BY:		Michael Sisko
 */
void ImageConverter::vertFlip() {
	double startTime = omp_get_wtime();

	Canvas canvas6(myWidth+50, 0, myWidth, myHeight, myImageFileName + " Flipped Vertically");
	canvas6.start();

	// loop through the image rows
	#pragma omp parallel for
	for (int row = 0; row < myHeight; row++) {
		// slow the processing to simulate a very large image
		canvas6.sleep();
		// loop through the image columns
		for (int col = 0; col < myWidth; col++) {
			// read the pixel at canvas1[row][col]
			ColorInt pixelColor = myCanvas1.getPixel(row, col);
			// draw the horizFlip at the same spot in canvas6
			canvas6.drawPixel( myWidth-row, col, ColorInt(pixelColor.R,pixelColor.G,pixelColor.B) );
		} // inner for
	} // outer for

	// compute and display the time difference
	double totalTime = omp_get_wtime() - startTime;
	cout << "\n\nImage vertical flip took "
			<< totalTime << " seconds.\n" << endl;

    //save converted image
	usleep(30000); // allows time for loading w/o lost pixels
    canvas6.takeScreenShot();
	canvas6.wait();
} // vertFlip








