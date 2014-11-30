mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0.3, 0.0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, 0.3, 0.0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0.3, 0.0)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, 0.3, 0.0)

checkered = gr.texture('CheckeredMap.png', {0.0, 0.0, 0.0}, 1.0)
globe = gr.texture('globe.png', {0.0, 0.0, 0.0}, 1.0)
frost = gr.texture('frost2.png', {0.0, 0.0, 0.0}, 1.0)
wood = gr.texture('wood.png', {0.0, 0.0, 0.0}, 1.0)
marble = gr.texture('marble.png', {0.0, 0.0, 0.0}, 1.0)
brick = gr.texture('brick.png', {0.0, 0.0, 0.0}, 1.0)
water = gr.texture('water.png', {0.0, 0.0, 0.0}, 1.0)


scene_root = gr.node('root')
-- scene_root:rotate('y',90)

s1 = gr.nh_box('s1', {0, 0, 0}, 50)
scene_root:add_child(s1)
s1:set_material(water)
s1:translate(-250, 250 , 0)
s1:scale(2,2,2)
-- s1:rotate('x', 45)
-- s1:rotate('y', 45)


s2 = gr.nh_box('s2', {0, 0, 0}, 50)
scene_root:add_child(s2)
s2:set_material(brick)
s2:translate(-250, 80 , 0)
s2:scale(2,2,2)

s3 = gr.nh_box('s3', {0, 0, 0}, 50)
scene_root:add_child(s3)
s3:set_material(marble)
s3:translate(-250, -80 , 0)
s3:scale(2,2,2)


s4 = gr.nh_box('s4', {0, 0, 0}, 50)
scene_root:add_child(s4)
s4:set_material(wood)
s4:translate(-250, -250 , 0)
s4:scale(2,2,2)



c1 = gr.nh_sphere('c1', {-80, 250, 0}, 60)
scene_root:add_child(c1)
c1:set_material(water)

c2 = gr.nh_sphere('c2', {-80, 80, 0}, 60)
scene_root:add_child(c2)
c2:set_material(brick)

c3 = gr.nh_sphere('c3', {-80, -80, 0}, 60)
scene_root:add_child(c3)
c3:set_material(marble)

c4 = gr.nh_sphere('c4', {-80, -250, 0}, 60)
scene_root:add_child(c4)
c4:set_material(wood)

cone1 = gr.cone('cone1')
scene_root:add_child(cone1)
cone1:set_material(water)
cone1:translate(80,330,0)
cone1:rotate('x', 90)
cone1:scale(60,60,110)

cone2 = gr.cone('cone2')
scene_root:add_child(cone2)
cone2:set_material(brick)
cone2:translate(80,140,0)
cone2:rotate('x', 90)
cone2:scale(60,60,110)

cone3 = gr.cone('cone3')
scene_root:add_child(cone3)
cone3:set_material(marble)
cone3:translate(80,-20,0)
cone3:rotate('x', 90)
cone3:scale(60,60,110)

cone4 = gr.cone('cone4')
scene_root:add_child(cone4)
cone4:set_material(wood)
cone4:translate(80,-190,0)
cone4:rotate('x', 90)
cone4:scale(60,60,110)


cyl1 = gr.cylinder('cyl1')
scene_root:add_child(cyl1)
cyl1:set_material(water)
cyl1:translate(250,330,0)
cyl1:rotate('x', 90)
cyl1:scale(50,50,110)

cyl2 = gr.cylinder('cyl2')
scene_root:add_child(cyl2)
cyl2:set_material(brick)
cyl2:translate(250,140,0)
cyl2:rotate('x', 90)
cyl2:scale(50,50,110)

cyl3 = gr.cylinder('cyl3')
scene_root:add_child(cyl3)
cyl3:set_material(marble)
cyl3:translate(250,-20,0)
cyl3:rotate('x', 90)
cyl3:scale(50,50,110)


cyl4 = gr.cylinder('cyl4')
scene_root:add_child(cyl4)
cyl4:set_material(wood)
cyl4:translate(250,-190,0)
cyl4:rotate('x', 90)
cyl4:scale(50,50,110)


white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'textures.png', 500, 500,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light})

