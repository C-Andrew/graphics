-- A more macho version of simple_cows.py, in which cows aren't
-- spheres, they're cow-shaped polyhedral models.

-- ##############################################
-- the scene
-- ##############################################

scene = gr.node('scene')
scene:translate(0,0,-40)
scene:rotate('X', 23)


-- We'll need an extra function that knows how to read Wavefront .OBJ
-- files.

require('readobj')

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
Aqua = gr.material({20/255.0, 206/255.0, 235/255.0}, {0.5, 0.4, 0.8}, 25, 0.0)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
Yellow = gr.material({230/255.0, 230/255.0, 10/255.0}, {0.5, 0.4, 0.8}, 25, 0.0, 0.0)

-- #############################################
-- Read in the regice model from a separate file.
-- #############################################

regice = gr.mesh('regice', readobj('regice.obj'))
factor = 2.0/(2.76+3.637)

regice:set_material(Aqua)

regice:translate(0.0, -1.0, 0.0)
regice:scale(factor, factor, factor)
regice:translate(0.0, 3.637, 0.0)

crystal = gr.mesh('crystal', readobj('crystal.obj'))
factor = 2.0/(2.76+3.637)

crystal:set_material(Aqua)
crystal:translate(0.0, -1.0, 0.0)
crystal:rotate('y', -45)
crystal:scale(1, 1, 1)




-- the floor

plane = gr.mesh('plane', {
		   { -1, 0, -1 }, 
		   {  1, 0, -1 }, 
		   {  1,  0, 1 }, 
		   { -1, 0, 1  }
		}, {
		   {3, 2, 1, 0}
		})
scene:add_child(plane)
plane:set_material(grass)
plane:scale(30, 30, 30)

-- Use the instanced cow model to place some actual cows in the scene.
-- For convenience, do this in a loop.



regice_instance = gr.node('regice')
scene:add_child(regice_instance)
regice_instance:add_child(regice)
regice_instance:translate(1,1.3,14)
regice_instance:rotate('Y', 0)
regice_instance:scale(3, 3, 3)

-- Vertical
eye1 = gr.sphere('eye1')
regice_instance:add_child(eye1)
eye1:set_material(Yellow)
eye1:translate(0, 4, 1.2)
eye1:scale(0.12,0.12,0.12)

eye2 = gr.sphere('eye2')
regice_instance:add_child(eye2)
eye2:set_material(Yellow)
eye2:translate(0, 4.5, 1.2)
eye2:scale(0.12,0.12,0.12)

eye3 = gr.sphere('eye3')
regice_instance:add_child(eye3)
eye3:set_material(Yellow)
eye3:translate(0, 5, 1.1)
eye3:scale(0.12,0.12,0.12)
  
-- Horizontal Left
eye4 = gr.sphere('eye4')
regice_instance:add_child(eye4)
eye4:set_material(Yellow)
eye4:translate(-0.38, 4.5, 1.0)
eye4:scale(0.12,0.12,0.12)

eye5 = gr.sphere('eye5')
regice_instance:add_child(eye5)
eye5:set_material(Yellow)
eye5:translate(-0.7, 4.5, 0.85)
eye5:scale(0.12,0.12,0.12)

-- Horizontal Right
eye6 = gr.sphere('eye6')
regice_instance:add_child(eye6)
eye6:set_material(Yellow)
eye6:translate(0.4, 4.5, 1.0)
eye6:scale(0.12,0.12,0.12)

eye7 = gr.sphere('eye7')
regice_instance:add_child(eye7)
eye7:set_material(Yellow)
eye7:translate(0.7, 4.5, 0.85)
eye7:scale(0.12,0.12,0.12)

-- crystal_node = gr.node('crystal')
-- scene:add_child(crystal_node)
-- crystal_node:add_child(crystal)
-- crystal_node:translate(1,1.3,14)
-- crystal_node:rotate('Y', 20)
-- crystal_node:scale(1.4, 1.4, 1.4)

require('buckyball')

scene:add_child(buckyball)
buckyball:set_material(stone)
buckyball:translate(0, 10, 0)
buckyball:scale(1.5, 1.5, 1.5)

-- white_light = gr.light({0.0, 0.0, 200.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orig_light = gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0})

gr.render(scene,
	  'regice.png',
	  256, 256,
	  {0, 2, 30}, {0, 0, -1},  -- Eye and View
	  {0, 1, 0}, 50,		   -- Up and FOV	
	  {0.4, 0.4, 0.4},		   -- Color ambient
	  { orig_light} -- List of lights
	  )
