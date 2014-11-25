mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0.3, 0.0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, 0.3, 0.0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0.3, 0.0)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, 0.3, 0.0)

checkered = gr.texture('CheckeredMap.png', {0.0, 0.0, 0.0}, 1.0)

scene_root = gr.node('root')


s1 = gr.nh_sphere('s1', {0, 0, 0}, 100)
scene_root:add_child(s1)
s1:set_material(checkered)
s1:rotate('z', 90)

white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'checkered.png', 500, 500,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, orange_light})
