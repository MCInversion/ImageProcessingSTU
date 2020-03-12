# ImageProcessingSTU

## Features 

### Mirror Extend by n pixels
<img src="https://github.com/MCInversion/ImageProcessingSTU/blob/master/screens/mirrorExtendTestGoodEnough.gif" height="50%" width="50%">
Extends the image by its mirror transformations and by reflections along its corners by a given amount of pixels.
Will be used in `Blur`.

### Histogram
<img src="https://github.com/MCInversion/ImageProcessingSTU/blob/master/screens/histogramTestGoodEnough.gif" height="80%" width="80%">
A fully resizeable histogram window with the following features:
- Grayscale / RGB histogram plot of the active image
- Cumulative histograms (CDF) available from the respective checkbox
- Histogram transformations (contrast stretch) with adjustable boundaries
- Possibility to transform individual channels of an RGB image
- Light/Dark mode

#### Histogram for grayscale images
<img src="https://github.com/MCInversion/ImageProcessingSTU/blob/master/screens/gsLightVsDark.jpg" height="80%" width="80%">

### Convolution with a square kernel of radius r
<img src="https://github.com/MCInversion/ImageProcessingSTU/blob/master/screens/blurTestGoodEnough.gif" height="70%" width="70%">
Applies a blur effect with the following kernels with adjustable radius `r` (pixels from the center):
- `Gaussian` (with weights given by a Gaussian distribution)
- `Averaging` (with equal weihgts adding up to 1)

#### Planned Features:
- Undo / Redo
- background processing thread
- active img <--> widgets communication/update


