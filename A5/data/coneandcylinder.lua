-- A simple scene with some miscellaneous geometry.
-- This file is very similar to nonhier.lua, but interposes
-- an additional transformation on the root node.  
-- The translation moves the scene, and the position of the camera
-- changes accordingly.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, 0.0, 1.0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)
Green = gr.material({35/255.0, 181/255.0, 50/255.0}, {0.5, 0.4, 0.8}, 25)
DGreen = gr.material({0/255.0, 153/255.0, 0/255.0}, {0.5, 0.4, 0.8}, 25)
LGreen = gr.material({51/255.0, 255/255.0, 51/255.0}, {0.5, 0.4, 0.8}, 25)
Black = gr.material({0, 0, 0}, {0.5, 0.4, 0.8}, 25)
Yellow = gr.material({196/255.0, 208/255.0, 30/255.0}, {0.5, 0.4, 0.8}, 25)
Aqua = gr.material({20/255.0, 206/255.0, 235/255.0}, {0.5, 0.4, 0.8}, 25)
Blue = gr.material({0/255.0, 0/255.0, 153/255.0}, {0.5, 0.4, 0.8}, 25)
LBlue = gr.material({51/255.0, 51/255.0, 255/255.0}, {0.5, 0.4, 0.8}, 25)
DBlue = gr.material({0/255.0, 0/255.0, 102/255.0}, {0.5, 0.4, 0.8}, 25)
LPurple = gr.material({153/255.0, 51/255.0, 255/255.0}, {0.5, 0.4, 0.8}, 25)
Purple = gr.material({51/255.0, 0/255.0, 102/255.0}, {0.5, 0.4, 0.8}, 25)
DPurple = gr.material({25/255.0, 12/255.0, 51/255.0}, {0.5, 0.4, 0.8}, 25)

scene = gr.node( 'scene' )
scene1 = gr.node( 'scene1' )
scene:add_child(scene1)
-- scene:rotate('y', 90)
 scene:translate(0, 0, -300)
  
-- Middle Cone 
c1 = gr.cone('c1')
scene:add_child(c1)
c1:set_material(Yellow)
c1:translate(250,100,0)
c1:rotate('x', 90)
c1:scale(90,90,200)
-- Top Cone
c2 = gr.cone('c2')
scene:add_child(c2)
c2:set_material(Yellow)
c2:translate(350,450,0)
c2:rotate('x', 90)
c2:rotate('y', -54)
c2:scale(90,90,200)
-- Bottom Cone
c3 = gr.cone('c3')
scene:add_child(c3)
c3:set_material(Yellow)
c3:translate(150,-400,100)
c3:rotate('z', 36)
c3:rotate('x', 180)
c3:rotate('y', 54)

c3:scale(90,90,200)

-- Middle Cylinder
a1 = gr.cylinder('a1')
scene:add_child(a1)
a1:set_material(Green)
a1:translate(-250,0,0)
a1:rotate('x', 90)
a1:scale(90,90,90)
-- Top Cylinder
a2 = gr.cylinder('a2')
scene:add_child(a2)
a2:set_material(Green)
a2:translate(-250,350,0)
a2:rotate('x', 45)
a2:rotate('y', -60)
a2:scale(30,30,180)
-- Bottom Cylinder
a3 = gr.cylinder('a3')
scene:add_child(a3)
a3:set_material(Green)
a3:translate(-250,-350,-100)
a3:rotate('x', 120)
a3:scale(90,90,90)


-- c2 = gr.cone('c2')
-- scene:add_child(c2)
-- c2:set_material(mat3)
-- c2:translate(0,200,200)
-- c2:scale(100,200,100)
-- c2:rotate('x', 150)


-- c3 = gr.cone('c3')
-- scene:add_child(c3)
-- c3:set_material(mat3)
-- c3:translate(250,200,200)
-- c3:scale(100,200,100)
-- c3:rotate('x', 140)

-- c4 = gr.cone('c4')
-- scene:add_child(c4)
-- c4:set_material(mat3)
-- c4:translate(0,-200,200)
-- c4:scale(100,200,100)
-- c4:rotate('x', 90)





white_light = gr.light({-100.0, 0.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
white_light2 = gr.light({-100.0, -450.0, 400.0}, {0.4, 0.4, 0.4}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.0, 0.0, 0.9}, {1, 0, 0})

gr.render(scene, 'coneandcylinder.png', 400, 400,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, orange_light})