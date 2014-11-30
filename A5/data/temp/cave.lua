
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
Yellow = gr.material({230/255.0, 230/255.0, 10/255.0}, {0.5, 0.4, 0.8}, 25, 0.0, 1.0)
Aqua = gr.material({20/255.0, 206/255.0, 235/255.0}, {0.5, 0.4, 0.8}, 25, 0.3, 0.0)
Blue = gr.material({0/255.0, 0/255.0, 153/255.0}, {0.5, 0.4, 0.8}, 25)
LBlue = gr.material({51/255.0, 51/255.0, 255/255.0}, {0.5, 0.4, 0.8}, 25, 0.4, 0.0)
DBlue = gr.material({0/255.0, 0/255.0, 102/255.0}, {0.5, 0.4, 0.8}, 25)
LPurple = gr.material({153/255.0, 51/255.0, 255/255.0}, {0.5, 0.4, 0.8}, 25)
Purple = gr.material({51/255.0, 0/255.0, 102/255.0}, {0.5, 0.4, 0.8}, 25)
DPurple = gr.material({25/255.0, 12/255.0, 51/255.0}, {0.5, 0.4, 0.8}, 25)
White = gr.material({1.0, 1.0, 1.0}, {0.5, 0.4, 0.8}, 25, 0.0, 0.0)

brown = gr.material({90/255.0, 53/255.0, 15/255.0}, {0.0, 0.0, 0.0}, 25, 0.1, 0.0)
dirt = gr.material({90/255.0, 53/255.0, 15/255.0}, {0.0, 0.0, 0.0}, 25, 0.0, 0.0)
regice_mat = gr.material({20/255.0, 206/255.0, 235/255.0}, {0.5, 0.4, 0.8}, 25, 0.0, 1.0)
icy = gr.material({115/255.0, 204/255.0, 255/255.0}, {0.5, 0.4, 0.8}, 25, 0.3, 0.0)
icy2 = gr.material({108/255.0, 185/255.0, 229/255.0}, {0.5, 0.4, 0.8}, 25, 0.4, 0.0)
icy3 = gr.material({44/255.0, 165/255.0, 235/255.0}, {0.5, 0.4, 0.8}, 25, 0.3, 0.0)


-- gr.bump(dirt, 'bump_rocks.png')

scene = gr.node('scene')
scene:translate(20, -50, -80)
-- scene:rotate('x',20)



floor = gr.nh_box('floor', {0,0,0}, 10)
scene:add_child(floor)
floor:set_material(icy)
floor:translate(0,30 , 0);
floor:scale(20,0.5,10)

back = gr.nh_box('back', {0,0,0}, 10)
scene:add_child(back)
back:set_material(dirt)
back:translate(-4,28,20)
back:rotate('y', -30)
back:scale(1,1,8)

require('regice_model')
-- scene:add_child(regice_instance)


regice_instance:translate(20,35,-20)
regice_instance:rotate('y', -25)
-- regice_instance:rotate('z', 5)
regice_instance:scale(5, 5, 5)

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

-- Trainer
-- trainer = gr.mesh('trainer', readobj('trainer.obj'))
-- scene:add_child(trainer)
-- trainer:set_material(brown)
-- trainer:translate(0,35,-10)
-- trainer:rotate('y', 270)

-- trainer:scale(1,1,1)

---------

-- sphere
rock_poly = gr.mesh('rock_poly', {
    {   1.,             1.,              1.},
    {   1.,             1.,              -1.},
    {   1.,             -1.,             1.},
    {   1.,             -1.,             -1.},
    {  -1.,             1.,              1.},
    {  -1.,             1.,              -1.},
    {  -1.,             -1.,             1.},
    {  -1.,             -1.,             -1.},
    {   0.618034,        1.618034,        0.},
    {  -0.618034,        1.618034,        0.},
    {   0.618034,       -1.618034,        0.},
    {  -0.618034,       -1.618034,        0.},
    {   1.618034,        0.,              0.618034},
    {   1.618034,        0.,             -0.618034},
    {  -1.618034,        0.,              0.618034},
    {  -1.618034,        0.,             -0.618034},
    {   0.,             0.618034,         1.618034},
    {   0.,             -0.618034,        1.618034},
    {   0.,             0.618034,        -1.618034},
    {   0.,             -0.618034,       -1.618034}
   }, {
    {  1,       8,       0,       12,      13 },
    {  4,       9,       5,       15,      14 },
    {  2,       10,      3,       13,      12 },
    {  7,       11,      6,       14,      15 },
    {  2,       12,      0,       16,      17 },
    {  1,       13,      3,       19,      18 },
    {  4,       14,      6,       17,      16 },
    {  7,       15,      5,       18,      19 },
    {  4,       16,      0,        8,       9 },
    {  2,       17,      6,       11,      10 },
    {  1,       18,      5,        9,       8 },
    {  7,       19,      3,       10,      11 }
 })
