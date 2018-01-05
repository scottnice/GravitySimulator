# GravitySimulator
N-body simulator with 3d physics

Download and run the latest version of Cmake point to the src/ dir as the directory containing the source code. Create a dir for the project to build. I use GravitySimulator/bin and tell Cmake to build the binaries there. Unselect option BUILD_TESTING in the Cmake GUI. 

There is a provided runnable exe located in the release\ folder. The exe must be run from this folder as it has the required relative path to find the shaders in src\shaders

Controls:
Use the up/down/left/right arrow keys to move the camera. 
Use the wsadqe keys to move the control the cameras, pitch, roll and yaw in that order.
Use left click to zoom to a target object.
