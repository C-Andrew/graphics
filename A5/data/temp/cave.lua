
gold = gr.material({0.9, 0.8, 0.4}, {0.8, 0.8, 0.4}, 25, 0.0)
green = gr.material({0.3, 0.8, 0.1}, {0.0, 0.0, 0.0}, 0, 0.0)
blue = gr.material({0.7, 0.6, 1}, {0.5, 0.4, 0.8}, 25, 0.6)
black = gr.material({0.0, 0.0, 0.0}, {0.5, 0.4, 0.8}, 25, 0.0, 0.0)
red = gr.material({1.0, 0.0, 0.0}, {0.5, 0.4, 0.8}, 25, 0.0, 0.0)
white = gr.material({0.8, 0.8, 0.8}, {0.5, 0.4, 0.8}, 25, 0.0, 0.0)

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
White = gr.material({1.0, 1.0, 1.0}, {0.5, 0.4, 0.8}, 25, 0.0, 0.0)


scene = gr.node('scene')
scene:translate(0, 0, -100)
scene:rotate('y', -2.5)

-- OBJECTS INSIDE THE ROOM

-- b1 = gr.nh_box('b1', {0,0,0}, 10)
-- scene:add_child(b1)
-- b1:set_material(blue)
-- b1:translate(-1, -2.5, 0);
-- b1:rotate('y', 30)
-- b1:rotate('x', 40)
-- b1:scale(1.5,1.5,1.5)

-- s1 = gr.nh_sphere('s1', {0,0,0}, 15)
-- scene:add_child(s1)
-- s1:set_material(blue)
-- s1:translate(-15,-15,10)

-- s2 = gr.nh_sphere('s2', {14, -25, 35}, 5)
-- scene:add_child(s2)
-- s2:set_material(red)

-- c1 = gr.cone('c1')
-- scene:add_child(c1)
-- c1:set_material(Yellow)
-- c1:translate(7,-18,20)
-- c1:rotate('x', 90)
-- c1:scale(6,6,12)

a1 = gr.cylinder('a1')
scene:add_child(a1)
a1:set_material(Aqua)
a1:translate(0,0,10)
a1:rotate('x', 90)
a1:scale(5,5,14)

require('regice_model')
-- scene:add_child(regice_instance)


regice_instance:translate(0,-5,10)
regice_instance:rotate('Y', 0)
regice_instance:scale(3, 3, 3)

regice_instance:add_child(regice_body)
regice_instance:add_child(regice_right_arm)
regice_instance:add_child(regice_left_arm)

regice_right_arm:rotate('z', -30)
regice_left_arm:rotate('y', 30)

regice_instance:add_child(eye1)
regice_instance:add_child(eye2)
regice_instance:add_child(eye3)
regice_instance:add_child(eye4)
regice_instance:add_child(eye5)
regice_instance:add_child(eye6)
regice_instance:add_child(eye7)



---------------------------

left = gr.nh_box('left', {0,0,0}, 10)
scene:add_child(left)
left:set_material(red)
left:translate(-37.5,0 , 10);
left:scale(0.5,10,20)

floor = gr.nh_box('floor', {0,0,0}, 10)
scene:add_child(floor)
floor:set_material(green)
floor:translate(0,-32 , 10);
floor:scale(20,0.5,20)

roof = gr.nh_box('roof', {0,0,0}, 10)
scene:add_child(roof)
roof:set_material(Aqua)
roof:translate(0,32 , 10);
roof:scale(10,0.5,15)

back = gr.nh_box('back', {0,0,0}, 10)
scene:add_child(back)
back:set_material(White)
back:translate(0,0 , -30);
back:scale(10,10,1)

-- The lights
l1 = gr.light({0,26,-114}, {0.8, 0.8, 0.8}, {1, 0, 0})
l2 = gr.light({0, 30, 0}, {0.4, 0.4, 0.8}, {1, 0, 0})
l3 = gr.light({0, 0, -114}, {0.0, 0.0, 0.8}, {1, 0, 0})


-- for i=0,30,1 do
--     s = 'cave' .. string.format("%.4d",i) .. '.png'
--     regice_right_arm:rotate('z', 1)
--     gr.render(scene, s, 500, 500, 
--           {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 50,
--           {0.4, 0.4, 0.4}, {l1, l2})  
--     print('Rendered Frame' .. i)
-- end


gr.render(scene, 'cave.png', 500, 500, 
      {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 50,
      {0.4, 0.4, 0.4}, {l1, l2})