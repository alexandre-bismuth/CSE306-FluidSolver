# CSE306 Fluid Solver Project

Author : Alexandre Bismuth

Date : June 2025

***The full report for this project can be found in the fluid_solver_report.pdf file***

## Project Overview

This project implements a free-surface 2D fluid solver using incompressible Euler's equations. My project implements all of the mandatory sections of the assignment (Vonoroï Diagrams, Power Diagram, Optimization with LBFGS, de Gallouet-Mérigot incompressible Euler scheme, spring force from each fluid particle to their Laguerre's cell centroid), as well as the ungraded labs (diffusion lab, tutte embedding).

All of the project's code can be found in the present GitHub repository. Files have been split in headers and sources for organizational purposes, and I added a Makefile to simplify compilation.

After generating the frames of the fluid animation by compiling the project with `make main` and running it with `./main`, convert the set of frames into a single GIF files using the following Magick++ command:

```sh
convert -delay 4 -loop 0 frames/frame*.png animation.gif
```

## Acknowledgments

For this project, labs were typically started during the live coding sessions and finished at home using the course textbook and lecture slides. My implementation follows Prof. Bonneel’s examples and each key function includes references to the textbook. Therefore, some functions and the overall structure of my code may nonetheless resemble what was written during tutorial sessions. I would also like to acknowledge the help of Andreea Patarlageanu for her explanations on Optimization with LBFGS. Note however that I did not take any of her code.

Furthermore, to solve an error code that I had with LBFGS *(error code -999, indicating that the current search direction increases the objective function value)* in the fluid simulation (only the last commit on the FluidDynamics branch), I also used Google Gemini. It indicated me that my centroid function was handling degenerate polygons badly, and told me about an issue with `memcpy`. I therefore integrated those changes in a separate commit, the last one on the `FluidDynamics` branch. Here, Gemini only contributed to 13 lines of code, a minor impact.

This use of Gemini was truly as a last resort after trying to debug for multiple hours. I understand that this can take points off Lab 8 but believe that using Gemini wouldn't discredit the rest of the work I have done. I have included both animations for the ultimate and penultimate commit, for you to see my result before and after those changes.
