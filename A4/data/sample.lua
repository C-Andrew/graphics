-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
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



scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {-300, 300, -400}, 100)
scene_root:add_child(s1)
s1:set_material(LGreen)


s2 = gr.nh_sphere('s2',  {0, 300, -400}, 100)
scene_root:add_child(s2)
s2:set_material(Green)

s3 = gr.nh_sphere('s3',  {300, 300, -400}, 100)
scene_root:add_child(s3)
s3:set_material(DGreen)

b1 = gr.nh_box('b1', {-350, -80, -400}, 150)
scene_root:add_child(b1)
b1:set_material(LPurple)

b2 = gr.nh_box('b2', {-70, -80, -400}, 150)
scene_root:add_child(b2)
b2:set_material(Purple)

b3 = gr.nh_box('b3', {210, -80, -400}, 150)
scene_root:add_child(b3)
b3:set_material(DPurple)

s4 = gr.nh_sphere('s4', {-300, -300, -400}, 100)
scene_root:add_child(s4)
s4:set_material(LBlue)

s5 = gr.nh_sphere('s5',  {0, -300, -400}, 100)
scene_root:add_child(s5)
s5:set_material(Blue)

s6 = gr.nh_sphere('s6',  {300, -300, -400}, 100)
scene_root:add_child(s6)
s6:set_material(DBlue)

big = gr.nh_sphere('big', {0, 0, -1000}, 500)
scene_root:add_child(big)
big:set_material(Aqua)


-- s2 = gr.nh_sphere('s2', {200, 50, -100}, 150)
-- scene_root:add_child(s2)
-- s2:set_material(mat1)

-- s3 = gr.nh_sphere('s3', {0, -1200, -500}, 1000)
-- scene_root:add_child(s3)
-- s3:set_material(mat2)

-- b1 = gr.nh_box('b1', {-200, -125, 0}, 100)
-- scene_root:add_child(b1)
-- b1:set_material(mat4)

-- s4 = gr.nh_sphere('s4', {-100, 25, -300}, 50)
-- scene_root:add_child(s4)
-- s4:set_material(mat3)

-- s5 = gr.nh_sphere('s5', {0, 100, -250}, 25)
-- scene_root:add_child(s5)
-- s5:set_material(mat1)

-- A small stellated dodecahedron.

-- require('smstdodeca')

-- steldodec:set_material(Purple)
-- steldodec:set_material()
-- scene_root:add_child(steldodec)

white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'unique.png', 512, 512,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, orange_light})