rock_poly:set_material(brown)

ice_poly = gr.mesh('ice_poly', {
    {   1.,             1.,              1.},
    {   1.,             1.,              -1.},
    {   1.,             -1.,             1.},
    {   1.,             -1.,             -1.},
    {  -1.,             1.,              1.},
    {  -1.,             1.,              -1.},
    {  -1.,             -1.,             1.},
    {  -1.,             -1.,             -1.},
    {   0.618034,        1.618034,        0.},
    {  -0.618034,        1.618034,        0.},
    {   0.618034,       -1.618034,        0.},
    {  -0.618034,       -1.618034,        0.},
    {   1.618034,        0.,              0.618034},
    {   1.618034,        0.,             -0.618034},
    {  -1.618034,        0.,              0.618034},
    {  -1.618034,        0.,             -0.618034},
    {   0.,             0.618034,         1.618034},
    {   0.,             -0.618034,        1.618034},
    {   0.,             0.618034,        -1.618034},
    {   0.,             -0.618034,       -1.618034}
   }, {
    {  1,       8,       0,       12,      13 },
    {  4,       9,       5,       15,      14 },
    {  2,       10,      3,       13,      12 },
    {  7,       11,      6,       14,      15 },
    {  2,       12,      0,       16,      17 },
    {  1,       13,      3,       19,      18 },
    {  4,       14,      6,       17,      16 },
    {  7,       15,      5,       18,      19 },
    {  4,       16,      0,        8,       9 },
    {  2,       17,      6,       11,      10 },
    {  1,       18,      5,        9,       8 },
    {  7,       19,      3,       10,      11 }
 })
ice_poly:set_material(icy2)

rock = gr.node('rock')
rock:add_child(rock_poly)
scene:add_child(rock)
rock:translate(20,35,-38)
rock:scale(3,3,3)

rock1 = gr.node('rock1')
rock1:add_child(rock_poly)
scene:add_child(rock1)
rock1:translate(9,35,-31)
rock1:rotate('y',21)
rock1:scale(1.5,1.5,1.5)


rock2 = gr.node('rock2')
rock2:add_child(rock_poly)
scene:add_child(rock2)
rock2:translate(37,35,-38)
rock2:rotate('x',23)
rock2:scale(5,5,5)

rock3 = gr.node('rock3')
rock3:add_child(rock_poly)
scene:add_child(rock3)
rock3:translate(36,37,-21)
rock3:rotate('y',21)
rock3:scale(2,2,2)

rock4 = gr.node('rock4')
rock4:add_child(rock_poly)
scene:add_child(rock4)
rock4:translate(5,33,-24)
rock4:rotate('y',21)
rock4:scale(1.5,1.5,1.5)

rock5 = gr.node('rock5')
rock5:add_child(rock_poly)
scene:add_child(rock5)
rock5:translate(7,33,-29)
rock5:rotate('y',12)
rock5:scale(1.5,1.5,1.5)


-- Add Rocks for background


-- rock100 = gr.node('rock100')
-- rock100:add_child(rock_poly)
-- scene:add_child(rock100)
-- rock100:translate(90,35,-85)
-- rock100:rotate('x',23)
-- rock100:scale(14,14,14)

-- rock101 = gr.node('rock101')
-- rock101:add_child(rock_poly)
-- scene:add_child(rock101)
-- rock101:translate(70,68,-65)
-- rock101:rotate('x',23)
-- rock101:scale(14,14,14)

-- rock102 = gr.node('rock102')
-- rock102:add_child(rock_poly)
-- scene:add_child(rock102)
-- rock102:translate(70,90,-65)
-- rock102:rotate('z',45)
-- rock102:scale(14,14,14)

-- rock104 = gr.node('rock104')
-- rock104:add_child(rock_poly)
-- scene:add_child(rock104)
-- rock104:translate(34,35,-70)
-- rock104:rotate('x',33)
-- rock104:scale(14,14,14)

-- rock105 = gr.node('rock105')
-- rock105:add_child(rock_poly)
-- scene:add_child(rock105)
-- rock105:translate(30,65,-80)
-- rock105:rotate('z',60)
-- rock105:scale(16,16,16)

-- rock1052 = gr.node('rock1052')
-- rock1052:add_child(rock_poly)
-- scene:add_child(rock1052)
-- rock1052:translate(40,40,-80)
-- rock1052:rotate('z',60)
-- rock1052:scale(16,16,16)


-- rock106 = gr.node('rock106')
-- rock106:add_child(rock_poly)
-- scene:add_child(rock106)
-- rock106:translate(32,90,-75)
-- rock106:rotate('x',45)
-- rock106:scale(14,14,14)

-- rock107 = gr.node('rock107')
-- rock107:add_child(rock_poly)
-- scene:add_child(rock107)
-- rock107:translate(0,35,-70)
-- rock107:rotate('x',59)
-- rock107:scale(14,14,14)

