# Velostat Development Kit

Welcome to the Velostat Development Kit (VDK) repository! This repository contains all the necessary components for the VDK, my Master's thesis. It is organized into several directories, each serving a specific purpose in the project development and implementation.

## Directories

### 1. Documentation

The `documentation` directory holds comprehensive and in-depth documentation related to the Project Name. It includes the following subdirectories and files:

- `vdk_demo.mp4`: This video shows the operation of the VDK.
- `vdk_literature.pdf`: This document presents the literature review done prior to the start of this project.
- `vdk_presentation`: This presentation was used during my thesis defence. 
- `vdk_thesis`: This document reports on my findings, implications and future work of the Velostat Development Kit in the form of a thesis.

### 2. Hardware

The `hardware` directory is dedicated to all things related to the hardware aspect of the Project Name. It includes:

- `velostat`: This directory contains the Arduino code that runs on the hardware component of the project. It's where you'll find the firmware responsible for controlling and interacting with the connected hardware devices.
- `pcb_design`: Here, you'll find all the design files necessary for the project's printed circuit board (PCB). This could include schematic diagrams, PCB layout files, BOM, and any associated documentation.

### 3. Web

The `web` directory holds the frontend part of the Project Name, developed using the Svelte framework. It includes:

- `src`: This directory contains the source code of the Svelte project. It's where you'll find all the components, styles, and logic that make up the web interface.
- `public`: Here, you'll find static assets like images, fonts, and any other files that should be served publicly.
- `README.md`: This file (you're reading it right now!) could contain specific instructions for setting up the development environment, building the Svelte project, and deploying the frontend.

### 4. images
The "images" directory is a collection of images showcasing the finished project. These images provide a visual representation of the project's appearance, functionality, and overall design. Feel free to browse through the images to get an idea of what the project looks like in action.


## Getting Started

If you're new to this project, here's how you can get started:

1. **Documentation**: Browse through the `documentation` directory to get an understanding of the project's purpose, its components, and how to use and contribute to it.

2. **Hardware**: If you're working with the hardware aspect of the project, navigate to the `hardware` directory. The `velostat` directory contains the firmware code for the Arduino component, and the `pcb_design` directory holds the PCB design documents. You can order the PCB with the included Gerber file. All the components of the BOM are widely available

3. **Web**: For frontend development using Svelte, head over to the `web` directory. The `src` directory houses the Svelte project source code. This folder is deployed to Netlify and can be accessed [here](https://vdk.louisdelooze.be).


## License
This project is licensed under GNU AGPLv3.

*Permissions of this strongest copyleft license are conditioned on making available complete source code of licensed works and modifications, which include larger works using a licensed work, under the same license. Copyright and license notices must be preserved. Contributors provide an express grant of patent rights. When a modified version is used to provide a service over a network, the complete source code of the modified version must be made available.*
