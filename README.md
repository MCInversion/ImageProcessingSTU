# ImageProcessingSTU

## Features 

### Mirror Extend by n pixels
![](https://github.com/MCInversion/ImageProcessingSTU/blob/master/screens/mirrorExtendTestGoodEnough.gif)

Extends the image by its mirror transformations and by reflections along its corners by a given amount of pixels.
Will be used in `Blur`.

### Histogram
![](https://github.com/MCInversion/ImageProcessingSTU/blob/master/screens/histogramTestGoodEnough.gif)

A fully resizeable histogram window with the following features:
- Grayscale / RGB histogram plot of the active image
- Cumulative histograms (CDF) available from the respective checkbox
- Histogram transformations (contrast stretch) with adjustable boundaries
- Possibility to transform individual channels of an RGB image
- Light/Dark mode

#### Histogram for grayscale images
![](https://github.com/MCInversion/ImageProcessingSTU/blob/master/screens/gsLightVsDark.jpg)

### Convolution with a square kernel of radius r
![](https://github.com/MCInversion/ImageProcessingSTU/blob/master/screens/blurTestGoodEnough.gif)

Applies a blur effect with the following kernels with adjustable radius `r` (pixels from the center):
- `Gaussian` (with weights given by a Gaussian distribution)
- `Averaging` (with equal weihgts adding up to 1)
Will add more kernels with different properties.
For debugging purposes one can then check the output in the console, if needed.

#### Planned Features:
- Undo / Redo
- background processing thread
- active img <--> widgets communication/update
- more kernels


