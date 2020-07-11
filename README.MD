<p align="center"><img width=100% src="media/metaballsTitle.png"></p>

<h4 align="center">Dynamic and Static Implementation of Blinn's "Blobby Surface" Algorithm</p>

[![License: Unlicense](https://img.shields.io/badge/license-Unlicense-blue.svg)](http://unlicense.org/)

---

---

## Purpose

This repository was created for the code written and discussed in my first Youtube programming tutorial, as linked below.

<p><img width=50% src="media/metaballsDynamicGif.gif"><img width=50% src="media/metaballsStaticImg.png"></p>

---

## How does it work?

I'm glad you asked, as mentioned above I created an introductory video in 2019 about this topic. Feel free to take a look, but I will admit I wish I could go back and fix quite a few things. None the less, that's what the future is for!

[![Metaballs Algorithm Explained](media/metaballsVideoImg.png)](https://www.youtube.com/watch?v=tEYxodOJadw "Introduction to Metaballs")

##

####

---

## Q&A

### _Q_: Why did you include both a static and dynamic implementation in this repository if you wont cover the GLSL implementation?

**A**: I feel I covered the core basics enough in the static video such that if you understand that video you can understand and implement the GLSL approach. I didn't and don't really want to get into OpenGL too much on my channel as there are [fantastic resources for learning those already](https://learnopengl.com/).

### _Q_: Why did you chose SFML?

**A**: SFML is an API for multi-media applications. It allows us to handle a window instance, OpenGL, and input all in one library and avoid a ton of boiler-plate code. Since I wanted to talk more about the algorithm and how simple it was, I didn't want to muddle that point in a complex API. That said, I more often use SDL2. SFML is a tad cluttered with a lot more overhead.