-- rock108 = gr.node('rock108')
-- rock108:add_child(rock_poly)
-- scene:add_child(rock108)
-- rock108:translate(15,55,-80)
-- rock108:rotate('z',9)
-- rock108:scale(13,13,13)

-- rock109 = gr.node('rock109')
-- rock109:add_child(rock_poly)
-- scene:add_child(rock109)
-- rock109:translate(0,90,-90)
-- rock109:rotate('x',21)
-- rock109:scale(17,17,17)

-- rock110 = gr.node('rock110')
-- rock110:add_child(rock_poly)
-- scene:add_child(rock110)
-- rock110:translate(-30,35,-70)
-- rock110:rotate('z',59)
-- rock110:scale(10,10,10)

-- rock111 = gr.node('rock111')
-- rock111:add_child(rock_poly)
-- scene:add_child(rock111)
-- rock111:translate(-15,55,-80)
-- rock111:rotate('x',9)
-- rock111:scale(15,15,15)

-- rock112 = gr.node('rock112')
-- rock112:add_child(rock_poly)
-- scene:add_child(rock112)
-- rock112:translate(-30,85,-90)
-- rock112:rotate('z',65)
-- rock112:scale(14,14,14)

-- rock113 = gr.node('rock113')
-- rock113:add_child(rock_poly)
-- scene:add_child(rock113)
-- rock113:translate(-65,35,-70)
-- rock113:rotate('x',9)
-- rock113:scale(16,16,16)

-- rock114 = gr.node('rock114')
-- rock114:add_child(rock_poly)
-- scene:add_child(rock114)
-- rock114:translate(-50,55,-80)
-- rock114:rotate('x',28)
-- rock114:scale(12,12,12)

-- rock115 = gr.node('rock115')
-- rock115:add_child(rock_poly)
-- scene:add_child(rock115)
-- rock115:translate(-55,90,-90)
-- rock115:rotate('x',65)
-- rock115:scale(14,14,14)


----------------------------
ice = gr.node('ice')
ice:add_child(ice_poly)
scene:add_child(ice)
ice:translate(14,35,-34)
ice:scale(2,2,2)

ice2 = gr.node('ice2')
ice2:add_child(ice_poly)
scene:add_child(ice2)
ice2:translate(25,35,-39)
ice2:rotate('y',45)
ice2:scale(4,4,4)

ice3 = gr.node('ice3')
ice3:add_child(ice_poly)
scene:add_child(ice3)
ice3:translate(36,35,-28)
ice3:rotate('y',19)
ice3:scale(3,3,3)

ice4 = gr.node('ice4')
ice4:add_child(ice_poly)
scene:add_child(ice4)
ice4:translate(36,35,-18)
ice4:rotate('y',19)
ice4:scale(2,2,2)

ice5 = gr.node('ice5')
ice5:add_child(ice_poly)
scene:add_child(ice5)
ice5:translate(33,35,-12)
ice5:rotate('y',19)
ice5:scale(1.7,1.7,1.7)


-- crystal_poly = gr.mesh('crystal', readobj('crystal.obj'))
-- crystal_poly:set_material(icy3)

-- crystal1 = gr.node('crystal1')
-- crystal1:add_child(crystal_poly)
-- scene:add_child(crystal1)
-- crystal1:translate(25,35,-12)
-- crystal1:rotate('y',19)
-- crystal1:scale(5,5,5)


a1 = gr.sphere('s1', {0, 0, 0}, 2)
scene:add_child(a1)
a1:set_material(icy3)
a1:translate(20,33,-20)
-- a1:rotate('z', 6)
-- a1:rotate('x', -2)
a1:scale(15,0.1,15)


---------------------------

-- left = gr.nh_box('left', {0,0,0}, 10)
-- scene:add_child(left)
-- left:set_material(brown)
-- left:translate(-37.5,0 , 10);
-- left:scale(0.5,10,20)

-- floor = gr.nh_box('floor', {0,0,0}, 10)
-- scene:add_child(floor)
-- floor:set_material(brown)
-- floor:translate(0,-32 , 0);
-- floor:rotate('x', 08)
-- floor:scale(10,0.5,10)

-- roof = gr.nh_box('roof', {0,0,0}, 10)
-- scene:add_child(roof)
-- roof:set_material(Aqua)
-- roof:translate(0,32 , 10);
-- roof:scale(10,0.5,15)

-- back = gr.nh_box('back', {0,0,0}, 10)
-- scene:add_child(back)
-- back:set_material(White)
-- back:translate(0,0 , -100);
-- back:scale(10,10,1)

-- The lights
l1 = gr.light({0,26,114}, {0.8, 0.8, 0.8}, {1, 0, 0})
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
      {0, 0, 0}, {0.20, 0.-0.1, -1}, {0, 1, 0}, 50,
      {0.4, 0.4, 0.4}, {l1})