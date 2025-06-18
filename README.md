# CSE306 Fluid Solver Project

Author : Alexandre Bismuth

Date : June 2025

***The full report for this project can be found in the fluid_solve_report.pdf file***

## Project Overview

This project implements a free-surface 2D fluid solver using incompressible Euler's equations. My project implements all of the mandatory sections of the assignment (Vonoroï Diagrams, Power Diagram, Optimization with LBFGS, de Gallouet-Mérigot incompressible Euler scheme, spring force from each fluid particle to their Laguerre's cell centroid), as well as the ungraded labs (diffusion lab, tutte embedding).

All of the project's code can be found in the present GitHub repository. Files have been split in headers and sources for organizational purposes, and I added a Makefile to simplify compilation.

## Acknowledgments

For this project, labs were typically started during the live coding sessions and finished at home using the course textbook and lecture slides. My implementation follows Prof. Bonneel’s examples and each key function includes references to the textbook. Therefore, some functions and the overall structure of my code may nonetheless resemble what was written during tutorial sessions. I would also like to acknowledge the help of Andreea Patarlageanu for her explanations on Optimization with LBFGS. Note however that I did not take any of her code.
