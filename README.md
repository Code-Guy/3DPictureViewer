# 3DPictureViewer
A simple 3d pciture viewer.

![main](https://github.com/Code-Guy/3DPictureViewer/blob/master/snapshots/1.png?raw=true)

###Introduction
We've got a lot of picture viewer applications: XnView, Picasa, IrfanView, FastStone... But nearly all of these softwares only display images in 2d form. Don't you think that's boring? So I thought of an idea to add some 3d stuff into the traditional picture viewer, without messing up users' regular interactions.

###How to use
 2. Double click to open the 3DPictureViewer.exe
 3. Drag the picture directory into application window, the pictures in the directory will be imported recursively
 4. View pictures in 3d mode with your mouse
 5. Press and drag the left mouse button to rotate picture array slowly
 6. Press and slip the left mouse button to rotate picture array quickly
 7. Double click the middle picture to enter into display mode, where you can zoom and pan your selected picture.
 8. Press "next", "previous", "delete", "edit", "display" tool buttons on the bottom to do what you want to do.
 9. Press ESC key to quit viewer.
 
###Feature
 10. 3D picture displaying
 11. picture file name and size editing
 12. Decorative particle system 
 13. Smooth and  elastic slide animation
 14. Dynamic fragment shader Gaussian blur
 15. Double thread architecture: main thread for opengl rendering, work thread for loading asset
 16. Anti-Aliasing(based on MSAA)
 17. Frustum culling
 
###Framework
 Qt + OGL
 
###More Examples
![overlook](https://github.com/Code-Guy/3DPictureViewer/blob/master/snapshots/%E5%BC%80%E5%A7%8B%E7%95%8C%E9%9D%A2.png?raw=true)

![overlook](https://github.com/Code-Guy/3DPictureViewer/blob/master/snapshots/%E9%BE%99%E7%8C%AB.png?raw=true)

![overlook](https://github.com/Code-Guy/3DPictureViewer/blob/master/snapshots/%E8%A5%BF%E5%9F%8E%E7%94%B7%E5%AD%A9.png?raw=true)
