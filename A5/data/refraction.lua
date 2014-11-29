-- test for hierarchical ray-tracers.
-- Thomas Pflaum 1996

gold = gr.material({0.9, 0.8, 0.4}, {0.8, 0.8, 0.4}, 25, 0.0)
green = gr.material({0.3, 0.8, 0.1}, {0.0, 0.0, 0.0}, 0, 1.0)
blue = gr.material({1, 1, 1}, {0.5, 0.4, 0.8}, 25, 0.0, 2.7)
black = gr.material({0.0, 0.0, 0.0}, {0.5, 0.4, 0.8}, 25, 0.0, 0.0)
red = gr.material({1.0, 0.0, 0.0}, {0.5, 0.4, 0.8}, 25, 1.0, 0.0)
white = gr.material({0.8, 0.8, 0.8}, {0.5, 0.4, 0.8}, 25, 0.0, 0.0)

Red = gr.material({1.0, 0.0, 0.0}, {0.5, 0.4, 0.8}, 25, 0.0, 0.0)
Green = gr.material({35/255.0, 181/255.0, 50/255.0}, {0.5, 0.4, 0.8}, 25, 0.0, 0.0)
DGreen = gr.material({0/255.0, 153/255.0, 0/255.0}, {0.5, 0.4, 0.8}, 25)
LGreen = gr.material({51/255.0, 255/255.0, 51/255.0}, {0.5, 0.4, 0.8}, 25)
Black = gr.material({0, 0, 0}, {0.5, 0.4, 0.8}, 25)
Yellow = gr.material({230/255.0, 230/255.0, 10/255.0}, {0.5, 0.4, 0.8}, 25, 0.0, 0.0)
Aqua = gr.material({20/255.0, 206/255.0, 235/255.0}, {0.5, 0.4, 0.8}, 25, 0.0, 0.0)
Blue = gr.material({0/255.0, 0/255.0, 153/255.0}, {0.5, 0.4, 0.8}, 25)
LBlue = gr.material({51/255.0, 51/255.0, 255/255.0}, {0.5, 0.4, 0.8}, 25, 0.4, 0.0)
DBlue = gr.material({0/255.0, 0/255.0, 102/255.0}, {0.5, 0.4, 0.8}, 25)
LPurple = gr.material({153/255.0, 51/255.0, 255/255.0}, {0.5, 0.4, 0.8}, 25)
Purple = gr.material({51/255.0, 0/255.0, 102/255.0}, {0.5, 0.4, 0.8}, 25)
DPurple = gr.material({25/255.0, 12/255.0, 51/255.0}, {0.5, 0.4, 0.8}, 25)
White = gr.material({1.0, 1.0, 1.0}, {0.5, 0.4, 0.8}, 25, 0, 0)


scene = gr.node('scene')
scene:translate(0, 0, -114)
-- scene:rotate('y', 90)


-- OBJECTS INSIDE THE ROOM

-- require('regice_model')
-- scene:add_child(regice_instance)
-- regice_instance:add_child(regice)
-- regice_instance:add_child(eye1)
-- regice_instance:add_child(eye2)
-- regice_instance:add_child(eye3)
-- regice_instance:add_child(eye4)
-- regice_instance:add_child(eye5)
-- regice_instance:add_child(eye6)
-- regice_instance:add_child(eye7)


-- regice_instance:translate(0,0,-15)
-- regice_instance:rotate('Y', 0)
-- regice_instance:scale(3, 3, 3)

b1 = gr.nh_box('b1', {0,0,0}, 10)
scene:add_child(b1)
b1:set_material(Red)
b1:translate(20,-15,-20)
-- b1:rotate('y', 30)
b1:scale(1,1,1)

s2 = gr.nh_sphere('s2', {0,0,0}, 10)
scene:add_child(s2)
s2:set_material(blue)
s2:translate(-45,0,30)
-- s2:scale()

c1 = gr.cone('c1')
scene:add_child(c1)
c1:set_material(Yellow)
c1:translate(0,-18,-15)
c1:rotate('x', 90)
c1:scale(6,6,12)

a1 = gr.cylinder('a1')
scene:add_child(a1)
a1:set_material(Blue)
a1:translate(-15,0,-15)
a1:rotate('x', 0)
a1:scale(8,8,20)

------------------------------------

left = gr.mesh('left', {
    {-1, 0, -1},
    { 1, 0, -1},
    {1,  0, 1},
    {-1, 0, 1},
     }, {
    {3, 2, 1, 0}
     })
scene:add_child(left)
left:set_material(red)
left:translate(-30,0,0)
left:rotate('z' , -90)
left:scale(50, 50, 50)

floor = gr.mesh('floor', {
    {-1, 0, -1},
    { 1, 0, -1},
    {1,  0, 1},
    {-1, 0, 1},
     }, {
    {3, 2, 1, 0}
     })

scene:add_child(floor)
floor:set_material(white)
floor:translate(0,-30,0)
floor:scale(50, 50, 50)

back = gr.mesh('back', {
    {-1, 0, -1},
    { 1, 0, -1},
    {1,  0, 1},
    {-1, 0, 1},
     }, {
    {3, 2, 1, 0}
     })
scene:add_child(back)
back:set_material(white)

back:translate(0,0,-30)
back:rotate('x' , 90)
back:scale(30, 30, 30)


right = gr.mesh('right', {
    {-1, 0, -1},
    { 1, 0, -1},
    {1,  0, 1},
    {-1, 0, 1},
     }, {
    {3, 2, 1, 0}
     })
scene:add_child(right)
right:set_material(green)
right:translate(30,0,0)
right:rotate('z' , 90)
right:scale(50, 50, 50)

roof = gr.mesh('roof', {
    {-1, 0, -1},
    { 1, 0, -1},
    {1,  0, 1},
    {-1, 0, 1},
     }, {
    {3, 2, 1, 0}
     })
scene:add_child(roof)
roof:set_material(white)
roof:translate(0,30,0)
roof:rotate('z', 180)
roof:scale(50, 50, 50)

front = gr.mesh('front', {
    {-1, 0, -1},
    { 1, 0, -1},
    {1,  0, 1},
    {-1, 0, 1},
     }, {
    {3, 2, 1, 0}
     })
scene:add_child(front)
front:set_material(black)
front:translate(0,0,114)
front:rotate('x' , 90)
front:scale(50, 50, 50)


-- The lights
l1 = gr.light({0,26,-114}, {0.8, 0.8, 0.8}, {1, 0, 0})
l2 = gr.light({0, 30, 0}, {0.4, 0.4, 0.8}, {1, 0, 0})
l3 = gr.light({0, 0, -114}, {0.0, 0.0, 0.8}, {1, 0, 0})


-- gr.render(scene, 'refraction.png', 500, 500, 
--       {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 50,
--       {0.4, 0.4, 0.4}, {l1, l2})  

for i=0,450,1 do
    s = './frames/refraction' .. string.format("%.4d",i) .. '.png'
    s2:translate(0 + 0.2,0,0)
    b1:rotate('z', 0.5)
    -- c1:rotate('x', 0.5)
    a1:rotate('x', 0.5)
    gr.render(scene, s, 1000, 1000, 
          {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 50,
          {0.4, 0.4, 0.4}, {l1, l2})  
    print('Rendered Frame' .. i)
end