Seam Carving Project-

This project implements Seam Carving, a content-aware image resizing algorithm. Seam carving removes or inserts seams of pixels from an image to reduce or increase its dimensions while minimizing distortion to the important content. It intelligently determines which areas are less important and removes those first, preserving the main objects in the image.

The code leverages OpenCV for image manipulation and handles both horizontal and vertical seam removal to resize an image to the specified dimensions.

Features
Content-aware resizing: Seam carving preserves the most important regions of an image.
Custom resizing: Users specify the desired height and width of the output image.
Energy-based seam selection: The seams are computed using pixel energy, minimizing changes to important regions.
Command-line input: The code prompts users to input the file path and desired dimensions.
How Seam Carving Works
Energy Calculation:
The energy of each pixel is calculated based on the difference between neighboring pixel intensities (horizontal and vertical gradients).

Seam Identification:
A seam is a connected path of pixels from the top to the bottom (or left to right) of the image. The algorithm selects seams with the least energy, indicating minimal visual importance.

Seam Removal:
The identified seams are removed iteratively to achieve the target dimensions. Horizontal seams reduce the height, while vertical seams reduce the width.

Resizing Process:
The algorithm first reduces the width to the target size and then adjusts the height to prevent distortion.


Original Image                                                                                         Resized Image(357x285)
![HJoceanSmall](https://github.com/user-attachments/assets/59129f9b-162f-427e-ba21-41b3863dba0f)      ![HJoceanSmall357x285](https://github.com/user-attachments/assets/f17aaf4c-f4be-4ea3-860d-206ba55f71aa)

Reference
https://www.cs.princeton.edu/courses/archive/fall17/cos226/assignments/seam/index.html
