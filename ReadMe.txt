<<--------------------------------------------------------------------------------Animation_Project_03---------------------------------------------------------------------------------------------------------->>

# Developers :
	- This project was done by Max Sherwin

# About Project :
	- The project reads the scene contents( i.e the model properties, light properties, etc) from the json file located in "BasicAnimationSequenceController->AnimationSequenceController->SceneDescription.json".
	- Project runs multiple animations of different models
## How to build and run : 
	- The solution "Animation_Project03.sln" has to be built(either in debug or release platform mode) and then once succeeded(probably will!), make the "AnimationSequenceController" project as the startup project and now it'll be able to run for some visual treat.
	- When building the solution, the "MaxGameEngine" project should be run first since it is a static project without which the project with contents(AnimationSequenceController) cannot be run.
	
### Controls :
	- W,A,S,D,Q,E for camera movement(Front, Left, Back, Right, Up and Down respectively)
	- Mouse movement for camera rotation
	- Spacebar to pause and resume mouse mouvement(by default it is set to pause, so click on Spacebar when the scene starts to rotate camera with mouse)
	- 1, 2, 3, 4, 5 buttons are default camera showcase angles.
	- "T" Key to swicth between bone weights color and mesh texture.