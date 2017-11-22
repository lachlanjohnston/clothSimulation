 # Cloth Simulation
 
 ### Abstract
 
 A 3D simulation of a cloth under the effects of various external forces. This cloth is also capable of colliding with other objects, and itself depending on its conditions.
 
 ### Goals
 
 1) The cloth will be able to move when exposed to a uniform field such as gravity or drag
 2) The cloth can collide with other simple objects, such as a cube or sphere
 3) The cloth can collide with, and thus fold inwards on itself
 
 ### Method
 
 The cloth will be rendered using OpenGL 3.2. A (TBD) linear algerbra library will be used in order to compute different aspects of the physics model. The cloth will be simulated using a simple display loop, where all objects are updated by their respective physics engine, and then pushed to an OpenGL renderer to be drawn to the screen.
 
 -- INSERT MATH HERE --
 
