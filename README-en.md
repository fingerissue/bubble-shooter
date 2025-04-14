# bubble-shooter
[README for Korean ver.](./README.md)

## Completion date
First : 2022.11.28 5:49 p.m.

Modified: 2025.04.15 00:02 a.m.

## Run Environment
Windows 11, Visual Studio (Admin)

This file uses **windows.h**, so other enviroments may be impossible to run.

If it doesn't run in **Windows and Admin**, the program may not run normally.

## How to run
### Install 
[Install Visual Studio](https://visualstudio.microsoft.com/ko/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&passive=false&cid=2030) > (Click Modify at Visual Studio Installer) > Desktop & Mobile > Install desktop development using C++
### Run
Run "BubbleShooter.sln" by **Admin** > Ctrl + F5

## How to play
![image](./images/GameLayout.bmp)

## FAQ
<details><summary>
1. I can't see the screen!

1-1. Only black appears on the screen!<br>
1-2. Black appears at the corner of the screen!</summary>

A1. Try it with administrator privileges

A2. If that doesn't work, please check the following settings!
 - Project > Properties > Configuration Properties > Advanced > Using Character Set Multi-Byte Matter Set
</details>
<details><summary>
2. It's too slow!

2-1. I don't think the keyboard is working!<br>
2-2. The screen stopped!</summary>

A2. Me too... If you wait patiently, it will happen someday.

Let's make it meaningful that the code works for now...!
</details>
<details><summary>
3. Isn't there an executable file?

3-1. Is there no exe file?</summary>

A3. There is no exe file because I don't know how to make it.
</details>

## Thank you for your understanding.
When I was a freshman in high school, the task was to make a console game (using ONLY C Language). This project was for that task!

In order to show the code written in my first year of high school as authentically as possible,

I will minimize the modifications such as making it easier for people to read.

Even if the code is messy, even if it is too slow, 

please consider it as the work of a first-year high school student.

Thank you for your understanding.

## I got help
### Image Processing Header File
|Original|Improvement|
|---|---|
|[Link](https://github.com/MinSeungHyun/CodeTheCompany/tree/master/ConsoleGame/ImageUtils)|[Link](https://hdox.de/manylayer)|
 > ./headers/ImageLayer.h
 >
 > ./headers/ImageLayerImpl.h
 > 
 > ./headers/ImageLayerInterface.h

### Image File
Help (Anonymous)
 > ./images/StartScreen_*.bmp
 >
 > ./images/Bubble_*.bmp